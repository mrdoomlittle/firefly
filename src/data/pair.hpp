# ifndef __pair__hpp
# define __pair__hpp
namespace mdl {
namespace firefly {
namespace data {
template<typename _T1, typename _T2>
struct pair {
	_T1 first;
	_T2 second;
};

template<typename _T1, typename _T2>
pair<_T1, _T2> static __pair__(_T1 __t1_val, _T2 __t2_val) {
	pair<_T1, _T2> data = {__t1_val, __t2_val};
	return data;
}

}
}
}
# endif /*__pair__hpp*/
