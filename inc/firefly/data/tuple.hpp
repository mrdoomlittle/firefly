# ifndef __tuple__hpp
# define __tuple__hpp
namespace mdl {
namespace firefly {
namespace data {
template<typename _F, typename... _E> struct tuple : tuple<_E...> {_F val;};
template<typename _F> struct tuple<_F> {_F val;};

template<size_t _I, typename _F, typename... _E>
struct tuple_impl {
	auto static val(tuple<_F, _E...>& __t) -> decltype(tuple_impl<_I - 1, _E...>::val(__t)) {
		return tuple_impl<_I - 1, _E...>::val(__t);}
};

template<typename _F, typename... _E>
struct tuple_impl<0, _F, _E...> {
	_F static& val(tuple<_F, _E...>& __t) {return __t.val;}};

template<size_t _I, typename _F, typename... _E>
auto tuple_get(tuple<_F, _E...>& __t) -> decltype(tuple_impl<_I, _F, _E...>::val(__t)) {
	return tuple_impl<_I, _F, _E...>::val(__t);}
}
}
}
# endif /*__tuple__hpp*/
