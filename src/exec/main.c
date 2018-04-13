# include "../exec.h"
# include "../types.h"
# include "../stdio.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc < 2) {
		printf("please provide file.\n");
		return -1;
	}

	ffexecf(__argv[1]);
}
