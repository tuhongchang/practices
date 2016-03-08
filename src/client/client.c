#include "client.h"

int main(int argc, char **argv)
{
	int sockfd;
	char cmd[4];
	datapack_st *mesgbuff = NULL;
	const char *serip = NULL;
#ifdef SER_ADDR
	serip = SER_ADDR;
#else
	if (2 != argc) {
		puts("using: ./client SERVERIP");
		return -1;
	}
	serip = argv[1];
	if (-1 == inet_addr(serip)) {
		puts("invalid ip address !");
		return -1;
	}
#endif

	mesgbuff = (datapack_st *)malloc(sizeof(datapack_st));
	memset(mesgbuff, 0, sizeof(datapack_st));
	sockfd = init_network(serip);

	while (1) {
		puts("\n*********************************");
		puts("1. register");
		puts("2. login");
		puts("3. annul");
		puts("#. quit");
		puts("*********************************");
		puts("enter your command:");
		get_input(cmd, 2);

		switch (cmd[0]) {
			case '1':
				submit_account(sockfd, mesgbuff, CMD_REGIST);
				break;
			case '2':
				if( 0 < submit_account(sockfd, mesgbuff, CMD_LOGIN))
					user_funtion(sockfd, mesgbuff);
				break;
			case '3':
				submit_account(sockfd, mesgbuff, CMD_ANNUL);
				break;
			case '#':
				goto LabelQuit;
			default:
				puts("command error !");
				break;
		}
	}
	return 0;

LabelQuit:
	close(sockfd);
	free(mesgbuff);
	exit(EXIT_SUCCESS);
}

int init_network(const char *serip)
{
	int sockfd;
	struct sockaddr_in serveraddr;

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SER_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(serip);

	if (0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
		error_handler("fail to open socket");
	if (0 > connect(sockfd, (SA *)&serveraddr, sizeof(SA)))
		error_handler("fail to connect");
	
	return sockfd;
}
