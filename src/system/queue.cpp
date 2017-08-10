# include "queue.hpp"
# include <cstdio>
# include "../memory/mem_free.h"
int main() {
	mdl::firefly::system::queue<mdl::uint_t> queue;
	mdl::firefly::types::err_t any_err;

	mdl::firefly::system::queue<mdl::uint_t>::itorator itor;

	queue.push(0);
	queue.push(1);
	queue.push(2);
	queue.push(3);

	for (mdl::uint_t x{}; x != 4; x ++) {
		printf("<----->\n");
//		if (x == 0){queue.pop(any_err);queue.push(21);queue.push(21);}
		queue.pop(any_err);


		for (itor = queue.begin(); itor != queue.end(); itor++) {
			printf("%d\n", *itor);
		}
	}

	mdl_u8_t *o = (mdl_u8_t*)mdl::firefly::memory::mem_alloc(100);


	o = (mdl_u8_t*)mdl::firefly::memory::mem_realloc(o, 200);

	mdl::firefly::memory::mem_free(o);

	printf("mem track: used: %d - freed: %d\n", ffly_mem_alloc_bc, ffly_mem_free_bc);
/*
	for (mdl::uint_t x{}; x != 33; x++) {
		if (queue.push(21) != FFLY_SUCCESS) {
			return -1;
		}

		//printf("%d\n", queue.front());
		if (x < 5) {
			queue.pop(any_err);
		}
//		if (x == 0)
//			queue.pop(any_err);

		queue.push(x);
		//queue.pop(any_err);
		//queue.dump();
		//printf("%d\n", queue.pop(any_err));
	}

	size_t qus = queue.size();
	for (mdl::uint_t x{}; x != qus; x++) {
		mdl::uint_t id = queue.pop(any_err);
		printf("%d\n", id);
		//queue.pop(any_err);
		if (any_err != FFLY_SUCCESS) {
        	printf("failed \n");
		}
	}
*/
	//printf("%c\n", queue.pop(any_err));
}
