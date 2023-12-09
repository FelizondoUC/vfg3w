#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "variables.h"
#include "tablero.h"
#include "humano.h"
#include "jugabilidad.h"
#include "computadora.h"


// Que player juega
int turno = 0;

//Nombre de los jugadores
//char nameP1[30] = "Jugador 1";
//char nameP2[30] = "Jugador 2";

//Color de los dados
int colorP1;
int colorP2;

//Tablero del juego
int board[TAM_MAX][TAM_MAX];
//Tablero espejo que se usa para evaluar jugadas
int mirror[TAM_MAX][TAM_MAX];
// modojuego define quienes juegan. Util que sea externo para cuando se come, ya que si es PC vs PC, se come automáticamente sin consultar posiciones.
int modoJuego = 9;

// Vaciar el buffer de scanf
void flushBuffer() {
int x;
while ((x = getchar()) != '\n' && x != EOF);
}


//Módulo principal: MAIN
int main_C(int opcion, int color, int IA) {

	// Semilla para la generación de números aleatorios
	srand(time(NULL));
		//Opciones de juego
		modoJuego= opcion;
		// Si juega la PC, elegir estrategia IA
		if( modoJuego == 1 || modoJuego == 2 || modoJuego == 3)
	//Inicializar e imprimir el tablero
	initBoard();
	//Jugar segun la opcion deseada
	switch (modoJuego) {
	case 1: //HUMANO VS PC
			if (turno++ % 2 != 0)
				juegaPc(PLAYER2, IA);
		break;
	case 3: //PC1 VS PC2
			if (turno++ % 2 == 0)
				juegaPc(PLAYER1, IA);
			else
				juegaPc(PLAYER2, IA);
			//printBoard();
		break;
		break;
	case 9: //Salir
		break;
	default:
		break;
	}

	return 0;
	}
