#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#define MAX_DEGREES 128

static void Calcola_prossima_tratta_veicolo(MYSQL* conn)
{
	MYSQL_STMT *distanza_stmt;
	MYSQL_BIND param[2];
	printf("Che veicolo stai guidando : ");
	int veicolo;
	scanf_s("%i", &veicolo);
	int tratta;
	if (!setup_prepared_stmt(&distanza_stmt, "call Calcola_prossima_tratta_veicolo(?, ?)", conn)) {
		print_stmt_error(distanza_stmt, "Unable to initialize login statement\n");
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_LONG;  //IN
	param[0].buffer = &veicolo;
	param[0].buffer_length = sizeof(veicolo);
	
	param[1].buffer_type = MYSQL_TYPE_LONG;  //OUT
	param[1].buffer = &tratta;
	param[1].buffer_length = sizeof(tratta);

	if (mysql_stmt_bind_param(distanza_stmt, param) != 0) {
		finish_with_stmt_error(conn, distanza_stmt, "Could not bind parameters for career report\n", true);
	}
	// Run procedure
	if (mysql_stmt_execute(distanza_stmt) != 0) {
		print_stmt_error(distanza_stmt, "An error occurred while retrieving the career report.");
		goto out;
	}
	// Prepare output parameters
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[0].buffer = &tratta;
	param[0].buffer_length = sizeof(tratta);

	if (mysql_stmt_bind_result(distanza_stmt, param)) {
		print_stmt_error(distanza_stmt, "Could not retrieve output parameter");
		
	}

	// Retrieve output parameter
	if (mysql_stmt_fetch(distanza_stmt)) {
		print_stmt_error(distanza_stmt, "Could not buffer results");
		
	}
	printf("La prossima tratta del veicolo %i e' la  %i\n", veicolo,tratta);
	system("pause");
	mysql_stmt_close(distanza_stmt);
	return;
	

	out:
	mysql_stmt_close(distanza_stmt);

}

void run_as_guidatore(MYSQL* conn)
{
	int s;
	int i = 0;
	int numero;
	while (true) {

		printf("-------------------------------------------------*** Cosa vuoi fare? ***-------------------------------------------------\n\n");
		printf("1) Calcola prossima tratta del veicolo\n");
		printf("2) Quit\n");
		scanf_s("%i", &numero);
		switch (numero) 
		{
		case 1:
			printf("----------------------------------Calcola la prossima tratta del veicolo---------------------------------------------\n");
			Calcola_prossima_tratta_veicolo(conn);
			break;
		case 2:
			printf("---------------------------------------------------------Quit---------------------------------------------------------\n");
			return;
		
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			return;
		}

		
		
	}
}
