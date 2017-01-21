# ifndef __comm__deamon__hpp
# define __comm__deamon__hpp
# include "comm_handler.hpp"
# include <boost/cstdint.hpp>
# define DEF_PORT_NUM 21299
# define DEF_IP_ADDR "192.168.0.100"
namespace mdl { class comm_deamon : public comm_handler
{
    public :
    comm_deamon(char * __ip_addr = DEF_IP_ADDR, boost::uint16_t __port_num = DEF_PORT_NUM) {}




} ;
}

# endif /*__comm__deamon__hpp*/
