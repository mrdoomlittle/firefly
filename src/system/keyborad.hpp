# ifndef __keyborad__hpp
# define __keyborad__hpp
# include <mdlint.h>
namespace mdl {
namespace firefly {
namespace system {
enum key_id : uint_t {
	KEY_W_ID,
	KEY_S_ID,
	KEY_A_ID,
	KEY_D_ID
};
typedef enum key_id key_id_t;
}
}
}

# endif /*__keyborad__hpp*/
