# include "types/err_t.h"
# include "memory/allocr.h"
# include "system/io.h"
ffly_err_t ffmain(int, char**);
int main(int __argc, char **__argv) {
    ffly_ar_init();
    ffly_io_init();
    ffmain(__argc, __argv);
    ffly_io_closeup();
    ffly_ar_cleanup();
    return 0; 
}
