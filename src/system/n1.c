# include "pipe.h"
# include <stdio.h>
# include "io.h"
# include "../data/bzero.h"
int main() {
    ffly_io_init();
    mdl_uint_t pipe = ffly_pipe(8, FF_PIPE_CREAT, 0);
    char buf[40];
    ffly_bzero(buf, 40);

    ffly_pipe_read(buf, 40, pipe);
    ffly_printf("%s\n", buf);

    ffly_pipe_close(pipe);
    ffly_io_closeup();
}
