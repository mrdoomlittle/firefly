# include "dc.h"
# include "context.h"
void ffly_g_start(void) {
	G_CONTEXT->driver.start();
}

void ffly_g_finish(void) {
	G_CONTEXT->driver.finish();
}

void ffly_g_done(void) {
	G_CONTEXT->driver.done();
}
