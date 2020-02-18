#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#define MAX_DEGREES 128

struct average_grades {
	char corso[46];
	char iscritto[11];
	char concluso[11];
	double avg;
};

static void Timbra_biglietto(MYSQL* conn) {
	MYSQL_STMT* distanza_stmt;
	MYSQL_BIND param[2];
}
static void Convalida_abbonamento(MYSQL* conn) {
	MYSQL_STMT* distanza_stmt;
	MYSQL_BIND param[2];
}
static void Calcola_distanza_veicolo(MYSQL* conn) {
	MYSQL_STMT* distanza_stmt;
	MYSQL_BIND param[2];
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
		printf("4) Quit\n");
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
			return;
		case 3:
			printf("---------------------------------------------Calcola distanza veicolo--------------------------------------------\n");
			Calcola_distanza_veicolo(conn);
			return;
		case 4:
			printf("----------------------------------------Quit----------------------------------------\n");
			return;

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			return;
		}



	}
	
}