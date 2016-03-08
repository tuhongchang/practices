/*************************************************/
/*************************************************/
/*     this is a simple online dictionary        */
/*     use TCP and DATABASE  technology          */
/*     author:james.feng@farsight                     */
/*     version: 1.0-beta                         */
/*************************************************/
/*************************************************/


#include <dictionary_comm.h>
#include <netdevice.h>
#include <sqlite3.h>
#include <signal.h>

//#define DEBUG	1

#define DICT_FILE	"dictionary.txt"
#define DB_PATH		"dictionary.db"
#define PSWD_TABLE	"SYSUSER"
#define WORD_TABLE	"WORD"
#define HIST_TABLE	"HIST"

#define LOADING_STYLE	1

// database demo:
// title | info
// table list:
// SYSUSER , WORD, user1, user2, usern ... 

typedef struct _user_status_{
	int status;
	char name[TITLE_LEN];
}userstatus_st;

extern userstatus_st us;

extern sqlite3 *sys_init_database(void);
extern int db_create(sqlite3 *mydb, char *table);
extern int db_insert(sqlite3 *mydb, char *table, datapack_st *data);
extern int db_query_data(sqlite3 *mydb, char *table, datapack_st *data);
extern int db_query_table(sqlite3 *mydb, char *table, datapack_st *data);
extern int db_load_data(sqlite3 *mydb);
extern int select_callback(void *arg, int nu, char **value, char **name);


extern int sys_init_networks(void);
extern int word_query(int connfd, datapack_st *mesgbuff, sqlite3 *mydb);
extern int user_history(int connfd, datapack_st *mesgbuff, sqlite3 *mydb);
extern int user_register(int connfd, datapack_st *mesgbuff, sqlite3 *mydb);
extern int user_login(int connfd, datapack_st *mesgbuff, sqlite3 *mydb);
extern int user_logout(int connfd, datapack_st *mesgbuff, sqlite3 *mydb);
extern int user_annul(int connfd, datapack_st *mesgbuff, sqlite3 *mydb);
