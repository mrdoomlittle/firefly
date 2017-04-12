# include <cstdio>
# include "cuda_helper.hpp"
# include <eint_t.hpp>
# include <math.h>
# include "system/task_handle.hpp"
using namespace mdl::firefly::system;
# include <atomic>
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
