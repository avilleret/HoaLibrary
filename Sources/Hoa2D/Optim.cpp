/*
// Copyright (c) 2012-2014 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "Optim.h"

namespace Hoa2D
{
    Optim::Optim(unsigned int order, Mode mode) : Ambisonic(order)
    {
        m_harmonics = new double[m_number_of_harmonics];
        setMode(mode);
    }
    
    void Optim::setMode(Mode mode)
    {
        m_mode = mode;
        if(m_mode == Basic)
        {
            for(int i = 0; i < m_number_of_harmonics; i++)
            {
                m_harmonics[i] = 1.;
            }
        }
        else if (m_mode == MaxRe)
        {
            for(int i = 0; i < m_number_of_harmonics; i++)
            {
                m_harmonics[i] = cos(abs(getHarmonicArgument(i)) * CICM_PI / (2 * m_order + 2));;
            }
        }
        else
        {
            for(int i = 0; i < m_number_of_harmonics; i++)
            {
                m_harmonics[i] = (long double)(factorial(m_order) * (double)factorial(m_order)) / (long double)(Factorial(m_order + getHarmonicBand(i)) * factorial(m_order - getHarmonicBand(i)));
            }
        }
    }
    
    long Optim::getMode()
    {
        return m_mode;
    }
    
    void Optim::process(const float* inputs, float* outputs)
    {
        for(int i = 0; i < m_number_of_harmonics; i++)
            outputs[i] = inputs[i] * m_harmonics[i];
    }
    
    void Optim::process(const double* inputs, double* outputs)
    {
        for(int i = 0; i < m_number_of_harmonics; i++)
            outputs[i] = inputs[i] * m_harmonics[i];
    }
    
    Optim::~Optim()
    {
        delete [] m_harmonics;
    }
}

