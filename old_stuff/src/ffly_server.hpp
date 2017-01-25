# ifndef __ffly__server__hpp
# define __ffly__server__hpp
# include <boost/cstdint.hpp>
# include <eint_t.hpp>
# include <boost/scoped_array.hpp>
# define DEF_PARTX_LEN 2
# define DEF_PARTY_LEN 2
# define DEF_PARTX_LEN 2
namespace mdl {
class ffly_server
{
    public:
	typedef struct {
		uint_t xlen = 0, ylen = 0, zlen = 0;
		uint_t partx_len = DEF_PARTX_LEN;
		uint_t party_len = DEF_PARTY_LEN;
		uint_t partz_len = DEF_PARTX_LEN;
	} uni_info_t;



	boost::uint8_t initialize(uni_info_t __uni_ino, char const * __ip_addr, boost::uint16_t __port_num);

	boost::uint8_t terminal();

	private:
	typedef struct {
		boost::uint8_t r = 0, g = 0, b = 0, a = 0;
	} colour_t;

	typedef struct {
		colour_t * rgba_colour;
	} uni_part_t;

	boost::scoped_array<uni_part_t> uni_parts;

	/* infomation like partical size and uni size will be stored hear.
	*/
	uni_info_t g_uni_info;
} ;

}


# endif /*__ffly__server__hpp*/
