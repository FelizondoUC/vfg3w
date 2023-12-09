/*
 * jugabilidad.c
 *
 *  Created on: 22 oct. 2023
 *      Author: lp1-2023
 */

#include <stdio.h>
#include "variables.h"
void funcion_comer_varios(x,y);
int sumaP1 = 0;
int sumaP2 = 0;
int count;
int adyElegidos = 0;
int adyx;
int adyy;
int adyXY[4][2];
int hayDadosAdy[4] = { FALSE, FALSE, FALSE, FALSE };
int tablero;

int adyacentes [4][2];


// Evalua si el tablero está en una posición final, no hay más posiciones libres
int finDelJuego() {
	for (int i = 0; i < TAM_MAX; i++)
		for (int j = 0; j < TAM_MAX; j++)
			if (board[i][j] == 0)
				return (FALSE);
	return (TRUE);
}

void contarDados(){
	for (int i = 0; i < TAM_MAX; i++)
		for (int j = 0; j < TAM_MAX; j++) {
			if (board[i][j] > 6)
				sumaP2 += board[i][j];
			else
				sumaP1 += board[i][j];
		}
}

//    Evalua el puntaje de una configuración del tablero
// Chequea si es una posición válida
int isValid(int i, int j) {
	return (i >= 0 && i < TAM_MAX && j >= 0 && j < TAM_MAX);
}
//Suma dados adyacentes
int sumAdjacentPips(int x, int y, int *Pcount) {
	int sum = 0;
	*Pcount = 0;
	int dx[] = { -1, 0, 1, 0 };
	int dy[] = { 0, 1, 0, -1 };
	for (int i = 0; i < 4; i++) {
		int newX = x + dx[i];
		int newY = y + dy[i];
		if (isValid(newX, newY) && mirror[newX][newY] > 0) {
			sum += mirror[newX][newY];
			(*Pcount)++;
		}
	}
	return sum;
}
// Asigna un valor al tablero
int evaluateBoard() {
	int score = 0;
	int player1Dice = 0;
	int player2Dice = 0;
	int potentialCaptureVal = 0;

	for (int i = 0; i < TAM_MAX; i++) {
		for (int j = 0; j < TAM_MAX; j++) {
			if (mirror[i][j] > 0 && mirror[i][j] <= 6) {
				player1Dice++;
			} else if (mirror[i][j] > 6) {
				player2Dice++;
			} else {

				int adjacentSum = sumAdjacentPips(i, j, &count);
				if (adjacentSum <= 6 && count >= 2) {
					potentialCaptureVal += 2;
				}
			}
		}
	}
	score = player1Dice - player2Dice + potentialCaptureVal;
	return score;
}

// Funciones para obtener mejor secuencia
// Función para verificar si una combinación es mejor que la actual mejor
int esMejorCombinacion(int mejorSuma, int sumaActual, int mejorCantidad, int cantidadActual, int player) {

    //if(sumaActual > mejorSuma && ( (sumaActual <= 6 && player == PLAYER1) || (sumaActual <= 12 && player == PLAYER2) ) ){
    if(sumaActual > mejorSuma && sumaActual <= 6 ){
        return(TRUE);
    }
    if (sumaActual == mejorSuma && sumaActual <= 6
        //( (sumaActual <= 6 && player == PLAYER1) ||
        //  (sumaActual <= 12 && player == PLAYER2) )
         && cantidadActual > mejorCantidad) {
        return(TRUE);
    }
    return(FALSE);
}

