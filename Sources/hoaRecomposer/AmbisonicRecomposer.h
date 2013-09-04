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

#ifndef DEF_AMBISONICRECOMPOSER
#define DEF_AMBISONICRECOMPOSER

#include "../HoaAmbisonics/Ambisonic.h"
#include "../hoaAmbisonics/Planewaves.h"
#include "../hoaEncoder/AmbisonicEncoder.h"
#include "../HoaWider/AmbisonicWider.h"

enum
{
    Hoa_Fixe  = 0,
    Hoa_Fisheye,
    Hoa_Free
};

class AmbisonicRecomposer : public Ambisonic//, public Planewaves
{
	
private:
    long                        m_number_of_loudspeakers;
    
    cicm_matrix_float           m_recomposer_matrix_float;
    cicm_matrix_double          m_recomposer_matrix_double;
    
	cicm_vector_float           m_harmonics_vector_float;
    cicm_vector_double          m_harmonics_vector_double;
    
    cicm_vector_float           m_microphones_vector_float;
    cicm_vector_double          m_microphones_vector_double;
    
    cicm_vector_float           m_angles_vector_float;
    cicm_vector_double          m_angles_vector_double;
    
    cicm_vector_float*          m_harmonics_matrix_float;
    cicm_vector_double*         m_harmonics_matrix_double;
    
	vector<AmbisonicEncoder*>  m_encoders;
    vector<CicmLine*>           m_lines;
    vector<AmbisonicWider*>    m_widers;
    vector<CicmLine*>           m_wider_lines;
    
    long                        m_mode;
    double                      m_fishEyeFactor;
    
    void computeMatrix(double aFishEyeFactor);
public:
	AmbisonicRecomposer(long anOrder = 1, long aNumberOfMicrophones = 4, long aMode = Hoa_Fixe, long aVectorSize = 0, long aSamplingRate = 44100);
    
    void setVectorSize(long aVectorSize);
    void setSamplingRate(long aSamplingRate);
    void setRampInSample(long aNumberOfSample);
    void setRampInMs(double aTimeInMs);
    void setMode(long aMode);
    void setFishEyeFactor(double aFishEyeFactor);
	void setMicrophoneAngle(long anIndex, double anAngle);
    void setMicrophoneWide(long anIndex, double aWidenValue);
    
    
    double getMicrophoneAngle(long anIndex);
    double getMicrophoneWide(long anIndex);
    double getFishEyeFactor();
    long getRampInSample();
    double getRampInMs();
    long getMode();
    std::string getMicrophonesName(long anIndex);
    
	~AmbisonicRecomposer();
	
    /************************************************************************************/
    /**************************************** FREE **************************************/
    /************************************************************************************/
    
	/************************************************************************************/
    /***************************** Perform sample by sample *****************************/
    /************************************************************************************/
    
    /*********************************** Out Of Place ***********************************/
    
	inline void processFree(const double* aInputs, double* aOutputs)
	{
        double angle    = m_lines[0]->process();
        double wide     = m_wider_lines[0]->process();
        m_encoders[0]->process(aInputs[0], aOutputs, angle);
        m_widers[0]->process(aOutputs, aOutputs, wide);
		for(int i = 1; i < m_number_of_loudspeakers; i++)
        {
            angle = m_lines[i]->process();
            wide  = m_wider_lines[i]->process();
            m_encoders[i]->process(aInputs[i], m_harmonics_vector_double, angle);
            m_widers[i]->process(m_harmonics_vector_double, wide);
            cicm_add_vec_vec_d(m_harmonics_vector_double, aOutputs, m_number_of_harmonics);
        }
	}
    
    inline void processFree(const float* aInputs, float* aOutputs)
	{
        float angle = m_lines[0]->process();
        float wide  = m_wider_lines[0]->process();
        m_encoders[0]->process(aInputs[0], aOutputs, angle);
        m_widers[0]->process(aOutputs, aOutputs, wide);
		for(int i = 1; i < m_number_of_loudspeakers; i++)
        {
            angle = m_lines[i]->process();
            wide  = m_wider_lines[i]->process();
            m_encoders[i]->process(aInputs[i], m_harmonics_vector_float, angle);
            m_widers[i]->process(m_harmonics_vector_float, m_harmonics_vector_float, wide);
            cicm_add_vec_vec_f(m_harmonics_vector_float, aOutputs, m_number_of_harmonics);
        }
	}
	
