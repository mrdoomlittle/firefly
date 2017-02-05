# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstdio>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <chrono>
int main() {
	int sock, n;
	struct sockaddr_in server, from;
	struct hostent *hp;
	socklen_t len;

	int const buff_len = 54000;
    char buff[buff_len];// = {'\0'};

	int port_num = 21299;

	len = sizeof(struct sockaddr_in);

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	server.sin_family = AF_INET;
	server.sin_port = htons(port_num);
	inet_pton(AF_INET, "192.168.0.10", &(server.sin_addr));


	char temp[] = "Hello World!";

	auto begin = std::chrono::high_resolution_clock::now();
	strcpy(buff, temp);
	sendto(sock, buff, buff_len, 0, (struct sockaddr*)&server, len);

	char ibuff[buff_len];// = {'\0'};
	recvfrom(sock, ibuff, buff_len, 0, (struct sockaddr*)&from, &len);

	auto now = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(now - begin);
    //std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

	printf("%s, time span: %lf\n", ibuff, duration.count());

	return 0;
}
