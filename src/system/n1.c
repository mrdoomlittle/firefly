# include "pipe.h"
# include <stdio.h>
# include "io.h"
# include "../data/bzero.h"
int main() {
    ffly_io_init();
    mdl_uint_t pipe = ffly_pipe(8, FF_PIPE_CREAT);
    char buf[20];
    ffly_bzero(buf, 20);

    ffly_pipe_read(buf, 20, pipe);
    ffly_printf("%s\n", buf);

    ffly_pipe_close(pipe);
    ffly_io_closeup();
}
