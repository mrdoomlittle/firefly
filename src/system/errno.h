# ifndef __ffly__errno__h
# define __ffly__errno__h
# ifdef __cplusplus
#	include <eint_t.hpp>
# else
#	include <eint_t.h>
# endif
# ifndef FFLY_SUCCESS
#	define FFLY_SUCCESS 0
# endif

# ifndef FFLY_FAILURE
#	define FFLY_FAILURE -1
# endif

# ifndef FFLY_NOP
#	define FFLY_NOP 1
# endif

# ifndef FF_ERR_CFTCM
#	define FF_ERR_CFTCM 0 /*cuda failed to copy memory*/
# endif
# ifndef FF_ERR_CFTAM
#	define FF_ERR_CFTAM 1 /*cuda failed to alloc memory*/
# endif
# ifndef FF_ERR_CFTCK
#	define FF_ERR_CFTCK 2 /*cuda failed to call kernel*/
# endif
# ifndef FF_ERR_CFTFM
#	define FF_ERR_CFTFM 3 /*cuda failed to free memory*/
# endif
# ifndef FF_ERR_CFTSD
#	define FF_ERR_CFTSD 4 /*cuda failed to sync device*/
# endif
# ifdef __cplusplus
extern "C" {mdl::u8_t extern ffly_errno;}
# else
mdl_u8_t extern ffly_errno;
# endif
# endif /*__ffly__errno__h*/
