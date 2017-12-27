# include "http.h"
# include "../ffly_def.h"
# include <stdio.h>
# include <string.h>
# include "../system/io.h"
int main() {
    ffly_io_init();
    char buf[1000];
    memset(buf, 0, 1000);
	ffly_http_get("qg-desktop-d00", "hello.txt", 80, buf, 1000);
    printf("got: '%s'\n", buf);

    ffly_io_closeup();
}
