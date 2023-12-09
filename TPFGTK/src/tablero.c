/*
 * tablero.c
 *
 *  Created on: 22 oct. 2023
 *      Author: lp1-2023
 */
#include <stdio.h>
#include "variables.h"
// Inicializa el tablero
void initBoard() {
	for (int i = 0; i < TAM_MAX; i++)
		for (int j = 0; j < TAM_MAX; j++)
			board[i][j] = VACIO;
}
// Imprime el tablero.
// Los dados del PLAYER2 se imprimen con un *
void printBoard() {
	for (int i = 0; i < TAM_MAX; i++) {
		printf("\n");
		for (int j = 0; j < TAM_MAX; j++) {
			if (board[i][j] >= 0 && board[i][j] <= 6)
				printf("  %d ", board[i][j]);
			else
				printf(" *%d ", board[i][j] - 6);
		}
	}
	printf("\n");
}
// Se duplica el tablero en mirrorBoard
void boardToMirror() {
	for (int i = 0; i < TAM_MAX; i++)
		for (int j = 0; j < TAM_MAX; j++)
			mirror[i][j] = board[i][j];
}
