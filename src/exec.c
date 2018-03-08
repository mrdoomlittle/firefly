# include "exec.h"
void ffexec(void *__p, mdl_u8_t __format) {
	switch(__format) {
		case _ffexec_bc:
			ffbci_exec(__p);	
		break;
	}
}
