# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstdio>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
using namespace std;
int main() {
	int client, server;
	int port_num = 21299;
	int const buff_len = 128;
	char buff[buff_len] = {'\0'};

	struct sockaddr_in server_addr;
	socklen_t len;


	client = socket(AF_INET, SOCK_STREAM, 0);

	if (client < 0) {
		printf("error establishing connection.\n");
		return 1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(port_num);

	if (bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("error binding socket.\n");
		return 1;
	}

	len = sizeof(server_addr);

	listen(client, 1);

	server = accept(client, (struct sockaddr*)&server_addr, &len);
	if (server < 0) {
		printf("error accepting.\n");
	}

	while (server > 0) {
		strcpy(buff, "test");
		send(server, buff, buff_len, 0);
	}
}
