
int main()
{
	network_init();
	database_init();

	pthread_create(TCP_LOGIN);
	pthread_create(UDP_SEARCH);

	pthread_join();
	pthread_join();
}

thread_login()
{
	while (1) {
		accept();
		recv();
		process_login();
		send();
		close();
	}
}

thread_service()
{
	while (1) {
		recvfrom();
		search_data();
		sendto();
	}
}
