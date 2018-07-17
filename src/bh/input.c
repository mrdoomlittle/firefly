# include "../bh.h"
ff_err_t ff_bh_oprcv(FF_SOCKET *__sock, struct bhop *__op) {
	ff_err_t err;
	ff_net_recv(__sock, &__op->kind, 1, &err);
	return err;
}
