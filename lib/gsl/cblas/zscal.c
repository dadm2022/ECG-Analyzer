#include <gsl/gsl_math.h>
#include <gsl/gsl_cblas.h>
#include "cblas.h"

void
cblas_zscal (const int N, const void *alpha, void *X, const int incX)
{
#define BASE double
#include "source_scal_c.h"
#undef BASE
}
