# include <stdio.h>
int main(int __argc, char const *__argv[]) {
	char c = *__argv[1];
	printf("'%c': row: %u: col: %u\n", c, c>>4, c-((c>>4)*(1<<4)));
}
