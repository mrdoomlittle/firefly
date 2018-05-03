//# include "pulse_audio.hpp"
//# include "alsa_audio.hpp"
//# include "ffly_audio.hpp"
# include "memory/mem_alloc.h"
# include "memory/alloc_pixmap.hpp"
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
# include "graphics/crop_pixmap.hpp"
# include "memory/alloc_pixmap.hpp"
# include <cuda_runtime.h>
# include "system/timer.hpp"
# include <string.h>
using namespace mdl::firefly;
# define NO_NULL 0x0
int main() {
	mdl::uint_t a = 200, b = 200, c = 4;
	types::_1d_pm_t pm = memory::alloc_pixmap(a,b,c);
	memset(pm, ';', 200 * 200 * 4 * 4);

	pm[0] = '@';

	types::_3d_pm_t oo = memory::make_3d_pm(pm, 200, 200, 4);
	oo[0][0][1] = '#';

	for (int y{}; y != 20; y ++) {
		memset(oo[0][y], 'A', 20*4);
	}

	for (mdl::uint_t ya{}; ya != 64; ya ++) {
		for (mdl::uint_t xa{}; xa != 64; xa ++) {
			printf("%c", oo[0][ya][xa]);
		}
		printf("\n");
	}

/*
	//types::_1d_pm_t pm_ = memory::alloc_pixmap(200, 200, (mdl::u8_t)3);
	types::_1d_pm_t pm_ = memory::alloc_pixmap(200, 200, 4, 1);
	types::_3d_pm_t oo = memory::make_3d_pm(pm_, 200, 200, 8, 1);
	types::_2d_pm_t pm = oo[0];
	for (mdl::uint_t y{}; y != 200; y ++) {
		for (mdl::uint_t x{}; x != 200; x ++){
			pm[y][x] = 'X';
			if (x > 5 && y > 5) {
				pm[y][x] = ';';
			}

			if (x > 11 && y > 11 && x < 20 && y < 20) {
				pm[y][x] = '#';
			}
		}
		//if (y > 16)
		//	pm[y][0] = 'D';
	}

	pm[0][0] = '&';

	types::_2d_pm_t pb = memory::alloc_2d_cmem_pm(64, 64, 1);
	for (mdl::uint_t y{}; y != 64; y ++) {
		for (mdl::uint_t x{}; x != 64; x ++)
			pb[y][x] = 'O';
	}

for (int x{}; x != 13; x ++) {
	graphics::crop_2d_pm(0, x, pb, 64, 64, 64, pm, 200, 200, 200, 0, 0, 1);
	printf("<------------------------------>\n");
	for (mdl::uint_t y{}; y != 64; y ++) {
		printf("%d	", y);
		for (mdl::uint_t x{}; x != 64; x ++) {
			printf("%c", pb[y][x]);
		}
		printf("\n");
	}
	printf("<------------------------------>\n");
}
	std::free(pb[0]);
	std::free(pm[0]);
	std::free(pb);
	std::free(pm);
	cudaDeviceReset();
*/
}

/*
# include "graphics/crop_pixmap.hpp"
# include "firefly.hpp"
# include <string.h>
# include <cstdio>
int main() {

//	mdl::firefly::init();
	types::_1d_pm_t _1dpm = memory::alloc_pixmap((mdl::uint_t)8, (mdl::uint_t)8, (mdl::uint_t)8, (mdl::u8_t)4);
	memset(_1dpm, 0, 8*8*8*4);
	memset(_1dpm + 8*8, 1, 8*8);

	types::_3d_pm_t _3dpm = memory::make_3d_pm(_1dpm, 4, 4, 4, (mdl::u8_t)4);

	for (mdl::uint_t za{}; za != 4; za ++) {
		for (mdl::uint_t ya{}; ya != 4; ya ++) {
			for (mdl::uint_t xa{}; xa != 4*4; xa ++) {
				printf("%d", _3dpm[za][ya][xa]);
			}
			printf("\n");
		}
		printf("\n");
	}
*/
//	printf("%d\n", mdl::ffly_memory::get_mem_usage());

//	mdl::firefly::de_init();

//	pulse_audio _pulse_audio;
//	_pulse_audio.play_sample("", "test", system::WAV, system::FLOAT32_LE);
//	alsa_audio _alsa_audio;
//	_alsa_audio.play_sample("", "test", system::WAV, system::FLOAT32_LE);
//}
