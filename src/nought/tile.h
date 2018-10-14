# ifndef __ffly__nt__tile__h
# define __ffly__nt__tile__h
# include "../ffint.h"
# define tilepx(__tile, __x, __y) \
	(((ff_u8_t*)(__tile)->map)+(((__x)+((__y)*(1<<(__tile)->sz)))*4))
enum {
	_nt_tile_8 = 3,
	_nt_tile_16,
	_nt_tile_32,
	_nt_tile_64
};

// phantom tile
/*
	tile is stored on the users side of things

	tile memory is allocated by user, size, bits, etc
*/
struct nt_ptile {
	void(*get)(ff_u8_t, long long, void*);
	void *tile;
};

struct nt_tile {
	void *map;
	ff_u8_t sz;
	ff_u8_t bits;
};

void nt_ptile_new(void);
void nt_ptile_destroy(void);
void nt_tile_map(struct nt_tile*);
void nt_tile_unmap(struct nt_tile*);
struct nt_tile* nt_tile_new(ff_u8_t);
void nt_tile_destroy(struct nt_tile*);
# endif /*__ffly__nt__tile__h*/
