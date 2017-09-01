# ifndef __uni__dlen__hpp
# define __uni__dlen__hpp
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace data {
enum uni_dlen : u8_t {
	UNI_DLEN_4,
	UNI_DLEN_8,
	UNI_DLEN_16,
	UNI_DLEN_32,
	UNI_DLEN_64,
	UNI_DLEN_128,
	UNI_DLEN_256,
	UNI_DLEN_512,
	UNI_DLEN_1024,
	UNI_DLEN_2048
};
}
}
}

# endif /*__uni__dlen__hpp*/
