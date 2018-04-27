# include "terrain.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
ffly_terrainp static top = NULL;
ffly_terrainp static end = NULL;

/*
ffly_terrainp ffly_terrain_alloc() {
	ffly_terrainp p = (ffly_terrainp)__ffly_mem_alloc(sizeof(struct ffly_terrain));
	if (!top)
		top = p;

	p->prev = end;
	if (end != NULL)
		end->next = p;
	end = p;
}


void ffly_terrain_free(ffly_terrainp) {

}


ffly_terrainp
ffly_terrain_create(ff_uint_t __x_stretch, ff_uint_t __y_stretch, ff_uint_t __z_stretch) {


}

ff_err_t ffly_terrain_destroy(ffly_terrainp __terrain) {

}

ffly_terrainp ffly_terrain_mapout(ffly_terrainp __terrain, ff_uint_t, ff_uint_t, ff_uint_t) {

}

ffly_terrainp ffly_terrain_unmap(ffly_terrainp __terrain) {

}

ffly_terrainp ffly_terrain_cleanup() {

}*/
