/*
 // Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "Hoa3D.pd.h"

class Map3DPolarLines
{
    
private:
    float*      m_values_old;
    float*      m_values_new;
    float*      m_values_step;
    unsigned int m_counter;
    unsigned int m_ramp;
    unsigned int m_number_of_sources;
    
public:
    Map3DPolarLines(unsigned int numberOfSources);
    ~Map3DPolarLines();
    
    inline unsigned int getNumberOfSources() const
    {
        return m_number_of_sources;
    }
    
    inline unsigned int getRamp() const
    {
        return m_ramp;
    }
    
    inline double getRadius(unsigned int index) const
    {
        assert(index < m_number_of_sources);
        return m_values_new[index];
    }
    
    inline double getAzimuth(unsigned int index) const
    {
        assert(index < m_number_of_sources);
        return m_values_new[m_number_of_sources +index];
    }
    
    void setRamp(unsigned int ramp);
    void setDistance(unsigned int index, double radius);
    void setAzimuth(unsigned int index, double azimuth);
    void setDistanceDirect(unsigned int index, double radius);
    void setAzimuthDirect(unsigned int index, double azimuth);
    
    void process(float* vector);
};

typedef struct _hoa_map_tilde_3D
{
    t_edspobj       f_ob;
    
    t_float*        f_sig_ins;
    t_float*        f_sig_outs;
    t_float*        f_lines_vector;
	int             f_mode;
    double          f_ramp;
    
    Map*              f_map;
    Map3DPolarLines*  f_lines;
} t_hoa_map_tilde_3D;

void *hoa_map_tilde_3D_new(t_symbol *s, long argc, t_atom *argv);
void hoa_map_tilde_3D_free(t_hoa_map_tilde_3D *x);
void hoa_map_tilde_3D_float(t_hoa_map_tilde_3D *x, float f);
void hoa_map_tilde_3D_list(t_hoa_map_tilde_3D *x, t_symbol *s, long argc, t_atom *argv);

void hoa_map_tilde_3D_dsp(t_hoa_map_tilde_3D *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags);

void hoa_map_tilde_3D_perform_multisources(t_hoa_map_tilde_3D *x, t_object *dsp64, float **ins, long numins, float **outs, long numouts, long sampleframes, long flags, void *userparam);

void hoa_map_tilde_3D_perform(t_hoa_map_tilde_3D *x, t_object *dsp64, float **ins, long numins, float **outs, long numouts, long sampleframes, long flags, void *userparam);
void hoa_map_tilde_3D_perform_in1(t_hoa_map_tilde_3D *x, t_object *dsp64, float **ins, long numins, float **outs, long numouts, long sampleframes, long flags, void *userparam);
void hoa_map_tilde_3D_perform_in2(t_hoa_map_tilde_3D *x, t_object *dsp64, float **ins, long numins, float **outs, long numouts, long sampleframes, long flags, void *userparam);
void hoa_map_tilde_3D_perform_in1_in2(t_hoa_map_tilde_3D *x, t_object *dsp64, float **ins, long numins, float **outs, long numouts, long sampleframes, long flags, void *userparam);

t_pd_err ramp_set(t_hoa_map_tilde_3D *x, t_object *attr, long argc, t_atom *argv);

t_hoa_err hoa_getinfos(t_hoa_map_tilde_3D* x, t_hoa_boxinfos* boxinfos);

t_eclass *hoa_map_tilde_3D_class;

extern "C" void setup_hoa0x2e3d0x2emap_tilde(void)
{
    t_eclass* c;
    
    c = eclass_new("hoa.3d.map~", (method)hoa_map_tilde_3D_new, (method)hoa_map_tilde_3D_free, (short)sizeof(t_hoa_map_tilde_3D), 0L, A_GIMME, 0);
    
	eclass_dspinit(c);
    hoa_initclass(c, (method)hoa_getinfos);
	eclass_addmethod(c, (method)hoa_map_tilde_3D_dsp,          "dsp",      A_CANT, 0);
    eclass_addmethod(c, (method)hoa_map_tilde_3D_list,         "list",     A_GIMME, 0);
    eclass_addmethod(c, (method)hoa_map_tilde_3D_float,        "float",    A_FLOAT, 0);
    
    CLASS_ATTR_DOUBLE           (c, "ramp", 0, t_hoa_map_tilde_3D, f_ramp);
	CLASS_ATTR_CATEGORY			(c, "ramp", 0, "Behavior");
	CLASS_ATTR_LABEL			(c, "ramp", 0, "Ramp Time (ms)");
	CLASS_ATTR_ORDER			(c, "ramp", 0, "2");
	CLASS_ATTR_ACCESSORS		(c, "ramp", NULL, ramp_set);
	CLASS_ATTR_SAVE				(c, "ramp", 1);
    
    eclass_register(CLASS_OBJ, c);
    hoa_map_tilde_3D_class = c;
}

void *hoa_map_tilde_3D_new(t_symbol *s, long argc, t_atom *argv)
{
    t_hoa_map_tilde_3D *x = NULL;
    t_binbuf *d;
    int	order = 1;
    int numberOfSources = 1;
    
    if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
    x = (t_hoa_map_tilde_3D *)eobj_new(hoa_map_tilde_3D_class);

	if (x)
	{
		if(atom_gettype(argv) == A_LONG)
			order = Hoa::clip_min(atom_getlong(argv), 0);
        if(argc > 1 && atom_gettype(argv+1) == A_LONG)
            numberOfSources = Hoa::clip_minmax(atom_getlong(argv+1), 1, 255);
        if(argc > 2 && atom_gettype(argv+2) == A_SYM)
        {
            if(atom_getsym(argv+2) == gensym("car") || atom_getsym(argv+2) == gensym("cartesian"))
                x->f_mode = 1;
            else
                x->f_mode = 0;
        }
        else
            x->f_mode = 0;
        
        x->f_ramp       = 100;
		x->f_map        = new Map(order, numberOfSources);
		x->f_lines      = new Map3DPolarLines(x->f_map->getNumberOfSources());
        x->f_lines->setRamp(0.1 * sys_getsr());
        for (int i = 0; i < x->f_map->getNumberOfSources(); i++)
        {
            x->f_lines->setDistanceDirect(i, 1);
            x->f_lines->setAzimuthDirect(i, 0.);
        }
        
		if(x->f_map->getNumberOfSources() == 1)
            eobj_dspsetup(x, 4, x->f_map->getNumberOfHarmonics());
        else
            eobj_dspsetup(x, x->f_map->getNumberOfSources(), x->f_map->getNumberOfHarmonics());
        
        if(x->f_map->getNumberOfSources() == 1)
            x->f_sig_ins    = new t_float[4 * SYS_MAXBLKSIZE];
        else
            x->f_sig_ins    = new t_float[x->f_map->getNumberOfSources() * SYS_MAXBLKSIZE];
		
        x->f_sig_outs       = new t_float[x->f_map->getNumberOfHarmonics() * SYS_MAXBLKSIZE];
        x->f_lines_vector   = new float[x->f_map->getNumberOfSources() * 2];

        ebox_attrprocess_viabinbuf(x, d);
	}

	return (x);
}

t_hoa_err hoa_getinfos(t_hoa_map_tilde_3D* x, t_hoa_boxinfos* boxinfos)
{
	boxinfos->object_type = HOA_OBJECT_3D;
	
	if(x->f_map->getNumberOfSources() == 1)
		boxinfos->autoconnect_inputs = 1;
	else
		boxinfos->autoconnect_inputs = x->f_map->getNumberOfSources();
	
	boxinfos->autoconnect_outputs = x->f_map->getNumberOfHarmonics();
	boxinfos->autoconnect_inputs_type = HOA_CONNECT_TYPE_STANDARD;
	boxinfos->autoconnect_outputs_type = HOA_CONNECT_TYPE_AMBISONICS;
	return HOA_ERR_NONE;
}

void hoa_map_tilde_3D_float(t_hoa_map_tilde_3D *x, float f)
{
    if(x->f_map->getNumberOfSources() == 1)
    {
		if(x->f_mode == 0)
		{
			if(eobj_getproxy((t_object *)x) == 1)
			{
				x->f_lines->setDistance(0, Hoa::clip_min(f, 0.));
			}
			else if(eobj_getproxy((t_object *)x) == 2)
			{
				x->f_lines->setAzimuth(0, f);
			}
		}
		else if(x->f_mode == 1)
		{
			if(eobj_getproxy((t_object *)x) == 1)
			{
                float ord = Hoa::ordinate(x->f_lines->getRadius(0), x->f_lines->getAzimuth(0));
				x->f_lines->setDistance(0, Hoa::radius(f, ord));
                x->f_lines->setAzimuth(0, azimuth(f, ord));
			}
			else if(eobj_getproxy((t_object *)x) == 2)
			{
				float abs = Hoa::abscissa(x->f_lines->getRadius(0), x->f_lines->getAzimuth(0));
                x->f_lines->setDistance(0, Hoa::radius(abs, f));
				x->f_lines->setAzimuth(0, azimuth(abs, f));
			}
		}
    }
}

void hoa_map_tilde_3D_list(t_hoa_map_tilde_3D *x, t_symbol* s, long argc, t_atom* argv)
{
    if(argc > 2 && argv && atom_gettype(argv) == A_LONG && atom_gettype(argv+1) == A_SYM)
    {
        int index = atom_getlong(argv);
        if(index < 1 || index > x->f_map->getNumberOfSources())
            return;
        
        if(argc > 3 && (atom_getsym(argv+1) == hoa_sym_polar || atom_getsym(argv+1) == hoa_sym_pol))
        {
            x->f_lines->setDistance(index-1, atom_getfloat(argv+2));
            x->f_lines->setAzimuth(index-1, atom_getfloat(argv+3));
        }
        else if(argc > 3 && (atom_getsym(argv+1) == hoa_sym_cartesian || atom_getsym(argv+1) == hoa_sym_car))
        {
            x->f_lines->setDistance(index-1, radius(atom_getfloat(argv+2), atom_getfloat(argv+3)));
            x->f_lines->setAzimuth(index-1, azimuth(atom_getfloat(argv+2), atom_getfloat(argv+3)));
        }
        else if(argc > 2 && atom_getsym(argv+1) == hoa_sym_mute)
        {
            //x->f_map->setMute(index-1, atom_getlong(argv+2));
        }
    }
}

t_pd_err ramp_set(t_hoa_map_tilde_3D *x, t_object *attr, long argc, t_atom *argv)
{
    if(argc && argv)
    {
        if(atom_gettype(argv) == A_LONG || atom_gettype(argv) == A_FLOAT)
        {
            x->f_ramp = Hoa::clip_min(atom_getfloat(argv), 0);
            x->f_lines->setRamp(x->f_ramp / 1000. * sys_getsr());
        }
    }
    
    return 0;
}

void hoa_map_tilde_3D_dsp(t_hoa_map_tilde_3D *x, t_object *dsp, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->f_lines->setRamp(x->f_ramp / 1000. * samplerate);
    
    if(x->f_map->getNumberOfSources() == 1)
    {
		if(count[1] && count[2])
            object_method(dsp, gensym("dsp_add64"), x, (method)hoa_map_tilde_3D_perform_in1_in2, 0, NULL);
        else if(count[1] && !count[2])
            object_method(dsp, gensym("dsp_add64"), x, (method)hoa_map_tilde_3D_perform_in1, 0, NULL);
        else if(!count[1] && count[2])
            object_method(dsp, gensym("dsp_add64"), x, (method)hoa_map_tilde_3D_perform_in2, 0, NULL);
		else if(!count[1] && !count[2])
            object_method(dsp, gensym("dsp_add"), x, (method)hoa_map_tilde_3D_perform, 0, NULL);
    }
    else
    {
        object_method(dsp, gensym("dsp_add"), x, (method)hoa_map_tilde_3D_perform_multisources, 0, NULL);
    }
}

void hoa_map_tilde_3D_perform_multisources(t_hoa_map_tilde_3D *x, t_object *dsp64, float **ins, long numins, float **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	int nsources = x->f_map->getNumberOfSources();
    for(int i = 0; i < numins; i++)
    {
        cblas_scopy(sampleframes, ins[i], 1, x->f_sig_ins+i, numins);
    }
    for(int i = 0; i < sampleframes; i++)
    {
        x->f_lines->process(x->f_lines_vector);
		for(int j = 0; j < nsources; j++)
			x->f_map->setDistance(j, x->f_lines_vector[j]);
        for(int j = 0; j < nsources; j++)
			x->f_map->setAzimuth(j, x->f_lines_vector[j + nsources]);
        
        x->f_map->process(x->f_sig_ins + numins * i, x->f_sig_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        cblas_scopy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_map_tilde_3D_perform(t_hoa_map_tilde_3D *x, t_object *dsp64, float **ins, long numins, float **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < sampleframes; i++)
    {
		x->f_lines->process(x->f_lines_vector);
		x->f_map->setDistance(0, x->f_lines_vector[0]);
		x->f_map->setAzimuth(0, x->f_lines_vector[1]);
        x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        cblas_scopy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_map_tilde_3D_perform_in1(t_hoa_map_tilde_3D *x, t_object *dsp64, float **ins, long numins, float **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < sampleframes; i++)
    {
		x->f_lines->process(x->f_lines_vector);
		if (x->f_mode == 0)
		{
			x->f_map->setDistance(0, ins[1][i]);
			x->f_map->setAzimuth(0, x->f_lines_vector[1]);
		}
		else if (x->f_mode == 1)
		{
			x->f_map->setAzimuth(0, azimuth(ins[1][i], x->f_lines_vector[1]));
			x->f_map->setDistance(0, radius(ins[1][i], x->f_lines_vector[1]));
		}
		
        x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        cblas_scopy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_map_tilde_3D_perform_in2(t_hoa_map_tilde_3D *x, t_object *dsp64, float **ins, long numins, float **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < sampleframes; i++)
    {
		x->f_lines->process(x->f_lines_vector);
		if (x->f_mode == 0)
		{
            x->f_map->setDistance(0, x->f_lines_vector[0]);
			x->f_map->setAzimuth(0, ins[2][i]);
		}
		else if (x->f_mode == 1)
		{
			x->f_map->setAzimuth(0, azimuth(x->f_lines_vector[0], ins[2][i]));
			x->f_map->setDistance(0, radius(x->f_lines_vector[0], ins[2][i]));
		}
		
        x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        cblas_scopy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_map_tilde_3D_perform_in1_in2(t_hoa_map_tilde_3D *x, t_object *dsp64, float **ins, long numins, float **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < sampleframes; i++)
    {
        if(x->f_mode == 0)
		{
			x->f_map->setDistance(0, ins[1][i]);
			x->f_map->setAzimuth(0, ins[2][i]);
		}
		else if(x->f_mode == 1)
		{
			x->f_map->setAzimuth(0, azimuth(ins[1][i], ins[2][i]));
			x->f_map->setDistance(0, radius(ins[1][i], ins[2][i]));
		}
        x->f_map->process(&ins[0][i], x->f_sig_outs + numouts * i);
    }
    for(int i = 0; i < numouts; i++)
    {
        cblas_scopy(sampleframes, x->f_sig_outs+i, numouts, outs[i], 1);
    }
}

void hoa_map_tilde_3D_free(t_hoa_map_tilde_3D *x)
{
	eobj_dspfree(x);
	delete x->f_lines;
	delete x->f_map;
    delete [] x->f_sig_ins;
    delete [] x->f_sig_outs;
	delete [] x->f_lines_vector;
}

Map3DPolarLines::Map3DPolarLines(unsigned int numberOfSources)
{
    assert(numberOfSources > 0);
    m_number_of_sources = numberOfSources;
    
    m_values_old    = new float[m_number_of_sources * 2];
    m_values_new    = new float[m_number_of_sources * 2];
    m_values_step   = new float[m_number_of_sources * 2];
}

Map3DPolarLines::~Map3DPolarLines()
{
    delete [] m_values_old;
    delete [] m_values_new;
    delete [] m_values_step;
}

void Map3DPolarLines::setRamp(unsigned int ramp)
{
    m_ramp = clip_min(ramp, (long)1);
}

void Map3DPolarLines::setDistance(unsigned int index, double radius)
{
    assert(index < m_number_of_sources);
    m_values_new[index]  = radius;
    m_values_step[index] = (m_values_new[index] - m_values_old[index]) / (double)m_ramp;
    m_counter = 0;
}

void Map3DPolarLines::setAzimuth(unsigned int index, double azimuth)
{
    assert(index < m_number_of_sources);
    m_values_new[index + m_number_of_sources] = wrap_twopi(azimuth);
    m_values_old[index + m_number_of_sources] = wrap_twopi(m_values_old[index + m_number_of_sources]);
    
    double distance;
    if(m_values_old[index + m_number_of_sources] > m_values_new[index + m_number_of_sources])
        distance = (m_values_old[index + m_number_of_sources] - m_values_new[index + m_number_of_sources]);
    else
        distance = (m_values_new[index + m_number_of_sources] - m_values_old[index + m_number_of_sources]);
    
    if(distance <= HOA_PI)
    {
        m_values_step[index + m_number_of_sources] = (m_values_new[index + m_number_of_sources] - m_values_old[index + m_number_of_sources]) / (double)m_ramp;
    }
    else
    {
        if(m_values_new[index + m_number_of_sources] > m_values_old[index + m_number_of_sources])
        {
            m_values_step[index + m_number_of_sources] = ((m_values_new[index + m_number_of_sources] - HOA_2PI) - m_values_old[index + m_number_of_sources]) / (double)m_ramp;
        }
        else
        {
            m_values_step[index + m_number_of_sources] = ((m_values_new[index + m_number_of_sources] + HOA_2PI) - m_values_old[index + m_number_of_sources]) / (double)m_ramp;
        }
    }
    m_counter = 0;
}

void Map3DPolarLines::setDistanceDirect(unsigned int index, double radius)
{
    assert(index < m_number_of_sources);
    m_values_old[index] = m_values_new[index] = radius;
    m_values_step[index] = 0.;
    m_counter = 0;
}

void Map3DPolarLines::setAzimuthDirect(unsigned int index, double azimuth)
{
    assert(index < m_number_of_sources);
    m_values_old[index + m_number_of_sources] = m_values_new[index + m_number_of_sources] = azimuth;
    m_values_step[index + m_number_of_sources] = 0.;
    m_counter = 0;
}

void Map3DPolarLines::process(float* vector)
{
    cblas_saxpy(m_number_of_sources * 2, 1., m_values_step, 1, m_values_old, 1);
    if(m_counter++ >= m_ramp)
    {
        cblas_scopy(m_number_of_sources * 2, m_values_new, 1, m_values_old, 1);
        memset(m_values_step, 0, sizeof(float) * m_number_of_sources * 2);
        m_counter    = 0;
    }
    cblas_scopy(m_number_of_sources * 2, m_values_old, 1, vector, 1);
}