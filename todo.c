# include "src/ffint.h"
# include <stdio.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <malloc.h>
# include <string.h>
struct task {
	void *data;
	ff_uint_t len;

	struct task *next;
};

ff_uint_t static
po10_t[] = {
	1,
	10,
	100,
	1000,
	10000,
	100000
};

ff_uint_t static
stdec(char *__buf, ff_uint_t __len) {
	ff_uint_t r;

	r = 0;

	char *p, *end;
	p = __buf;
	end = p+__len;
	ff_uint_t i;
	i = __len;
	while(p != end) {
		r+=(*(p++)-'0')*po10_t[--i];
	}
	return r;
}

void decipher(struct task *__tsk, void *__data) {


}

// task tray
struct task static *tt;
static ff_uint_t nt;

#define is_space(__c) \
	(__c == ' ' || __c == '\n' || __c == '\t')

int static fd;
void load(void) {
	ff_u8_t *bed;
	struct stat st;
	fstat(fd, &st);

	bed = (ff_u8_t*)malloc(st.st_size);
	read(fd, bed, st.st_size);

	ff_uint_t i, ib;
	char buf[86];
	char c;
	i = 0;
	ib = 0;

_a0:
	c = *(bed+(i++));
	*(buf+(ib++)) = c;
	if (c != '^') {
		goto _a0;
	}
	nt = stdec(buf, ib-1);
	tt = (struct task*)malloc(nt*sizeof(struct task));
	printf("tray size: %u\n", nt);

_ss0:
	c = *(bed+i);
	if (is_space(c)) {
		i++;
		goto _ss0;
	}

	while(i != st.st_size-1) {
		ib = 0;
	_again:
		c = *(bed+(i++));
		*(buf+(ib++)) = c; 
		if (c != ':') {
			goto _again;
		}

		ff_uint_t key;
		key = stdec(buf, ib-1);
		struct task *tsk;
		tsk = tt+key;
		ff_uint_t start, stop;
		start = i;
		while(*(bed+(i++)) != '#');
		stop = i-1;
		tsk->data = malloc((tsk->len = (stop-start))+1);
		memcpy(tsk->data, bed+start, tsk->len);
		*((ff_u8_t*)tsk->data+tsk->len) = '\0';

		printf("%u:%s\n", key, tsk->data);
	_ss1:
		c = *(bed+i);
		if (is_space(c) && i<st.st_size-1) {
			i++;
			goto _ss1;
		}
	}

	free(bed);
}


void save(void) {
}

int main(int __argc, char const *__argv[]) {
	if (__argc<2) {
		printf("error.\n");
		return -1;
	}

	fd = open(__argv[1], O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

	load();


	save();

	close(fd);
	return 0;
}
