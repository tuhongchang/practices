#include "comm.h"
#include "sqlite3.h"

int network_init(void);
void network_destroy(int sockfd);
int recv_mesg(int sockfd, void *buff, int size);
int send_mesg(int sockfd, void *buff, int size);
sqlite3 *database_init(const char *name);
void database_destroy(sqlite3 *db);
int search_database(sqlite3 *db, qmesg_t *msgbuf);
int search_callback(void *arg, int argc, char **data, char **column);

struct sockaddr_in peeraddr;

int main(int argc, const char **argv)
{
	qmesg_t msgbuf;

	sqlite3 *db;
	int sockfd;

	if (argc != 2) {
		puts("invalid argument !");
		return -1;
	}

	sockfd = network_init();
	db = database_init(argv[1]);

	while (1) {
		if (-1 == recv_mesg(sockfd, &msgbuf, sizeof(msgbuf)))
			break;
		
		search_database(db, &msgbuf);

		if (-1 == send_mesg(sockfd, &msgbuf, sizeof(msgbuf)))
			break;
	}

	network_destroy(sockfd);
	database_destroy(db);

	return 0;
}

int network_init(void)
{
	struct sockaddr_in seraddr;
	int sockfd;

	if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
		return -1;

	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(SER_PORT);
	seraddr.sin_addr.s_addr = inet_addr(SER_ADDR);

	if (-1 == bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)))
		return -1;

	return sockfd;
}

void network_destroy(int sockfd)
{
	close(sockfd);
}

int recv_mesg(int sockfd, void *buff, int size)
{
	socklen_t addrlen = sizeof(struct sockaddr_in);
	return recvfrom(sockfd, buff, size, 0, (struct sockaddr *)&peeraddr, &addrlen);
}

int send_mesg(int sockfd, void *buff, int size)
{
	return sendto(sockfd, buff, size, 0, (struct sockaddr *)&peeraddr, sizeof(peeraddr));
}

sqlite3 *database_init(const char *dbname)
{
	sqlite3 *db;

	if (SQLITE_OK != sqlite3_open(dbname, &db)) {
		fprintf(stderr, "sqlite3_open: %s\n", sqlite3_errmsg(db));
		exit(1);
	}

	return db;
}

void database_destroy(sqlite3 *db)
{
	sqlite3_close(db);
}

int search_database(sqlite3 *db, qmesg_t *msgbuf)
{
	char *errmsg;
	char sqlbuf[1024];

	sprintf(sqlbuf, "SELECT explan from %s where word = '%s'",
			DB_TABLE_NAME, msgbuf->word);
	msgbuf->flag = QMESG_FLAG_ERR;

	if (-1 == sqlite3_exec(db, sqlbuf, search_callback, msgbuf, &errmsg)) {
		fprintf(stderr, "sqlite3_exec: %s\n", errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
}

int search_callback(void *arg, int argc, char **data, char **column)
{
	qmesg_t *msgbuf = arg;
	
	strcpy(msgbuf->explan, data[0]);
	msgbuf->flag = QMESG_FLAG_ACK;

	return 0;
}
