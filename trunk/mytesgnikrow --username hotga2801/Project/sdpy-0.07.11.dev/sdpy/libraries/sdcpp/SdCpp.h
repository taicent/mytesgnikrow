#ifndef __SD_SDCPP_H
#define __SD_SDCPP_H


#if defined(__cplusplus)

// Define macro HAVE_MACSTL if you want to use macstl's valarray (faster)
// Otherwise i'll use stl's valarray

#ifdef HAVE_MACSTL
#include <macstl/valarray.h>
#define VALARRAY stdext::valarray
#else
#include <valarray>
#define VALARRAY std::valarray
#endif


#endif


#endif
