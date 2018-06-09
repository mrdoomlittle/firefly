# ifndef __ffly__terrain__h
# define __ffly__terrain__h
# include "ffint.h"
# include "types.h"
# include "physics/body.h"
typedef struct ffly_terrain {
		ff_uint_t phy_body;
		ff_uint_t x, y, z;
		ff_uint_t x_stretch;
		ff_uint_t y_stretch;
		ff_uint_t z_stretch;

		// cleanup
		struct ffly_terrain **bk, *next;
		void **nodes;
} *ffly_terrainp;

ffly_terrainp ffly_terrain_alloc(ff_uint_t);
void ffly_terrain_free(ffly_terrainp);
ffly_terrainp ffly_terrain_mapout(ffly_terrainp, ff_uint_t, ff_uint_t, ff_uint_t);
ffly_terrainp ffly_terrain_unmap(ffly_terrainp);
void ffly_terrain_cleanup(void);
# endif /*__ffly__terrain__h*/
