# include "../src/mod/printf.h"
# include "../src/mod/malloc.h"
void main(void) {
	setmalopt(FF_MAL_O_OSD);
	void *p = malloc(20);
	free(p);
}
