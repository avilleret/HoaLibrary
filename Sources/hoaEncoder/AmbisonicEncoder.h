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

#ifndef DEF_AMBISONICENCODER
#define DEF_AMBISONICENCODER

#include "../HoaAmbisonics/Ambisonic.h"

class AmbisonicEncoder : public Ambisonic
{
	
private:
	cicm_vector_double*	m_encoder_matrix;
    
	cicm_vector_float	m_harmonics_vector_float;
    cicm_vector_double	m_harmonics_vector_double;
    
    int*				m_index_vector;
	cicm_vector_float	m_vector_float;
    cicm_vector_double	m_vector_double;
    
public:
	AmbisonicEncoder(long anOrder = 1, long aVectorSize = 0);

    void    setVectorSize(long aVectorSize);
	void	setAngle(double anAngle);
    
	~AmbisonicEncoder();
    
    /********************************************************************/
    /**************************** Basic Mode ****************************/
    /********************************************************************/
    
    /************************************************************************************/
    /***************************** Perform sample by sample *****************************/
    /************************************************************************************/
    
    /*********************************** Out Of Place ***********************************/
    
    inline void process(const double input, double* outputs)
	{
        cicm_product_vec_sca_vec_d(m_harmonics_vector_double, input, outputs, m_number_of_harmonics);
	}

    inline void process(const float input, float* outputs)
	{
        cicm_product_vec_sca_vec_f(m_harmonics_vector_float, input, outputs, m_number_of_harmonics);
	}
    
    inline void process(const double input, double* outputs, const double angle)
	{
		int index = Tools::radian_wrap(angle) * CICM_1OVER2PI_RATIO;
		for(int i = 0; i < m_number_of_harmonics; i++)
        {
            m_harmonics_vector_double[i] = m_encoder_matrix[i][index];
        }
		process(input, outputs);
	}
    
    inline void process(const float input, float* outputs, const float angle)
	{
        int index = Tools::radian_wrap(angle) * CICM_1OVER2PI_RATIO;
		for(int i = 0; i < m_number_of_harmonics; i++)
        {
            m_harmonics_vector_float[i] = m_encoder_matrix[i][index];
        }
		process(input, outputs);
	}
		
    /************************************* In Place *************************************/
    
    inline void process(double* ioVector)
	{
        double input = ioVector[0];
        process(input, ioVector);
	}
    
    inline void process(float* ioVector)
	{
        float input = ioVector[0];
        process(input, ioVector);
	}
    
    inline void process(double* ioVector, const double angle)
	{
		double input = ioVector[0];
        process(input, ioVector, angle);
	}
    
    inline void process(float* ioVector, const float angle)
	{
        float input = ioVector[0];
        process(input, ioVector, angle);
	}
    
    /************************************************************************************/
    /******************************* Perform sample block *******************************/
    /************************************************************************************/
    
    /*********************************** Out Of Place ***********************************/
    
    inline void process(const double* inputs, double** outputs)
	{
		for(int i = 0; i < m_number_of_harmonics; i++)
			cicm_product_vec_sca_vec_d(inputs, m_harmonics_vector_double[i], outputs[i], m_vector_size);
	}
    
    inline void process(const float* inputs, float** outputs)
	{
        for(int i = 0; i < m_number_of_harmonics; i++)
			cicm_product_vec_sca_vec_f(inputs, m_harmonics_vector_float[i], outputs[i], m_vector_size);
	}
    
	inline void process(const double* inputs, double** outputs, const double* angles)
	{
		int index;
		cicm_vector_double pointor;
        
		for(int i = 0; i < m_vector_size; i++)
        {
			*(m_index_vector+i) = Tools::radian_wrap(*(angles+i)) * CICM_1OVER2PI_RATIO;
        }
        
        cicm_copy_vec_vec_d((double* )inputs, outputs[0], m_vector_size);
		for(int i = 1; i < m_number_of_harmonics; i++)
		{
			pointor = m_encoder_matrix[i];
			for(int j = 0; j < m_vector_size; j++)
			{
				index = m_index_vector[j];
				outputs[i][j] = pointor[index] * inputs[j];
			}
			//cicm_product_vec_vec_d(inputs, m_vector_double, outputs[i], m_vector_size);
		}
	}
    
    inline void process(const float* inputs, float** outputs, const float* angles)
	{
		int index;
		cicm_vector_double pointor;
        
		for(int i = 0; i < m_vector_size; i++)
        {
			*(m_index_vector+i) = Tools::radian_wrap(*(angles+i)) * CICM_1OVER2PI_RATIO;
        }
        
        cicm_copy_vec_vec_f(inputs, outputs[0], m_vector_size);
		for(int i = 1; i < m_number_of_harmonics; i++)
		{
			pointor = m_encoder_matrix[i];
			for(int j = 0; j < m_vector_size; j++)
			{
				index = m_index_vector[j];
				m_vector_float[j] = pointor[index];
			}
			cicm_product_vec_vec_f(inputs, m_vector_float, outputs[i], m_vector_size);
		}
	}
    
    /************************************* In Place *************************************/
    
    inline void process(double** ioVectors)
	{
		for(int i = 1; i < m_number_of_harmonics; i++)
			cicm_product_vec_sca_vec_d(ioVectors[0], m_harmonics_vector_double[i], ioVectors[i], m_vector_size);
	}
    
    inline void process(float** ioVectors)
	{
        for(int i = 1; i < m_number_of_harmonics; i++)
			cicm_product_vec_sca_vec_f(ioVectors[0], m_harmonics_vector_float[i], ioVectors[i], m_vector_size);
	}
    
