# include "bond.h"
# include "../types.h"
# include "../stdio.h"
# include "../malloc.h"
# include "../system/err.h"
# include "../opt.h"
# include "../depart.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffoe_prep();
	char const *s = NULL;
	char const *d = NULL;

	struct ffpcll pcl;
	pcl.cur = __argv+1;
	pcl.end = __argv+__argc;
	ffoe(ffoe_pcll, (void*)&pcl);

	s = ffoptval(ffoe_get("s"));
	d = ffoptval(ffoe_get("d"));

	ffoe_end();

	if (!s || !d) {
		printf("please provide source and dest.\n");
		reterr;
	}

	printf("src: %s, dst: %s\n", s, d);
	bond(s, d);

	ffly_depart(NULL);
	retok;
}
