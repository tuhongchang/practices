#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#define error_exit(_errmsg_) error(EXIT_FAILURE, errno, _errmsg_)
#define BUFF_SIZE	1024

struct word {
	char name[32];
	char explan[300];
};

void parse_word(char *buff, struct word *wordbuf);

int main(int argc, const char **argv)
{
	const char *srcname = argv[1];
	const char *dbname = argv[2];
	FILE *fp;
	sqlite3 *db;
	char buff[BUFF_SIZE];
	char *p;
	struct word wordbuf;
	char *errmsg;
	int n = 0;
	int tsize;
	int csize;
	
	if (argc != 3) {
		puts("USAGE:");
		puts("./loaddatabase srcfile databasefile");
		return -1;
	}

	if (NULL == (fp = fopen(srcname, "r")))
		error_exit("fopen");

	/* open database */
	if (SQLITE_OK != sqlite3_open(dbname, &db)) {
		fprintf(stderr, "sqlite3_open: %s\n", sqlite3_errmsg(db));
		exit(EXIT_FAILURE);
	}

	/* create table */
	if (SQLITE_OK != sqlite3_exec(db, "CREATE TABLE word (word char, explan char)",
			NULL, NULL, &errmsg)) {
		fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
		sqlite3_free(errmsg);
		exit(EXIT_FAILURE);
	}

	fseek(fp, 0, SEEK_END);
	tsize = ftell(fp);
	rewind(fp);

	while (NULL != fgets(buff, BUFF_SIZE, fp)) {
		
		parse_word(buff, &wordbuf);
		sprintf(buff, "INSERT INTO word VALUES (\"%s\", \"%s\")", wordbuf.name, wordbuf.explan);

		if (SQLITE_OK != sqlite3_exec(db, buff, NULL, NULL, &errmsg)) {
			fprintf(stderr, "sqlite3_exec:%s\n", errmsg);
			sqlite3_free(errmsg);
			break;
		}
		n++;

		csize = ftell(fp);
		printf("%%%.2f\r", csize / (float)tsize * 100);
//		fflush(stdout);
	}

	printf("\r%d\n", n);

	fclose(fp);
	sqlite3_close(db);

	return 0;
}

void parse_word(char *buff, struct word *wordbuf)
{
	char *p = buff;
	int i = 0;

	while (*p != ' ')
		wordbuf->name[i++] = *p++;
	wordbuf->name[i] = '\0';

	while (*p == ' ')
		p++;

	i = 0;
	while (*p != '\r')
		wordbuf->explan[i++] = *p++;
	wordbuf->explan[i] = '\0';
}
