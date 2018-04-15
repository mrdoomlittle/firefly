# include <stdio.h>
void test() {
	test();
}
int main() {
	char c = 'p';
	printf("%u,%u\n", c>>4, c-((c>>4)*(1<<4)));
test();
		}
