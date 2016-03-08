#include "server.h"

int _getfilesize_(const char *filename);

sqlite3 *sys_init_database()
{
	sqlite3 *mydb = NULL;
	
/*  open database */
	if (SQLITE_OK != sqlite3_open(DB_PATH, &mydb))
		error_handler("fail to open database");
	
/*   make sql , create userpasswd table */
	db_create(mydb, PSWD_TABLE);	
/* make sql, create word table */
	if (0 <= db_create(mydb, WORD_TABLE))
		db_load_data(mydb);		/* load dictionary data into database */
	return mydb;
}

int db_create(sqlite3 *mydb, char *table)
{
	char sql[INFO_LEN];
	char *errmsg = NULL;

	sprintf(sql, "create table %s (title vchar(%d), info vchar(%d))",
			table, TITLE_LEN, INFO_LEN);
#if DEBUG
	puts(sql);
#endif
	if (SQLITE_OK != sqlite3_exec(mydb, sql, NULL, NULL, &errmsg)) {
		printf("create table %s:%s\n", table, errmsg);
		return -1;
	}
	return 0;
}	

int db_insert(sqlite3 *mydb, char *table, datapack_st *data)
{
	char sql[INFO_LEN];
	char *errmsg = NULL;

	sprintf(sql, "insert into %s values(\"%s\", \"%s\")", table,data->title, data->info);
	if (SQLITE_OK != sqlite3_exec(mydb, sql, NULL, NULL, &errmsg)) {
		puts("db_insert-> fail to insert data:");
		error_handler(errmsg);
	}
	return 0;
}

int db_query_data(sqlite3 *mydb, char *table, datapack_st *data)
{
	int row = -1, column = 0;
	char **result = NULL;
	char *errmsg = NULL;
	char sql[INFO_LEN];
	
	sprintf(sql, "select * from %s where title = \"%s\"", table ,data->title);
	if (SQLITE_OK != sqlite3_get_table(mydb, sql, &result, &row, &column, &errmsg)) {
		puts("db_query_data->fail to select");
		error_handler(errmsg);
	}
	if (row > 0)
		strcpy(data->info, result[column + 1]);
	sqlite3_free_table(result);
	return row;
}

int db_query_table(sqlite3 *mydb, char *table, datapack_st *data)
{
	char *errmsg = NULL;
	char sql[INFO_LEN];
	
	sprintf(sql, "select * from %s", table);
	if (SQLITE_OK != sqlite3_exec(mydb, sql, select_callback, data, &errmsg)) {
		puts("db_query_table->fail to select");
		error_handler(errmsg);
	}
	return 0;
}

int db_load_data(sqlite3 *mydb)
{
	FILE *fp = NULL;
	char buff[INFO_LEN];
	datapack_st *data = NULL;

	int index = 0;
	int count = 0;
	char *tmp = NULL;
	int file_size;

	file_size = _getfilesize_(DICT_FILE);

	if (NULL == (fp = fopen(DICT_FILE, "r")))
		error_handler("fail to open file");
	
	data = (datapack_st *)malloc(sizeof(datapack_st));
	puts("loading data ... ... ");

	while (NULL != fgets(buff, INFO_LEN, fp)) {
		index = 0;
		tmp = data->title;
/*  get word */
		while (32 != buff[index])
			*tmp ++ = buff[index ++];
		*tmp = '\0';
		while (32 == buff[index])
			index ++;
		tmp = data->info;
/*  get info */
		while ('\n' != buff[index])
			*tmp ++ = buff[index ++];
		*tmp = '\0';
		
		db_insert(mydb, WORD_TABLE, data);
		count ++;
		
		if (!(count % 300)) {
#if LOADING_STYLE
			printf("completed [%.2f%%]\r", ftell(fp) / (double)file_size * 100);
			fflush(stdout);
#else
			putchar('.');
			fflush(stdout);
			if (!(count % 5000))
				putchar('\n');
#endif	/* LOADING_STYLE */
		}
	}
#if LOADING_STYLE
	puts("completed [100%].");
#else
	putchar('\n');
#endif
	free(data);
	printf("load %d words.\n", count);
	return count;
}

int _getfilesize_(const char *filename)
{
	struct stat stat_buf;

	if (-1 == stat(filename, &stat_buf))
		error_handler("getfilesize");
	return stat_buf.st_size;
}
