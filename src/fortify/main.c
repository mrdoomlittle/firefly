# include "../ffint.h"
# include "../types.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../opt.h"
# include "../depart.h"
# include "../linux/unistd.h"
# include "../linux/fcntl.h"
# include "../string.h"
# include "../crypto.h"
# include "../system/util/hash.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	char const *method;
	char const *src, *dst;
	char const *key;
	char const *algo;
	ffoe_prep();
	struct ffpcll pcl;
	pcl.cur = __argv+1;
	pcl.end = __argv+__argc;
	ffoe(ffoe_pcll, (void*)&pcl);
	method = ffoptval(ffoe_get("m"));
	src = ffoptval(ffoe_get("s"));
	dst = ffoptval(ffoe_get("d"));
	key = ffoptval(ffoe_get("k"));
	algo = ffoptval(ffoe_get("algo"));
	ffoe_end();

	if (!method || !src || !dst || !key) {
		goto _end;
	}

	printf("%s, %s, %s\n", src, dst, method);

	int s, d;
	if ((s = open(src, O_RDONLY, 0)) == -1) {
		goto _end;
	}

	if ((d = open(dst, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {
		close(s);
		goto _end;
	}

	if (algo != NULL) {
		if (!strcmp(algo, "frzz")) {
			ffly_encrypt = ffly_frzz_enc;
			ffly_decrypt = ffly_frzz_dec;
		}
	}

	{
		struct stat st;
		fstat(s, &st);

		ff_uint_t size;
		ff_u8_t *p = (ff_u8_t*)malloc(size = st.st_size);
		read(s, p, size);
		if (!strcmp(method, "enc"))
			ffly_encrypt(p, size, ffly_hash(key, strlen(key)));
		else if (!strcmp(method, "dec"))
			ffly_decrypt(p, size, ffly_hash(key, strlen(key)));
		write(d, p, size);
		free(p);
	}

_end:
	ffly_depart(NULL);
}
