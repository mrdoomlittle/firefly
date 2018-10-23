# include "pkg.h"
# include <linux/limits.h>
# include "system/file.h"
# include "types/err_t.h"
# include "data/str_cpy.h"
# include "ffly_def.h"
void pkg_build(struct file *__top, char const *__dst) {
    ffly_err_t err;
    FF_FILE *dst = ffly_fopen(__dst, FF_O_RDWR|FF_O_CREAT, FF_S_IRUSR|FF_S_IWUSR, &err);

    char path[PATH_MAX];
    char *p;
    struct file *f = __top;
    while(f != NULL) {
        p = path; 
        p+= ffly_str_cpy(p, f->dir);
        *(p++) = '/';
        ffly_str_cpy(p, f->name); 

        ffly_printf("file path: %s\n", path);
       // FF_FILE *file = ffly_fopen(); 

        f = f->next;
    }  

    ffly_fclose(dst);
}
