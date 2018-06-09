# include "terrain.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"

ffly_terrainp static top = NULL;
struct node {
	ff_uint_t x, y, z;
};

ffly_terrainp ffly_terrain_alloc(ff_uint_t __nodes) {
	ffly_terrainp t = (ffly_terrainp)__ffly_mem_alloc(sizeof(struct ffly_terrain));
	if (top != NULL)
		top->bk = &t->next;
	t->next = top;
	t->bk = &top;
	top = t;

	*(t->nodes = (void**)__ffly_mem_alloc((__nodes+1)*sizeof(void*))) = NULL;
	t->nodes++;
	return t;
}


void ffly_terrain_free(ffly_terrainp __ter) {
	while(*(--__ter->nodes) != NULL);
	__ffly_mem_free(__ter->nodes);
	__ffly_mem_free(__ter);
}

void put_node(ffly_terrainp __ter, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	struct node *p = (struct node*)__ffly_mem_alloc(sizeof(struct node));
	*p = (struct node){
		.x = __x, 
		.y = __y,
		.z = __z
	};

	*(__ter->nodes++) = (void*)p;
}

ffly_terrainp
ffly_terrain_create(ff_uint_t __nodes, ff_uint_t __x_stretch, ff_uint_t __y_stretch, ff_uint_t __z_stretch) {
	ffly_terrainp ret = ffly_terrain_alloc(__nodes);
	ret->x_stretch = __x_stretch;
	ret->y_stretch = __y_stretch;
	ret->z_stretch = __z_stretch;
	return ret;
}

ff_err_t ffly_terrain_destroy(ffly_terrainp __terrain) {

}

/*
	mapout terrain
*/
ffly_terrainp ffly_terrain_mapout(ffly_terrainp __ter, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	ff_uint_t x_stretch = __ter->x_stretch;
	ff_uint_t y_stretch = __ter->y_stretch;
	ff_uint_t z_stretch = __ter->z_stretch;
	put_node(__ter, __x, __y, __z);
	put_node(__ter, __x+x_stretch, __y, __z);
	put_node(__ter, __x+x_stretch, __y+y_stretch, __z);
	put_node(__ter, __x, __y+y_stretch, __z);

	put_node(__ter, __x, __y, __z+z_stretch);
	put_node(__ter, __x+x_stretch, __y, __z+z_stretch);
	put_node(__ter, __x+x_stretch, __y+y_stretch, __z+z_stretch);
	put_node(__ter, __x, __y+y_stretch, __z+z_stretch);
	__ter->x = __x;
	__ter->y = __y;
	__ter->z = __z;

}

ffly_terrainp ffly_terrain_unmap(ffly_terrainp __terrain) {

}

ffly_terrainp ffly_terrain_remap(ffly_terrainp __terrain) {

}

void ffly_terrain_cleanup(void) {
	ffly_terrainp cur = top, bk = NULL;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ffly_terrain_free(bk);
	}
}
