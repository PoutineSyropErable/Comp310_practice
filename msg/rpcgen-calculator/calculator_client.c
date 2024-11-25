#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//???
char* host;

typedef enum {
	OP_ADD,
	OP_SUB,
	OP_MULT,
	OP_DIV,
} Operation;

double c_add(double num1, double num2);
double c_sub(double num1, double num2);
double c_mult(double num1, double num2);
double c_div(double num1, double num2);

double calculator_prog_1(Operation operation, double num1, double num2) {
	CLIENT* clnt;
	double* result;
	operands ops;

	// Create client handle
	clnt = clnt_create(host, CALCULATOR_PROG, CALCULATOR_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	// Prepare operands
	ops.a = num1;
	ops.b = num2;

	switch (operation) {
	case OP_ADD:
		result = add_1(&ops, clnt);
		break;

	case OP_SUB:
		result = subtract_1(&ops, clnt);
		break;

	case OP_MULT:
		result = multiply_1(&ops, clnt);
		break;

	case OP_DIV:
		result = divide_1(&ops, clnt);
		break;
	}

	// Check for RPC call failure
	if (result == NULL) {
		clnt_perror(clnt, "RPC call failed");
	} else {
		/* printf("Result: %.2f\n", *result); */
	}

	// Clean up
	clnt_destroy(clnt);

	return *result;
}

int main(int argc, char* argv[]) {
	if (argc != 5) {
		fprintf(stderr, "Usage: %s <server_host> <operation> <num1> <num2>\n", argv[0]);
		exit(1);
	}

	host = argv[1];
	char* operation_input = argv[2];
	double num1 = atof(argv[3]);
	double num2 = atof(argv[4]);

	Operation operation;

	// Determine the operation and call the corresponding remote procedure
	if (strcmp(operation_input, "add") == 0) {
		operation = OP_ADD;
	} else if (strcmp(operation_input, "sub") == 0) {
		operation = OP_SUB;
	} else if (strcmp(operation_input, "mult") == 0) {
		operation = OP_MULT;
	} else if (strcmp(operation_input, "div") == 0) {
		operation = OP_DIV;
	} else {
		fprintf(stderr, "Invalid operation. Use add, sub, mult, or div.\n");
		exit(1);
	}

	double sys_result = calculator_prog_1(operation, num1, num2);

	printf("the result is %f\n", sys_result);

	double add_result = c_add(1.0, 2.0);
	double sub_result = c_sub(1.0, 1.0);
	double mult_result = c_mult(2.0, 3.0);
	double div_result = c_div(6.0, 2.0);

	printf("\na:%f s:%f m:%f d:%f\n", add_result, sub_result, mult_result, div_result);

	return 0;
}

double c_add(double num1, double num2) {
	CLIENT* clnt;
	double* result;
	operands args;

	// Create client
	clnt = clnt_create(host, CALCULATOR_PROG, CALCULATOR_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	// Prepare arguments
	args.a = num1;
	args.b = num2;

	// Call add RPC
	result = add_1(&args, clnt);
	if (result == NULL) {
		clnt_perror(clnt, "RPC call failed");
	} else {
		/* printf("Result of addition: %.2f\n", *result); */
	}

	// Cleanup
	clnt_destroy(clnt);
	return *result;
}

double c_sub(double num1, double num2) {
	CLIENT* clnt;
	double* result;
	operands args;

	// Create client
	clnt = clnt_create(host, CALCULATOR_PROG, CALCULATOR_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	// Prepare arguments
	args.a = num1;
	args.b = num2;

	// Call subtract RPC
	result = subtract_1(&args, clnt);
	if (result == NULL) {
		clnt_perror(clnt, "RPC call failed");
	} else {
		/* printf("Result of subtraction: %.2f\n", *result); */
	}

	// Cleanup
	clnt_destroy(clnt);
	return *result;
}

double c_mult(double num1, double num2) {
	CLIENT* clnt;
	double* result;
	operands args;

	// Create client
	clnt = clnt_create(host, CALCULATOR_PROG, CALCULATOR_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	// Prepare arguments
	args.a = num1;
	args.b = num2;

	// Call multiply RPC
	result = multiply_1(&args, clnt);
	if (result == NULL) {
		clnt_perror(clnt, "RPC call failed");
	} else {
		/* printf("Result of multiplication: %.2f\n", *result); */
	}

	// Cleanup
	clnt_destroy(clnt);
	return *result;
}

double c_div(double num1, double num2) {
	CLIENT* clnt;
	double* result;
	operands args;

	// Create client
	clnt = clnt_create(host, CALCULATOR_PROG, CALCULATOR_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror(host);
		exit(1);
	}

	// Prepare arguments
	args.a = num1;
	args.b = num2;

	// Call divide RPC
	result = divide_1(&args, clnt);
	if (result == NULL) {
		clnt_perror(clnt, "RPC call failed");
	} else {
		/* printf("Result of division: %.2f\n", *result); */
	}

	// Cleanup
	clnt_destroy(clnt);
	return *result;
}
