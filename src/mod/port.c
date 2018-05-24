# include "port.h"
# include "../system/port.h"
ff_uint_t static port;
ff_uint_t ffmod_port_shmid(void) {
	return ffly_port_get_shmid(port);
}

ff_uint_t ffmod_portno(void) {
	return port;
}

void ffmod_port(void) {
	ff_err_t err;
	port = ffly_port(8, FF_PORT_CREAT, 0, &err);
}

void ffmod_port_open(ff_uint_t __shm_id) {
	ff_err_t err;
	port = ffly_port(8, FF_PORT_SHMM, __shm_id, &err);
}

void ffmod_port_close(void) {
	ffly_port_close(port);
}
