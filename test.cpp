
# include <pthread.h>
# include <stdint.h>
# include <cstdio>
extern uint8_t *test();

void *th(void *ptr) {
	uint8_t *data = test();
	if (data == nullptr) return NULL;
	for (std::size_t o = 0; o != 8; o ++) printf("%d", data[o]);
	printf("\n");
}

int main(void) {
	pthread_t th_0, th_1;

	pthread_create(&th_0, NULL, th, NULL);
	pthread_create(&th_1, NULL, th, NULL);

	pthread_join(th_0, NULL);
	pthread_join(th_1, NULL);
}
