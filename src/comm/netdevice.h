#ifndef __HEAD_H__
#define __HEAD_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>

extern int getipaddr(const char * devname, struct sockaddr * addr_buff);
extern int gethwaddr(const char * devname, struct sockaddr * addr_buff);
extern int getifindex(const char *devname);


#endif
