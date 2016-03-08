#include "comm.h"
#include <stddef.h>

#define BUFF_SIZE	1024

int network_init(void);
int recv_mesg(int sockfd, void *buff, int size);
int send_mesg(int sockfd, void *buff, int size);

int main()
{
	int sockfd;
	char buff[BUFF_SIZE];
	qmesg_t msgbuf;

	sockfd = network_init();

	while (1) {
		fgets(buff, BUFF_SIZE, stdin);
		buff[strlen(buff)-1] = '\0';
		
		if (!strcmp(buff, "#"))
			break;

		msgbuf.flag = QMESG_FLAG_REQ;
		strncpy(msgbuf.word, buff, QMESG_WORD_LEN);
		if (-1 == send_mesg(sockfd, &msgbuf, offsetof(qmesg_t, explan)))
			break;

		if (-1 == recv_mesg(sockfd, &msgbuf, sizeof(msgbuf)))
			break;
		if (msgbuf.flag == QMESG_FLAG_ACK)
			printf("%s --- %s\n", msgbuf.word, msgbuf.explan);
		else
			printf("%s *** NO FOUND !!\n", msgbuf.word);
	}

	close(sockfd);

	return 0;
}

int network_init(void)
{
	return socket(AF_INET, SOCK_DGRAM, 0);
}

int recv_mesg(int sockfd, void *buff, int size)
{
	return recvfrom(sockfd, buff, size, 0, NULL, NULL);
}

int send_mesg(int sockfd, void *buff, int size)
{
	struct sockaddr_in seraddr;

	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(SER_PORT);
	seraddr.sin_addr.s_addr = inet_addr(SER_ADDR);

	return sendto(sockfd, buff, size, 0, (struct sockaddr *)&seraddr, sizeof(seraddr));
}
