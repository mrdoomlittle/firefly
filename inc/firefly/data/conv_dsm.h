# ifndef __conv__dsm__h
# define __conv__dsm__h

# ifdef __cplusplus
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace data {
float conv_dsm(float __val, u8_t __conv_typ);
template<typename _T>
__inline__ _T conv_dsm(uint_t __val, u8_t __conv_typ) {
	return (_T)conv_dsm((float)__val, __conv_typ);
}

enum : u8_t {
	NONE,
	BYTE_TO_BIT,
	BIT_TO_BYTE,
	BIT_TO_KILOBIT,
	KILOBIT_TO_BIT,
	BIT_TO_KILOBYTE,
	KILOBYTE_TO_BIT,
	BYTE_TO_KILOBYTE,
	KILOBYTE_TO_BYTE
};
}
}
}
# endif



# endif /*__conv__dsm__h*/
