# ifndef __ffly__pkg__h
# define __ffly__pkg__h
# include <dirent.h>

struct file {
    char const *name, *dir;
    struct file *next;
};
struct file* next(DIR***, struct dirent**);
# endif /*__ffly__pkg__h*/
