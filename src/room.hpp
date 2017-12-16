# ifndef __ffly__room__hpp__
# define __ffly__room__hpp__
# include <mdlint.h>
# include "types/pixelmap_t.h"
# include "types/err_t.h"
namespace mdl {
namespace firefly {
class room {
	public:
	room(){}
	~room(){}

	types::err_t init(u16_t __width, u16_t __height);
	types::err_t draw(types::pixelmap_t __pixelbuff, u16_t __width, u16_t __height);
	private:
	types::pixelmap_t pixelmap;
};
}
}
# endif /*__ffly__room__hpp__*/
