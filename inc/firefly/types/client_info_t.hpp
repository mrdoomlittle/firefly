# ifndef __client__info__t__hpp
# define __client__info__t__hpp
# include <serializer.hpp>
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {

typedef struct client_info {
	int key_code = 0;
	void achieve(serializer& __arc) {
		__arc & sizeof(int);
		__arc << key_code;
	}
} client_info_t;

}
}
}

# endif /*__client__info__t__hpp*/
