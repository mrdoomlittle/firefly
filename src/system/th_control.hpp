# ifndef __th__control__hpp
# define __th__control__hpp
# include <mdlint.h>
# include <boost/cstdint.hpp>
# define TH_SUCCESS 0
# define TH_FAILURE -1
# define TH_NULL 1
namespace mdl {
namespace firefly {
namespace system {
struct th_control {
	bool th_pause, th_waiting;
	uint_t th_id;
	uint_t th_msg;
	boost::int8_t th_reply;
};
}
}
}

# endif /*__th__control__hpp*/
