# include "pipe.h"
# include <string.h>
# include "io.h"
int main() {
    ffly_io_init();
    mdl_uint_t pipe = ffly_pipe(8, 0, 0);
    char const *s = "Hello World!!!!!!!!!";
    ffly_pipe_write(s, strlen(s), pipe);
    ffly_pipe_close(pipe);
    ffly_io_closeup();
}
