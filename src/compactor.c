# include "compactor.h"
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>

void prb(ff_u8_t __v) {
	ff_u8_t i = 0;
	while(i != 8) {
		printf("%u", __v>>(7-i)&0x1);
		i++;
	}
	printf("\n");
}

ff_u16_t buf[1024];
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
	__com->put(prior);
	while(__com->at_eof() == -1) {
		ff_u8_t tmp = __com->get();
		ff_u8_t val = prior^tmp;
		prior = tmp;
		ff_u8_t l = 0;
		while(!((val>>(7-l))&0x1) && l<8)
			l++;
		ff_u8_t bits = 8-l;

		bit_put(__com, bits, 4);
		bit_put(__com, val, bits);
	}
	bit_flush(__com);
}

void ff_decompact(ff_compactorp __com) {
	__com->left = 0;
	__com->buf = 0x0;
	ff_u8_t prior = __com->get();
	__com->put(prior);
	while(__com->at_eof() == -1) {
		ff_u8_t bits = bit_get(__com, 4);
		ff_u8_t b = bit_get(__com, bits);
		ff_u8_t tmp = b;
		b = prior^b;
		prior = b;
		__com->put(b);
	}
}

int unsigned in_off = 0, in_size = 0;
int in;
int out;

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

# include <string.h>
int main(int __argc, char const *__argv[]) {
	struct ff_compactor com;
	com.put = put;
	com.get = get;
	com.at_eof = at_eof;
	com.reset = reset;
	in = open("in", O_RDONLY);
	struct stat st;
	fstat(in, &st);
	in_size = st.st_size;
	out = open("out", O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
	if (!strcmp(__argv[1], "de"))
		ff_decompact(&com);
	else
		ff_compact(&com);

	close(in);
	close(out);
}
