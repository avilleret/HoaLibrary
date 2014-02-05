/*
// Copyright (c) 2012-2013 Julien Colafrancesco, Pierre Guillot, Eliott Paris, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef DEF_HOA_3D_LEGENDRE
#define DEF_HOA_3D_LEGENDRE

namespace Hoa3D
{
	int Factorial(int x);
	int DoubleFactorial(int x);
    template <int l, int m> float Legendre(float x);
    float LegendrePolynomial(int l, int m, float x);    
}



#endif