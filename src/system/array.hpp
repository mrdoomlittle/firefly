# ifndef __array__hpp
# define __array__hpp
# include <cstdlib>
namespace mdl {
namespace firefly {
namespace system {
template <typename __T, std::size_t __N>
class array {
	public:
	array() {
		this-> data = static_cast<__T *>(malloc(__N * sizeof(__T)));
	}

	__T &operator[](std::size_t __el) {
		return this-> data[__el];
	}

	~array() {
		std::free(this-> data);
	}

	private:
	__T *data = nullptr;
};
}
}
}


# endif /*__array__hpp*/
