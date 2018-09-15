# ifndef __ffly__sr__tile__h
# define __ffly__sr__tile__h
# include "../ffint.h"
# define tilepx(__tile, __x, __y) \
	(((ff_u8_t*)(__tile)->map)+(((__x)+((__y)*(1<<(__tile)->sz)))*4))
enum {
	_sr_tile_8 = 3,
	_sr_tile_16,
	_sr_tile_32,
	_sr_tile_64
};

struct sr_tile {
	void *map;
	ff_u8_t sz;
	ff_u8_t bits;
};

void sr_tile_map(struct sr_tile*);
void sr_tile_unmap(struct sr_tile*);
struct sr_tile* sr_tile_new(ff_u8_t);
void sr_tile_destroy(struct sr_tile*);
# endif /*__ffly__sr__tile__h*/