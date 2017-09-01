# ifndef __camera__hpp
# define __camera__hpp
# include <eint_t.hpp>
# include <errno.h>
# include <cstdio>
# include "../graphics/draw_pixmap.hpp"
# include "../types/err_t.h"
# include "../uni_manager.hpp"
# include "../types/coords_t.hpp"
# include "../memory/alloc_pixmap.hpp"
# include "../memory/mem_free.h"
# include "../graphics/crop_pixmap.hpp"
# include "../graphics/fill_pixmap.hpp"
# include "../system/io.h"
namespace mdl {
namespace firefly {
namespace ui {
class camera {
	public:
	camera(uni_manager *__uni_manager) : _uni_manager(__uni_manager){}

	types::err_t init(uint_t __xaxis_len, uint_t __yaxis_len);
	types::err_t de_init();
	types::err_t handle();

	types::err_t draw_camera(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen);

	uint_t get_xaxis_len() {return this-> xaxis_len;}
	uint_t get_yaxis_len() {return this-> yaxis_len;}

	void xaxis_pull() {this->set_xaxis(this-> xaxis+1);}
	void xaxis_push() {this->set_xaxis(this-> xaxis-1);}

	void yaxis_pull() {this->set_yaxis(this-> yaxis+1);}
	void yaxis_push() {this->set_yaxis(this-> yaxis-1);}

	void zaxis_pull() {this->set_zaxis(this-> zaxis+1);}
	void zaxis_push() {this->set_zaxis(this-> zaxis-1);}

	void set_xaxis(uint_t __xaxis) {if (__xaxis < 0) return; this->xaxis = __xaxis;}
	void set_yaxis(uint_t __yaxis) {if (__yaxis < 0) return; this->yaxis = __yaxis;}
	void set_zaxis(uint_t __zaxis) {if (__zaxis < 0) return; this->zaxis = __zaxis;}

	uint_t get_xaxis() {return this->xaxis;}
	uint_t get_yaxis() {return this->yaxis;}
	uint_t get_zaxis() {return this->zaxis;}

	void set_coords(types::_3d_coords_t<> __coords) {
		this->set_xaxis(__coords.xaxis);
		this->set_yaxis(__coords.yaxis);
		this->set_zaxis(__coords.zaxis);
	}

	types::_3d_coords_t<> get_coords() {
		return (types::_3d_coords_t<>) {
			.xaxis = this->xaxis,
			.yaxis = this->yaxis,
			.zaxis = this->zaxis
		};
	}
	bool inited = false;
	private:
	types::_2d_pm_t _2d_pm;
	types::pixmap_t pixmap;
	uni_manager *_uni_manager;
	uint_t xaxis = 0, yaxis = 0, zaxis = 0;
	uint_t xaxis_len, yaxis_len;
};
}
}
}







# endif /*__camera__hpp*/
