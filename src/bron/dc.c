# include "dc.h"
# include "context.h"
void static
_bron_start(void) {
	BRON_CONTEXT->driver.start();
}

void static
_bron_finish(void) {
	BRON_CONTEXT->driver.finish();
}

void static
_bron_done(void) {
	BRON_CONTEXT->driver.done();
}

void ffly_bron_start(void) {
	_bron_start();
}

void ffly_bron_finish(void) {
	_bron_finish();
}

void ffly_bron_done(void) {
	_bron_done();
}
