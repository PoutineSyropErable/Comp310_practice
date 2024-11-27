#include "time_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	CLIENT* client;
	char* server_host;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <server_host>\n", argv[0]);
		exit(1);
	}

	server_host = argv[1];

	// Create an RPC client
	client = clnt_create(server_host, TIMESERVICE_PROG, TIMESERVICE_VERS, "udp");
	if (client == NULL) {
		clnt_pcreateerror(server_host);
		exit(1);
	}

	// Call get_time
	char** time_result = get_time_1(NULL, client);
	if (time_result == NULL) {
		clnt_perror(client, "get_time call failed");
		exit(1);
	}
	printf("Server responded: %s\n", *time_result);

	// Call set_time
	const char* time_to_set = "2024-11-24 16:30:00";
	if (set_time_1((char**)&time_to_set, client) == NULL) {
		printf("Set time completed successfully.\n");
	}

	// Cleanup
	clnt_destroy(client);
	return 0;
}
