/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "calculator.h"


void
calculate_prog_1(char *host, int operation, float operand1, float operand2)
{
	CLIENT *clnt;
	float  *result_1;
	operands  calculate_1_arg;

#ifndef	DEBUG
	printf("\n\tCreate connection to server");
	clnt = clnt_create (host, CALCULATE_PROG, CALCULATE_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	//assign parameter values to be passed
	calculate_1_arg.operation=operation;
	calculate_1_arg.op1=operand1;
	calculate_1_arg.op2=operand2;
	printf("\n\tCall calculator funtion on server");

	result_1 = calculate_1(&calculate_1_arg, clnt);
	if (result_1 == (float *) NULL) {
		clnt_perror (clnt, "server call failed");
	}
	else
	{
		printf("\n\tReceive reply from server");
		printf("\n\tAnswer is %f\n", *result_1);
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;
	int opr,ch;
	float myop1, myop2;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	
	do{
		printf("\nThis is a calculator with the following operations");
		printf("\n\tEnter 1 for addition");
		printf("\n\tEnter 2 for subtraction");
		printf("\n\tEnter 3 for multiplication");
		printf("\n\tEnter 4 for division");
		printf("\n\n\tSelect your operation:: ");
		scanf("%d", &opr);
		printf("\n\tEnter first operand:: ");
		scanf("%f", &myop1);
		printf("\n\tEnter second operand:: ");
		scanf("%f", &myop2);
		printf("\n\tCalling calculator function stub");
		calculate_prog_1 (host, opr, myop1, myop2);
		printf("\nTry again: (1- Yes/0- No) :: ");
		scanf("%d",&ch);
	}while(ch==1);
exit (0);
}