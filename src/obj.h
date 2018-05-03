# ifndef __ffly__obj__h
# define __ffly__obj__h
# include "types.h" 
# include "graphics/image.h"
# include "types/pixelmap_t.h"
# include "system/vec.h"
# include "script.h"
# include "types/no_t.h"
# include "uni.h"

# define ffly_set_objx(__obj, __x) \
	(__obj)->x = __x;	 
# define ffly_set_objy(__obj, __y) \
	(__obj)->y = __y;
# define ffly_set_objz(__obj, __z) \
	(__obj)->z = __z;

# define ffly_get_objx(__obj) \
	(__obj)->x
# define ffly_get_objy(__obj) \
	(__obj)->y
# define ffly_get_objz(__obj) \
	(__obj)->z

# define ffly_set_objxl(__obj, __xl) \
	(__obj)->xl = __xl;    
# define ffly_set_objyl(__obj, __yl) \
	(__obj)->yl = __yl;
# define ffly_set_objzl(__obj, __zl) \
	(__obj)->zl = __zl;
# define ffly_obj_vertex(__obj, __x, __y, __z) \
	ffly_vertex3(&(__obj)->shape, __x, __y, __z)

struct ffly_obj {
	ff_uint_t x, y, z;
	ffscriptp script;
  
	ff_uint_t phy_body;
	// debug
	ffly_no_t no;
	float angle;

	/*
	*	what lot is this object within.
	*/
	float light;

	// later - main events
	void(*create)(void*);
	void(*destroy)(void*);
	void(*free)(void*);
	void(*init)(void*);

	// cleanup
	struct ffly_obj *prev, *next;
};

typedef struct ffly_obj* ffly_objp;
typedef ffly_objp* ffly_objpp;
typedef ffly_objpp* ffly_objppp;

# ifdef __cplusplus
extern "C" {
# endif
void ffly_obj_rotate(ffly_objp, float);
ffly_objp ffly_obj_alloc(ff_err_t*);
ff_err_t ffly_obj_free(ffly_objp);
ff_err_t ffly_obj_handle(ffly_unip, ffly_objp);
ff_err_t ffly_obj_prepare(ffly_objp);
ff_err_t ffly_obj_cleanup();
# ifdef __cplusplus
}
namespace ff_ {
namespace firefly {
typedef ffly_objp objp;
typedef ffly_objpp objpp;
typedef ffly_objppp objppp;
/*
	x, y, z can only be set before being attached to the univirse
	for an object to be moved it must be thru ffly_uni_obj_move.
*/
struct obj {
	types::err_t prepare() {
		return ffly_obj_prepare(this->raw);
	}
	types::err_t alloc() {
		types::err_t err;
		this->raw = ffly_obj_alloc(&err);
		return err;
	}
	void vertex(ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
		ffly_obj_vertex(this->raw, __x, __y, __z);
	}
	void setx(uint_t __x){ffly_set_objx(this->raw, __x);}
	void sety(uint_t __y){ffly_set_objy(this->raw, __y);}
	void setz(uint_t __z){ffly_set_objz(this->raw, __z);}
	void setxl(uint_t __xl){ffly_set_objxl(this->raw, __xl);}
	void setyl(uint_t __yl){ffly_set_objyl(this->raw, __yl);}
	void setzl(uint_t __zl){ffly_set_objzl(this->raw, __zl);}
	void __inline__ set_size(uint_t __xl, uint_t __yl, uint_t __zl) {
		this->setxl(__xl);
		this->setyl(__yl);
		this->setzl(__zl);
	}
	void __inline__ set_coords(uint_t __x, uint_t __y, uint_t __z) {
		this->setx(__x);
		this->sety(__y);
		this->setz(__z);
	}
	types::err_t free() {
		return ffly_obj_free(this->raw);
	}
	types::err_t draw(types::byte_t *__frame, uint_t __x, uint_t __y, uint_t __z, uint_t __width, uint_t __height, uint_t __xmax, uint_t __ymax) {
		return ffly_obj_draw(this->raw, __frame, __x, __y, __z, __width, __height, __xmax, __ymax);
	}
	types::err_t handle() {
		return ffly_obj_handle(this->raw);
	}
	void set_texture(ff_byte_t *__texture) {
		this->raw->texture = __texture;
	}
	uint_t getx(){return ffly_get_objx(this->raw);}
	uint_t gety(){return ffly_get_objy(this->raw);}
	uint_t getz(){return ffly_get_objz(this->raw);}
	struct ffly_obj *raw;
};
}
}
# endif
# endif /*__ffly__obj__h*/
