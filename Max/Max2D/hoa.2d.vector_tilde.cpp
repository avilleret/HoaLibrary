/*
// Copyright (c) 2012-2013 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.2d.vector~.cpp
 @name      hoa.2d.vector~
 @realname  hoa.2d.vector~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A 2d soundfield energy and velocity vectors calculator.
 
 @description
 <o>hoa.2d.vector~</o> computes the coordinates of the velocity vector and the energy vector that characterize a sound field. It's an useful tool to analyse restitution quality. For futher information : Michael A. Gerzon, General metatheorie of auditory localisation. Audio Engineering Society Preprint, 3306, 1992.
 
 @discussion
 <o>hoa.2d.vector~</o> computes the coordinates of the velocity vector and the energy vector that characterize a sound field. It's an useful tool to analyse restitution quality. For futher information : Michael A. Gerzon, General metatheorie of auditory localisation. Audio Engineering Society Preprint, 3306, 1992.
 
 @category ambisonics, hoa objects, audio, MSP
 
 @seealso hoa.2d.meter~, hoa.2d.scope~, hoa.2d.decoder~
 */

#include "Hoa2D.max.h"

typedef struct _hoa_2d_vector 
{
	t_pxobject          f_ob;
	
    void*               f_attrs;
    double*             f_ins;
    double*             f_outs;
    Hoa2D::Vector*      f_vector;
    
} t_hoa_2d_vector;

void *hoa_2d_vector_new(t_symbol *s, long argc, t_atom *argv);
void hoa_2d_vector_free(t_hoa_2d_vector *x);
void hoa_2d_vector_assist(t_hoa_2d_vector *x, void *b, long m, long a, char *s);

