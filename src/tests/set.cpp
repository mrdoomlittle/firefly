# include <iostream>
# include "../system/set.hpp"
using namespace mdl::firefly::system;
# include <mdlint.h>
# include "../system/err.h"
# include "../system/io.h"
# include "../types/err_t.h"
# include "../system/errno.h"
# define failure {\
	err = FFLY_FAILURE;\
	goto _end;}
mdl::firefly::types::err_t err = FFLY_SUCCESS;
int main() {
	ffly_io_init();
	set<mdl::uint_t> nos;

	mdl::uint_t i = 0;
	while(i < 200) {
		nos.insert(i);
		io::printf("insert: %u\n", i);
		i++;
	}

	mdl::uint_t del_range[2] = {10, 20};

	i = del_range[0];
	while(i <= del_range[1]) {
		set<mdl::uint_t>::iterator no = nos.find(i, err);
		if (_err(err)) failure;
		nos.erase(no);
		i++;
	}


	i = 0;
	while(i < 200) {
		set<mdl::uint_t>::iterator itr = nos.find(i, err);
		if (_ok(err))
			io::printf("got: %u, correct? %s\n", *itr, *itr == i?"yes":"no");
		else {
			if (i < del_range[0] || i > del_range[1])
				failure;
			io::printf("can't find element: %u\n", i);
		}
		i++;
	}

	_end:
	ffly_io_closeup();
	return err;
}
