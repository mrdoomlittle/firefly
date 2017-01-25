# include "ffly_server.hpp"
boost::uint8_t mdl::ffly_server::initialize(uni_info_t __uni_ino, char const * __ip_addr, boost::uint16_t __port_num) {
	this-> g_uni_info = __uni_ino;

	std::size_t uni_size = (
		(__uni_ino.xlen * __uni_ino.partx_len) +
		(__uni_ino.ylen * __uni_ino.party_len) +
		(__uni_ino.zlen * __uni_ino.partz_len)
	);
	
	this-> uni_parts.reset(static_cast<uni_part_t *>(malloc(uni_size * sizeof(uni_part_t))));
}

boost::uint8_t mdl::ffly_server::terminal() {

}

int main() {
	mdl::ffly_server ffly_server;
	mdl::ffly_server::uni_info_t uni_info;
	uni_info.xlen = 64;
	uni_info.ylen = 64;
	uni_info.zlen = 64;

	ffly_server.initialize(uni_info, "192.168.0.10", 21299);

}
