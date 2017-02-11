# include "serial.hpp"
# include "../eint_t/inc/eint_t.hpp"
namespace mdl {
typedef struct {
	int chunk_xlen = 0, chunk_ylen = 0;

	void arc(serial & __arc) {
		__arc & sizeof(int);
		__arc << chunk_xlen;

		__arc & sizeof(int);
		__arc << chunk_ylen;
	}
} worker_info_t;
}
