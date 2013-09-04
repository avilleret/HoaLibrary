/**
 * HoaLibrary : A High Order Ambisonics Library
 * Copyright (c) 2012-2013 Julien Colafrancesco, Pierre Guillot, Eliott Paris, CICM, Universite Paris-8.
 * All rights reserved.
 *
 * Website  : http://www.mshparisnord.fr/hoalibrary/
 * Contacts : cicm.mshparisnord@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *	- Redistributions may not be sold, nor may they be used in a commercial product or activity.
 *  - Redistributions of source code must retain the above copyright notice, 
 *		this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *		this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  - Neither the name of the CICM nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CicmFilterDiffuser.h"

FilterDiffuser::FilterDiffuser(long aBufferSize, double aCoefficient) : Filter()
{
   
	m_buffer_size_max = Tools::clip_min(aBufferSize, 2);
    cicm_malloc_vec_d(m_buffer, m_buffer_size_max);
	m_buffer = new double[m_buffer_size_max];
	m_buffer_size = m_buffer_size_max;

	m_ramp = 0;
    setCoefficient(aCoefficient);
}

void FilterDiffuser::setCoefficient(double aCoefficient)
{
    m_coefficient = Tools::clip(aCoefficient, 0., 1.);
}

double FilterDiffuser::getCoefficient()
{
    return m_coefficient;
}

void FilterDiffuser::setBufferSizeMax(long aBufferSize)
{
	cicm_free(m_buffer);
    cicm_malloc_vec_d(m_buffer, m_buffer_size_max);
	m_buffer = new double[m_buffer_size_max];
	m_buffer_size = m_buffer_size_max;
    m_ramp = 0;
}

long FilterDiffuser::getBufferSizeMax()
{
	return m_buffer_size_max;
}

void FilterDiffuser::setBufferSize(long aBufferSize)
{
    m_buffer_size = Tools::clip(aBufferSize, 1, m_buffer_size_max);
    if(m_ramp >= m_buffer_size)
        m_ramp -= m_buffer_size;
}

long FilterDiffuser::getBufferSize()
{
	return m_buffer_size;
}

FilterDiffuser::~FilterDiffuser()
{
	cicm_free(m_buffer);
}
