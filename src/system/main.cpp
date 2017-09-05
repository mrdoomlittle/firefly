# include "thread.h"
# include <typeinfo>
# include <cstdio>
# include <unistd.h>
void test(mdl_u8_t __a, mdl_u8_t __b, mdl_u8_t __c, mdl_u8_t __d) {
	while(1) {
		usleep(1000000);
		printf("hello, %u, %u, %u, %u\n", __a, __b, __c, __d);
	}
}

mdl_uint_t o  = 0;
using namespace mdl::firefly::system;
/*
template<typename _ARG, typename... _E>
struct test_ : test_<_E...> {
	_ARG val = o++;
};

template<typename _ARG>
struct test_<_ARG>{
	_ARG val = o++;
};


template<mdl_uint_t _I, typename _ARG, typename... _E>
struct test_impl {
	auto static val(test_<_ARG, _E...>& __r) -> decltype(test_impl<_I-1, _E...>::val(__r)) {
		return test_impl<_I-1, _E...>::val(__r);
	}
};

template<typename _ARG, typename... _E>
struct test_impl<0, _ARG, _E...> {
	_ARG static val(test_<_ARG, _E...>& __r) {
		return __r.val;
	}
};

template<mdl_uint_t _I, typename _ARG, typename... _E>
auto static g(test_<_ARG, _E...>& __r) -> decltype(test_impl<_I, _ARG, _E...>::val(__r)) {
	return test_impl<_I, _ARG, _E...>::val(__r);
}

template<typename _ARG, typename... _E>
void test(_ARG, _E...) {
	
	struct test_<_ARG, _E...> r;
	printf("%u\n", g<1>(r));
}
*/
int main() {
	thread th(&test, 1, 2, 3, 4);
	printf("hello fu\n");
	while(1);
//	thread_create(&ts, 1, 2);
//	test((int)1, (int)2, (int)3);
}
