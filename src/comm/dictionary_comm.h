/*************************************************/
/*************************************************/
/*     this is a simple online dictionary        */
/*     use TCP and DATABASE  technology          */
/*     author:james.feng@farsight                */
/*     version: 1.0-beta                         */
/*	   date: 2013-3								 */
/*************************************************/
/*************************************************/

#ifndef _COMM_H_
#define _COMM_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>

#define error_handler(ErrMsg)	\
	do{perror(ErrMsg);exit(EXIT_FAILURE);}while(0)
#define warning_handler(WarMsg)	\
	do{perror(WarMsg);}while(0)

#define NETDEV_NAME	"eth0"
#define SER_PORT	60001
#define LISTEN_QLEN	128

typedef struct sockaddr	SA;

#define TITLE_LEN	128
#define INFO_LEN	512
#define CMD_QUERY	'Q'
#define CMD_HISTORY	'H'
#define CMD_LOGIN	'I'
#define CMD_LOGOUT	'o'
#define CMD_REGIST	'r'
#define	CMD_ANNUL	'a'	
#define CMD_ERROR	-1

typedef struct _data_pack_{
	int type;
	char title[TITLE_LEN];
	char info[INFO_LEN];
}datapack_st;

#endif
