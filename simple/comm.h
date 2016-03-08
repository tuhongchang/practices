#ifndef __HEAD_H__
#define __HEAD_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SER_PORT	10000
#define SER_ADDR	"192.168.3.10"


#define DB_TABLE_NAME	"word"
#define QMESG_WORD_LEN	30
#define QMESG_EXPLAN_LEN	300

#define QMESG_FLAG_REQ	'R'
#define QMESG_FLAG_ACK	'A'
#define QMESG_FLAG_ERR	'E'
typedef struct query_protocol {
	int flag;
	char word[QMESG_WORD_LEN];
	char explan[QMESG_EXPLAN_LEN];
}qmesg_t;

#endif /*__HEAD_H__*/
