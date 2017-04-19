# include "pulse_audio.hpp"
# include "alsa_audio.hpp"
# include "ffly_audio.hpp"
using namespace mdl::firefly;
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
/*
# include "memory/alloc_pixmap.hpp"
# include <cuda_runtime.h>
# include "system/timer.hpp"
using namespace mdl::firefly;
int main() {
	types::_2d_pm_t pm = memory::alloc_2d_cmem_pm(200, 200, 1);
	for (mdl::uint_t y{}; y != 200; y ++) {
		for (mdl::uint_t x{}; x != 200; x ++)
			pm[y][x] = 'X';
	}

	pm[0][0] = 'D';
	pm[1][1] = 'D';

	types::_2d_pm_t pb = memory::alloc_2d_cmem_pm(64, 64, 1);
	for (mdl::uint_t y{}; y != 64; y ++) {
		for (mdl::uint_t x{}; x != 64; x ++)
			pb[y][x] = 'O';
	}

	if (graphics::crop_2d_pm(1, 1, pb, 64, 64, pm, 200, 200, 1) != FFLY_SUCCESS) {
		printf("hekrjeohrijuehriheirheu\n");
		return 0;
	}


	for (mdl::uint_t y{}; y != 64; y ++) {
		for (mdl::uint_t x{}; x != 64; x ++) {
			printf("%c", pb[y][x]);
		}
		printf("\n");
	}

	char o = 'A';
	std::cin >> o;

	std::free(pb[0]);
	std::free(pm[0]);
	std::free(pb);
	std::free(pm);
	cudaDeviceReset();
}
*/
# include <cstdio>
int main() {
	pulse_audio _pulse_audio;
	_pulse_audio.play_sample("", "test.wav", system::WAV, system::FLOAT32_LE);
}