	/************************************************************************************/
    /******************************* Perform sample block *******************************/
    /************************************************************************************/
    
    /*********************************** Out Of Place ***********************************/
    
	inline void processFree(const double* const* aInputs, double** aOutputs)
	{
        m_lines[0]->process(m_angles_vector_double);
        m_encoders[0]->process(aInputs[0], aOutputs, m_angles_vector_double);
        m_wider_lines[0]->process(m_angles_vector_double);
        m_widers[0]->process(aOutputs, aOutputs, m_angles_vector_double);
        for(int i = 1; i < m_number_of_loudspeakers; i++)
        {
            m_lines[i]->process(m_angles_vector_double);
            m_encoders[i]->process(aInputs[i], m_harmonics_matrix_double, m_angles_vector_double);
            m_wider_lines[i]->process(m_angles_vector_double);
            m_widers[i]->process(m_harmonics_matrix_double, m_angles_vector_double);
            for(int k = 0; k < m_number_of_harmonics; k++)
            {
                cicm_add_vec_vec_d(m_harmonics_matrix_double[k], aOutputs[k], m_vector_size);
            }
        }
	}
    
    inline void processFree(const float* const* aInputs, float** aOutputs)
	{
        m_lines[0]->process(m_angles_vector_float);
        m_encoders[0]->process(aInputs[0], aOutputs, m_angles_vector_float);
        m_wider_lines[0]->process(m_angles_vector_float);
        m_widers[0]->process(aOutputs, aOutputs, m_angles_vector_float);
        for(int i = 1; i < m_number_of_loudspeakers; i++)
        {
            m_lines[i]->process(m_angles_vector_float);
            m_encoders[i]->process(aInputs[i], m_harmonics_matrix_float, m_angles_vector_float);
            m_wider_lines[i]->process(m_angles_vector_float);
            m_widers[i]->process(m_harmonics_matrix_float, m_harmonics_matrix_float, m_angles_vector_float);
            for(int k = 0; k < m_number_of_harmonics; k++)
            {
                cicm_add_vec_vec_f(m_harmonics_matrix_float[k], aOutputs[k], m_vector_size);
            }
        }
	}
    
    /*******************************************************/
    /************************ fisheye **********************/
    /*******************************************************/
    
    /* Perform sample by sample */
    inline void processFisheye(double* aInputs, double* aOutputs)
	{
        cicm_product_mat_vec_d(m_recomposer_matrix_double, aInputs, aOutputs, m_number_of_harmonics, m_number_of_loudspeakers);
	}
    
    inline void processFisheye(float* aInputs, float* aOutputs)
	{
        cicm_product_mat_vec_f(m_recomposer_matrix_float, aInputs, aOutputs, m_number_of_harmonics, m_number_of_loudspeakers);
	}
    
    inline void processFisheye(double* aInputs, double* aOutputs, double fisheyeFactor)
	{
        
        Tools::clip(fisheyeFactor, 0., 1.);
        double distanceBetwenTwoDefMics = CICM_2PI / m_number_of_loudspeakers;
        double angle = 0;
        m_encoders[0]->process(aInputs[0], aOutputs, angle);
        
        for(int i = 1; i < m_number_of_loudspeakers; i++)
        {
            angle = Tools::radianInterp(fisheyeFactor, 0., distanceBetwenTwoDefMics * (float)i);
            m_encoders[i]->process(aInputs[i], m_harmonics_vector_double, angle);
            cicm_add_vec_vec_d(m_harmonics_vector_double, aOutputs, m_number_of_harmonics);
        }
	}
    
    inline void processFisheye(float* aInputs, float* aOutputs, float fisheyeFactor)
	{
        Tools::clip(fisheyeFactor, 0.f, 1.f);
        float distanceBetwenTwoDefMics = CICM_2PI / m_number_of_loudspeakers;
        float angle = 0;
        m_encoders[0]->process(aInputs[0], aOutputs, angle);
        
        for(int i = 1; i < m_number_of_loudspeakers; i++)
        {
            angle = Tools::radianInterp(fisheyeFactor, 0., distanceBetwenTwoDefMics * (float)i);
            m_encoders[i]->process(aInputs[i], m_harmonics_vector_float, angle);
            cicm_add_vec_vec_f(m_harmonics_vector_float, aOutputs, m_number_of_harmonics);
        }
	}
    
