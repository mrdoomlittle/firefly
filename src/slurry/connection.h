# ifndef __ffly__slurry__connection__h
# define __ffly__slurry__connection__h
# include "../ffint.h"
# ifndef __fflib
# include <netinet/in.h>
# else
# include "../linux/in.h"
# endif
typedef struct s_conn {
	int sock;
	struct sockaddr_in addr;
} *s_connp;

s_connp s_open(void);
void s_connect(s_connp, ff_u16_t, char const*);
void s_close(s_connp);
void s_test(void);
# endif /*__ffly__slurry__connection__h*/
