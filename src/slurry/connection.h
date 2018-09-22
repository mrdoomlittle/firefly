# ifndef __ffly__slurry__connection__h
# define __ffly__slurry__connection__h
# include "../ffint.h"
# include <netinet/in.h>
typedef struct s_conn {
	int sock;
	struct sockaddr_in addr;
} *s_connp;

s_connp s_open(void);
void s_connect(s_connp, ff_u16_t, char const*);
void s_close(s_connp);
# endif /*__ffly__slurry__connection__h*/
