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
    typedef boost::int64_t int_t;
# elif ARC32
    typedef boost::uint32_t uint_t;
    typedef boost::int32_t int_t;
# else
    typedef int unsigned uint_t;
    typedef int int_t;
# endif
namespace mdl { class ffly_client 
{
    public:
    typedef struct {
        bool internal_loop = false;
        bool no_graphics = false;
    } init_options_t;

    typedef struct {
        char * server_ip_addr;
        boost::uint16_t server_port_num;
    } client_settings_t;

    ~ffly_client();

    ffly_client(uint_t __parts_x, uint_t __parts_y, uint_t __parts_z);

    boost::uint8_t initialize(uint_t __parts_x, uint_t __parts_y, char const * __wtitle, init_options_t & __init_options);

    boost::uint8_t event_tick();

    private:
    sf::RenderWindow * rw_ptr = nullptr;
    init_options_t init_options;

    boost::int8_t * * * un;
   
    int_t uni_x, uni_y, uni_z;
} ;
    typedef ffly_client::client_settings_t client_settings_t;
    typedef ffly_client::init_options_t init_options_t;
}


# endif /*__ffly__client__hpp*/
