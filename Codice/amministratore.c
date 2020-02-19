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
	MYSQL_STMT* turno;
	MYSQL_BIND param[4];
	MYSQL_TIME  data;
	MYSQL_TIME  inizio;
	MYSQL_TIME  fine;
	char Conducente_CF[16];

	printf("Inserisci il CF dell'guidatore : ");
	scanf_s("%s", Conducente_CF, 16);
	printf("Inserisci il giorno\n");
	printf("Anno : ");
	scanf_s("%d", &data.year, 4);
	printf("Mese : ");
	scanf_s("%d", &data.month, 2);
	printf("Giorno : ");
	scanf_s("%d", &data.day, 2);
	printf("Gli orari disponibili specificano solo l'ora\n ");

	printf("Inserisci l'orario di inizio turno : \n");
	printf("Ora");
	scanf_s("%d:00:00", &inizio.hour, 2);

	printf("Inserisci l'orario di fine turno : \n");
	printf("Ora: ");
	scanf_s("%d:00:00", &fine.hour, 2);
	


	if (!setup_prepared_stmt(&turno, "call Assegna_turno_al_conducente(?, ?, ?, ?)", conn)) {
		print_stmt_error(turno, "Unable to initialize login statement\n");
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[0].buffer = Conducente_CF;
	param[0].buffer_length = strlen(Conducente_CF);

	param[1].buffer_type = MYSQL_TYPE_DATE; //IN
	param[1].buffer = (char*)&data;
	param[1].buffer_length = sizeof(data);

	param[2].buffer_type = MYSQL_TYPE_DATE; //IN
	param[2].buffer = (char*)&inizio;
	param[2].buffer_length = sizeof(inizio);

	param[3].buffer_type = MYSQL_TYPE_DATE;  //IN  DATAAA
	param[3].buffer = (char*)&fine;
	param[3].buffer_length = sizeof(fine);

	if (mysql_stmt_bind_param(turno, param) != 0) {
		finish_with_stmt_error(conn, turno, "Could not bind parameters for career report\n", true);
	}
	// Run procedure
	if (mysql_stmt_execute(turno) != 0) {
		print_stmt_error(turno, "An error occurred while retrieving the career report.");
		goto out;
	}

	printf("Hai assegnato ad : CF %s, il giorno %d il turno dalle %d alle %d", Conducente_CF,data, inizio, fine);
	system("pause");
	mysql_stmt_close(turno);
	return;


out:
	mysql_stmt_close(turno);

}
static void Assegna_veicolo_alla_tratta(MYSQL* conn) {
	MYSQL_STMT* veicl_stmt;
	MYSQL_BIND param[4];
	MYSQL_TIME  ts;
	char Conducente[16];
	int Veicolo;
	int Tratta;


	printf("Conducente : ");
	scanf_s("%s", Conducente, 16);
	printf("Veicolo : ");
	scanf_s("%d", &Veicolo);
	printf("Tratta : ");
	scanf_s("%d",&Tratta);

	printf("Inserisci la data di percorrenza\n");
	printf("Anno : ");
	scanf_s("%d", &ts.year, 4);
	printf("Mese : ");
	scanf_s("%d", &ts.month, 2);
	printf("Giorno : ");
	scanf_s("%d", &ts.day, 2);


	if (!setup_prepared_stmt(&veicl_stmt, "call Assegna_veicolo_alla_tratta(?, ?, ?, ?)", conn)) {
		print_stmt_error(veicl_stmt, "Unable to initialize login statement\n");
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	

	param[0].buffer_type = MYSQL_TYPE_LONG;  //IN
	param[0].buffer = &Veicolo;
	param[0].buffer_length = sizeof(Veicolo);

	param[1].buffer_type = MYSQL_TYPE_LONG;  //IN
	param[1].buffer = &Tratta;
	param[1].buffer_length = sizeof(Tratta);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[2].buffer = Conducente;
	param[2].buffer_length = strlen(Conducente);

	param[3].buffer_type = MYSQL_TYPE_DATE;  //IN  DATAAA
	param[3].buffer = (char*)&ts;
	param[3].buffer_length = sizeof(ts);

	if (mysql_stmt_bind_param(veicl_stmt, param) != 0) {
		finish_with_stmt_error(conn, veicl_stmt, "Could not bind parameters for career report\n", true);
	}
	// Run procedure
	if (mysql_stmt_execute(veicl_stmt) != 0) {
		print_stmt_error(veicl_stmt, "An error occurred while retrieving the career report.");
		goto out;
	}

	printf("Hai assegnato al conducente : CF %s la tratta %d con il veicolo %d il giorno %d:%d:%d", Conducente, Tratta, Veicolo, ts.year,ts.month,ts.day);
	system("pause");
	mysql_stmt_close(veicl_stmt);
	return;


out:
	mysql_stmt_close(veicl_stmt);

}
static void Assumi_conducente(MYSQL* conn) {
	MYSQL_STMT* Assum_cond;
	MYSQL_BIND param[8];
	MYSQL_TIME  ts;
	MYSQL_TIME  tl;
	char CF[16];
	char nome[45];
	char username[45];
	char cognome[45];
	char patente[10];
	char nascita[16];
	char scadenza[16];
	char luogo_nascita[45];

	printf("Inserisci l'username dell'utente da assumere come conducente : ");
	scanf_s("%s", username,45);
	printf("Inserisci il codice fiscale : ");
	scanf_s("%s", CF,16);
	printf("Inserisci il nome : ");
	scanf_s("%s", nome,45);
	printf("Inserisci il cognome : ");
	scanf_s("%s", cognome,45);
	printf("Inserisci la data di nascita\n");
	printf("Anno : ");
	scanf_s("%d", &ts.year,4);
	printf("Mese : ");
	scanf_s("%d", &ts.month,2);
	printf("Giorno : ");
	scanf_s("%d", &ts.day,2);
	printf("Inserisci il luogo di nascita : ");
	scanf_s("%s", luogo_nascita,45);
	printf("Inserisci la data di scadenza della patente \n");
	printf("Anno : ");
	scanf_s("%d", &tl.year,4);
	printf("Mese : ");
	scanf_s("%d", &tl.month,2);
	printf("Giorno : ");
	scanf_s("%d", &tl.day,2);
	printf("Inserisci il numero di patente: ");
	scanf_s("%s", patente,10);


	if (!setup_prepared_stmt(&Assum_cond, "call Assumi_conducente(?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		print_stmt_error(Assum_cond, "Unable to initialize login statement\n");
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[0].buffer = CF;
	param[0].buffer_length = strlen(CF);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[1].buffer = nome;
	param[1].buffer_length = strlen(nome);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[2].buffer = cognome;
	param[2].buffer_length = strlen(cognome);

	param[3].buffer_type = MYSQL_TYPE_DATE;  //IN  DATAAA
	param[3].buffer = (char*)&ts;
	param[3].buffer_length = sizeof(ts);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[4].buffer = luogo_nascita;
	param[4].buffer_length = strlen(luogo_nascita);

	param[5].buffer_type = MYSQL_TYPE_DATE;  //IN  DATAAA
	param[5].buffer = (char*)&tl;
	param[5].buffer_length = sizeof(tl);

	param[6].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[6].buffer = patente;
	param[6].buffer_length = strlen(patente);

	param[7].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[7].buffer = username;
	param[7].buffer_length = strlen(username);

	if (mysql_stmt_bind_param(Assum_cond, param) != 0) {
		finish_with_stmt_error(conn, Assum_cond, "Could not bind parameters for career report\n", true);
	}
	// Run procedure
	if (mysql_stmt_execute(Assum_cond) != 0) {
		print_stmt_error(Assum_cond, "An error occurred while retrieving the career report.");
		goto out;
	}

	printf("Hai aggiunto : CF %s, Nome: %s, Cognome: %s con l'username :%s", CF, nome,cognome,username);
	system("pause");
	mysql_stmt_close(Assum_cond);
	return;


out:
	mysql_stmt_close(Assum_cond);

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

	param[0].buffer_type = MYSQL_TYPE_LONG;
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

} /*SBAGLIA IL CODICE DEI VEICOLI*/
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
} /*SBAGLIA IL CODICE DEI VEICOLI*/
static void Elimina_conducente(MYSQL* conn) {
	MYSQL_STMT* Elimina;
	MYSQL_BIND param[2];
	char codice[16];
	char patente[10];
	printf("Inserisci il codice fiscale : ");
	scanf_s("%s", codice,16);
	printf("Inserisci il numero della patente : ");
	scanf_s("%s",patente,10);

	if (!setup_prepared_stmt(&Elimina, "call Elimina_conducente(?, ?)", conn)) {
		print_stmt_error(Elimina, "Unable to initialize login statement\n");
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[0].buffer = codice;
	param[0].buffer_length = strlen(codice);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[1].buffer = patente;
	param[1].buffer_length = strlen(patente);

	if (mysql_stmt_bind_param(Elimina, param) != 0) {
		finish_with_stmt_error(conn, Elimina, "Could not bind parameters for career report\n", true);
	}
	// Run procedure
	if (mysql_stmt_execute(Elimina) != 0) {
		print_stmt_error(Elimina, "An error occurred while retrieving the career report.");
		goto out;
	}
	printf("Hai eliminato | CF: %s   Patente: %s\n", codice, patente);
	system("pause");
	mysql_stmt_close(Elimina);
	return;


out:
	mysql_stmt_close(Elimina);

} /*MI CONFERMA IL TUTTO MA NON LO ELIMINA SUL DATABASE*/
static void Emetti_biglietto(MYSQL* conn) {
	MYSQL_STMT* EmettiB=NULL;
	int biglietti;
	printf("Inserisci il numero di biglietti da emettere : ");
	scanf_s("%d", &biglietti);

	for (int j = 0; j < biglietti; j++)
	{
		if (!setup_prepared_stmt(&EmettiB, "call Emetti_biglietto()", conn)) {
			print_stmt_error(EmettiB, "Unable to initialize login statement\n");
		}
		// Run procedure
		if (mysql_stmt_execute(EmettiB) != 0) {
			print_stmt_error(EmettiB, "An error occurred while retrieving the career report.");
			goto out;
		}
	}
	printf("Hai emanato %d biglietti !\n",biglietti);
	system("pause");
	mysql_stmt_close(EmettiB);
	return;
out:
	mysql_stmt_close(EmettiB);
}
static void Emetti_abbonamenti(MYSQL* conn) {
	MYSQL_STMT* EmettiA=NULL;
	int abbonamenti;
	printf("Inserisci il numero di biglietti da emettere : ");
	scanf_s("%d", &abbonamenti);

	for (int j = 0; j < abbonamenti; j++)
	{
		if (!setup_prepared_stmt(&EmettiA, "call Emetti_abbonamenti()", conn)) {
			print_stmt_error(EmettiA, "Unable to initialize login statement\n");
		}
		// Run procedure
		if (mysql_stmt_execute(EmettiA) != 0) {
			print_stmt_error(EmettiA, "An error occurred while retrieving the career report.");
			goto out;
		}
	}
	printf("Hai emanato %d abbonamenti !\n", abbonamenti);
	system("pause");
	mysql_stmt_close(EmettiA);
	return;
out:
	mysql_stmt_close(EmettiA);
}
static void Aggiungi_utente(MYSQL* conn)
{
	MYSQL_STMT* distanza_stmt;
	MYSQL_BIND param[3];
	
	char username[45];
	char password[45];
	char ruolo[45];


	printf("Inserisci l'username : ");
	scanf_s("%s", username,45);
	printf("Inserisci la password (e' consigliato usare pippo) : ");
	scanf_s("%s", password,45);
	printf("Inserisci il ruolo (AMMINISTRATORE|GUIDATORE|PASSEGGERO) : ");
	scanf_s("%s", ruolo,45);
	
	

	if (!setup_prepared_stmt(&distanza_stmt, "call Aggiungi_utente(?, ?, ?)", conn)) {
		print_stmt_error(distanza_stmt, "Unable to initialize login statement\n");
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[1].buffer = password;
	param[1].buffer_length = strlen(password);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;  //IN
	param[2].buffer = ruolo;
	param[2].buffer_length = strlen(ruolo);

	if (mysql_stmt_bind_param(distanza_stmt, param) != 0) {
		finish_with_stmt_error(conn, distanza_stmt, "Could not bind parameters for career report\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(distanza_stmt) != 0) {
		print_stmt_error(distanza_stmt, "An error occurred while retrieving the career report.");
		goto out;
	}


	printf("Hai aggiunto : Username %s, Ruolo: %s\n", username, ruolo);
	system("pause");
	mysql_stmt_close(distanza_stmt);
	return;


out:
	mysql_stmt_close(distanza_stmt);

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
		printf("9) Elimina un conducente\n");
		printf("10) Emetti biglietti\n");
		printf("11) Emetti abbonamenti\n");
		printf("12) Aggiungi utente\n");
		printf("13) Quit\n");
		scanf_s("%i", &numero);
		switch (numero)
		{
		case 1:
			printf("-----------------------------------------------Assegna_turno_al_conducente-----------------------------------------\n");
			Assegna_turno_al_conducente(conn);
			break;
		case 2:
			printf("---------------------------------------------Assegna_veicolo_alla_tratta-------------------------------------------\n");
			Assegna_veicolo_alla_tratta(conn);
			break;
		case 3:
			printf("---------------------------------------------Assumi_conducente------------------------------------------------------\n");
			Assumi_conducente(conn);
			break;
		case 4:
			printf("---------------------------------------------Cambia_conducente_turno------------------------------------------------\n");
			Cambia_conducente_turno(conn);
			break;
		case 5:
			printf("---------------------------------------------Visualizza_conducenti_attivi-------------------------------------------\n");
			Visualizza_conducenti_attivi(conn);
			break;
		case 6:
			printf("---------------------------------------------Visualizza_conducenti_fermi--------------------------------------------\n");
			Visualizza_conducenti_fermi(conn);
			break;
		case 7:
			printf("---------------------------------------------Visualizza_veicoli_attivi----------------------------------------------\n");
			Visualizza_veicoli_attivi(conn);
			break;
		case 8:
			printf("---------------------------------------------Visualizza_veicoli_fermi-----------------------------------------------\n");
			Visualizza_veicoli_fermi(conn);
			break;
		case 9:
			printf("---------------------------------------------Elimina_conducente-----------------------------------------------------\n");
			Elimina_conducente(conn);
			break;
		case 10:
			printf("---------------------------------------------Emetti_biglietto-------------------------------------------------------\n");
			Emetti_biglietto(conn);
			break;
		case 11:
			printf("---------------------------------------------Emetti_abbonamenti-----------------------------------------------------\n");
			Emetti_abbonamenti(conn);
			break;
		case 12:
			printf("---------------------------------------------Aggiungi utente-----------------------------------------------------\n");
			Aggiungi_utente(conn);
			break;
		case 13:
			printf("---------------------------------------------------------Logout---------------------------------------------------------\n");
			return;
		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			break;
		}
	}
	
	
}