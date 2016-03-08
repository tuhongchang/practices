#include "client.h"

int get_input(char *dest, int len)
{
	int i = 0;
	char ch;

	while ('\n' != (ch = getchar())) {
		if(i < len-1)
			dest[i++] = ch;
	}
	dest[i] = '\0';
	return i;
}

int get_user_info(datapack_st *mesgbuff)
{
	int packsize = offsetof(datapack_st, info);

	puts("enter your name:");
	get_input(mesgbuff->title, TITLE_LEN);
	puts("enter your password");
	get_input(mesgbuff->info, INFO_LEN);
	packsize += strlen(mesgbuff->info) + 1;

	return packsize;
}

int submit_account(int sockfd, datapack_st *mesgbuff, int cmd)
{
	int packsize;

	packsize = get_user_info(mesgbuff);
	mesgbuff->type = cmd;

	if (0 > send(sockfd, mesgbuff, packsize, 0)) {
		printf("submit_account[%c]->",cmd);
		perror("fail to send");
		return errno;
	}

	if (0 > recv(sockfd, mesgbuff, sizeof(datapack_st), 0)) {
		printf("submit_account[%c]->",cmd);
		perror("fail to recv");
		return errno;
	}
	printf("%s:%s\n",mesgbuff->title, mesgbuff->info);
	return mesgbuff->type;
}

int user_funtion(int sockfd, datapack_st *mesgbuff)
{
	char cmd[16];

	while (1) {
		puts("\n*******************************************");
		puts("1. show history.");
		puts("2. query new word.");
		puts("#. quit");
		puts("********************************************");
		get_input(cmd, 2);

		switch (cmd[0]) {
		case '1':
			user_history(sockfd, mesgbuff);
			break;
		case '2':
			process_query(sockfd, mesgbuff);
			break;
		case '#':
			return 0;
		default:
			puts("command error !");
			break;
		}
	}
	return 0;
}

int user_history(int sockfd, datapack_st *mesgbuff)
{
	int ret;
	int packsize = offsetof(datapack_st, info);

	puts("user_history");

	mesgbuff->type = CMD_HISTORY; 
	if (0 >= (ret = send(sockfd, mesgbuff, packsize, 0))) {
		perror("query.send-> fail to send");
		return errno;
	}
	
	while (1) {
		ret = recv(sockfd, mesgbuff, sizeof(datapack_st), 0);
		if (mesgbuff->type < 0)
			break;
#if DEBUG
		printf("[%d]%s:  %s\n",ret, mesgbuff->title, mesgbuff->info);
#else
		printf("%s:  %s\n", mesgbuff->title, mesgbuff->info);
#endif
	}
	
	return 0;
}

int process_query(int sockfd, datapack_st *mesgbuff)
{
	int packsize = offsetof(datapack_st, info);
	int ret;

	while (1) {
		puts("word:");
		get_input(mesgbuff->title, TITLE_LEN);
		if (!strcmp(mesgbuff->title, "#"))
			return 0;
		mesgbuff->type = CMD_QUERY;

		if (0 >= (ret = send(sockfd, mesgbuff, packsize, 0))) {
			perror("query.send-> fail to send");
			return errno;
		}
#if DEBUG
		printf("query.send->[%d]%s:%s\n", ret, mesgbuff->title, mesgbuff->info);
#endif
		if (0 >= (ret = recv(sockfd, mesgbuff, sizeof(datapack_st), 0))) {
			perror("query-> fail to recv");
			return errno;
		}
#if DEBUG
		printf("query.recv->[%d]%s:%s\n", ret, mesgbuff->title, mesgbuff->info);
#else
		printf("%s:%s\n", mesgbuff->title, mesgbuff->info);
#endif
	}
	return 0;
}
