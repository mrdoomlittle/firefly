# include "hash.h"
# define _GNU_SOURCE
# include <unistd.h>
# include <malloc.h>
# include <stdio.h>
# define MAP_SIZE 200
# define NONFF
# include "../../tools/printbin.c"
void ts0() {
	mdl_u64_t const n = 20000;
	mdl_u64_t table[8] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};

	mdl_u64_t i = 0;
	while(i != n) {
		mdl_u64_t val, in = i;
		val = ffly_hash((mdl_u8_t const*)&in, sizeof(mdl_u64_t));
		table[0]+=val&0xff;
		table[1]+=val>>8&0xff;
		table[2]+=val>>16&0xff;
		table[3]+=val>>24&0xff;
		table[4]+=val>>32&0xff;
		table[5]+=val>>40&0xff;
		table[6]+=val>>48&0xff;
		table[7]+=val>>56&0xff;
		printf("out: %lu\n", val);
		i++;
	}

	printf("0: %lu\n", table[0]);
	printf("1: %lu\n", table[1]);
	printf("2: %lu\n", table[2]);
	printf("3: %lu\n", table[3]);
	printf("4: %lu\n", table[4]);
	printf("5: %lu\n", table[5]);
	printf("6: %lu\n", table[6]);
	printf("7: %lu\n", table[7]);
}

# include <pthread.h>
# define SLAVES 2
mdl_u64_t err = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
struct job {
	mdl_u64_t *beg, *end;
	mdl_u64_t locate;
};

struct job *inbound = NULL;

void *slave(void *__arg_p) {
	mdl_u64_t *cur, *end;
	mdl_u64_t locate;
_bk:
	pthread_mutex_lock(&lock);
	if (inbound != NULL) {
		cur = inbound->beg;
		end = inbound->end;
		locate = inbound->locate;
		inbound = NULL;

		pthread_mutex_unlock(&lock);
		goto _sk;
	}
	pthread_mutex_unlock(&lock);
	goto _bk;
_sk:
	while(cur != end) {
		if (*cur == locate) {
			__asm__("lock incq %0\n\t" : "=m"(err));
		}
		cur++;
	}

	goto _bk;
}

# include <signal.h>
# include <stdlib.h>
void ts1() {
	mdl_u64_t const n = 100000;
	mdl_u64_t *map = (mdl_u64_t*)malloc(n*sizeof(mdl_u64_t));
	mdl_u64_t *cur = map;
	while(cur != map+n)
		*(cur++) = 0;

	double ground = 0.0;
	double one = 100.0/(double)n;
	double v;

	mdl_u64_t mul = 2584193;//rand()%20000;
	pthread_t slaves[SLAVES];

	mdl_u64_t i = 0;
	while(i != SLAVES) {
		pthread_create(&slaves[i], NULL, slave, NULL);
		i++;
	}

	i = 0;
	while(i != n) {
		mdl_u64_t val, in = i*mul;
		val = ffly_hash((mdl_u8_t const*)&in, sizeof(mdl_u64_t));

		struct job static j;
		j.beg = map;
		j.end = map+i;
		j.locate = val;
		while(inbound != NULL) {
			usleep(10);
		}
		inbound = &j;

		if ((v = ((double)i*one)-ground)>1.0) {
			printf("%lf done.\n", i*one);
			ground+=v;
		}

		*(map+i) = val;
		i++;
	}

	printf("%lu error/s\n", err);
	while(inbound != NULL) {
		usleep(10);
	}
	i = 0;
	while(i != SLAVES) {
		pthread_kill(slaves[i], SIGKILL);
		i++;
	}

	free(map);
}

int main() {
	ts1();
}
