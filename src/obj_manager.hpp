# ifndef __obj__manager__hpp
# define __obj__manager__hpp
# include <eint_t.hpp>
# include "types/pixmap_t.h"
# include "types/obj_info_t.hpp"
# include <boost/numeric/ublas/vector.hpp>
# include <utility>
# include <math.h>
# include "maths/cal_dist.hpp"
# include <string.h>
# include "system/stop_watch.hpp"
# include <boost/thread.hpp>
# include "graphics/draw_pixmap.hpp"
# include "graphics/fill_pixmap.hpp"
# include "types/atom_t.hpp"
# include "types/coords_t.hpp"
# include <initializer_list>
# include "gravy_manager.hpp"
# include "types/coords_t.hpp"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "types/pixmap_t.h"
# include <set>
# include <errno.h>
# include <queue>
# include <boost/array.hpp>
# include "types/shape_info_t.hpp"
# include "flip_dir.hpp"
# include <mutex>
// how many obj's will be handled in 1 thread
# define BLOCK_SIZE 12

// edge, face, edge,
// face        face
// edge, face, edge,
namespace ublas = boost::numeric::ublas;
namespace mdl {
namespace firefly {
class gravy_manager;
class obj_manager {
	public:
	obj_manager(types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, uint_t __pb_zlen)
	: pixbuff(__pixbuff), pb_xlen(__pb_xlen), pb_ylen(__pb_ylen), pb_zlen(__pb_zlen) { this-> active_threads = 0; }

	~obj_manager();

	boost::int8_t de_init();

	types::pixmap_t get_pixmap(uint_t __obj_id) {
		return this-> obj_index[__obj_id].second;
	}

	uint_t add(types::coords_t<> __coords, types::shape_info_t __shape_info, uint_t __xaxis_len, uint_t __yaxis_len, uint_t __zaxis_len, boost::int8_t& __any_error, types::pixmap_t __pixmap = nullptr);
	void del(uint_t __obj_id);
	//uint_t add(uint_t __xlen, uint_t __ylen, uint_t __zlen, uint_t __xaxis, uint_t __yaxis, uint_t __zaxis); // return obj_id
/*
	void move_forwards(uint_t __obj_id, uint_t __amount) {
		this-> set_zaxis(this-> get_zaxis() + __amount);
	}

	void move_backwards(uint_t __obj_id, uint_t __amount) {
		this-> set_zaxis(this-> get_zaxis() - __amount);
	}
*/
	void add_to_gravy_pool(uint_t __obj_id, uint_t __other_id);

	types::coords_t<> get_coords(uint_t __obj_id) {
		types::coords_t<> coords = {
			.xaxis = this-> obj_index[__obj_id].first-> coords.xaxis,
			.yaxis = this-> obj_index[__obj_id].first-> coords.yaxis,
			.zaxis = this-> obj_index[__obj_id].first-> coords.zaxis
		};
		return coords;
	}


	void set_mass(uint_t __obj_id, uint_t __mass) {
		this-> obj_index[__obj_id].first-> mass = __mass;
	}

	uint_t get_mass(uint_t __obj_id) {
		return this-> obj_index[__obj_id].first-> mass;
	}

	uint_t& mass(uint_t __obj_id) {
		return this-> obj_index[__obj_id].first-> mass;
	}

	std::atomic<boost::int8_t>& xaxis_direction(uint_t __obj_id) {
		return this-> obj_index[__obj_id].first-> velocity.direction.xaxis;
	}

	std::atomic<boost::int8_t>& yaxis_direction(uint_t __obj_id) {
		return this-> obj_index[__obj_id].first-> velocity.direction.yaxis;
	}

	void add_xaxis_velos(uint_t __obj_id, uint_t __amount) {
		this-> set_xaxis_velos(__obj_id, this-> get_xaxis_velos(__obj_id) + __amount);
	}

	void rm_xaxis_velos(uint_t __obj_id, uint_t __amount) {
		this-> set_xaxis_velos(__obj_id, ((int_t)this-> get_xaxis_velos(__obj_id) - (int_t)__amount) < 0? 0 : (this-> get_xaxis_velos(__obj_id) - __amount));
	}

	void add_yaxis_velos(uint_t __obj_id, uint_t __amount) {
		this-> set_yaxis_velos(__obj_id, this-> get_yaxis_velos(__obj_id) + __amount);
	}

