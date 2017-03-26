# include <cstdio>
# include "chunk_keeper.hpp"
# include "uni_manager.hpp"
# include "chunk_manager.hpp"
# include "types/coords_t.hpp"
int main() {
	mdl::firefly::chunk_handler chunk_handler;

/*
	mdl::firefly::uni_manager uni_manager(12,12,12);
	uni_manager.init(2,2,2);
	mdl::firefly::types::coords_t<> coords = {8, 8, 0};

	uni_manager.chunk_pixmap(coords);
*/
/*
	mdl::firefly::chunk_manager chunk_manager(12, 12, 12);

	mdl::uint_t *chunk_id = nullptr;
	chunk_manager.add_chunk(chunk_id, 0, 0, 0);
	printf("chunk_id: %d\n", *chunk_id);
	chunk_id = nullptr;
	chunk_manager.add_chunk(chunk_id, 12, 0, 0);
    printf("chunk_id: %d\n", *chunk_id);


	printf("%d\n", *chunk_manager.coords_to_id(11,0,0));
*/
}

