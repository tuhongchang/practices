#include "server.h"

userstatus_st us;

int main()
{
	int sockfd,
		connfd;
	struct sockaddr_in peeraddr;
	socklen_t addrlen = sizeof(SA);
	datapack_st *mesgbuff = NULL;

	pid_t pid;
	sqlite3 *mydb = NULL;

	mydb = sys_init_database();	/*  load database, and so on ... */
	sockfd = sys_init_networks();
	mesgbuff = (datapack_st *)malloc(sizeof(datapack_st));
	memset(mesgbuff, 0, sizeof(datapack_st));

	/* avoid zombie process */
	signal(SIGCHLD, SIG_IGN);

	while (1) {
		if (0 > (connfd = accept(sockfd, (SA *)&peeraddr, &addrlen )))
			error_handler("fail to accept");

		if (-1 == (pid = fork()))
			error_handler("fork");

		/* father continue, and waiting for new client  */
		if (pid > 0) {
			close(connfd);
			continue;
		}
		/* child, process data request */
		while (1) {
			if (0 >= recv(connfd, mesgbuff, sizeof(datapack_st), 0))
				error_handler("main-> fail to recv");

			switch (mesgbuff->type) {
			case CMD_QUERY:
				word_query(connfd, mesgbuff, mydb);
				break;
			case CMD_HISTORY:
				user_history(connfd, mesgbuff, mydb);
				break;
			case CMD_LOGIN:
				user_login(connfd, mesgbuff, mydb);
				break;
			case CMD_LOGOUT:
				user_logout(connfd, mesgbuff, mydb);
				break;
			case CMD_REGIST:
				user_register(connfd, mesgbuff, mydb);
				break;
			case CMD_ANNUL:
				user_annul(connfd, mesgbuff, mydb);
				break;
			default:
				warning_handler("command error !");
				break;
			}	/* switch */
		} /* child.while(1) */
	} /*main.while(1) */

	return 0;
} /* main */

int sys_init_networks(void)
{
	int sockfd;
	struct sockaddr_in myaddr;
	int opt = 1;

	memset(&myaddr, 0, sizeof(SA));
	if (-1 == getipaddr(NETDEV_NAME, (SA *)&myaddr))
		error_handler("getipaddr");
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(SER_PORT);

	if (0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
		error_handler("fail to open socket");
	if (0 > setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		error_handler("fail to setsockopt");
	if (0 > bind(sockfd, (SA *)&myaddr, sizeof(SA)))
		error_handler("fail to bind");
	if (0 > listen(sockfd, LISTEN_QLEN))
		error_handler("fail to listen");

	return sockfd;
}
