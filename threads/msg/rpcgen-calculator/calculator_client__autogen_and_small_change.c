/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calculator.h"

void calculator_prog_1(char* host) {
	CLIENT* clnt;
	double* result_1;
	operands add_1_arg;
	double* result_2;
	operands subtract_1_arg;
	double* result_3;
	operands multiply_1_arg;
	double* result_4;
	operands divide_1_arg;

#ifndef DEBUG
	clnt = clnt_create(host, CALCULATOR_PROG, CALCULATOR_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}
#endif /* DEBUG */

	add_1_arg.a = 1.0;
	add_1_arg.b = 2.0;
	result_1 = add_1(&add_1_arg, clnt);
	printf("baked in result: %f\n", *result_1);

	if (result_1 == (double*)NULL) {
		clnt_perror(clnt, "call failed");
	}
	result_2 = subtract_1(&subtract_1_arg, clnt);
	if (result_2 == (double*)NULL) {
		clnt_perror(clnt, "call failed");
	}
	result_3 = multiply_1(&multiply_1_arg, clnt);
	if (result_3 == (double*)NULL) {
		clnt_perror(clnt, "call failed");
	}
	result_4 = divide_1(&divide_1_arg, clnt);
	if (result_4 == (double*)NULL) {
		clnt_perror(clnt, "call failed");
	}
#ifndef DEBUG
	clnt_destroy(clnt);
#endif /* DEBUG */
}

int main(int argc, char* argv[]) {
	char* host;

	if (argc < 2) {
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}
	host = argv[1];
	calculator_prog_1(host);
	exit(0);
}
