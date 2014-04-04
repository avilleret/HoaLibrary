/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "HoaCommon.max.h"

typedef struct _pi 
{	
	t_pxobject  f_ob;
	double      f_value;
    double      f_phase;
} t_pi;

void pi_int(t_pi *x, long n);
void pi_float(t_pi *x, double n) ;
void pi_assist(t_pi *x, void *b, long m, long a, char *s);
void *pi_new(t_symbol *s, int argc, t_atom *argv);

void pi_dsp64(t_pi *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void pi_perform64(t_pi *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void pi_perform64_phase(t_pi *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void pi_perform64_offset(t_pi *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

t_class *pi_class;

int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("hoa.pi~", (method)pi_new, (method)dsp_free, sizeof(t_pi), 0L, A_GIMME, 0);
	
	hoa_initclass(c, NULL);
	
    class_addmethod(c, (method)pi_int,		"int",		A_LONG, 0);
	class_addmethod(c, (method)pi_float,	"float",	A_FLOAT, 0);
    class_addmethod(c, (method)pi_assist,	"assist",	A_CANT, 0);
	class_addmethod(c, (method)pi_dsp64,	"dsp64",    A_CANT, 0);
	
    class_dspinit(c);
	class_register(CLASS_BOX, c);
	pi_class = c;
	return 0;
}

void *pi_new(t_symbol *s, int argc, t_atom *argv)
{
	t_pi *x = (t_pi *)object_alloc(pi_class);
	x->f_value = 1.0f;
    x->f_phase = 1.0f;
    
	if (atom_gettype(argv) == A_LONG)
		x->f_value = atom_getlong(argv);
	else if (atom_gettype(argv) == A_FLOAT)
		x->f_value = atom_getfloat(argv);
    
    dsp_setup((t_pxobject *)x, 2);
    outlet_new(x, "signal");
    
	return(x);
}


void pi_dsp64(t_pi *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	if(count[0])
		object_method(dsp64, gensym("dsp_add64"), x, pi_perform64, 0, NULL);
    else if(count[1])
		object_method(dsp64, gensym("dsp_add64"), x, pi_perform64_phase, 0, NULL);
    else
		object_method(dsp64, gensym("dsp_add64"), x, pi_perform64_offset, 0, NULL);
}

void pi_perform64(t_pi *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < sampleframes; i++)
    {
        x->f_value = ins[0][i];
        outs[0][i] = HOA_PI * ins[0][i];
    }
}

void pi_perform64_phase(t_pi *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < sampleframes; i++)
    {
        x->f_phase = wrap(ins[1][i], 0.0f, 1.0f);
        outs[0][i] = HOA_PI * x->f_value * x->f_phase;
    }
}

void pi_perform64_offset(t_pi *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    for(int i = 0; i < sampleframes; i++)
        outs[0][i] = HOA_PI * x->f_value * x->f_phase;
}

void pi_assist(t_pi *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET)
		sprintf(s,"(signal) \u03C0 Result");
	else
    {
        if(a)
            sprintf(s,"(signal/float) \u03C0 Phase");
        else
            sprintf(s,"(signal/float) \u03C0 Multiplier");
    }
}

void pi_int(t_pi *x, long n)
{
    if (proxy_getinlet((t_object*)x))
        x->f_phase = n != 0;
    else
    {
        x->f_value = n;
        x->f_phase = 1;
    }
}


void pi_float(t_pi *x, double n) 
{
    if (proxy_getinlet((t_object*)x))
        x->f_phase = wrap(n, 0.0f, 1.0f);
    else
    {
        x->f_value = n;
        x->f_phase = 1;
    }
}