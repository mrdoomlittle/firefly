# include "src/ffly_server.hpp"
int main() {
	mdl::ffly_server server(800, 800, 1);
	if (server.init() == -1) {
		fprintf(stderr, "init error.\n");
		return 1;
	}

	server.begin();
	printf("ending.\n");

	return 0;
}
