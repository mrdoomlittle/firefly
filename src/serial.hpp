# ifndef __serial__hpp
# define __serial__hpp
# include <boost/cstdint.hpp>
namespace mdl {
struct serial {
	serial(char __typ) : typ(__typ) {}

	void operator&(std::size_t __size) {
		if (typ == 's')
			this-> obj_size += __size;
		else
			this-> obj_size = __size;
	}

	template<typename __T>
	void operator<<(__T& __obj) {
		switch(this-> typ) {
			case 'r':
				for (std::size_t o = 0; o != this-> obj_size; o ++) {
					if (o == 0)
						this-> _serial[curr_point] = __obj & 0xFF;
					else
						this-> _serial[curr_point] = (__obj >> (o * 8)) & 0xFF;
					this-> curr_point ++;
				}
			break;
			case 'w':
				for (std::size_t o = 0; o != this-> obj_size; o ++) {
					__obj |= (__obj & 0xFF) | (this-> _serial[curr_point] << (o * 8));
					this-> curr_point ++;
				}
			break;
		}
	}

	void operator|(char __typ) {
		obj_size = 0;
		curr_point = 0;
		typ = __typ;
	}

	void init(std::size_t __bytes) {
		this-> _serial = static_cast<boost::uint8_t *>(malloc(__bytes));
		memset(this-> _serial, 0x0, __bytes);
	}

	template<typename __T>
	std::size_t get_size(__T * __struct) {
		typ = 's';
		__struct-> arc(*this);
		return obj_size;
		obj_size = 0;
	}

	boost::uint8_t * get_data() {
		return _serial;
	}

	char typ = '\0';
	std::size_t obj_size = 0;
	std::size_t curr_point = 0;
	boost::uint8_t * _serial;
};
}
# endif /*__serial__hpp*/
