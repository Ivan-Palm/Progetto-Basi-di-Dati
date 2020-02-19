#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#define MAX_DEGREES 128

struct average_grades {
	char CF[16];
	double avg;
};
struct average_gradess {
	int Matricola;
	double avg;
};

static void Assegna_turno_al_conducente(MYSQL* conn) {
	MYSQL_STMT* distanza_stmt;
	MYSQL_BIND param[4];
}
static void Assegna_veicolo_alla_tratta(MYSQL* conn) {
	MYSQL_STMT* distanza_stmt;
	MYSQL_BIND param[4];
}
static void Assumi_conducente(MYSQL* conn) {
	MYSQL_STMT* distanza_stmt;
	MYSQL_BIND param[7];
}
static void Cambia_conducente_turno(MYSQL* conn) {
	MYSQL_STMT* distanza_stmt;
	MYSQL_BIND param[4];
}
static size_t parse_avgs(MYSQL* conn, MYSQL_STMT* stmt, struct average_grades** ret)
{
	
	int status;
	size_t row = 0;
	MYSQL_BIND param[2];
	my_bool is_null;
	double avg;
	char CF[16];

	if (mysql_stmt_store_result(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	*ret = malloc(mysql_stmt_num_rows(stmt) * sizeof(struct average_grades));

	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = CF;
	param[0].buffer_length = 16;

	/*Devo restiturilo nella store procedure*/
	param[1].buffer_type = MYSQL_TYPE_DOUBLE;
	param[1].buffer = &avg;
	param[1].buffer_length = sizeof(avg);

	if (mysql_stmt_bind_result(stmt, param)) {
		finish_with_stmt_error(conn, stmt, "Unable to bind column parameters\n", true);
	}
	int i = 1;
	printf("Conducenti: \n");
	/* assemble course general information */
	while (true) {
		status = mysql_stmt_fetch(stmt);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		strcpy_s((*ret)[row].CF,16, CF);
		printf_s("%d) Conducente : %s\n",i,CF);
		i++;
		row++;
	}

	return row;
}
static size_t parse_avgss(MYSQL* conn, MYSQL_STMT* stmt, struct average_gradess** ret)
{

	int status;
	size_t row = 0;
	MYSQL_BIND param[2];
	my_bool is_null;
	double avg;
	int Matricola;

	if (mysql_stmt_store_result(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	*ret = malloc(mysql_stmt_num_rows(stmt) * sizeof(struct average_gradess));

	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = &Matricola;
	param[0].buffer_length = 16;

	/*Devo restiturilo nella store procedure*/
	param[1].buffer_type = MYSQL_TYPE_DOUBLE;
	param[1].buffer = &avg;
	param[1].buffer_length = sizeof(avg);

	if (mysql_stmt_bind_result(stmt, param)) {
		finish_with_stmt_error(conn, stmt, "Unable to bind column parameters\n", true);
	}
	int i = 1;
	printf("Veicoli: \n");
	/* assemble course general information */
	while (true) {
		status = mysql_stmt_fetch(stmt);

		if (status == 1 || status == MYSQL_NO_DATA)
			break;

		(*ret)[row].Matricola = Matricola;
		printf_s("%d) Veicolo : %d\n", i, Matricola);
		i++;
		row++;
	}

	return row;
}
static void Visualizza_conducenti_attivi(MYSQL* conn) {

	MYSQL_STMT* Visualizza_conducenti;
	int status;
	bool first = true;
	struct average_grades *avgs=NULL; /*DEVO TOGLIERE IL NULL MA MI DA ERRORE*/
	size_t conducenti = 0;
	char header[512];

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&Visualizza_conducenti, "call Visualizza_conducenti_attivi()", conn)) {
		finish_with_stmt_error(conn, Visualizza_conducenti, "Unable to initialize career report statement\n", false);
	}

	

	// Run procedure
	if (mysql_stmt_execute(Visualizza_conducenti) != 0) {
		print_stmt_error(Visualizza_conducenti, "An error occurred while retrieving the career report.");
		goto out;
	}

	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if (conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}

		if (first) {
			parse_avgs(conn, Visualizza_conducenti, &avgs);
			first = false;
		}
		else {
			sprintf_s(header,512, "\nConducente: %s", avgs[conducenti].CF);
			dump_result_set(conn, Visualizza_conducenti, header);
			conducenti++;
		}

		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	next:
		status = mysql_stmt_next_result(Visualizza_conducenti);
		if (status > 0)
			finish_with_stmt_error(conn, Visualizza_conducenti, "Unexpected condition", true);

	} while (status == 0);

out:
	mysql_stmt_close(Visualizza_conducenti);

}
static void Visualizza_conducenti_fermi(MYSQL* conn) {

	MYSQL_STMT* Visualizza_conducenti_f;
	int status;
	bool first = true;
	struct average_grades* avgs = NULL; /*DEVO TOGLIERE IL NULL MA MI DA ERRORE*/
	size_t conducenti = 0;
	char header[512];

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&Visualizza_conducenti_f, "call Visualizza_conducenti_fermi()", conn)) {
		finish_with_stmt_error(conn, Visualizza_conducenti_f, "Unable to initialize career report statement\n", false);
	}



	// Run procedure
	if (mysql_stmt_execute(Visualizza_conducenti_f) != 0) {
		print_stmt_error(Visualizza_conducenti_f, "An error occurred while retrieving the career report.");
		goto out;
	}

	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if (conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}

		if (first) {
			parse_avgs(conn, Visualizza_conducenti_f, &avgs);
			first = false;
		}
		else {
			sprintf_s(header, 512, "\nConducente: %s", avgs[conducenti].CF);
			dump_result_set(conn, Visualizza_conducenti_f, header);
			conducenti++;
		}

		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	next:
		status = mysql_stmt_next_result(Visualizza_conducenti_f);
		if (status > 0)
			finish_with_stmt_error(conn, Visualizza_conducenti_f, "Unexpected condition", true);

	} while (status == 0);