// Encontrar mejor secuencia
// lista tiene los adyacentes en el orden en que se encontraron los adyacentes conforme se recorrió adyacentes. El indice de lista permite recuperar en adyacentes la ubicación en el tablero del dado y se guarda en mejorGrupo junto al valor candidato de la secuencia.
void encontrarMejorGrupo(int *lista, int mejorGrupo[][2], int player) {
    int mejorSuma = 0, mejorCantidad =0;
    int sumaActual, cantidadActual;

    // Inicializar el mejor grupo
    for (int i = 0; i < 4; i++){
        mejorGrupo[i][0] = -1;
        mejorGrupo[i][1] = -1;
    }

    // Recorrer todas las combinaciones posibles de los números en la lista
    for (int i = 0; i < (1 << 4); i++) {
        sumaActual = 0;
        cantidadActual = 0;

        // Generar cada combinación posible, ignorando los valores -1
        for (int j = 0; j < 4; j++) {
            if ((i & (1 << j)) && lista[j] != -1) {
                sumaActual += lista[j];
                cantidadActual++;
            }
        }

        // Verificar si la combinación actual es mejor que la mejor encontrada
        if (esMejorCombinacion(mejorSuma, sumaActual, mejorCantidad, cantidadActual, player)) {
            mejorSuma = sumaActual;
            mejorCantidad= cantidadActual;
            // Reiniciar el mejor grupo y guardar la combinación actual
            for (int k = 0; k < 4; k++){
                mejorGrupo[k][0] = -1;
                mejorGrupo[k][1] = -1;
            }

            int idx = 0;
            for (int j = 0; j < 4; j++) {
                if ((i & (1 << j)) && lista[j] != -1) {
                    mejorGrupo[idx][0] = lista[j];
                    // El indice de lista me dice que adyacente es: ARR, AB, IZQ, DER
                    mejorGrupo[idx++][1]=j;
                }
            }
        }
    }
}

