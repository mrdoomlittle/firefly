# include "view.h"
# include "raise.h"
# include "context.h"
void nt_viewport(void) {
	struct nt_viewport *vp;
	vp = (struct nt_viewport*)nt_raise_p;
	nt_ctx->viewport = *vp;
}
