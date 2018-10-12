# include "compactor.h"
# include "system/io.h"
# include "linux/fcntl.h"
# include "linux/unistd.h"
# include "linux/stat.h"
# include "opt.h"
# include "depart.h"

/*
	need to work on
*/
# ifndef __ffly_compactor_bin
void prb(ff_u8_t __v) {
	ff_u8_t i = 0;
	while(i != 8) {
		ffly_printf("%u", __v>>(7-i)&0x1);
		i++;
	}
	ffly_printf("\n");
}

void static
bit_put(ff_compactorp __com, ff_u8_t __bits, ff_u8_t __n) {
	__bits &= 0xff>>(8-__n);
	if (!__com->left) {
		__com->left = 8;
		__com->put(__com->buf);
		__com->buf = 0x0;
	}

	if (__com->left<__n) {
		ff_u8_t ext = __n-__com->left;
		__com->buf = __com->buf>>__com->left|((__bits&(0xff>>(8-__com->left)))<<(8-__com->left));
		__com->put(__com->buf);
		__com->left = 8;
		__com->buf = 0x0;
		bit_put(__com, __bits>>(__n-ext), ext);
		return;
	}

	__com->buf = __com->buf>>__n|((__bits&(0xff>>(8-__n)))<<(8-__n));
	__com->left-=__n;
}

ff_u8_t static
bit_get(ff_compactorp __com, ff_u8_t __n) {
	ff_u8_t ret;
	if (!__com->left) {
		__com->left = 8;
		__com->buf = __com->get();
	}

	if (__com->left<__n) {
		ff_u8_t ext = __n-__com->left;
		ret = __com->buf;
		__com->left = 8;
		__com->buf = __com->get();
		ret = ret|(bit_get(__com, ext)<<(__n-ext));
		return ret;
	}

	ret = __com->buf&(0xff>>(8-__n));
	__com->buf>>=__n;
	__com->left-=__n;
	return ret;
}

void static
bit_flush(ff_compactorp __com) {
	if (__com->left != 8)
		__com->put(__com->buf>>__com->left);
}

void ff_compact(ff_compactorp __com) {
	__com->left = 8;
	__com->buf = 0x0;
	ff_u8_t prior = __com->get();
	ff_u8_t past = 0;
	__com->put(prior);
	ff_u8_t tmp, val, l, bits;
	ff_u8_t i;
	while(__com->at_eof() == -1) {
		tmp = __com->get();
		val = prior^tmp;
	_bk:
		prior = tmp;
		l = 0;
		while(!((val>>(7-l))&0x1) && l<8)
			l++;
		bits = 8-l;

		i = 0;
		if (val == past) {
			bit_put(__com, 1, 1);
		_again:
			tmp = __com->get();
			if ((val = (prior^tmp)) != past || i > 2) {
				bit_put(__com, i, 2);
				goto _bk;
			}
			prior = tmp;
			i++;
			goto _again;
		}
		bit_put(__com, 0, 1);
		bit_put(__com, bits, 4);
		bit_put(__com, val, bits);
		past = val;
	}
	bit_flush(__com);
}

void ff_decompact(ff_compactorp __com) {
	__com->left = 0;
	__com->buf = 0x0;
	ff_u8_t prior = __com->get();
	ff_u8_t past = 0;
	__com->put(prior);
	while(__com->at_eof() == -1) {
		ff_u8_t sk = bit_get(__com, 1);
		ff_u8_t tmp;
		ff_u8_t b;
		if (!sk) {
			ff_u8_t bits = bit_get(__com, 4);
			b = bit_get(__com, bits);
			tmp = b;
		} else {
			ff_u8_t i;
			i = bit_get(__com, 2);
		_again:
			__com->put(prior = (prior^past));
			if (i>0) {
				i--;
				goto _again;
			}
			continue;
		}
		b = prior^b;
		prior = b;
		past = tmp;
		__com->put(b);
	}
}
# endif
# ifdef __ffly_compactor_bin
int unsigned static in_off = 0, in_size = 0;
int static in;
int static out;

void put(ff_u8_t __byte) {
	write(out, &__byte, 1);
}

ff_u8_t get() {
	ff_u8_t ret;
	read(in, &ret, 1);
	in_off++;
	return ret;
}

ff_i8_t at_eof(void) {
	if (in_off>=in_size)
		return 0;
	return -1;	
}

void reset() {
	in_off = 0;
}

# include "compactor.h"
# include "dep/str_cmp.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	char const *method = NULL;
	char const *infile = NULL, *outfile = NULL;
	ffoe_prep();
	struct ffpcll pcl;
	pcl.cur = __argv+1;
	pcl.end = __argv+__argc;
	ffoe(ffoe_pcll, (void*)&pcl);
	method = ffoptval(ffoe_get("m"));
	infile = ffoptval(ffoe_get("i"));
	outfile = ffoptval(ffoe_get("o"));
	ffoe_end();

	if (!method || !infile || !outfile) {
		ffly_printf("usage: ./ -m <method{c, d}> -i <input> -o <output>");
		return -1;
	}

	struct ff_compactor com;
	com.put = put;
	com.get = get;
	com.at_eof = at_eof;
	com.reset = reset;
	in = open(infile, O_RDONLY, 0);
	struct stat st;
	fstat(in, &st);
	in_size = st.st_size;
	out = open(outfile, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
	if (!ffly_str_cmp(method, "d"))
		ff_decompact(&com);
	else if (!ffly_str_cmp(method, "c"))
		ff_compact(&com);
	else {
		ffly_printf("unknown method.\n");
	}
	close(in);
	close(out);
	ffly_depart(NULL);
}
# endif