out:
	mysql_stmt_close(Visualizza_conducenti_f);

}
static void Visualizza_veicoli_attivi(MYSQL* conn) {
	MYSQL_STMT* Visualizza_veicoli;
	int status;
	bool first = true;
	struct average_gradess* avgs = NULL; 
	size_t veicoli = 0;
	char header[512];

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&Visualizza_veicoli, "call Visualizza_veicoli_attivi()", conn)) {
		finish_with_stmt_error(conn, Visualizza_veicoli, "Unable to initialize career report statement\n", false);
	}



	// Run procedure
	if (mysql_stmt_execute(Visualizza_veicoli) != 0) {
		print_stmt_error(Visualizza_veicoli, "An error occurred while retrieving the career report.");
		goto out;
	}

	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if (conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}

		if (first) {
			parse_avgss(conn, Visualizza_veicoli, &avgs);
			first = false;
		}
		else {
			sprintf_s(header, 512, "\nVeicolo: %i", avgs[veicoli].Matricola);
			dump_result_set(conn, Visualizza_veicoli, header);
			veicoli++;
		}

		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	next:
		status = mysql_stmt_next_result(Visualizza_veicoli);
		if (status > 0)
			finish_with_stmt_error(conn, Visualizza_veicoli, "Unexpected condition", true);

	} while (status == 0);