	inline void process(double** ioVectors, const double* angles)
	{
		int index;
		cicm_vector_double pointor;
        
		for(int i = 0; i < m_vector_size; i++)
        {
			m_index_vector[i] = Tools::radian_wrap(angles[i]) / CICM_2PI * (double)(NUMBEROFCIRCLEPOINTS-1);
        }
        
		for(int i = 1; i < m_number_of_harmonics; i++)
		{
			pointor = m_encoder_matrix[i];
			for(int j = 0; j < m_vector_size; j++)
			{
				index = m_index_vector[j];
				m_vector_double[j] = pointor[index];
			}
            cicm_copy_vec_vec_d(ioVectors[0], ioVectors[i], m_vector_size);
			cicm_product_vec_d(m_vector_double, ioVectors[i], m_vector_size);
		}
	}
    
    inline void process(float** ioVectors, const float* angles)
	{
		int index;
		cicm_vector_double pointor;
        
		for(int i = 0; i < m_vector_size; i++)
        {
			m_index_vector[i] = Tools::radian_wrap(angles[i]) / CICM_2PI * (double)(NUMBEROFCIRCLEPOINTS-1);
        }
        
		for(int i = 1; i < m_number_of_harmonics; i++)
		{
			pointor = m_encoder_matrix[i];
			for(int j = 0; j < m_vector_size; j++)
			{
				index = m_index_vector[j];
				m_vector_float[j] = pointor[index];
			}
            cicm_copy_vec_vec_f(ioVectors[0], ioVectors[i], m_vector_size);
			cicm_product_vec_f(m_vector_float, ioVectors[i], m_vector_size);
		}
	}
    
    /********************************************************************/
    /**************************** Add Mode ******************************/
    /********************************************************************/
    
	/************************************************************************************/
    /***************************** Perform sample by sample *****************************/
    /************************************************************************************/
    
    /*********************************** Out Of Place ***********************************/
    
    inline void processAdd(double anInput, double* outputs)
	{
        cicm_product_sum_vec_sca_vec_d(m_harmonics_vector_double, anInput, outputs, m_number_of_harmonics);
	}
    
    inline void processAdd(float anInput, float* outputs)
	{
        cicm_product_sum_vec_sca_vec_f(m_harmonics_vector_float, anInput, outputs, m_number_of_harmonics);
	}
    
    inline void processAdd(const double anInput, double* outputs, const double angle)
	{
		float angle_value = Tools::radian_wrap(angle) / CICM_2PI;
		for(int i = 0; i < m_number_of_harmonics; i++)
            m_harmonics_vector_double[i] = m_encoder_matrix[i][(int)(angle_value*(double)(NUMBEROFCIRCLEPOINTS-1))];
		process(anInput, outputs);
	}
    
    inline void processAdd(const float anInput, float* outputs, const float angle)
	{
        double angle_value = Tools::radian_wrap(angle) / CICM_2PI;
		for(int i = 0; i < m_number_of_harmonics; i++)
            m_harmonics_vector_float[i] = m_encoder_matrix[i][(int)(angle_value*(double)(NUMBEROFCIRCLEPOINTS-1))];
		process(anInput, outputs);
	}
    
    /************************************************************************************/
    /******************************* Perform sample block *******************************/
    /************************************************************************************/
    
    /*********************************** Out Of Place ***********************************/
    
    inline void processAdd(double* inputs, double** outputs)
	{
		for(int i = 0; i < m_number_of_harmonics; i++)
			cicm_product_sum_vec_sca_vec_d(inputs, m_harmonics_vector_double[i], outputs[i], m_vector_size);
	}
    
    inline void processAdd(float* inputs, float** outputs)
	{
        for(int i = 0; i < m_number_of_harmonics; i++)
			cicm_product_sum_vec_sca_vec_f(inputs, m_harmonics_vector_float[i], outputs[i], m_vector_size);
	}
    
	inline void processAdd(const double* inputs, double** outputs, const double* angles)
	{
		int index;
		cicm_vector_double pointor;
        
		for(int i = 0; i < m_vector_size; i++)
        {
			m_index_vector[i] = Tools::radian_wrap(angles[i]) / CICM_2PI * (double)(NUMBEROFCIRCLEPOINTS-1);
        }
        
        cicm_add_vec_vec_d(inputs, outputs[0], m_vector_size);
		for(int i = 1; i < m_number_of_harmonics; i++)
		{
			pointor = m_encoder_matrix[i];
			for(int j = 0; j < m_vector_size; j++)
			{
				index = m_index_vector[j];
				m_vector_double[j] = pointor[index];
			}
			cicm_product_vec_d(inputs, m_vector_double, m_vector_size);
            cicm_add_vec_vec_d(m_vector_double, outputs[i], m_vector_size);
		}
	}
    
    inline void processAdd(const float* inputs, float** outputs, const float* angles)
	{
		int index;
		cicm_vector_double pointor;
        
		for(int i = 0; i < m_vector_size; i++)
        {
			m_index_vector[i] = Tools::radian_wrap(angles[i]) / CICM_2PI * (double)(NUMBEROFCIRCLEPOINTS-1);
        }
        
        cicm_add_vec_vec_f(inputs, outputs[0], m_vector_size);
		for(int i = 1; i < m_number_of_harmonics; i++)
		{
			pointor = m_encoder_matrix[i];
			for(int j = 0; j < m_vector_size; j++)
			{
				index = m_index_vector[j];
				m_vector_float[j] = pointor[index];
			}
			cicm_product_vec_f(inputs, m_vector_float, m_vector_size);
            cicm_add_vec_vec_f(m_vector_float, outputs[i], m_vector_size);
		}
	}    
};

#endif