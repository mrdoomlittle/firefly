# ifndef __ffly__engine__hpp
# define __ffly__engine__hpp
# if defined(FFLY_SERVER)
	# include "ffly_server.hpp"
# elif defined(FFLY_CLIENT)
	# include "ffly_client.hpp"
# endif
# endif /*__ffly__engine__hpp*/
