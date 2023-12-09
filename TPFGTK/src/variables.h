/*
 * variables.h
 *
 *  Created on: 22 oct. 2023
 *      Author: lp1-2023
 */
#ifndef VARIABLES_H_
#define VARIABLES_H_

// Player1, tiene dados del 1 al 6
#define PLAYER1     1
// Player2, tiene dados del 7 al 12
#define PLAYER2     2
// Tableros, principal y espejo para evaluación
#define MAINBOARD   1
#define MIRRORBOARD 2
// Valor inicial mejor jugada.
#define MENOS_INFINITO  -20000
#define VACIO       0
#define TAM_MAX     5
// Valores booleanos
#define TRUE        1
#define FALSE       0
// Modo de juego de IA
#define AZAR_IA         1
#define FIRSTSPACE_IA   2
#define INTELIGENTE          3

//Nombre de los jugadores
extern char *nameP1;
extern char *nameP2;

//Tablero del juego
extern int board[TAM_MAX][TAM_MAX];
//Tablero espejo que se usa para evaluar jugadas
extern int mirror[TAM_MAX][TAM_MAX];
// modojuego define quienes juegan. Util que sea externo para cuando se come, ya que si es PC vs PC, se come automáticamente sin consultar posiciones.
extern int modoJuego;
extern int opcion;
extern int colorP1;
extern int colorP2;
extern int IA;
extern int turno;
extern int count;
extern int adyacentes[4][2];
extern int adyElegidos;
extern int adyx;
extern int adyy;
extern int adyXY[4][2];
extern int hayDadosAdy[4];
extern int tablero;

#endif /* VARIABLES_H_ */
