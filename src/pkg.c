# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "pkg.h"
# include "system/io.h"
# define MAX_DIRS 20
# include "data/str_cmp.h"
# include "data/str_len.h"
# include "data/str_cpy.h"
# include "data/str_dupe.h"
char path[PATH_MAX], *end = NULL;
char **lpath;
mdl_uint_t level = 0;
struct file* next(DIR ***__dir, struct dirent **__en) {
    DIR *dir;
    _next:
    dir = **__dir;
    if ((*__en = readdir(dir)) != NULL) {
        struct dirent *en = *__en;
        if (!ffly_str_cmp(en->d_name, ".") || !ffly_str_cmp(en->d_name, ".."))
            goto _next;
        if (en->d_type == DT_DIR) {
            *(end++) = '/';
            ffly_str_cpy(end, en->d_name); 
            ffly_printf("leveldir: %s\n", path);
            (*__dir)++;
            level++;

            if (!(**__dir = opendir(path))) {
                ffly_printf("failed to open directory.\n");
            }

            *(++lpath) = (char*)__ffly_mem_alloc(PATH_MAX);
            ffly_str_cpy(*lpath, path);
            goto _next;
        } 

        if (en->d_type == DT_REG) {
            struct file *f = (struct file*)__ffly_mem_alloc(sizeof(struct file));
            f->dir = *lpath;
            f->name = ffly_str_dupe(en->d_name);
            return f;
        }
    }

    if (level>0) {
        closedir(**__dir);
        level--;
        (*__dir)--;
        goto _next; 
    } 

    return NULL;
}

void pkg_build(struct file*, char const*);

# include <unistd.h>
# include <linux/limits.h>
# include <stdlib.h>
DIR **dir = NULL;
struct dirent *en = NULL;
int main() {
    ffly_io_init();
    dir = (DIR**)__ffly_mem_alloc(MAX_DIRS*sizeof(DIR*));
    lpath = (char**)__ffly_mem_alloc(MAX_DIRS*sizeof(char*));
    *(lpath++) = NULL;
    *lpath = (char*)__ffly_mem_alloc(PATH_MAX);

    realpath("pkg", path);
    if (!(*dir = opendir("pkg"))) {
        return -1;
    }

    ffly_str_cpy(*lpath, path);

    end = path+ffly_str_len(path);
    ffly_printf("leveldir: %s\n", path);

    struct file *top = NULL;
    struct file *f, *prev = NULL;
    while((f = next(&dir, &en)) != NULL) {
        if (!top) top = f;
        ffly_printf("dir: %s\n", f->dir);
        ffly_printf("name: %s\n\n", f->name);
        if (prev != NULL)
            prev->next = f;
        prev = f; 
    }

    pkg_build(top, "hello.pkg");
    
    while(*lpath != NULL) {
        __ffly_mem_free(*lpath);
        ffly_printf("--->\n");
        lpath--;
    }
    __ffly_mem_free(lpath);
    closedir(*dir);    
    __ffly_mem_free(dir);
    ffly_io_closeup();
}
