# include "mem_cmp.h"
# include <string.h>
# include <stdio.h>
# include "find.h"
int main() {
	int unsigned a[] = {'a', 'v', 'd'};

	int unsigned x = 'd';
	printf("%c\n", *(char*)ffly_find((void*)a, (void*)&x, sizeof(int unsigned), 3));
}
