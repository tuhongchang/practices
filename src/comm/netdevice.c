#include <netdevice.h>

int getipaddr(const char *devname, struct sockaddr * addr_buff)
{
	struct ifreq ifreq_buf[4];
	struct ifconf ifconf_buf;
	int sockfd;
	int index;
	int max;
	int retval;

	ifconf_buf.ifc_len = sizeof(ifreq_buf);
	ifconf_buf.ifc_req = ifreq_buf;

	if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0))) {
		retval = -1; goto goto_return;
	}

	if (-1 == ioctl(sockfd, SIOCGIFCONF, &ifconf_buf)) {
		retval = -1; goto goto_return;
	}
			
	max = ifconf_buf.ifc_len / sizeof(*ifreq_buf);

	for (index = 0; index < max; index ++) {
		if (!strcmp(devname, ifreq_buf[index].ifr_name)) {
			*addr_buff = ifreq_buf[index].ifr_addr;
			retval = index;
			goto goto_return;
		}
	}
	retval = -1;

goto_return:
	close(sockfd);
	return retval;
}

int gethwaddr(const char *devname, struct sockaddr * addr_buff)
{
	struct ifreq ifreq_buf[4];
	struct ifconf ifconf_buf;
	int sockfd;
	int index;
	int max;
	int retval;

	ifconf_buf.ifc_len = sizeof(ifreq_buf);
	ifconf_buf.ifc_req = ifreq_buf;

	if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0))) {
		retval = -1; goto goto_return;
	}

	if (-1 == ioctl(sockfd, SIOCGIFCONF, &ifconf_buf)) {
		retval = -1; goto goto_return;
	}
			
	max = ifconf_buf.ifc_len / sizeof(*ifreq_buf);

	for (index = 0; index < max; index ++) {
		if (strcmp(devname, ifreq_buf[index].ifr_name))
			continue;

		if (-1 == ioctl(sockfd, SIOCGIFHWADDR, &ifreq_buf[index])) {
			retval = 1; goto goto_return;
		}
		*addr_buff = ifreq_buf[index].ifr_hwaddr;
		retval = index;
		goto goto_return;
	}
	retval = -1;

goto_return:
	close(sockfd);
	return retval;
}

int getifindex(const char *devname)
{
	struct ifreq ifreq_buf;
	int sockfd;
	int retval;

	if (-1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0))) {
		retval = -1; goto goto_return;
	}

	strcpy(ifreq_buf.ifr_name, devname);
	if (-1 == ioctl(sockfd, SIOCGIFINDEX, &ifreq_buf)) {
		retval = -1; goto goto_return;
	}
			
	retval = ifreq_buf.ifr_ifindex;
goto_return:
	close(sockfd);
	return retval;
}
