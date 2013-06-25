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
 *  - Redistributions may not be sold, nor may they be used in a commercial product or activity.
 *  - Redistributions of source code must retain the above copyright notice, 
 *      this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  - Neither the name of the CICM nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DEF_AMBISONICSFREEVERB
#define DEF_AMBISONICSFREEVERB

#include "../HoaAmbisonics/Ambisonics.h"
#include "../CicmLibrary/CicmReverb/CicmFreeverb.h"

class AmbisonicsFreeverb : public Ambisonics
{
private:
    
	double	m_gain;
    double  m_wet;
    double  m_dry;
	double	roomsize;
	double	damp;
	double	mode;
    double  m_diffuse;
    vector <Freeverb*> m_freeverb;
    double  m_diffuse_spread;
    double  m_directional_spread;
    
	void	update();

public:
	AmbisonicsFreeverb(long anOrder = 1, long aVectorSize = 0, double aSamplingRate = 44100.);
    
    void	setVectorSize(long aVectorSize);
    void	setSamplingRate(long aSamplingRate);
    
 	void	setDryValue(double value);
	double	getDryValue();
    void	setWetValue(double value);
	double	getWetValue();
	void	setroomsize(double value);
	double	getroomsize();
	void	setdamp(double value);
	double	getdamp();
	void	setmode(double value);
	double	getmode();
    void	setSpread(double value);
    void	setDiffuseSpread(double value);
    void	setDirectionalSpread(double value);
	double	getDiffuseSpread();
    double  getDirectionalSpread();

	inline void	process(float **anInput, float **anOutput)
    {
        float* input;
        float* output;
        for(int i = 0; i < m_number_of_harmonics; i++)
        {
            input = anInput[i];
            output = anOutput[i];
            m_freeverb[i]->process(input, output);
        }
    };
    
    inline void	process(double **anInput, double **anOutput)
    {
        double* input;
        double* output;
        
        for(int i = 0; i < m_number_of_harmonics; i++)
        {
            input = anInput[i];
            output = anOutput[i];
            m_freeverb[i]->process(input, output);
        }
    }
    
	~AmbisonicsFreeverb();
};


#endif