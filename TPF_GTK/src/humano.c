/*
 * humano.c
 *
 *  Created on: 22 oct. 2023
 *      Author: lp1-2023
 */

#include <stdio.h>
#include "variables.h"
#include "jugabilidad.h"

// JUEGA HUMANO. Elegir fila y columna
void juegaHumano(int x, int y, int player) {
	if (player == PLAYER1) // Player1, dados 1-6
		board[x][y] = 1;
	else
		// Player2, dados 7-12
		board[x][y] = 7;
	// Si se puede, comer
	comeAdyacente(x, y, MAINBOARD, player);
}
