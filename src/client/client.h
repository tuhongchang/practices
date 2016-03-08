/*************************************************/
/*************************************************/
/*     this is a simple online dictionary        */
/*     use TCP and DATABASE  technology          */
/*     author:james.feng@farsight                     */
/*     version: 1.0-beta                         */
/*************************************************/
/*************************************************/

#include <dictionary_comm.h>
//#define SER_ADDR	""

extern int init_network(const char *serip);
extern int get_input(char *dest, int len);
extern int get_user_info(datapack_st *mesgbuff);
extern int submit_account(int sockfd, datapack_st *mesgbuff, int cmd);
extern int process_query(int sockfd, datapack_st *mesgbuff);
extern int user_funtion(int sockfd, datapack_st *mesgbuff);
extern int user_history(int sockfd, datapack_st *mesgbuff);
