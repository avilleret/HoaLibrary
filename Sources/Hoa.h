/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __DEF_HOA_LIBRARY__
#define __DEF_HOA_LIBRARY__

#ifdef HAVE_LIBBLAS
# include "cblas.h"
#else 
# include "../ThirdParty/CBlas/Header/cblas.h"
#endif /* HAVE_LIBBLAS */

namespace Hoa{};

#include "HoaDefs.h"
#include "HoaMath.h"
#include "HoaUtils.h"

using namespace Hoa;

#endif


