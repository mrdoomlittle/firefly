# ifndef __uni__manager__hpp
# define __uni__manager__hpp
# include "types/pixmap_t.h"
# include "types/uni_par_t.h"
# include <mdlint.h>
# include "ffly_config.hpp"
# include "chunk_manager.h"
# include "chunk_data_t.h"
# include "chunk_info_t.h"
# include "system/errno.h"
# include <math.h>
# include <cstdio>
# include "types/err_t.h"
# include "types/id_t.h"
# include "types/coords_t.hpp"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "graphics/draw_pixmap.hpp"
# include "types/uni_dlen_t.hpp"
# include "data/uni_dlen_val.hpp"
# include "ffly_graphics.hpp"
namespace mdl {
namespace firefly {
class uni_manager {
	public:
	uni_manager(types::uni_dlen_t __xaxis_len, types::uni_dlen_t __yaxis_len, types::uni_dlen_t __zaxis_len)
	: xaxis_len(data::uni_dlen_val(__xaxis_len)), yaxis_len(data::uni_dlen_val(__yaxis_len)), zaxis_len(data::uni_dlen_val(__zaxis_len)) {}

	types::err_t init(uint_t __xa_split, uint_t __ya_split, uint_t __za_split);
	void get_cnk_coords(uint_t __xa, uint_t __ya, uint_t __za, uint_t& __cnk_xa, uint_t& __cnk_ya, uint_t& __cnk_za);
	void __inline__ get_cnk_coords(types::_3d_coords_t<> __coords, uint_t& __cnk_xa, uint_t& __cnk_ya, uint_t& __cnk_za) {
		this->get_cnk_coords(__coords.xaxis, __coords.yaxis, __coords.zaxis, __cnk_xa, __cnk_ya, __cnk_za);}

	types::err_t de_init() {
		if (this->_chunk_manager != nullptr)
			this->_chunk_manager->de_init();

		if (this->cnk_id_indx != nullptr)
			memory::mem_free(this->cnk_id_indx);
		if (this->_1d_uni_pm != nullptr)
			memory::mem_free(this->_1d_uni_pm);
		if (this->uni_particles != nullptr)
			memory::mem_free(this->uni_particles);
		return FFLY_SUCCESS;
	}

	types::_1d_pm_t _1d_cnk_pm(uint_t __xcnk, uint_t __ycnk, uint_t __zcnk) {
		return this->_1d_uni_pm[__xcnk + (__ycnk * this->cnk_xc) + (__zcnk * this->cnk_xc * this->cnk_yc)];}
	types::_3d_pm_t _3d_cnk_pm(uint_t __xcnk, uint_t __ycnk, uint_t __zcnk) {
		return this->_3d_uni_pm[__xcnk + (__ycnk * this->cnk_xc) + (__zcnk * this->cnk_xc * this->cnk_yc)];}

	types::_1d_pm_t get_1d_cnk_pm(uint_t __xa, uint_t __ya, uint_t __za);
	types::_3d_pm_t get_3d_cnk_pm(uint_t __xa, uint_t __ya, uint_t __za);

	types::_1d_pm_t __inline__ _1d_cnk_pm(types::_3d_coords_t<> __coords) {
		return this->get_1d_cnk_pm(__coords.xaxis, __coords.yaxis, __coords.zaxis);}
	types::_3d_pm_t __inline__ _3d_cnk_pm(types::_3d_coords_t<> __coords) {
		return this->get_3d_cnk_pm(__coords.xaxis, __coords.yaxis, __coords.zaxis);}

	types::uni_par_t* cnk_particles(uint_t __xcnk, uint_t __ycnk, uint_t __zcnk) {
		return this->uni_particles[__xcnk + (__ycnk * this->cnk_xc) + (__zcnk * this->cnk_xc * this->cnk_yc)];}

	types::uni_par_t* cnk_particles(types::_3d_coords_t<> __coords) {
		uint_t xcnk, ycnk, zcnk;
		this->get_cnk_coords(__coords, xcnk, ycnk, zcnk);
		return this->cnk_particles(xcnk, ycnk, zcnk);
	}

	void set_par_colour(uint_t __xa, uint_t __ya, uint_t __za, u8_t __r, u8_t __g, u8_t __b, u8_t __a);
	void get_par_colour(uint_t __xa, uint_t __ya, uint_t __za, u8_t& __r, u8_t& __g, u8_t& __b, u8_t& __a);

	uint_t get_cnk_xlen() {return this->_chunk_manager->get_cnk_xlen();}
	uint_t get_cnk_ylen() {return this->_chunk_manager->get_cnk_ylen();}
	uint_t get_cnk_zlen() {return this->_chunk_manager->get_cnk_zlen();}

	types::err_t draw_cnk(uint_t __xfs, uint_t __yfs, uint_t __zfs, types::id_t __chunk_id, types::pixmap_t __pixbuff, uint_t __xaxis_len, uint_t __yaxis_len);
	chunk_manager *_chunk_manager = nullptr;
	private:
	uint_t cnk_xc, cnk_yc, cnk_zc;
	uint_t const xaxis_len, yaxis_len, zaxis_len;
	uint_t **cnk_id_indx = nullptr;
	types::_1d_pm_t *_1d_uni_pm = nullptr;
	types::_3d_pm_t *_3d_uni_pm = nullptr;

	types::uni_par_t **uni_particles = nullptr;
};
}
}

# endif /*__uni__manager__hpp*/
