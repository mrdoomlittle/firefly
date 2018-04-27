# include "terrain.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"

ffly_terrainp static top = NULL;
ffly_terrainp static end = NULL;

struct node {
	ff_uint_t x, y, z;
};

ffly_terrainp ffly_terrain_alloc() {
	ffly_terrainp p = (ffly_terrainp)__ffly_mem_alloc(sizeof(struct ffly_terrain));
	if (!top)
		top = p;

	p->prev = end;
	p->next = NULL;
	if (end != NULL)
		end->next = p;
	end = p;
	return p;
}


void ffly_terrain_free(ffly_terrainp __terrain) {

}

void put_node(ffly_terrainp __terrain, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {

}

ffly_terrainp
ffly_terrain_create(ff_uint_t __x_stretch, ff_uint_t __y_stretch, ff_uint_t __z_stretch) {
	ffly_terrainp ret = ffly_terrain_alloc();

	return ret;
}

ff_err_t ffly_terrain_destroy(ffly_terrainp __terrain) {

}

ffly_terrainp ffly_terrain_mapout(ffly_terrainp __terrain, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	ff_uint_t x_stretch = __terrain->x_stretch;
	ff_uint_t y_stretch = __terrain->y_stretch;
	ff_uint_t z_stretch = __terrain->z_stretch;
	put_node(__terrain, __x, __y, __z);
	put_node(__terrain, __x+x_stretch, __y, __z);
	put_node(__terrain, __x+x_stretch, __y+y_stretch, __z);
	put_node(__terrain, __x, __y+y_stretch, __z);

	put_node(__terrain, __x, __y, __z+z_stretch);
	put_node(__terrain, __x+x_stretch, __y, __z+z_stretch);
	put_node(__terrain, __x+x_stretch, __y+y_stretch, __z+z_stretch);
	put_node(__terrain, __x, __y+y_stretch, __z+z_stretch);
	__terrain->x = __x;
	__terrain->y = __y;
	__terrain->z = __z;
}

ffly_terrainp ffly_terrain_unmap(ffly_terrainp __terrain) {

}

ffly_terrainp ffly_terrain_remap(ffly_terrainp __terrain) {

}

ffly_terrainp ffly_terrain_cleanup() {
	ffly_terrainp cur = top, bk = NULL;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ffly_terrain_free(bk);
		__ffly_mem_free(bk);
	}
}