    /* Perform sample block */
    inline void processFisheye(double** aInputs, double** aOutputs)
	{
        for(int i = 0; i < m_vector_size; i++)
		{
            for(int j = 0; j < m_number_of_loudspeakers; j++)
            {
                m_microphones_vector_double[j] = aInputs[j][i];
            }
            processFisheye(m_microphones_vector_double, m_harmonics_vector_double);
            for(int j = 0; j < m_number_of_harmonics; j++)
            {
                aOutputs[j][i] = m_harmonics_vector_double[j];
            }
		}
	}
    
    inline void processFisheye(float** aInputs, float** aOutputs)
	{
        for(int i = 0; i < m_vector_size; i++)
		{
            for(int j = 0; j < m_number_of_loudspeakers; j++)
            {
                m_microphones_vector_float[j] = aInputs[j][i];
            }
            processFisheye(m_microphones_vector_float, m_harmonics_vector_float);
            for(int j = 0; j < m_number_of_harmonics; j++)
            {
                aOutputs[j][i] = m_harmonics_vector_float[j];
            }
		}
	}
    
	inline void processFisheye(double** aInputs, double** aOutputs, double* fisheyeFactor)
	{
        Tools::clip(fisheyeFactor, m_vector_size, 0., 1.);
        double distanceBetwenTwoDefMics = CICM_2PI / m_number_of_loudspeakers;
        
        m_encoders[0]->setAngle(0.);
        m_encoders[0]->process(aInputs[0], aOutputs);
        
        for(int i = 1; i < m_number_of_loudspeakers; i++)
        {
            for (int j = 0; j < m_vector_size; j++)
            {
                m_angles_vector_double[j] = Tools::radianInterp(fisheyeFactor[j], 0., distanceBetwenTwoDefMics * (float)i);
            }
            m_encoders[i]->processAdd(aInputs[i], aOutputs, m_angles_vector_double);
        }
	}
    
    inline void processFisheye(float** aInputs, float** aOutputs, float* fisheyeFactor)
	{
        Tools::clip(fisheyeFactor, m_vector_size, 0.f, 1.f);
        double distanceBetwenTwoDefMics = CICM_2PI / m_number_of_loudspeakers;
        
        m_encoders[0]->setAngle(0.f);
        m_encoders[0]->process(aInputs[0], aOutputs);
        
        for(int i = 1; i < m_number_of_loudspeakers; i++)
        {
            for (int j = 0; j < m_vector_size; j++)
            {
                m_angles_vector_float[j] = Tools::radianInterp(fisheyeFactor[j], 0., distanceBetwenTwoDefMics * (float)i);
            }
            m_encoders[i]->processAdd(aInputs[i], aOutputs, m_angles_vector_float);
        }
    }
    
    
    /*******************************************************/
    /************************* fixe ************************/
    /*******************************************************/
    
    /* Perform sample by sample */
    inline void processFixe(double* aInputs, double* aOutputs)
	{
         cicm_product_mat_vec_d(m_recomposer_matrix_double, aInputs, aOutputs, m_number_of_harmonics, m_number_of_loudspeakers);
	}
    
    inline void processFixe(float* aInputs, float* aOutputs)
	{
        cicm_product_mat_vec_f(m_recomposer_matrix_float, aInputs, aOutputs, m_number_of_harmonics, m_number_of_loudspeakers);
	}
    
    /* Perform sample block */
	inline void processFixe(double** aInputs, double** aOutputs)
	{
        for(int i = 0; i < m_vector_size; i++)
		{
            for(int j = 0; j < m_number_of_loudspeakers; j++)
            {
                m_microphones_vector_double[j] = aInputs[j][i];
            }
            processFixe(m_microphones_vector_double, m_harmonics_vector_double);
            for(int j = 0; j < m_number_of_harmonics; j++)
            {
                aOutputs[j][i] = m_harmonics_vector_double[j];
            }
		}
	}
    
    inline void processFixe(float** aInputs, float** aOutputs)
	{
        for(int i = 0; i < m_vector_size; i++)
		{
            for(int j = 0; j < m_number_of_loudspeakers; j++)
            {
                m_microphones_vector_float[j] = aInputs[j][i];
            }
            processFixe(m_microphones_vector_float, m_harmonics_vector_float);
            for(int j = 0; j < m_number_of_harmonics; j++)
            {
                aOutputs[j][i] = m_harmonics_vector_float[j];
            }
		}
	}
};



#endif


