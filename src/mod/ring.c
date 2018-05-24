# include "ring.h"
# include "../call.h"
# include "port.h"
# include "../system/port.h"
# include "../ffly_def.h"
# include "../types.h"
void static
ring_printf(void *__ret, void *__params) {
	ffly_port_send(PORT_BAND, __params, sizeof(ff_u64_t), ffmod_portno());
}

void static
ring_malloc(void *__ret, void *__params) {
	ff_err_t err;
	ffly_port_send(PORT_BAND, __params, sizeof(ff_uint_t), ffmod_portno());
	ffly_port_recv(PORT_BAND, __ret, sizeof(void*), ffmod_portno());
}

void static
ring_free(void *__ret, void *__params) {
	ffly_port_send(PORT_BAND, __params, sizeof(void*), ffmod_portno());
}


void static
ring_dcp(void *__ret, void *__params) {
	void *dst = *(void**)__params;
	void *src = *(void**)((ff_u8_t*)__params+8);
	ff_uint_t *n = (ff_uint_t*)((ff_u8_t*)__params+16);

	ffly_port_send(PORT_BAND, (void*)&src, sizeof(void*), ffmod_portno());
	ffly_port_send(PORT_BAND, n, sizeof(ff_uint_t), ffmod_portno());
	ffly_port_recv(PORT_BAND, dst, *n, ffmod_portno());
}

void static
ring_scp(void *__ret, void *__params) {
	void *dst = *(void**)__params;
	void *src = *(void**)((ff_u8_t*)__params+8);
	ff_uint_t *n = (ff_uint_t*)((ff_u8_t*)__params+16);

	ffly_port_send(PORT_BAND, (void*)&dst, sizeof(void*), ffmod_portno());
	ffly_port_send(PORT_BAND, n, sizeof(ff_uint_t), ffmod_portno());
	ffly_port_send(PORT_BAND, src, *n, ffmod_portno());
}

static void(*ring[])(void*, void*) = {
	ring_printf,
	NULL,
	ring_malloc,
	ring_free,
	NULL,
	ring_dcp,
	ring_scp
};

void ffmod_ring(ff_u8_t __no, void *__ret, void *__params) {
	ffly_port_send(PORT_BAND, &__no, 1, ffmod_portno());
	ring[__no](__ret, __params);
}
