#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#include "defines.h"

typedef enum {
	AMMINISTRATORE = 1,
	CONDUCENTE=2,
	PASSEGGERO=3,
} role_t;


struct configuration conf;

static MYSQL* conn;


static role_t attempt_login(MYSQL* conn, char* username, char* password)
{
	MYSQL_STMT *login_procedure;

	MYSQL_BIND param[3]; // Used both for input and output
	int role = 0;

	if (!setup_prepared_stmt(&login_procedure, "call Login(?, ?, ?)", conn)) {
		print_stmt_error(login_procedure, "Unable to initialize login statement\n");
		
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param[1].buffer = password;
	param[1].buffer_length = strlen(password);

	param[2].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[2].buffer = &role;
	param[2].buffer_length = sizeof(role);

	if (mysql_stmt_bind_param(login_procedure, param) != 0) { // Note _param
		print_stmt_error(login_procedure, "Could not bind parameters for login");
		goto err;
	}

	// Run procedure
	if (mysql_stmt_execute(login_procedure) != 0) {
		print_stmt_error(login_procedure, "Could not execute login procedure");
		goto err;
	}

	// Prepare output parameters
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[0].buffer = &role;
	param[0].buffer_length = sizeof(role);
	if (mysql_stmt_bind_result(login_procedure, param)) {
		print_stmt_error(login_procedure, "Could not retrieve output parameter");
		goto err;
	}

	// Retrieve output parameter
	if (mysql_stmt_fetch(login_procedure)) {
		print_stmt_error(login_procedure, "Could not buffer results");
		goto err;
	}

	mysql_stmt_close(login_procedure);
	return role;

err:
	mysql_stmt_close(login_procedure);

}


main(void) {
	printf("/**************	Benevenuto nel nuovo sistema di trasporto pubblico di Ivan Palmieri	**************/\n");
	role_t role;
	conn = mysql_init(NULL);
	if (conn == NULL) {
		fprintf(stderr, "mysql_init() failed (probably out of memory)\n");
		exit(EXIT_FAILURE);
	}
	/*si connette*/
	if (mysql_real_connect(conn, "localhost", "root", conf.db_password, "sistematrasportopubblico3", conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
		fprintf(stderr, "mysql_real_connect() failed\n");
		mysql_close(conn);
		exit(EXIT_FAILURE);
	}
	char us[25];
	char pass[25];
	int choise;
	while (true)
	{
		printf("*** Cosa vuoi fare? *** \n\n");
		printf("1) Login\n");
		printf("2) Quit\n");
		scanf_s("%i", &choise);
		switch (choise) 
		{
		case 1:
			printf("-----------------------Login----------------------\n");
			printf("Username: ");
			scanf_s("%s", &us);
			printf("Password: ");
			scanf_s("%s", &pass);
			printf("----------------------------------------------------------------\n");
			printf("Hai inserito [%s %s]\n", us, pass);
			role = attempt_login(conn, us, pass);
			switch (role) {
			case PASSEGGERO:
				printf("Connessione riuscita come passeggero\n");
				run_as_passeggero(conn);
				break;

			case CONDUCENTE:
				printf("Connessione riuscita come guidatore\n");
				run_as_guidatore(conn);
				break;

			case AMMINISTRATORE:
				printf("Connessione riuscita come amministratore\n");
				run_as_administrator(conn);
				break;
			default:
				printf("Credenziali sbagliate\n");
				
			}
		case 2:
			printf("-------------------------------------------\n");
			printf("Bye!\n");
			system("pause");
			mysql_close(conn);
			return 0;
		}
		


	}
}