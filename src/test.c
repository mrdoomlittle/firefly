# include "types/err_t.h"
# include "dep/mem_cpy.h"
# include "system/thread.h"
# include "stdio.h"
# include "malloc.h"
void* thr(void *__arg_p) {
	printf("Hello...\n");
}

# include "system/errno.h"
# include "linux/socket.h"
# include "linux/net.h"
# include "linux/in.h"
# include "network/in.h"
# include "dep/bzero.h"
# include "system/nanosleep.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
//	printf(".....\n");
//	ffly_tid_t t0;
//	ffly_thread_create(&t0, thr, NULL);
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd == -1) {
		strerror(errno);
	}

	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(21299);
	if (bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1) {
		strerror(errno);
	}

	ffly_nanosleep(20, 0);

	close(fd);
}
