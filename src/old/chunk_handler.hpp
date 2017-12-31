# ifndef __chunk__handler__hpp
# define __chunk__handler__hpp
# include <pthread.h>
# include <boost/cstdint.hpp>
# include "chunk_manager.hpp"
# include "obj_manager.hpp"
namespace mdl {
namespace firefly {
class chunk_handler {
	public:
	boost::int8_t add_handle();
	boost::int8_t kill_handle();

};
}
}

# endif /*__chunk__handler__hpp*/
