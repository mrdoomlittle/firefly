
/*
# include "types/err_t.h"
# include "system/io.h"
# include "main.h"
extern "C" {
void pr();
void pf();
}

# include <mdlint.h>
# include "system/map.h"
# include "system/buff.h"
# include "system/vec.h"
# include "ffly_memory.hpp"
# include "system/set.hpp"
using namespace mdl::firefly::system;
void static ts0() {
	map<mdl::uint_t> *ptr = new map<mdl::uint_t>;

	mdl_u8_t i = 0;
	char const *keys[] = {
		"aij", "bdf", "csf", "ddd", "gfge", "rhkf", "adg", "grgh", "adi", "ffj", "gjk", "aal", "ffsem", "jhaiun"
	};

	mdl_u8_t const n = sizeof(keys)/sizeof(char const*);
	while(i != n) {
		ptr->put(i, keys[i++]);
	}

	mdl::firefly::types::err_t err;
	ptr->del(keys[5], err);

	i = 0;
	while(i != n) {
		if (i == 5) {i++;continue;}
		io::cout << ptr->get(keys[i++], err) << ffly_endl;	
	}
	delete ptr;
}

void static ts1() {
	buff<mdl::uint_t> *ptr = new buff<mdl::uint_t>(100);
	buff<mdl::uint_t>& r = *ptr;
	mdl_u8_t const n = 20;
	mdl_u8_t i = 0;

	while(i != n) {
		*r = i;
		r++;
		i++;
	}

	while(i != 0) {
		r--;
		io::cout << *r << ffly_endl;
		i--;
	}

	delete ptr;
}

void static ts2() {
	set<mdl::uint_t> *ptr = new set<mdl::uint_t>;
	ptr->insert(21299);

	set<mdl::uint_t>::iterator itr = ptr->begin();
	io::cout << "Hello" << ' ' << *itr << ffly_endl;

	delete ptr;
}

ffly_err_t ffmain(int __argc, char const **__argv) {
//	ts0();
	pr();
	pf();
}
*/

