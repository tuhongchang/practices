#include"server.h"

int select_callback(void *arg, int nu, char **value, char **name)
{
	int ret;
	datapack_st *data = arg;
	int connfd = data->type; 
	int packsize = offsetof(datapack_st, info);
	
	strcpy(data->title, value[0]);
	strcpy(data->info, value[1]);
	packsize += strlen(data->info) + 1;

	usleep(50000);
	if (0 >= (ret = send(connfd, data, packsize, 0)))
		error_handler("callback->fail to send");
#if DEBUG
	printf("history->[%d]%s:%s\n",ret, data->title, data->info);
#endif
	return 0;
}

int pack_net_mesg(datapack_st *mesgbuff, char *title, char *info, int type)
{
	int packsize = offsetof(datapack_st, info);
	
	strncpy(mesgbuff->title, title, TITLE_LEN-1);
	strncpy(mesgbuff->info, info, INFO_LEN-1);
	mesgbuff->type = type;
	packsize += strlen(mesgbuff->info) + 1;

	return packsize;
}

int word_query(int connfd, datapack_st *mesgbuff, sqlite3 *mydb)
{
	int packsize = 0;
	int ret = 0;
	
	if (!us.status) {
		puts("you are not login !");
		return -1;
	}

	ret = db_query_data(mydb, WORD_TABLE, mesgbuff);
#if DEBUG
	printf("query->[%d]%s:%s\n",ret ,mesgbuff->title, mesgbuff->info);
#endif
	if (ret > 0) {
		packsize = offsetof(datapack_st, info) + strlen(mesgbuff->info) + 1;
		if(0 >= db_query_data(mydb, us.name, mesgbuff))
			db_insert(mydb, us.name, mesgbuff);
	} else
		packsize = pack_net_mesg(mesgbuff, "error", "unkown word !", CMD_ERROR);
	usleep(50000);
	
	if (0 >= (ret = send(connfd, mesgbuff, packsize, 0)))
		error_handler("query->fail to send");

#if DEBUG
	printf("query.send->[%d]%s:%s\n",ret ,mesgbuff->title, mesgbuff->info);
#endif
	return 0;
}

int user_history(int connfd, datapack_st *mesgbuff, sqlite3 *mydb)
{
	int ret;
	
	mesgbuff->type = connfd;
	ret = db_query_table(mydb, us.name, mesgbuff);

//finish send
	mesgbuff->type = -1;
	if (0 >= (ret = send(connfd, mesgbuff, 4, 0)))
		error_handler("history->fail to send");

	return 0;
}

int user_login(int connfd, datapack_st *mesgbuff, sqlite3 *mydb)
{
	int packsize;
	int ret;
	char pwd[TITLE_LEN];
	
	strcpy(pwd, mesgbuff->info);
	ret = db_query_data(mydb, PSWD_TABLE, mesgbuff);
	if (ret > 0) {
		if (!strcmp(pwd, mesgbuff->info)) {
			us.status = 1;
			strcpy(us.name, mesgbuff->title);
			packsize = pack_net_mesg(mesgbuff, us.name, "welcom  back home !", CMD_LOGIN);
		} else
			packsize = pack_net_mesg(mesgbuff, "error", "passwd error !", CMD_ERROR);
	} else
		packsize = pack_net_mesg(mesgbuff, "error", "user not exists !", CMD_ERROR);

	if (0 > (ret = send(connfd, mesgbuff, packsize, 0)))
		error_handler("login->fail to send");
#if DEBUG
	printf("login->[%d]%s:%s\n", ret, mesgbuff->title, mesgbuff->info);
#endif
	return 0;
}

int user_logout(int connfd, datapack_st *mesgbuff, sqlite3 *mydb)
{
	us.status = 0;
	puts("user_logout");
	return 0;
}

int user_register(int connfd, datapack_st *mesgbuff, sqlite3 *mydb)
{
	int packsize;
	int ret;
	
	ret = db_query_data(mydb, PSWD_TABLE, mesgbuff);
	if (ret > 0)
		packsize = pack_net_mesg(mesgbuff, "error", "user already exists !", CMD_ERROR);
	else {
		db_insert(mydb, PSWD_TABLE, mesgbuff);
		db_create(mydb, mesgbuff->title);
		packsize = pack_net_mesg(mesgbuff, mesgbuff->title, "welcom to farsight online dictionary !", CMD_REGIST);
	}
	
	if (0 > send(connfd, mesgbuff, packsize, 0))
		error_handler("register->fail to send");

	return 0;
}


int user_annul(int connfd, datapack_st *mesgbuff, sqlite3 *mydb)
{
	puts("user_annul");
	return 0;
}
