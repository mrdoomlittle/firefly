# include "view.h"
# include "context.h"
void bron_viewport(struct bron_viewport *__vp) {
	BRON_CONTEXT->driver.viewport(__vp);	
}
