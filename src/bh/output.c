# include "../bh.h"
ff_err_t ff_bh_opsnd(FF_SOCKET *__sock, struct bhop *__op) {
	ff_err_t err;
	ff_net_send(__sock, &__op->kind, 1, &err);
	return err;
}