int comeAdyacente(int x, int y, int tablero, int player)
{
	// Posicion de las piezas que pueden ser adyacentes
	// ARRIBA, ABAJO, IZQ, DER
	int adyacentes[4][2] = { { x - 1, y }, { x + 1, y }, { x, y - 1 }, { x, y
				+ 1 } };
	// Que posiciones adyacentes tienen dados
	int hayDadosAdy[4] = { FALSE, FALSE, FALSE, FALSE };
	// Las coord de los  dados adyacentes que se van a comer
	int adyXY[4][2] = { {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1} };
	// Cantidad de adyacentes
	count = 0;
	// Valores de los ady. Se usa cuando hay más de 2 para saber la mejor secuencia que comer. BANZAI
    int lista[]={-1,-1,-1,-1};
    int mejorGrupo[4][2]={{-1,-1},{-1,-1},{-1,-1},{-1,-1}};

	// Revisar las piezas adyacentes
	for (int i = 0; i < 4; i++) {
		int nx = adyacentes[i][0];
		int ny = adyacentes[i][1];

		// Es una posición valida
		if(isValid(nx, ny)){
		    // Está ocupada y no es un 6 ni 12, entonces es candidato adyacente
		    if( ( tablero == MAINBOARD && board[nx][ny] != VACIO &&
		          board[nx][ny] != 6 && board[nx][ny] != 12 ) ||
				( tablero == MIRRORBOARD && mirror[nx][ny] != VACIO &&
				  mirror[nx][ny] != 6 && mirror[nx][ny] != 12 ) ) {

			    hayDadosAdy[i] = TRUE;
			    // Guardar el valor en lista
			    if( tablero == MAINBOARD )
			        lista[i] = board[nx][ny]+ (board[nx][ny] < 6 ? 0: -6);
			    else
			    // Es mirror
			        lista[i] = mirror[nx][ny]+ (mirror[nx][ny] < 6 ? 0:-6);

			    count++;
		    }
		}
	}
	// Si hay más de 2 adyacentes:
	//      Si es PC, se aplica método de selección mejor lista
	//      Si es HUMANO, se pregunta
	// Si hay sólo 2, se come directamente en ambos casos. Opción única de 2
	// Si hay menos de 2, no se come

	if (count > 2) {
		//Hay 3 o 4 adyacentes.

		// Si juega la PC, se elije la mejor cantidad de adyacentes que sumen 6 o menos que 6.
		// Mejor cantidad de adyacentes:max valor individual que de suma más cercana a 6 o 12 dependiendo del modo.

		// La PC juega en modojuego=1 y PLAYER2, modojuego=2 y PLAYER1 y modojuego=3 sin importar el player.
		if ((modoJuego == 1 && player == PLAYER2) ||
		(modoJuego == 2 && player == PLAYER1) ||
		(modoJuego == 3)) {
		  // Mejor secuencia de ady a comer
		  encontrarMejorGrupo(lista, mejorGrupo, player);
		  // En mejorGrupo: [0] valores que se tiene que comer y [1] posicion: 0-ARR, 1-ABJ, 2-IZQ, 3-DER
		  // Se tiene la mejor secuencia en mejorGrupo, ahora se debe agregar todas las posiciones ady
			for (int i = 0, j = 0; i < 4; i++){
        //printf("\n mejorGrupo %d es %d en %d", i, mejorGrupo[i][0], mejorGrupo[i][1]);
				if ( mejorGrupo[i][0] != -1 ){
					adyXY[j][0] = adyacentes[mejorGrupo[i][1]][0];
					adyXY[j++][1] = adyacentes[mejorGrupo[i][1]][1];
				}
			}
		//Es HUMANO ?
		} else {
			funcion_comer_varios(x,y);

		}


	} else if (count == 2) {
		// Si count es 2, se come automáticamente los ady
		// Verificar que no se pase!!!!!!!
		for (int i = 0, j = 0; i < 4; i++)
			if (hayDadosAdy[i]) {
				adyXY[j][0] = adyacentes[i][0];
				adyXY[j++][1] = adyacentes[i][1];
			}
      // Se tiene dos asyacentes y sus posiciones, pero no debe sumar más de 6.
      int sumaADY=0;
	    for(int i=0; i<4; i++){
	        if(adyXY[i][0] != -1 && adyXY[i][1] != -1){
            if( tablero == MAINBOARD)
		            sumaADY += board[adyXY[i][0]][adyXY[i][1]] +
		              (board[adyXY[i][0]][adyXY[i][1]] < 6 ? 0 : -6);
            else // Es MIRRORBOARD
                sumaADY += mirror[adyXY[i][0]][adyXY[i][1]] +
		              (mirror[adyXY[i][0]][adyXY[i][1]] < 6 ? 0 : -6);
          }
      }
      //Si es más que 6, entonces no se puede comer a pesar que hay 2 adyacentes
      if( sumaADY > 6 )
        return(FALSE);
	} else {
		//No hay adyacentes suficientes, no se puede comer
		return (FALSE);
	}
	//Si hay algo que comer, está en adyXY. Si no, ya se retorno FALSE
	//Se suma y acumula en x, y y se elimina los dados que están en adyXY. En teoría la suma no deberia dar más de 6 (o 12 si es PLAYER2)
	if (tablero == MAINBOARD) {
	    // El tablero deberia estar en cero pero se inicializa igual
	    board[x][y]=0;
	    for(int i=0; i<4; i++){
	        if(adyXY[i][0] != -1 && adyXY[i][1] != -1){
		        board[x][y] += board[adyXY[i][0]][adyXY[i][1]] +
		            (board[adyXY[i][0]][adyXY[i][1]] < 6 ? 0 : -6);
	        }
	    }

		//Si es PLAYER2, hay que poner el valor entre 7 y 12
		if (player == PLAYER2)
			board[x][y] += 6;
	} else {
		//Es el tablero MIRROR
		mirror[x][y]=0;
	    for(int i=0; i<4; i++){
	        if(adyXY[i][0] != -1 && adyXY[i][1] != -1){
		        mirror[x][y] += mirror[adyXY[i][0]][adyXY[i][1]] +
		            (mirror[adyXY[i][0]][adyXY[i][1]] < 6 ? 0 : -6);
	        }
	    }

		//Si es PLAYER2, hay que poner el valor entre 7 y 11
		if (player == PLAYER2)
			mirror[x][y] += 6;
	}
	// Ya se sumo. Eliminar los adyacentes sumados (comer)
	if (tablero == MAINBOARD) {
	    for(int i=0; i<4; i++)
	         if(adyXY[i][0] != -1 && adyXY[i][1] != -1)
	            board[adyXY[i][0]][adyXY[i][1]] = VACIO;
	} else {
		for(int i=0; i<4; i++)
	         if(adyXY[i][0] != -1 && adyXY[i][1] != -1)
	            mirror[adyXY[i][0]][adyXY[i][1]] = VACIO;
	}
	return (TRUE);
}
