# include "prog.h"
# include "context.h"
void bron_slgprog(ff_u8_t *__p, ff_u32_t __size) {
	BRON_CONTEXT->driver.slgprog(__p, __size);
}
