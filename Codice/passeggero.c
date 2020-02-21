#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defines.h"
#define MAX_DEGREES 128

struct average_grades {
	int latitudineI;
	int longitudineI;
	double avg;
};
float ftemp;
float ftemp2;

static void Timbra_biglietto(MYSQL* conn) {
	MYSQL_STMT* TimbraB;
	MYSQL_BIND param[3];
	int biglietto;
	int veicolo;
	int tratta;
	printf("Inserisci il numero del biglietto : ");
	scanf_s("%d", &biglietto);
	printf("Su quale veicolo ti trovi : ");
	scanf_s("%d", &veicolo);
	printf("Quale tratta stai percorrendo : ");
	scanf_s("%d", &tratta);

	if (!setup_prepared_stmt(&TimbraB, "call Timbra_biglietto(?, ?, ?)", conn)) {
		print_stmt_error(TimbraB, "Unable to initialize login statement\n");
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;  //IN
	param[0].buffer = &biglietto;
	param[0].buffer_length = sizeof(biglietto);

	param[1].buffer_type = MYSQL_TYPE_LONG;  //IN
	param[1].buffer = &tratta;
	param[1].buffer_length = sizeof(tratta);

	param[2].buffer_type = MYSQL_TYPE_LONG;  //IN
	param[2].buffer = &veicolo;
	param[2].buffer_length = sizeof(veicolo);

	if (mysql_stmt_bind_param(TimbraB, param) != 0) {
		finish_with_stmt_error(conn, TimbraB, "Could not bind parameters for career report\n", true);
	}
	// Run procedure
	if (mysql_stmt_execute(TimbraB) != 0) {
		print_stmt_error(TimbraB, "An error occurred while retrieving the career report.");
		goto out;
	}
	printf("Hai tibrato il bigletto [%d]\n", biglietto);
	system("pause");
	mysql_stmt_close(TimbraB);
	return;


out:
	mysql_stmt_close(TimbraB);
}
static void Convalida_abbonamento(MYSQL* conn) {
	MYSQL_STMT* TimbraA;
	MYSQL_BIND param[3];
	int abbonamento;
	int veicolo;
	int tratta;
	printf("Inserisci il numero dell'abbonamento : ");
	scanf_s("%d", &abbonamento);
	printf("Su quale veicolo ti trovi : ");
	scanf_s("%d", &veicolo);
	printf("Quale tratta stai percorrendo : ");
	scanf_s("%d", &tratta);

	if (!setup_prepared_stmt(&TimbraA, "call Convalida_abbonamento(?, ?, ?)", conn)) {
		print_stmt_error(TimbraA, "Unable to initialize login statement\n");
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;  //IN
	param[0].buffer = &abbonamento;
	param[0].buffer_length = sizeof(abbonamento);

	param[1].buffer_type = MYSQL_TYPE_LONG;  //IN
	param[1].buffer = &tratta;
	param[1].buffer_length = sizeof(tratta);

	param[2].buffer_type = MYSQL_TYPE_LONG;  //IN
	param[2].buffer = &veicolo;
	param[2].buffer_length = sizeof(veicolo);

	if (mysql_stmt_bind_param(TimbraA, param) != 0) {
		finish_with_stmt_error(conn, TimbraA, "Could not bind parameters for career report\n", true);
	}
	// Run procedure
	if (mysql_stmt_execute(TimbraA) != 0) {
		print_stmt_error(TimbraA, "An error occurred while retrieving the career report.");
		goto out;
	}
	printf("Hai convalidato l'abbonamento [%d]\n", abbonamento);
	system("pause");
	mysql_stmt_close(TimbraA);
	return;


out:
	mysql_stmt_close(TimbraA);
}
static size_t parse_avgs(MYSQL* conn, MYSQL_STMT* stmt, struct average_grades** ret)
{

	int status;
	size_t row = 0;
	MYSQL_BIND param[3];
	my_bool is_null;
	double avg;
	char latitudine[12];
	char longitudine[12];
	int veicolo;

	if (mysql_stmt_store_result(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	*ret = malloc(mysql_stmt_num_rows(stmt) * sizeof(struct average_grades));

	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = latitudine;
	param[0].buffer_length = 12;

	/*Devo restiturilo nella store procedure*/
	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = longitudine;
	param[1].buffer_length = 12;

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &veicolo;
	param[2].buffer_length = sizeof(veicolo);

	if (mysql_stmt_bind_result(stmt, param)) {
		finish_with_stmt_error(conn, stmt, "Unable to bind column parameters\n", true);
	}
	int i = 1;
	/* assemble course general information */
	while (true) {
		status = mysql_stmt_fetch(stmt);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		(*ret)[row].latitudineI= latitudine;
		(*ret)[row].longitudineI = longitudine;
		printf_s("La tua fermata si trova qui:\nLatitudine : %s	Longitudine : %s\n", latitudine, longitudine);

		/*VALORE DELLA FERMATA DOVE SI TROVANO*/
		 ftemp = atof(latitudine);
		 ftemp2 = atof(longitudine);
		row++;
	}

	return row;
}
static void Calcola_distanza_veicolo(MYSQL* conn) {
	MYSQL_STMT* Distanza;
	int status;
	MYSQL_BIND param[1];
	bool first = true;
	struct average_grades* avgs = NULL; /*DEVO TOGLIERE IL NULL MA MI DA ERRORE*/
	size_t longitudineI = 0;
	size_t latitudineI = 0;
	int Tratta;
	char header[512];

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&Distanza, "call Calcola_distanza_veicolo(?)", conn)) {
		finish_with_stmt_error(conn, Distanza, "Unable to initialize career report statement\n", false);
	}

	printf("A quale fermata ti trovi ? \n");
	printf("SCELTA: ");
	scanf_s("%d", &Tratta);
	printf("\n\n\n");


	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;  //IN
	param[0].buffer = &Tratta;
	param[0].buffer_length = sizeof(Tratta);



	if (mysql_stmt_bind_param(Distanza, param) != 0) {
		finish_with_stmt_error(conn, Distanza, "Could not bind parameters for career report\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(Distanza) != 0) {
		print_stmt_error(Distanza, "An error occurred while retrieving the career report.");
		goto out;
	}

	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if (conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}

		if (first) {
			parse_avgs(conn, Distanza, &avgs);
			first = false;
		}
		else {
			sprintf_s(header, 512, "\nI veicoli si trovano: " ,avgs[latitudineI].latitudineI, avgs[longitudineI].longitudineI);
			dump_result_set(conn, Distanza, header);

			latitudineI++;
			longitudineI++;
			
		}
		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	next:
		status = mysql_stmt_next_result(Distanza);
		if (status > 0)
			finish_with_stmt_error(conn, Distanza, "Unexpected condition", true);

	} while (status == 0);

out:
	mysql_stmt_close(Distanza);
}

void run_as_passeggero(MYSQL* conn)
{
	int s;
	int i = 0;
	int numero;
	while (true) {

		printf("------------------------------*** Cosa vuoi fare? ***------------------------------\n\n");
		printf("1) Timbra un biglietto\n");
		printf("2) Convalida un abbonamento\n");
		printf("3) Calcola distanza veicolo\n");
		printf("4) Logout\n");
		printf("SCELTA: ");
		scanf_s("%i", &numero);
		switch (numero)
		{
		case 1:
			printf("---------------------------Timbra un biglietto--------------------------------------------\n");
			Timbra_biglietto(conn);
			break;
		case 2:
			printf("---------------------------------------------Convalida un abbonamento---------------------------------------------\n");
			Convalida_abbonamento(conn);
			break;
		case 3:
			printf("---------------------------------------------Calcola distanza veicolo--------------------------------------------\n");
			Calcola_distanza_veicolo(conn);
			float lat;
			float lon;
			printf("\n\n Inserisci i dati del veicolo di interesse \n");
			printf("Latitudine: ");
			scanf_s("%f", &lat);
			printf("Longitudine: ");
			scanf_s("%f", &lon);
			float risultato;
			risultato = 2 * 6371 * asin(sqrt(((sin(lat - ftemp) / 2) * (sin(lat - ftemp) / 2)) + cos(ftemp) * cos(lat) * (sin((lon - ftemp2) / 2) * sin((lon - ftemp2) / 2))));
			printf("\nIl veicolo si trova a %f Km di distanza\n risultato ");
			break;
		case 4:
			printf("----------------------------------------Logout----------------------------------------\n");
			return;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			return;
		}



	}
	
}