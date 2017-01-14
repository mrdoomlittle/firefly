# ifndef __ffly__client__hpp
# define __ffly__client__hpp
# include <boost/cstdint.hpp>
# include <SFML/Graphics.hpp>
# include <SFML/Window.hpp>
# include <cstdio>
# define ARC64
# define PART_X 2 
# define PART_Y 2
# define PART_Z 2
# ifdef ARC64
    typedef boost::uint64_t uint_t;
# elif ARC32
    typedef boost::uint32_t uint_t;
# else
    typedef int unsigned uint_t;
# endif

namespace mdl { class ffly_client 
{
    public:
    ffly_client(uint_t __parts_x, uint_t __parts_y, uint_t __parts_z);

    boost::uint8_t initialize(uint_t __parts_x, uint_t __parts_y, char const * __wtitle);

    private:
    uint_t uni_x, uni_y, uni_z;
} ;
}


# endif /*__ffly__client__hpp*/
