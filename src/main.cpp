# include <cstdio>
# include "chunk_keeper.hpp"
# include "uni_manager.hpp"
# include "chunk_manager.hpp"
int main() {
	mdl::firefly::chunk_manager chunk_manager(12, 12, 12);

	mdl::uint_t *chunk_id = nullptr;
	chunk_manager.add_chunk(chunk_id, 0, 0, 0);
	printf("chunk_id: %d\n", *chunk_id);
	chunk_id = nullptr;
	chunk_manager.add_chunk(chunk_id, 12, 0, 0);
    printf("chunk_id: %d\n", *chunk_id);


	printf("%d\n", *chunk_manager.coords_to_id(11,0,0));
}

