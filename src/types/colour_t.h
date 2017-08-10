# ifndef __colour__t__h
# define __colour__t__h

# if defined(USING_OPENCL) && defined(USE_CL_TYPES)
typedef struct {
	uchar r, g, b, a;
} colour_t;
# else
# include <stdint.h>
typedef struct {
	uint8_t r, g, b, a;
} colour_t;
# endif

# endif /*__colour__t__h*/
