# include <cstdio>
# include "cuda_helper.hpp"
# include <eint_t.hpp>
# include <math.h>
# include "system/task_handle.hpp"
# include "graphics/crop_pixmap.hpp"
using namespace mdl::firefly::system;
# include <atomic>
# include "types/byte_t.h"
# include <string.h>
/*
int unsigned c = 0;
static std::atomic<int unsigned> o;
void *tsk_0(void *d) {
	for (int x = 0; x != 2898; x ++) {}
	o ++;
}

void *tsk_1(void *d) {
	printf("t1\n");
	usleep(10000);
}

void *tsk_2(void *d) {
	printf("t2\n");
	usleep(100000);
}
int unsigned tt = 20, pp = 0;
int main() {
	_task_handle.init(12);
	o = 0;
for (;;) {
	printf("tick. %d\n", pp);
	int cx = 21;
	for (int c = 0; c != tt; c ++)
		_task_handle.handle(&tsk_0, &cx, true, true);

	while(o != tt){
	}
	o = 0;
	pp++;
}
	_task_handle.de_init();
}
*/

int main() {
	mdl::firefly::types::byte_t a[16384];
	mdl::firefly::types::byte_t b[1024];
	memset(a, 'X', 16384);
	memset(b, 'O', 1024);

	a[0] = 'B';
	a[(64 + 1)*4] = 'A';
	mdl::firefly::graphics::crop_pixmap(0, 0, b, 16, 16, a, 64, 64, 4);

	for (int y = 0; y != 16; y ++) {
		for (int x = 0; x != 16; x ++) {
			int point = (x + (y * 16))*4;
			printf("%c", b[point]);
			printf("%c", b[point + 1]);
			printf("%c", b[point + 2]);
			printf("%c", b[point + 3]);
		}
		printf("\n");
	}
}