out:
	mysql_stmt_close(Visualizza_veicoli);

}
static void Visualizza_veicoli_fermi(MYSQL* conn) {
	MYSQL_STMT* Visualizza_veicoli_f;
	int status;
	bool first = true;
	struct average_gradess* avgs = NULL;
	size_t veicoli = 0;
	char header[512];

	// Prepare stored procedure call
	if (!setup_prepared_stmt(&Visualizza_veicoli_f, "call Visualizza_veicoli_fermi()", conn)) {
		finish_with_stmt_error(conn, Visualizza_veicoli_f, "Unable to initialize career report statement\n", false);
	}



	// Run procedure
	if (mysql_stmt_execute(Visualizza_veicoli_f) != 0) {
		print_stmt_error(Visualizza_veicoli_f, "An error occurred while retrieving the career report.");
		goto out;
	}

	// We have multiple result sets here!
	do {
		// Skip OUT variables (although they are not present in the procedure...)
		if (conn->server_status & SERVER_PS_OUT_PARAMS) {
			goto next;
		}

		if (first) {
			parse_avgss(conn, Visualizza_veicoli_f, &avgs);
			first = false;
		}
		else {
			sprintf_s(header, 512, "\nVeicolo: %i", avgs[veicoli].Matricola);
			dump_result_set(conn, Visualizza_veicoli_f, header);
			veicoli++;
		}

		// more results? -1 = no, >0 = error, 0 = yes (keep looking)
	next:
		status = mysql_stmt_next_result(Visualizza_veicoli_f);
		if (status > 0)
			finish_with_stmt_error(conn, Visualizza_veicoli_f, "Unexpected condition", true);

	} while (status == 0);

out:
	mysql_stmt_close(Visualizza_veicoli_f);
}
static void Elimina_conducente(MYSQL* conn) {
	MYSQL_STMT* Elimina;
	MYSQL_BIND param[2];
	char codice[16];
	char patente[10];
	printf("Inserisci il codice fiscale : ");
	scanf_s("%s", codice);
	printf("Inserisci il numero della patente : ");
	scanf_s("%s", patente);
	if (!setup_prepared_stmt(&Elimina, "call Elimina_conducente(?, ?)", conn)) {
		print_stmt_error(Elimina, "Unable to initialize login statement\n");
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[0].buffer = codice;
	param[0].buffer_length = 16;

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[1].buffer = patente;
	param[1].buffer_length = 10;

	if (mysql_stmt_bind_param(Elimina, param) != 0) {
		finish_with_stmt_error(conn, Elimina, "Could not bind parameters for career report\n", true);
	}
	// Run procedure
	if (mysql_stmt_execute(Elimina) != 0) {
		print_stmt_error(Elimina, "An error occurred while retrieving the career report.");
		goto out;
	}


	// Retrieve output parameter
	if (mysql_stmt_fetch(Elimina)) {
		print_stmt_error(Elimina, "Could not buffer results");

	}
	printf("Hai eliminato | CF: %s   Patente: %s\n", codice, patente);
	system("pause");
	mysql_stmt_close(Elimina);
	return;


out:
	mysql_stmt_close(Elimina);

}
static void Emetti_biglietto(MYSQL* conn) {
	MYSQL_STMT* distanza_stmt;
}
static void Emetti_abbonamenti(MYSQL* conn) {
	MYSQL_STMT* distanza_stmt;
}

void run_as_administrator(MYSQL* conn)
{
	int s;
	int i = 0;
	int numero;
	while (true) {

		printf("------------------------------*** Cosa vuoi fare? ***------------------------------\n\n");
		printf("1) Assegna turno ad un conducente\n");
		printf("2) Assegna un veicolo ad una tratta\n");
		printf("3) Assumi un conducente\n");
		printf("4) Cambia turno ad un conducente\n");
		printf("5) Visualizza conducenti attivi\n");
		printf("6) Visualizza conducenti fermi\n");
		printf("7) Visualizza veicoli attivi\n");
		printf("8) Visualizza veicoli fermi\n");
		printf("9)  Elimina un conducente\n");
		printf("10) Emetti biglietti\n");
		printf("11) Emetti abbonamenti\n");
		printf("12) Quit\n");
		scanf_s("%i", &numero);
		switch (numero)
		{
		case 1:
			printf("---------------------------Assegna_turno_al_conducente--------------------------------------------\n");
			Assegna_turno_al_conducente(conn);
			break;
		case 2:
			printf("---------------------------------------------Assegna_veicolo_alla_tratta---------------------------------------------\n");
			Assegna_veicolo_alla_tratta(conn);
			return;
		case 3:
			printf("---------------------------------------------Assumi_conducente-------------------------------------------\n");
			Assumi_conducente(conn);
			return;
		case 4:
			printf("---------------------------------------------Cambia_conducente_turno-------------------------------------------\n");
			Cambia_conducente_turno(conn);
			return;
		case 5:
			printf("---------------------------------------------Visualizza_conducenti_attivi-------------------------------------------\n");
			Visualizza_conducenti_attivi(conn);
			return;
		case 6:
			printf("---------------------------------------------Visualizza_conducenti_fermi-------------------------------------------\n");
			Visualizza_conducenti_fermi(conn);
			return;
		case 7:
			printf("---------------------------------------------Visualizza_veicoli_attivi-------------------------------------------\n");
			Visualizza_veicoli_attivi(conn);
			return;
		case 8:
			printf("---------------------------------------------Visualizza_veicoli_fermi-------------------------------------------\n");
			Visualizza_veicoli_fermi(conn);
			return;
		case 9:
			printf("---------------------------------------------Elimina_conducente-------------------------------------------\n");
			Elimina_conducente(conn);
			return;
		case 10:
			printf("---------------------------------------------Emetti_biglietto-------------------------------------------\n");
			Emetti_biglietto(conn);
			return;
		case 11:
			printf("---------------------------------------------Emetti_abbonamenti-------------------------------------------\n");
			Emetti_abbonamenti(conn);
			return;
		case 12:
			printf("----------------------------------------Quit----------------------------------------\n");
			return;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			return;
		}
	}
	
	
}