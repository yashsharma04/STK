/** Visual studio workarounds in one place
 *  Note that Visual Studio 2013 does have the maths functions defined,
 *  so we define the work arounds only for compiler versions before 18.00
 */

#if defined(WIN32) && _MSC_VER < 1800
#  include <math.h>

#  define isnan _isnan
#  define roundf(x) (floorf(x + 0.5f))
#endif

