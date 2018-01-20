# include "../types/err_t.h"
# include <time.h>
# include "allocr.h"
# include <mdlint.h>
# include "../system/io.h"
void pr();
void pf();
ffly_err_t ffmain(int __argc, char **__argv) {
    ffly_alloc(3740);

    ffly_free(ffly_alloc(100));
    
    pr();
    pf();
}