	void rm_yaxis_velos(uint_t __obj_id, uint_t __amount) {
		this-> set_yaxis_velos(__obj_id, ((int_t)this-> get_yaxis_velos(__obj_id) - (int_t)__amount) < 0? 0 : (this-> get_yaxis_velos(__obj_id) - __amount));
	}

	void set_xaxis_velos(uint_t __obj_id, uint_t __velos) {
		this-> obj_index[__obj_id].first-> velocity.xaxis = __velos;
	}

	uint_t get_xaxis_velos(uint_t __obj_id) {
		return this-> obj_index[__obj_id].first-> velocity.xaxis;
	}

	void set_yaxis_velos(uint_t __obj_id, uint_t __velos) {
		this-> obj_index[__obj_id].first-> velocity.yaxis = __velos;
	}

	uint_t get_yaxis_velos(uint_t __obj_id) {
		return this-> obj_index[__obj_id].first-> velocity.yaxis;
	}

	void push_xaxis(uint_t __obj_id, uint_t __amount) {
		this-> set_xaxis(__obj_id, this-> get_xaxis(__obj_id) + __amount);
	}

	void pull_xaxis(uint_t __obj_id, uint_t __amount) {
		this-> set_xaxis(__obj_id, this-> get_xaxis(__obj_id) - __amount);
	}

	void push_yaxis(uint_t __obj_id, uint_t __amount) {
		this-> set_yaxis(__obj_id, this-> get_yaxis(__obj_id) + __amount);
	}

	void pull_yaxis(uint_t __obj_id, uint_t __amount) {
		this-> set_yaxis(__obj_id, this-> get_yaxis(__obj_id) - __amount);
	}

	uint_t get_xaxis(uint_t __obj_id) {
		return this-> obj_index[__obj_id].first-> coords.xaxis;
	}

	uint_t get_yaxis(uint_t __obj_id) {
		return this-> obj_index[__obj_id].first-> coords.yaxis;
	}

	void enable_gravity(std::initializer_list<uint_t> __obj_id) {
		for (std::initializer_list<uint_t>::iterator itor = __obj_id.begin(); itor != __obj_id.end(); ++itor)
			this-> enable_gravity(*itor);
	}

	void enable_gravity(uint_t __obj_id) {
		this-> obj_index[__obj_id].first-> gravity_enabled = true;
	}

	void disable_gravity(std::initializer_list<uint_t> __obj_id) {
		for (std::initializer_list<uint_t>::iterator itor = __obj_id.begin(); itor != __obj_id.end(); ++itor)
			this-> disable_gravity(*itor);
	}

	void disable_gravity(uint_t __obj_id) {
		this-> obj_index[__obj_id].first-> gravity_enabled = false;
	}

	void set_xaxis(uint_t __obj_id, uint_t __xaxis) {
		if (this-> obj_index[__obj_id].first-> bound_enabled) {
			if (__xaxis < this-> obj_index[__obj_id].first-> xaxis_bound[0] || (__xaxis + this-> obj_index[__obj_id].first-> xaxis_len) >= obj_index[__obj_id].first-> xaxis_bound[1]) return;
		}

//		this-> collision_handler(__obj_id);

		this-> obj_index[__obj_id].first-> coords.xaxis = __xaxis;
	}

	void enable_bound(std::initializer_list<uint_t> __obj_id) {
		for (std::initializer_list<uint_t>::iterator itor = __obj_id.begin(); itor != __obj_id.end(); ++itor)
			this-> enable_bound(*itor);
	}

	void enable_bound(uint_t __obj_id) {
		this-> obj_index[__obj_id].first-> bound_enabled = true;
	}

	void disable_bound(std::initializer_list<uint_t> __obj_id) {
		for (std::initializer_list<uint_t>::iterator itor = __obj_id.begin(); itor != __obj_id.end(); ++itor)
			this-> disable_bound(*itor);
	}

	void disable_bound(uint_t __obj_id) {
		this-> obj_index[__obj_id].first-> bound_enabled = false;
	}

	void set_yaxis(uint_t __obj_id, uint_t __yaxis) {
		if (this-> obj_index[__obj_id].first-> bound_enabled) {
			if (__yaxis < this-> obj_index[__obj_id].first-> yaxis_bound[0] || (__yaxis + this-> obj_index[__obj_id].first-> yaxis_len) >= obj_index[__obj_id].first-> yaxis_bound[1]) return;
		}

//		this-> collision_handler(__obj_id);

		this-> obj_index[__obj_id].first-> coords.yaxis = __yaxis;
	}

