# include <stdio.h>
int main() {
%ifdef _a
	printf("a\n");
%endif
%ifdef _b
	printf("b\n");
%endif
}
