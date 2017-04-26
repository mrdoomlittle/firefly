# ifndef __skel__creator__hpp
# define __skel__creator__hpp
# include "types/err_t.h"
# include "types/pixmap_t.h"
# include "memory/mem_alloc.h"
# include <eint_t.hpp>
# include "system/errno.h"
# include <errno.h>
# include "types/colour_t.hpp"
namespace mdl {
namespace firefly {
class skel_creator {
	types::err_t init();
	types::err_t de_init();
	types::err_t tick();
	types::err_t draw(types::_1d_pixmap_t __pixbuff);
	private:
	types::_1d_pixmap_t grid_pm = nullptr;
};
}
}

# endif /*__skel__creator__hpp*/