	boost::int8_t set_xaxis_bound(std::initializer_list<uint_t> __obj_id, std::initializer_list<uint_t> __xaxis_min, std::initializer_list<uint_t> __xaxis_max) {
		uint_t obj_id_c = __obj_id.size();
		if (obj_id_c != __xaxis_min.size() || obj_id_c != __xaxis_max.size()) return FFLY_FAILURE;

		for (std::size_t o = 0; o != obj_id_c; o ++)
			this-> set_xaxis_bound(*(__obj_id.begin() + o), *(__xaxis_min.begin() + o), *(__xaxis_max.begin() + o));
		return FFLY_SUCCESS;
	}

	void set_xaxis_bound(std::initializer_list<uint_t> __obj_id, uint_t __xaxis_min, uint_t __xaxis_max) {
		for (std::initializer_list<uint_t>::iterator itor = __obj_id.begin(); itor != __obj_id.end(); ++itor)
			set_xaxis_bound(*itor, __xaxis_min, __xaxis_max);
	}

	void set_xaxis_bound(uint_t __obj_id, uint_t __xaxis_min, uint_t __xaxis_max) {
		this-> obj_index[__obj_id].first-> xaxis_bound[0] = __xaxis_min;
		this-> obj_index[__obj_id].first-> xaxis_bound[1] = __xaxis_max;
	}

	boost::int8_t set_yaxis_bound(std::initializer_list<uint_t> __obj_id, std::initializer_list<uint_t> __yaxis_min, std::initializer_list<uint_t> __yaxis_max) {
		uint_t obj_id_c = __obj_id.size();
		if (obj_id_c != __yaxis_min.size() || obj_id_c != __yaxis_max.size()) return FFLY_FAILURE;

		for (std::size_t o = 0; o != obj_id_c; o ++)
			this-> set_yaxis_bound(*(__obj_id.begin() + o), *(__yaxis_min.begin() + o), *(__yaxis_max.begin() + o));
		return FFLY_SUCCESS;
	}

	void set_yaxis_bound(std::initializer_list<uint_t> __obj_id, uint_t __yaxis_min, uint_t __yaxis_max) {
		for (std::initializer_list<uint_t>::iterator itor = __obj_id.begin(); itor != __obj_id.end(); ++itor)
			this-> set_yaxis_bound(*itor, __yaxis_min, __yaxis_max);
	}

	void set_yaxis_bound(uint_t __obj_id, uint_t __yaxis_min, uint_t __yaxis_max) {
		this-> obj_index[__obj_id].first-> yaxis_bound[0] = __yaxis_min;
		this-> obj_index[__obj_id].first-> yaxis_bound[1] = __yaxis_max;
	}

	void set_zaxis_bound(uint_t __obj_id, uint_t __zaxis_min, uint_t __zaxis_max) {
		this-> obj_index[__obj_id].first-> zaxis_bound[0] = __zaxis_min;
		this-> obj_index[__obj_id].first-> zaxis_bound[1] = __zaxis_min;
	}

	boost::int8_t manage();

	typedef struct {
		bool thread_state;
		uint_t amount, offset, id;
		boost::thread::native_handle_type native_handle;
	} thr_config_t;

	void handle_objs(thr_config_t *thread_config);
	boost::int8_t draw_objs();

	typedef struct {
		boost::uint8_t command;
		uint_t obj_id, block_id;
		uint_t add_to_pool;
	} gravy_queue_t;

	private:
	bool to_shutdown = false, th_wait = false;
	std::atomic<uint_t> active_threads;
	bool th_waiting = false;
	uint_t th_id_to_wait = 0;

	ublas::vector<thr_config_t *> thread_index;
	types::pixmap_t pixbuff = nullptr;
	uint_t const pb_xlen, pb_ylen, pb_zlen;
	uint_t obj_count = 0, thr_offset = 0;
	std::set<uint_t> unused_ids;
	std::queue<gravy_queue_t> gravy_queue;
	ublas::vector<std::pair<types::obj_info_t *, types::pixmap_t> > obj_index;
};
}
}

# endif /*__obj__manager__hpp*/
