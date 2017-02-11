# ifndef __ffly__client__hpp
# define __ffly__client__hpp
# include "x11_window.hpp"
# include "tcp_client.hpp"
# include "udp_client.hpp"
# include "draw.hpp"
# include <boost/cstdint.hpp>
namespace mdl { class ffly_client
{
	public:
	ffly_client(uint_t __window_xlen, uint_t __window_ylen);

	boost::uint8_t begin(char const *__frame_title);

	private:
	uint_t window_xlen = 0, window_ylen = 0;
};
}

# endif /*__ffly__client__hpp*/
