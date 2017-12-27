# include "resolve.h"
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
# include "../ffly_def.h"
# include "../system/io.h"
# include "../data/mem_cpy.h"
# include "../data/bzero.h"
# include "../data/bcopy.h"
struct in_addr ffly_resolve(char *__host) {
    struct hostent *host = gethostbyname(__host);
    char **list = host->h_addr_list;
    if (*list == NULL) {
        // err
    }
    while(*list != NULL) list++;
    list--;
    struct in_addr addr;
    ffly_bzero(&addr.s_addr, sizeof(in_addr_t));
    ffly_bcopy(&addr.s_addr, *list, host->h_length);
    return addr;
}
