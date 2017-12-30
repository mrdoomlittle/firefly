# include "http.h"
# include "../ffly_def.h"
# include <stdio.h>
# include <string.h>
# include "../system/io.h"
int main() {
    ffly_io_init();
    char *buf;
    ffly_size_t size;
	ffly_http_get("qg-desktop-d00", "hello.txt", 80, (void*)&buf, &size);
    printf("got: '%s'\n", buf);

    ffly_io_closeup();
}
