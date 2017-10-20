# include <cstdio>
template<typename _T>
struct t {
	t() {
		printf("inited.\n");
	}
};
int main() {
	t<int> _t;
}
