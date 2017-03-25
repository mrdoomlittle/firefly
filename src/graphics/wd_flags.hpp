# ifndef __wd__flags__hpp
# define __wd__flags__hpp
# define FLG_WD_TO_CLOSE 16
# define FLG_WD_CLOSED 1
# define FLG_WD_OPEN 2
# define FLG_WD_WAITING 4
# define FLG_WD_DONE_DRAW 8
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace graphics {
struct wd_flags {
	boost::int8_t wd_flags = FLG_WD_CLOSED;
	bool is_wd_flag(boost::int8_t __wd_flag) {
		boost::int8_t to_compare = 0;

		to_compare = this-> wd_flags & __wd_flag;

		return to_compare == __wd_flag? true : false;
	}

	void add_wd_flag(boost::int8_t __wd_flag, bool __over_write = false) {
		if (__over_write) {
			this-> wd_flags = __wd_flag;
			return;
		}
		this-> wd_flags |= __wd_flag;
	}

	void rm_wd_flag(boost::int8_t __wd_flag) {
		this-> wd_flags ^= this-> wd_flags & __wd_flag;
	}
};
}
}
}


# endif /*__wd__flags__hpp*/
