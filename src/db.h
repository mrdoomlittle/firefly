# ifndef __ffly__db__h
# define __ffly__db__h
# include <mdlint.h>
# include "net.h"
# include "types/err_t.h"
struct ffdb {

};

struct ffdb_pile {


};

struct ffdb_record {
    
};

typedef struct ffdb_record* ffdb_recordp;
typedef struct ffdb_pile* ffdb_pilep;
typedef struct ffdb ffdbp;
ffly_err_t ffdb_init(ffdbp);
ffly_err_t ffdb_cleanup(ffdbp);
# endif /*__ffly__db__h*/
