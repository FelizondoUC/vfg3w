/*
 * computadora.c
 *
 *  Created on: 22 oct. 2023
 *      Author: lp1-2023
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "variables.h"
#include "tablero.h"
#include "jugabilidad.h"

// ESTRATEGIA ALEATORIA
// Cuando juega la PC, lo hace en forma aleatoria y carga en azarX,azarY la posición donde juega
// Siempre se carga en board directo.
int jugadaAleatoria(int *azarX, int *azarY) {
	int buscarSgte = FALSE;
	int i, j, count = 2;
	// Genera un número aleatorio en el rango de 0 a 4
	*azarX = rand() % 5;
	*azarY = rand() % 5;
	// Si azarX,azarY no corresponde a una posición vacía, se busca a partir de esa posición la siguiente vacía. Si no hay, se busca desde el principio.
	do {
		for (i = 0; i < TAM_MAX; i++) {
			for (j = 0; j < TAM_MAX; j++) {
				if (i == *azarX && j == *azarY &&
				!buscarSgte) {
					if (board[i][j] == VACIO)
						// Los valores random de azar son de una pos libre
						return ( TRUE);
					else
						buscarSgte = TRUE;
				}
				// La pos de azar estaba ocupada
				if (buscarSgte) {
					if (board[i][j] == VACIO) {
						*azarX = i;
						*azarY = j;
						return ( TRUE);
					}
				}
			}
		}
	} while (--count > 0);
	return ( FALSE);
}

void juegaPc(int player, int IA) {
	int maxVal = MENOS_INFINITO, iMax, jMax;
	int valor;
	if (IA == FIRSTSPACE_IA) {
		for (int i = 0; i < TAM_MAX; i++) {
			for (int j = 0; j < TAM_MAX; j++) {
				//Es una posición libre, evaluar
				if (board[i][j] == VACIO) {
					//Se copia el contenido del tablero
					boardToMirror();
					// Se hace la jugada en la copia
					mirror[i][j] = (player == PLAYER1 ? 1 : 7);
					//Si se puede, comer fichas
					comeAdyacente(i, j, MIRRORBOARD, player);
					//Obtener evaluación del tablero
					if ((valor = evaluateBoard()) > maxVal) {
						maxVal = valor;
						iMax = i;
						jMax = j;
					}
				}
			}
		}
	} else if (IA == AZAR_IA)
		jugadaAleatoria(&iMax, &jMax);
	// Hacer la jugada con mayor valor
	board[iMax][jMax] = (player == PLAYER1 ? 1 : 7);
	comeAdyacente(iMax, jMax, MAINBOARD, player);
	// Indicar donde jugó la PC
	//printf("\n %s jugó en %d, %d \n", player == PLAYER1 ? nameP1 : nameP2, iMax, jMax);
}
