
# include "uni.h"
# include "system/io.h"
int main() {
    ffly_io_init();
    struct ffly_uni uni;
    ffly_uni_build(&uni, 32, 32, 32, 1);

    ffly_uni_free(&uni);
    ffly_io_closeup();
}