void hoa_2d_vector_dsp64(t_hoa_2d_vector *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void hoa_2d_vector_perform64(t_hoa_2d_vector *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

t_hoa_err hoa_getinfos(t_hoa_2d_vector* x, t_hoa_boxinfos* boxinfos);

t_max_err channels_set(t_hoa_2d_vector *x, t_object *attr, long argc, t_atom *argv);
t_max_err channels_get(t_hoa_2d_vector *x, t_object *attr, long *argc, t_atom **argv);
t_max_err offset_get(t_hoa_2d_vector *x, t_object *attr, long *argc, t_atom **argv);
t_max_err offset_set(t_hoa_2d_vector *x, t_object *attr, long argc, t_atom *argv);
t_max_err angles_set(t_hoa_2d_vector *x, t_object *attr, long argc, t_atom *argv);
t_max_err angles_get(t_hoa_2d_vector *x, t_object *attr, long *argc, t_atom **argv);

t_class *hoa_2d_vector_class;

#ifdef HOA_PACKED_LIB
int hoa_2d_vector_main(void)
#else
int C74_EXPORT main(void)
#endif
{
	t_class *c;
	
	c = class_new("hoa.2d.vector~", (method)hoa_2d_vector_new, (method)hoa_2d_vector_free, (long)sizeof(t_hoa_2d_vector), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.2d.vector~", (char *)"hoa.2d.vector~");
	
    hoa_initclass(c, (method)hoa_getinfos);
	
	// @method signal @digest Array of channels signals.
	// @description Array of channels signals.
	class_addmethod(c, (method)hoa_2d_vector_dsp64,	"dsp64",	A_CANT, 0);
	class_addmethod(c, (method)hoa_2d_vector_assist,   "assist",	A_CANT, 0);
    
    CLASS_ATTR_LONG                 (c, "channels", 0, t_hoa_2d_vector, f_attrs);
	CLASS_ATTR_CATEGORY             (c, "channels", 0, "Planewaves");
    CLASS_ATTR_LABEL                (c, "channels", 0, "Number of Channels");
	CLASS_ATTR_ACCESSORS            (c, "channels", channels_get, channels_set);
    CLASS_ATTR_ORDER                (c, "channels", 0, "1");
	// @description The number of channels.
    
    CLASS_ATTR_DOUBLE               (c, "offset", 0, t_hoa_2d_vector, f_attrs);
	CLASS_ATTR_CATEGORY             (c, "offset", 0, "Planewaves");
    CLASS_ATTR_LABEL                (c, "offset", 0, "Offset of Channels");
	CLASS_ATTR_ACCESSORS            (c, "offset", offset_get, offset_set);
    CLASS_ATTR_ORDER                (c, "offset", 0, "2");
	// @description The offset of the channels.
    
    CLASS_ATTR_FLOAT_VARSIZE        (c, "angles", 0, t_hoa_2d_vector, f_attrs, f_attrs, MAX_CHANNELS);
	CLASS_ATTR_CATEGORY             (c, "angles", 0, "Planewaves");
    CLASS_ATTR_LABEL                (c, "angles", 0, "Angles of Channels");
	CLASS_ATTR_ACCESSORS            (c, "angles", angles_get, angles_set);
    CLASS_ATTR_ORDER                (c, "angles", 0, "3");
	// @description The angles of the channels. Angles are in degree wrapped between 0. and 360.
    
    class_dspinit(c);
	class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.vector~"));
	hoa_2d_vector_class = c;
    
	return 0;
}

void *hoa_2d_vector_new(t_symbol *s, long argc, t_atom *argv)
{
	// @arg 0 @name number-of-channels @optional 0 @type int @digest The number of channels
	// @description First argument sets the number of channels.
	
	t_hoa_2d_vector *x = NULL;
    x = (t_hoa_2d_vector *)object_alloc(hoa_2d_vector_class);
    
	if(x)
	{
        long number_of_channels = 4;
        if (argc && atom_gettype(argv) == A_LONG)
            number_of_channels = clip_min(atom_getlong(argv), 1);
        
		x->f_vector = new Hoa2D::Vector(number_of_channels);
		
		dsp_setup((t_pxobject *)x, x->f_vector->getNumberOfChannels());
		for (int i = 0; i < 4; i++)
			outlet_new(x, "signal");
        
		x->f_ins = new double[x->f_vector->getNumberOfChannels() * SYS_MAXBLKSIZE];
        x->f_outs = new double[4 * SYS_MAXBLKSIZE];
        
        attr_args_process(x, argc, argv);
	}

	return (x);
}

t_hoa_err hoa_getinfos(t_hoa_2d_vector* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_2D;
	boxinfos->autoconnect_inputs = x->f_vector->getNumberOfChannels();
	boxinfos->autoconnect_outputs = 0;
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_PLANEWAVES;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_STANDARD;
	return HOA_ERR_NONE;
}

void hoa_2d_vector_dsp64(t_hoa_2d_vector *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    object_method(dsp64, gensym("dsp_add64"), x, hoa_2d_vector_perform64, 0, NULL);
}

void hoa_2d_vector_perform64(t_hoa_2d_vector *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < numins; i++)
    {
        cblas_dcopy(sampleframes, ins[i], 1, x->f_ins+i, numins);
    }
	for(int i = 0; i < sampleframes; i++)
    {
        x->f_vector->process(x->f_ins + numins * i, x->f_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        cblas_dcopy(sampleframes, x->f_outs+i, numouts, outs[i], 1);
    }
}

void hoa_2d_vector_assist(t_hoa_2d_vector *x, void *b, long m, long a, char *s)
{
    if(m == ASSIST_INLET)
        sprintf(s,"(signal) %s", x->f_vector->getChannelName(a).c_str());
    else
        sprintf(s,"(signal) %s", x->f_vector->getChannelName(a).c_str());
}


void hoa_2d_vector_free(t_hoa_2d_vector *x) 
{
	dsp_free((t_pxobject *)x);
	delete x->f_vector;
    delete [] x->f_ins;
	delete [] x->f_outs;
}

t_max_err channels_get(t_hoa_2d_vector *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = 1;
    argv[0] = new t_atom[argc[0]];
    atom_setlong(argv[0], x->f_vector->getNumberOfChannels());
    
    return MAX_ERR_NONE;
}

t_max_err channels_set(t_hoa_2d_vector *x, t_object *attr, long argc, t_atom *argv)
{
    t_object *b = NULL;
    if(argc && argv && atom_gettype(argv) == A_LONG)
    {
        int number_of_loudspeakers = atom_getlong(argv);
        if(number_of_loudspeakers != x->f_vector->getNumberOfChannels() &&
           number_of_loudspeakers > 0 &&
           number_of_loudspeakers <= MAX_CHANNELS)
        {
            object_method(gensym("dsp")->s_thing, hoa_sym_stop);
            
            delete x->f_vector;
            x->f_vector = new Hoa2D::Vector(number_of_loudspeakers);
        
            object_obex_lookup(x, gensym("#B"), (t_object **)&b);
            object_method(b, hoa_sym_dynlet_begin);
        
            dsp_resize((t_pxobject*)x, x->f_vector->getNumberOfChannels());
            object_method(b, hoa_sym_dynlet_end);
        
            object_attr_setvalueof(x, hoa_sym_angles, 0, NULL);
        }
    }
    return MAX_ERR_NONE;
}

t_max_err offset_get(t_hoa_2d_vector *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = 1;
    argv[0] = (t_atom *)sysmem_newptr(argc[0] * sizeof(t_atom));
    
    if(argv[0])
    {
		atom_setfloat(argv[0], wrap(x->f_vector->getChannelsOffset() / HOA_2PI * 360.f, -180, 180));
    }
    else
    {
        argc[0] = 0;
        argv[0] = NULL;
    }
    return MAX_ERR_NONE;
}

t_max_err offset_set(t_hoa_2d_vector *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv && (atom_gettype(argv) == A_FLOAT || atom_gettype(argv) == A_LONG))
    {
        double offset = wrap_twopi(wrap(atom_getfloat(argv), -180, 180) / 360. * HOA_2PI);
        if(offset != x->f_vector->getChannelsOffset())
        {
            x->f_vector->setChannelsOffset(offset);
        }
    }
    return MAX_ERR_NONE;
}

t_max_err angles_get(t_hoa_2d_vector *x, t_object *attr, long *argc, t_atom **argv)
{
    argc[0] = x->f_vector->getNumberOfChannels();
    argv[0] = new t_atom[argc[0]];
    
    for(int i = 0; i < x->f_vector->getNumberOfChannels(); i++)
    {
        atom_setfloat(argv[0]+i, x->f_vector->getChannelAzimuth(i) / HOA_2PI * 360.);
    }
    
    return MAX_ERR_NONE;
}

t_max_err angles_set(t_hoa_2d_vector *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
        for(int i = 0; i < argc && i < x->f_vector->getNumberOfChannels(); i++)
        {
            if(atom_gettype(argv+i) == A_LONG || atom_gettype(argv+i) == A_FLOAT);
               x->f_vector->setChannelAzimuth(i, clip_minmax(atom_getfloat(argv+i), -360., 360.) / 360. * HOA_2PI);
        }
    }
    return MAX_ERR_NONE;
}

