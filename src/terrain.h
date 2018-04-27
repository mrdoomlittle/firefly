# ifndef __ffly__terrain__h
# define __ffly__terrain__h
# include "ffint.h"
# include "types.h"


typedef struct ffly_terrain {
		ff_uint_t x_stretch;
		ff_uint_t y_stretch;
		ff_uint_t z_stretch;

		ff_uint_t x, y, z;

		// cleanup
		struct ffly_terrain *prev, *next;
} *ffly_terrainp;

ffly_terrainp ffly_terrain_alloc();
void ffly_terrain_free(ffly_terrainp);
ffly_terrainp ffly_terrain_mapout(ffly_terrainp, ff_uint_t, ff_uint_t, ff_uint_t);
ffly_terrainp ffly_terrain_unmap(ffly_terrainp);
# endif /*__ffly__terrain__h*/
