/*
 ============================================================================
 Name        : TPF_GTK.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in GTK+
 ============================================================================
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include "variables.h"
#include "TPF.h"
#include "humano.h"
#include "tablero.h"
#include "computadora.h"
#include "jugabilidad.h"

//Punteros a ventanas del glade
GtkWidget *Ventana_Principal;
GtkWidget *ventanaReglas1;
GtkWidget *Ventana_Reglas2;
GtkWidget *Ventana_Creditos;
GtkWidget *Ventana_Jugar;
GtkWidget *Ventana_Jugar2;
GtkWidget *Ventana_Empezar_Partida;
GtkWidget *Ventana_Rendirse;
GtkWidget *Ventana_Reiniciar;
GtkWidget *Ventana_Perdedor;
GtkWidget *Ventana_Ganador;
GtkWidget *Estadisticas;
GtkWidget *Ventana_Num_Capturas;
GtkWidget *Ventana_Captura2;

GtkLabel *Fichas_humano;
GtkLabel *Fichas_PC;

//Punteros a los distintos botones del glade
GObject *Boton_Reglas_Menu;
GObject *Volver_A_Menu_Reglas1;
GObject *Boton_Siguiente_Reglas;
GObject *Boton_PaganteReglas1_Reglas2;
GObject *Volver_A_Menu_Reglas2;
GObject *Boton_Creditos;
GObject *Volver_A_Menu_Creditos;
GObject *Boton_Jugar;
GObject *Volver_al_Menu_Jugar;
GtkToggleButton *Humano_VS_PC;
GtkToggleButton *PC_vs_Humano;
GtkToggleButton *Al_Azar;
GtkToggleButton *PC_vs_PC;
GtkToggleButton *Seleccionar_Red;
GtkToggleButton *Seleccionar_Green;
GObject *Siguiente_Jugar;
GObject *Volver_al_Menu_Jugar2;
GtkToggleButton *IA_Aleatoria;
GtkToggleButton *IA_Inteligente;
GtkBuilder *builder;
GtkEntry *Nombre_Jugador1;
GObject *Siguiente_Jugar2;
GObject *Boton_Ayuda_Juego;
GObject *Boton_Acerca_de_Juego;
GObject *Boton_Rendirse_Juego;
GObject *Boton_continuar_ventRendirse;
GObject *Rendirse_Ventana_Rendirse;
GObject *Boton_Reiniciar_Partida;
GObject *Boton_continuar_Reiniciar;
GObject *Color_Random;
GObject *Boton_Restart;
GObject *Boton_Estadisticas_inic;
GObject *Volver_Estadisticas;
GObject *Estadisticas_Tablero;
GObject *Volver_al_Menu_Perdedor;
GObject *Jugar_de_Nuevo_Perdedor;
GObject *Volver_al_Menu_Ganador;
GObject *Jugar_de_Nuevo_Ganador;
GObject *Turno_Jugador;
GObject *BotonIrCaptura;
GtkToggleButton *Capturar2;
GtkToggleButton *Capturar3;
GtkToggleButton *Capturar4;
GtkToggleButton *Arriba;
GtkToggleButton *Abajo;
GtkToggleButton *Izquierda;
GtkToggleButton *Derecha;
GtkButton *Capturar;

GtkEntry *Nombre_1;
GtkEntry *Nombre_2;

GObject *Botones[TAM_MAX][TAM_MAX];
GtkWidget *Dados[TAM_MAX][TAM_MAX];
int board_GTK[TAM_MAX][TAM_MAX];

int resultado;
int opcion;
int IA;
int player;
int board_gtk[TAM_MAX][TAM_MAX];
int finalJuego = 0;
int chequeoGanador = 0;
int SUMAP1;
int SUMAP2;
int comer;

void calculoDeEstadisticas(const char *nombreArchivo) {
	char nombresJugadores[150][50];
	int partidasJugadas[150];
	int partidasGanadas[150];
	int partidasPerdidas[150];

	FILE *archivo = fopen(nombreArchivo, "r");
	if (archivo == NULL) {
		printf("Error al abrir el archivo de estadisticas.\n");
		return;
	}

	int cantJugadores = 0;

	// Lee los datos del archivo
	while (fscanf(archivo, "%s %d %d %d", nombresJugadores[cantJugadores],
			&partidasJugadas[cantJugadores], &partidasGanadas[cantJugadores],
			&partidasPerdidas[cantJugadores]) == 4) {
		cantJugadores++;
	}

	fclose(archivo);

	//Calcula las estadisticas totales
	for (int i = 0; i < cantJugadores - 1; i++) {
		for (int j = i + 1; j < cantJugadores; j++) {
			if (strcmp(nombresJugadores[i], nombresJugadores[j]) == 0) {

				//Si los nombres son iguales, entonces suma dichas estadisticas
				partidasJugadas[i] += partidasJugadas[j];
				partidasGanadas[i] += partidasGanadas[j];
				partidasPerdidas[i] += partidasPerdidas[j];

				//Marca el jugador duplicado para eliminar
				nombresJugadores[j][0] = '\0';
			}
		}
	}

	//Elimina los jugadores duplicados
	int nuevaCantJugadores = 0;
	for (int i = 0; i < cantJugadores; i++) {
		if (nombresJugadores[i][0] != '\0') {
			strcpy(nombresJugadores[nuevaCantJugadores], nombresJugadores[i]);
			partidasJugadas[nuevaCantJugadores] = partidasJugadas[i];
			partidasGanadas[nuevaCantJugadores] = partidasGanadas[i];
			partidasPerdidas[nuevaCantJugadores] = partidasPerdidas[i];
			nuevaCantJugadores++;
		}
	}

	// Ordenar jugadores por partidas ganadas (de mayor a menor)
	for (int i = 0; i < nuevaCantJugadores - 1; i++) {
		for (int j = 0; j < nuevaCantJugadores - i - 1; j++) {
			if (partidasGanadas[j] < partidasGanadas[j + 1]) {
				// Intercambiar jugadores
				char tempNombre[50];
				strcpy(tempNombre, nombresJugadores[j]);
				strcpy(nombresJugadores[j], nombresJugadores[j + 1]);
				strcpy(nombresJugadores[j + 1], tempNombre);

				int tempPartidasJugadas = partidasJugadas[j];
				partidasJugadas[j] = partidasJugadas[j + 1];
				partidasJugadas[j + 1] = tempPartidasJugadas;

				int tempPartidasGanadas = partidasGanadas[j];
				partidasGanadas[j] = partidasGanadas[j + 1];
				partidasGanadas[j + 1] = tempPartidasGanadas;

				int tempPartidasPerdidas = partidasPerdidas[j];
				partidasPerdidas[j] = partidasPerdidas[j + 1];
				partidasPerdidas[j + 1] = tempPartidasPerdidas;
			}
		}
	}

	// Muestra estadisticas totales ordenadas por partidas ganadas
	char buffer[5000];
	sprintf(buffer,
			"Estadisticas totales (ordenado mediante partidas ganadas):\n\n");

	for (int i = 0; i < nuevaCantJugadores; i++) {
		sprintf(buffer + strlen(buffer),
				"%s - Partidas Jugadas: %d \n Partidas Ganadas: %d / Partidas Perdidas: %d\n",
				nombresJugadores[i], partidasJugadas[i], partidasGanadas[i],
				partidasPerdidas[i]);
	}
	GtkLabel *Texto_Stats = GTK_LABEL(
			gtk_builder_get_object(builder, "Texto_Stats"));
	// Establecer la cadena construida como texto en el GtkLabel
	gtk_label_set_text(Texto_Stats, buffer);
}

void actualizarEstadisticas(const gchar *nombre, int resultado) {
	FILE *archivo = fopen("estadisticas.txt", "a");

	if (archivo == NULL) {
		printf("Error al abrir el archivo de estadisticas.\n");
		return;
	}

	//Actualiza las estadisticas segun resultados de las partidas
	if (resultado == 1) {
		fprintf(archivo, "%s 1 1 0\n", nombre); //Partida Ganada
	} else {
		fprintf(archivo, "%s 1 0 1\n", nombre); //Partida Perdida
	}

	fclose(archivo);

	//Vuelve a abrir el archivo para asegurar los cambios inmediatos
	archivo = fopen("estadisticas.txt", "r");
	if (archivo == NULL) {
		printf("Error al abrir el archivo de estadisticas para lectura.\n");
		return;
	}

	fclose(archivo);
}

void registrar_partida() {
	GtkEntry *entry1 = GTK_ENTRY(
			gtk_builder_get_object(builder, "Nombre_Jugador1"));

	// Obtiene el texto del GtkEntry
	const gchar *entry1_text = gtk_entry_get_text(GTK_ENTRY(entry1));

	// Actualiza las estadisticas
	actualizarEstadisticas(entry1_text, resultado);

	// Muestra las estadisticas
	calculoDeEstadisticas("estadisticas.txt");
}

void funcOcultar(GObject *widet, gpointer data) {
	gtk_widget_hide(data);
}

void funcLlamar(GObject *cosito, gpointer data) {
	gtk_widget_show_all(data);
}

void funcMostrarOcultar1(GObject *boton, gpointer data) {
	gtk_widget_hide(ventanaReglas1);
	gtk_widget_show_all(Ventana_Reglas2);
}

void funcMostrarOcultar2(GObject *boton, gpointer data) {
	gtk_widget_hide(Ventana_Reglas2);
	gtk_widget_show_all(ventanaReglas1);
}

void funcMostrarOcultar3(GObject *boton, gpointer data) {
	gtk_widget_hide(Ventana_Principal);
	gtk_widget_show_all(Ventana_Jugar);
}

void funcMostrarOcultar4(GObject *boton, gpointer data) {
	gtk_widget_hide(Ventana_Jugar);
	gtk_widget_show_all(Ventana_Principal);
}

void funcMostrarOcultar5(GObject *boton, gpointer data) {
	gtk_widget_hide(Ventana_Jugar);
	gtk_widget_show_all(Ventana_Jugar2);
}

void funcMostrarOcultar6(GObject *boton, gpointer data) {
	gtk_widget_hide(Ventana_Jugar2);
	gtk_widget_show_all(Ventana_Jugar);
}

void funcMostrarOcultar7(GObject *boton, gpointer data) {
	gtk_widget_hide(Ventana_Empezar_Partida);
	gtk_widget_hide(Ventana_Rendirse);
	gtk_widget_show_all(Ventana_Principal);
}

void funcMostrarOcultar8(GObject *boton, gpointer data) {
	gtk_widget_hide(Ventana_Rendirse);
	gtk_widget_hide(Ventana_Empezar_Partida);
	gtk_widget_show_all(Ventana_Perdedor);
}

void funcMostrarOcultar9(GObject *boton, gpointer data) {
	reiniciar_partida();
	gtk_widget_hide(Ventana_Perdedor);
	gtk_widget_show_all(data);
	registrar_partida();
}

void funcMostrarOcultar10(GObject *boton, gpointer data) {
	reiniciar_partida();
	gtk_widget_hide(Ventana_Ganador);
	gtk_widget_show_all(data);
	registrar_partida();
}

void funcMostrarOcultar11(GObject *boton, gpointer data) {
	gtk_widget_hide(Ventana_Num_Capturas);
	gtk_widget_show_all(Ventana_Captura2);
}

void funcMostrarStats(GObject *boton, gpointer data) {

	// Muestra las estadisticas
	calculoDeEstadisticas("estadisticas.txt");
	gtk_widget_show_all(data);
}

void capturar2(GtkToggleButton *boton, gpointer data) {
	comer = 2;
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Capturar3), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Capturar4), FALSE);
	}
}

void capturar3(GtkToggleButton *boton, gpointer data) {
	comer = 3;
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Capturar2), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Capturar4), FALSE);
	}
}

void capturar4(GtkToggleButton *boton, gpointer data) {
	comer = 4;
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Capturar3), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Capturar2), FALSE);
	}
}

void ir_captura(GtkButton *boton, gpointer data) {
	gtk_widget_hide(Ventana_Num_Capturas);
	gtk_widget_show_all(Ventana_Captura2);
}

void funcBotonesModoJuego1(GtkToggleButton *boton, gpointer data) {
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(PC_vs_Humano), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Al_Azar), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(PC_vs_PC), FALSE);
		opcion = 1;
		player = PLAYER1;
	}
}

void funcBotonesModoJuego2(GtkToggleButton *boton, gpointer data) {
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Humano_VS_PC), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Al_Azar), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(PC_vs_PC), FALSE);
		opcion = 2;
		player = PLAYER2;
	}
}

void funcBotonesModoJuego3(GtkToggleButton *boton, gpointer data) {
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(PC_vs_Humano), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Humano_VS_PC), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(PC_vs_PC), FALSE);

		// Generar un número aleatorio entre 1 y 2
		opcion = (rand() % 2) + 1;
	}
}

void funcBotonesModoJuego4(GtkToggleButton *boton, gpointer data) {
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(PC_vs_Humano), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Al_Azar), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Humano_VS_PC), FALSE);
		opcion = 3;
	}
}
void funcColorRed(GtkToggleButton *boton, gpointer data) {
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Seleccionar_Green),
				FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Color_Random), FALSE);
		colorP1 = 1;
		colorP2 = 2;
		//Asigna dados del 1 al 6 (dados rojos)

	}
}

void funcColorGreen(GtkToggleButton *boton, gpointer data) {
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Seleccionar_Red), FALSE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Color_Random), FALSE);
		colorP1 = 2;
		colorP2 = 1;
	}
}

void funcColorRandom(GtkToggleButton *boton, gpointer data) {
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Seleccionar_Red), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Seleccionar_Green), FALSE);
	colorP1 = (rand() % 2) + 1;
	if (colorP1 == 1)
		colorP2 = 2;
	else if (colorP1 == 2)
		colorP2 = 1;
}

void funcBotonesIA1(GtkToggleButton *boton, gpointer data) {
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(IA_Inteligente), FALSE);
		IA = 1;
	}
}

void funcBotonesIA2(GtkToggleButton *boton, gpointer data) {
	if (gtk_toggle_button_get_active(boton)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(IA_Aleatoria), FALSE);
		IA = 2;
	}
}

void comenzar_juego(GObject *boton, gpointer data) {
	const gchar *texto_1 = gtk_entry_get_text(Nombre_1);

	// Obtener el GtkLabel
	GtkLabel *label1 = GTK_LABEL(gtk_builder_get_object(builder, "label1"));
	GtkLabel *label2 = GTK_LABEL(gtk_builder_get_object(builder, "label2"));
	GtkLabel *Turno_Jugador = GTK_LABEL(
			gtk_builder_get_object(builder, "Turno_Jugador"));

	gtk_label_set_text(Turno_Jugador, texto_1);

	// Establecer el texto del GtkLabel con el texto del GtkEntry
	if (opcion == 1) {
		gtk_label_set_text(label1, texto_1);
		gtk_label_set_text(label2, "PC");
		gtk_label_set_text(Fichas_humano, "24");
		gtk_label_set_text(Fichas_PC, "24");
	} else if (opcion == 2) {
		gtk_label_set_text(label1, "PC");
		gtk_label_set_text(label2, texto_1);
		gtk_label_set_text(Fichas_humano, "24");
		gtk_label_set_text(Fichas_PC, "23");
	} else if (opcion == 3) {
		gtk_label_set_text(label1, "PC");
		gtk_label_set_text(label2, texto_1);
		gtk_label_set_text(Fichas_humano, "24");
		gtk_label_set_text(Fichas_PC, "23");
	}

	gtk_widget_hide(Ventana_Jugar2);
	gtk_widget_show_all(Ventana_Empezar_Partida);
	//main_C (opcion, colorP1, IA);
	if (opcion == 2) {
		juegaPc(PLAYER1, IA);
		if (colorP2 == 2) {
			for (int filas = 0; filas < 5; filas++) {
				for (int columnas = 0; columnas < 5; columnas++) {
					//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
					int estado = board[filas][columnas];
					char *nombreImagen = "Nada.jpeg";
					if (estado == 1) {
						nombreImagen = "Dado_Verde1.jpeg";
					} else if (estado == 2) {
						nombreImagen = "Dado_Verde2.jpeg";
					} else if (estado == 3) {
						nombreImagen = "Dado_Verde3.jpeg";
					} else if (estado == 4) {
						nombreImagen = "Dado_Verde4.jpeg";
					} else if (estado == 5) {
						nombreImagen = "Dado_Verde5.jpeg";
					} else if (estado == 6) {
						nombreImagen = "Dado_Verde6.jpeg";
					} else if (estado == 7) {
						nombreImagen = "Dado_Rojo1.jpeg";
					} else if (estado == 8) {
						nombreImagen = "Dado_Rojo2.jpeg";
					} else if (estado == 9) {
						nombreImagen = "Dado_Rojo3.jpeg";
					} else if (estado == 10) {
						nombreImagen = "Dado_Rojo4.jpeg";
					} else if (estado == 11) {
						nombreImagen = "Dado_Rojo5.jpeg";
					} else if (estado == 12) {
						nombreImagen = "Dado_Rojo6.jpeg";
					}
					if (estado != 0)
						gtk_widget_set_sensitive(
								GTK_WIDGET(Botones[filas][columnas]), FALSE);
					else
						gtk_widget_set_sensitive(
								GTK_WIDGET(Botones[filas][columnas]), TRUE);

					// Construir la ruta completa del archivo de imagen
					char rutaImagen[100];
					snprintf(rutaImagen, sizeof(rutaImagen),
							"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
							nombreImagen);

					// Actualizar la imagen de todos los botones
					gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
							nombreImagen);
				}
			}
		} else {
			for (int filas = 0; filas < 5; filas++) {
				for (int columnas = 0; columnas < 5; columnas++) {
					//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
					int estado = board[filas][columnas];
					char *nombreImagen = "Nada.jpeg";
					if (estado == 1) {
						nombreImagen = "Dado_Rojo1.jpeg";
					} else if (estado == 2) {
						nombreImagen = "Dado_Rojo2.jpeg";
					} else if (estado == 3) {
						nombreImagen = "Dado_Rojo3.jpeg";
					} else if (estado == 4) {
						nombreImagen = "Dado_Rojo4.jpeg";
					} else if (estado == 5) {
						nombreImagen = "Dado_Rojo5.jpeg";
					} else if (estado == 6) {
						nombreImagen = "Dado_Rojo6.jpeg";
					} else if (estado == 7) {
						nombreImagen = "Dado_Verde1.jpeg";
					} else if (estado == 8) {
						nombreImagen = "Dado_Verde2.jpeg";
					} else if (estado == 9) {
						nombreImagen = "Dado_Verde3.jpeg";
					} else if (estado == 10) {
						nombreImagen = "Dado_Verde4.jpeg";
					} else if (estado == 11) {
						nombreImagen = "Dado_Verde5.jpeg";
					} else if (estado == 12) {
						nombreImagen = "Dado_Verde6.jpeg";
					}
					if (estado != 0)
						gtk_widget_set_sensitive(
								GTK_WIDGET(Botones[filas][columnas]), FALSE);
					else
						gtk_widget_set_sensitive(
								GTK_WIDGET(Botones[filas][columnas]), TRUE);

					// Construir la ruta completa del archivo de imagen
					char rutaImagen[100];
					snprintf(rutaImagen, sizeof(rutaImagen),
							"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
							nombreImagen);

					// Actualizar la imagen de todos los botones
					gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
							nombreImagen);
				}
			}
		}
	} else if (opcion == 3) {
		if (SUMAP1 + SUMAP2 != 25) {
			juegaPc(PLAYER1, 1);
			juegaPc(PLAYER2, IA);
			if (colorP1 == 2) {
				for (int filas = 0; filas < 5; filas++) {
					for (int columnas = 0; columnas < 5; columnas++) {
						//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
						int estado = board[filas][columnas];
						char *nombreImagen = "Nada.jpeg";
						if (estado == 1) {
							nombreImagen = "Dado_Verde1.jpeg";
						} else if (estado == 2) {
							nombreImagen = "Dado_Verde2.jpeg";
						} else if (estado == 3) {
							nombreImagen = "Dado_Verde3.jpeg";
						} else if (estado == 4) {
							nombreImagen = "Dado_Verde4.jpeg";
						} else if (estado == 5) {
							nombreImagen = "Dado_Verde5.jpeg";
						} else if (estado == 6) {
							nombreImagen = "Dado_Verde6.jpeg";
						} else if (estado == 7) {
							nombreImagen = "Dado_Rojo1.jpeg";
						} else if (estado == 8) {
							nombreImagen = "Dado_Rojo2.jpeg";
						} else if (estado == 9) {
							nombreImagen = "Dado_Rojo3.jpeg";
						} else if (estado == 10) {
							nombreImagen = "Dado_Rojo4.jpeg";
						} else if (estado == 11) {
							nombreImagen = "Dado_Rojo5.jpeg";
						} else if (estado == 12) {
							nombreImagen = "Dado_Rojo6.jpeg";
						}
						if (estado != 0)
							gtk_widget_set_sensitive(
									GTK_WIDGET(Botones[filas][columnas]),
									FALSE);
						else
							gtk_widget_set_sensitive(
									GTK_WIDGET(Botones[filas][columnas]), TRUE);

						// Construir la ruta completa del archivo de imagen
						char rutaImagen[100];
						snprintf(rutaImagen, sizeof(rutaImagen),
								"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
								nombreImagen);

						// Actualizar la imagen de todos los botones
						gtk_image_set_from_file(
								GTK_IMAGE(Dados[filas][columnas]),
								nombreImagen);
					}
				}
			} else {
				for (int filas = 0; filas < 5; filas++) {
					for (int columnas = 0; columnas < 5; columnas++) {
						//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
						int estado = board[filas][columnas];
						char *nombreImagen = "Nada.jpeg";
						if (estado == 1) {
							nombreImagen = "Dado_Rojo1.jpeg";
						} else if (estado == 2) {
							nombreImagen = "Dado_Rojo2.jpeg";
						} else if (estado == 3) {
							nombreImagen = "Dado_Rojo3.jpeg";
						} else if (estado == 4) {
							nombreImagen = "Dado_Rojo4.jpeg";
						} else if (estado == 5) {
							nombreImagen = "Dado_Rojo5.jpeg";
						} else if (estado == 6) {
							nombreImagen = "Dado_Rojo6.jpeg";
						} else if (estado == 7) {
							nombreImagen = "Dado_Verde1.jpeg";
						} else if (estado == 8) {
							nombreImagen = "Dado_Verde2.jpeg";
						} else if (estado == 9) {
							nombreImagen = "Dado_Verde3.jpeg";
						} else if (estado == 10) {
							nombreImagen = "Dado_Verde4.jpeg";
						} else if (estado == 11) {
							nombreImagen = "Dado_Verde5.jpeg";
						} else if (estado == 12) {
							nombreImagen = "Dado_Verde6.jpeg";
						}
						if (estado != 0)
							gtk_widget_set_sensitive(
									GTK_WIDGET(Botones[filas][columnas]),
									FALSE);
						else
							gtk_widget_set_sensitive(
									GTK_WIDGET(Botones[filas][columnas]), TRUE);

						// Construir la ruta completa del archivo de imagen
						char rutaImagen[100];
						snprintf(rutaImagen, sizeof(rutaImagen),
								"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
								nombreImagen);

						// Actualizar la imagen de todos los botones
						gtk_image_set_from_file(
								GTK_IMAGE(Dados[filas][columnas]),
								nombreImagen);
					}
				}
			}
		}

	}
}

void obtener_tablero(int x, int y, int turno) {

	int sumaAcumulada = 0;
	int tempDado = 0;
	switch (opcion) {
	case 1: //HUMANO VS PC
		juegaHumano(x, y, PLAYER1);
		printf("%d", count);
		if (count > 2) {

			// Las coord de los  dados adyacentes que se van a comer
			gtk_widget_show_all(Ventana_Captura2);
			//Ve el estado del boton arriba
			if (gtk_toggle_button_get_active(Arriba) == TRUE) {
				adyx = x - 1;
				adyy = y;
				for (int i = 0; i < 4; i++)
					if (hayDadosAdy[i] && adyacentes[i][0] == adyx
							&& adyacentes[i][1] == adyy) {
						// Controlar que no pase 6 y que el dado adyacente no sea 6
						if (tablero == MAINBOARD && board[adyx][adyy] != 6
								&& board[adyx][adyy] != 12)
							tempDado = board[adyx][adyy]
									+ (board[adyx][adyy] < 6 ? 0 : -6);
						else if (tablero == MIRRORBOARD
								&& mirror[adyx][adyy] != 6
								&& mirror[adyx][adyy] != 12)
							tempDado = mirror[adyx][adyy]
									+ (mirror[adyx][adyy] < 6 ? 0 : -6);
						else {
							printf(
									"\nError. No se puede comer un dado con valor 6");
							continue;
						}

						if (sumaAcumulada + tempDado <= 6)
							sumaAcumulada += tempDado;
						else {
							printf(
									"\nError. La suma de dados a comer supera 6");
							continue;
						}

						adyXY[adyElegidos][0] = adyx;
						adyXY[adyElegidos++][1] = adyy;
					}
			}

		 else if (gtk_toggle_button_get_active(Abajo) == TRUE) {
			adyx = x + 1;
			adyy = y;
			for (int i = 0; i < 4; i++)
				if (hayDadosAdy[i] && adyacentes[i][0] == adyx
						&& adyacentes[i][1] == adyy) {
					// Controlar que no pase 6 y que el dado adyacente no sea 6
					if (tablero == MAINBOARD && board[adyx][adyy] != 6
							&& board[adyx][adyy] != 12)
						tempDado = board[adyx][adyy]
								+ (board[adyx][adyy] < 6 ? 0 : -6);
					else if (tablero == MIRRORBOARD && mirror[adyx][adyy] != 6
							&& mirror[adyx][adyy] != 12)
						tempDado = mirror[adyx][adyy]
								+ (mirror[adyx][adyy] < 6 ? 0 : -6);
					else {
						printf(
								"\nError. No se puede comer un dado con valor 6");
						continue;
					}

					if (sumaAcumulada + tempDado <= 6)
						sumaAcumulada += tempDado;
					else {
						printf("\nError. La suma de dados a comer supera 6");
						continue;
					}

					adyXY[adyElegidos][0] = adyx;
					adyXY[adyElegidos++][1] = adyy;
				}
	}else if (gtk_toggle_button_get_active(Abajo) == FALSE){

	}else if (gtk_toggle_button_get_active(Izquierda) == TRUE){
		adyx = x;
		adyy = y-1;
		for (int i = 0; i < 4; i++)
			if (hayDadosAdy[i] && adyacentes[i][0] == adyx
					&& adyacentes[i][1] == adyy) {
				// Controlar que no pase 6 y que el dado adyacente no sea 6
				if (tablero == MAINBOARD && board[adyx][adyy] != 6
						&& board[adyx][adyy] != 12)
					tempDado = board[adyx][adyy]
							+ (board[adyx][adyy] < 6 ? 0 : -6);
				else if (tablero == MIRRORBOARD && mirror[adyx][adyy] != 6
						&& mirror[adyx][adyy] != 12)
					tempDado = mirror[adyx][adyy]
							+ (mirror[adyx][adyy] < 6 ? 0 : -6);
				else {
					printf(
							"\nError. No se puede comer un dado con valor 6");
					continue;
				}

				if (sumaAcumulada + tempDado <= 6)
					sumaAcumulada += tempDado;
				else {
					printf("\nError. La suma de dados a comer supera 6");
					continue;
				}

				adyXY[adyElegidos][0] = adyx;
				adyXY[adyElegidos++][1] = adyy;
			}
	}else if (gtk_toggle_button_get_active(Izquierda) == FALSE){

	}else if (gtk_toggle_button_get_active(Derecha) == TRUE){
		adyx = x;
		adyy = y+1;
		for (int i = 0; i < 4; i++)
			if (hayDadosAdy[i] && adyacentes[i][0] == adyx
					&& adyacentes[i][1] == adyy) {
				// Controlar que no pase 6 y que el dado adyacente no sea 6
				if (tablero == MAINBOARD && board[adyx][adyy] != 6
						&& board[adyx][adyy] != 12)
					tempDado = board[adyx][adyy]
							+ (board[adyx][adyy] < 6 ? 0 : -6);
				else if (tablero == MIRRORBOARD && mirror[adyx][adyy] != 6
						&& mirror[adyx][adyy] != 12)
					tempDado = mirror[adyx][adyy]
							+ (mirror[adyx][adyy] < 6 ? 0 : -6);
				else {
					printf(
							"\nError. No se puede comer un dado con valor 6");
					continue;
				}

				if (sumaAcumulada + tempDado <= 6)
					sumaAcumulada += tempDado;
				else {
					printf("\nError. La suma de dados a comer supera 6");
					continue;
				}

				adyXY[adyElegidos][0] = adyx;
				adyXY[adyElegidos++][1] = adyy;
			}
	}
	}else {
		juegaPc(PLAYER2, IA);
	}
	printBoard();
	break;
	case 2:
	//PC VS HUMANO
	juegaHumano(x, y, PLAYER2);
	break;
	case 3:
	//PC1 VS PC2
	if (turno++ % 2 == 0)
		juegaPc(PLAYER1, IA);
	else
		juegaPc(PLAYER2, IA);
	//printBoard();
	break;
	break;
	case 9:
	//Salir
	break;
	default:
	break;
}
}

void cambiar_texto() {
const gchar *texto_1 = gtk_entry_get_text(Nombre_1);
// Obtener el GtkLabel
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, texto_1);
}

void actualizar_boton1() {
int SUMAP1 = 0, SUMAP2 = 0;
obtener_tablero(0, 0, turno);

// Obtener el GtkLabel
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);

if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}

} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	// Obtener el GtkLabel
	GtkLabel *Turno_Jugador = GTK_LABEL(
			gtk_builder_get_object(builder, "Turno_Jugador"));
	gtk_label_set_text(Turno_Jugador, "PC");
	g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}

			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	GtkLabel *Turno_Jugador = GTK_LABEL(
			gtk_builder_get_object(builder, "Turno_Jugador"));
	gtk_label_set_text(Turno_Jugador, "PC");
	g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	GtkLabel *Turno_Jugador = GTK_LABEL(
			gtk_builder_get_object(builder, "Turno_Jugador"));
	gtk_label_set_text(Turno_Jugador, "PC");
	g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton2() {
int SUMAP1 = 0, SUMAP2 = 0;
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
obtener_tablero(0, 1, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton3() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 0, y = 2;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}

			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton4() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 0, y = 3;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton5() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 0, y = 4;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton6() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 1, y = 0;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton7() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 1, y = 1;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton8() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 1, y = 2;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton9() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 1, y = 3;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton10() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 1, y = 4;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton11() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 2, y = 0;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton12() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 2, y = 1;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton13() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 2, y = 2;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton14() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 2, y = 3;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton15() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 2, y = 4;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton16() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 3, y = 0;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton17() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 3, y = 1;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton18() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 3, y = 2;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton19() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 3, y = 3;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton20() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 3, y = 4;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton21() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 4, y = 0;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}

}

void actualizar_boton22() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 4, y = 1;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton23() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 4, y = 2;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton24() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 4, y = 3;
obtener_tablero(x, y, turno);
if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void actualizar_boton25() {
GtkLabel *Turno_Jugador = GTK_LABEL(
		gtk_builder_get_object(builder, "Turno_Jugador"));
gtk_label_set_text(Turno_Jugador, "PC");
g_timeout_add(100, (GSourceFunc) cambiar_texto, NULL);
int SUMAP1 = 0, SUMAP2 = 0;
int x = 4, y = 4;
obtener_tablero(x, y, turno);

if (opcion == 1 && colorP1 == 1 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 1 && colorP1 == 2 && (IA == 1 || IA == 2)) {
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//obtenemos el estado de la posicion en la matriz
			int estado = board[filas][columnas];
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 > SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 2 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Verde6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Rojo6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
} else if (opcion == 2 && colorP2 == 1 && (IA == 1 || IA == 2)) {
	juegaPc(PLAYER1, IA);
	for (int filas = 0; filas < 5; filas++) {
		for (int columnas = 0; columnas < 5; columnas++) {
			//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
			int estado = board[filas][columnas];
			char *nombreImagen = "Nada.jpeg";
			if (estado == 1) {
				nombreImagen = "Dado_Rojo1.jpeg";
			} else if (estado == 2) {
				nombreImagen = "Dado_Rojo2.jpeg";
			} else if (estado == 3) {
				nombreImagen = "Dado_Rojo3.jpeg";
			} else if (estado == 4) {
				nombreImagen = "Dado_Rojo4.jpeg";
			} else if (estado == 5) {
				nombreImagen = "Dado_Rojo5.jpeg";
			} else if (estado == 6) {
				nombreImagen = "Dado_Rojo6.jpeg";
			} else if (estado == 7) {
				nombreImagen = "Dado_Verde1.jpeg";
			} else if (estado == 8) {
				nombreImagen = "Dado_Verde2.jpeg";
			} else if (estado == 9) {
				nombreImagen = "Dado_Verde3.jpeg";
			} else if (estado == 10) {
				nombreImagen = "Dado_Verde4.jpeg";
			} else if (estado == 11) {
				nombreImagen = "Dado_Verde5.jpeg";
			} else if (estado == 12) {
				nombreImagen = "Dado_Verde6.jpeg";
			}
			if (estado != 0)
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						FALSE);
			else
				gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]),
						TRUE);

			// Construir la ruta completa del archivo de imagen
			char rutaImagen[100];
			snprintf(rutaImagen, sizeof(rutaImagen),
					"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
					nombreImagen);

			// Actualizar la imagen de todos los botones
			gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
					nombreImagen);
			if (board[filas][columnas] <= 6 && board[filas][columnas] != 0) {
				SUMAP1++;
			} else if (board[filas][columnas] > 6) {
				SUMAP2++;
			}
		}
	}
	int dados1, dados2;
	dados1 = 24 - SUMAP1;
	dados2 = 24 - SUMAP2;
	printf("%d %d", SUMAP1, SUMAP2);
	// Convertir los valores a cadenas de caracteres
	char *dados1_str = g_strdup_printf("%d", dados1);
	char *dados2_str = g_strdup_printf("%d", dados2);

	// Establecer el texto en las etiquetas
	gtk_label_set_text(GTK_LABEL(Fichas_PC), dados1_str);
	gtk_label_set_text(GTK_LABEL(Fichas_humano), dados2_str);

	// Liberar la memoria asignada para las cadenas
	g_free(dados1_str);
	g_free(dados2_str);
	int SUMA = SUMAP1 + SUMAP2;
	if (SUMA == 25) {
		gtk_widget_hide(Ventana_Empezar_Partida);
		if (SUMAP1 < SUMAP2) {
			gtk_widget_show_all(Ventana_Ganador);
			resultado = 1;
		} else {
			gtk_widget_show_all(Ventana_Perdedor);
			resultado = 0;
		}
	}
}
}

void reiniciar_partida() {
for (int filas = 0; filas < 5; filas++) {
	for (int columnas = 0; columnas < 5; columnas++) {
		board[filas][columnas] = 0;
		gtk_widget_set_sensitive(GTK_WIDGET(Botones[filas][columnas]), TRUE);
		// Construir la ruta completa del archivo de imagen
		char rutaImagen[100];
		snprintf(rutaImagen, sizeof(rutaImagen),
				"/home/lp1-2023/eclipse-workspace/TPF_GTK/Nada.jpeg");
		// Actualizar la imagen de todos los botones
		gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]), rutaImagen);
	}
}
if (opcion == 2) {
	juegaPc(PLAYER1, IA);
	if (colorP2 == 2) {
		for (int filas = 0; filas < 5; filas++) {
			for (int columnas = 0; columnas < 5; columnas++) {
				//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
				int estado = board[filas][columnas];
				char *nombreImagen = "Nada.jpeg";
				if (estado == 1) {
					nombreImagen = "Dado_Verde1.jpeg";
				} else if (estado == 2) {
					nombreImagen = "Dado_Verde2.jpeg";
				} else if (estado == 3) {
					nombreImagen = "Dado_Verde3.jpeg";
				} else if (estado == 4) {
					nombreImagen = "Dado_Verde4.jpeg";
				} else if (estado == 5) {
					nombreImagen = "Dado_Verde5.jpeg";
				} else if (estado == 6) {
					nombreImagen = "Dado_Verde6.jpeg";
				} else if (estado == 7) {
					nombreImagen = "Dado_Rojo1.jpeg";
				} else if (estado == 8) {
					nombreImagen = "Dado_Rojo2.jpeg";
				} else if (estado == 9) {
					nombreImagen = "Dado_Rojo3.jpeg";
				} else if (estado == 10) {
					nombreImagen = "Dado_Rojo4.jpeg";
				} else if (estado == 11) {
					nombreImagen = "Dado_Rojo5.jpeg";
				} else if (estado == 12) {
					nombreImagen = "Dado_Rojo6.jpeg";
				}
				if (estado != 0)
					gtk_widget_set_sensitive(
							GTK_WIDGET(Botones[filas][columnas]), FALSE);
				else
					gtk_widget_set_sensitive(
							GTK_WIDGET(Botones[filas][columnas]), TRUE);

				// Construir la ruta completa del archivo de imagen
				char rutaImagen[100];
				snprintf(rutaImagen, sizeof(rutaImagen),
						"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
						nombreImagen);

				// Actualizar la imagen de todos los botones
				gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
						nombreImagen);
			}
		}
	} else {
		for (int filas = 0; filas < 5; filas++) {
			for (int columnas = 0; columnas < 5; columnas++) {
				//Asignamos a una variable el nombre de la imagen dependiendo del numero de la matriz que tendremos y de la opcion elegida
				int estado = board[filas][columnas];
				char *nombreImagen = "Nada.jpeg";
				if (estado == 1) {
					nombreImagen = "Dado_Rojo1.jpeg";
				} else if (estado == 2) {
					nombreImagen = "Dado_Rojo2.jpeg";
				} else if (estado == 3) {
					nombreImagen = "Dado_Rojo3.jpeg";
				} else if (estado == 4) {
					nombreImagen = "Dado_Rojo4.jpeg";
				} else if (estado == 5) {
					nombreImagen = "Dado_Rojo5.jpeg";
				} else if (estado == 6) {
					nombreImagen = "Dado_Rojo6.jpeg";
				} else if (estado == 7) {
					nombreImagen = "Dado_Verde1.jpeg";
				} else if (estado == 8) {
					nombreImagen = "Dado_Verde2.jpeg";
				} else if (estado == 9) {
					nombreImagen = "Dado_Verde3.jpeg";
				} else if (estado == 10) {
					nombreImagen = "Dado_Verde4.jpeg";
				} else if (estado == 11) {
					nombreImagen = "Dado_Verde5.jpeg";
				} else if (estado == 12) {
					nombreImagen = "Dado_Verde6.jpeg";
				}
				if (estado != 0)
					gtk_widget_set_sensitive(
							GTK_WIDGET(Botones[filas][columnas]), FALSE);
				else
					gtk_widget_set_sensitive(
							GTK_WIDGET(Botones[filas][columnas]), TRUE);

				// Construir la ruta completa del archivo de imagen
				char rutaImagen[100];
				snprintf(rutaImagen, sizeof(rutaImagen),
						"/home/lp1-2023/eclipse-workspace/TPF_GTK/%s",
						nombreImagen);

				// Actualizar la imagen de todos los botones
				gtk_image_set_from_file(GTK_IMAGE(Dados[filas][columnas]),
						nombreImagen);
			}
		}
	}
}
gtk_widget_hide(Ventana_Reiniciar);
}

void funcCapturar (GObject *boton, gpointer data)
{

	gtk_widget_hide(Ventana_Captura2);
}

int main(int argc, char *argv[]) {
// Inicializamos la semilla para generar n
srand(time(NULL));

//Punteros a elementos del Glade
gtk_init(&argc, &argv);

builder = gtk_builder_new_from_file("TPFgtk.V1");
Ventana_Principal = GTK_WIDGET(
		gtk_builder_get_object(builder, "Ventana_Principal"));
Ventana_Creditos = GTK_WIDGET(
		gtk_builder_get_object(builder, "Ventana_Creditos"));
ventanaReglas1 = GTK_WIDGET(gtk_builder_get_object(builder, "Ventana_Reglas1"));
Ventana_Reglas2 = GTK_WIDGET(
		gtk_builder_get_object(builder, "Ventana_Reglas2"));
Ventana_Jugar = GTK_WIDGET(gtk_builder_get_object(builder, "Ventana_Jugar"));
Ventana_Jugar2 = GTK_WIDGET(gtk_builder_get_object(builder, "Ventana_Jugar2"));
Ventana_Empezar_Partida = GTK_WIDGET(
		gtk_builder_get_object(builder, "Ventana_Empezar_Partida"));
Ventana_Rendirse = GTK_WIDGET(
		gtk_builder_get_object(builder, "Ventana_Rendirse"));
Ventana_Reiniciar = GTK_WIDGET(
		gtk_builder_get_object(builder, "Ventana_Reiniciar"));
Ventana_Ganador = GTK_WIDGET(
		gtk_builder_get_object(builder, "Ventana_Ganador"));
Ventana_Perdedor = GTK_WIDGET(
		gtk_builder_get_object(builder, "Ventana_Perdedor"));
Estadisticas = GTK_WIDGET(gtk_builder_get_object(builder, "Estadisticas"));
Ventana_Num_Capturas = GTK_WIDGET(
		gtk_builder_get_object(builder, "Ventana_Num_Capturas"));
Ventana_Captura2 = GTK_WIDGET(
		gtk_builder_get_object(builder, "Ventana_Captura2"));

//Asignacion a los labels de numero de dados
Fichas_humano = GTK_LABEL(gtk_builder_get_object(builder, "Fichas_humano"));
Fichas_PC = GTK_LABEL(gtk_builder_get_object(builder, "Fichas_PC"));

//Asiganción de los botones de las distintas ventanas
Boton_Reglas_Menu = gtk_builder_get_object(builder, "Boton_Reglas_Menu");
Volver_A_Menu_Reglas1 = gtk_builder_get_object(builder,
		"Volver_A_Menu_Reglas1");
Boton_Siguiente_Reglas = gtk_builder_get_object(builder,
		"Boton_Siguiente_Reglas");
Boton_PaganteReglas1_Reglas2 = gtk_builder_get_object(builder,
		"Boton_PaganteReglas1_Reglas2");
Volver_A_Menu_Reglas2 = gtk_builder_get_object(builder,
		"Volver_A_Menu_Reglas2");
Boton_Creditos = gtk_builder_get_object(builder, "Boton_Creditos");
Volver_A_Menu_Creditos = gtk_builder_get_object(builder,
		"Volver_A_Menu_Creditos");
Boton_Jugar = gtk_builder_get_object(builder, "Boton_Jugar");
Volver_al_Menu_Jugar = gtk_builder_get_object(builder, "Volver_al_Menu_Jugar");
Humano_VS_PC = GTK_TOGGLE_BUTTON(
		gtk_builder_get_object(builder, "Humano_VS_PC"));
PC_vs_Humano = GTK_TOGGLE_BUTTON(
		gtk_builder_get_object(builder, "PC_vs_Humano"));
Al_Azar = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "Al_Azar"));
PC_vs_PC = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "PC_vs_PC"));
Seleccionar_Red = GTK_TOGGLE_BUTTON(
		gtk_builder_get_object(builder, "Seleccionar_Red"));
Seleccionar_Green = GTK_TOGGLE_BUTTON(
		gtk_builder_get_object(builder, "Seleccionar_Green"));
Siguiente_Jugar = gtk_builder_get_object(builder, "Siguiente_Jugar");
Volver_al_Menu_Jugar2 = gtk_builder_get_object(builder,
		"Volver_al_Menu_Jugar2");
IA_Aleatoria = GTK_TOGGLE_BUTTON(
		gtk_builder_get_object(builder, "IA_Aleatoria"));
IA_Inteligente = GTK_TOGGLE_BUTTON(
		gtk_builder_get_object(builder, "IA_Inteligente"));
Nombre_Jugador1 = GTK_ENTRY(gtk_builder_get_object(builder, "Nombre_Jugador1"));
Siguiente_Jugar2 = gtk_builder_get_object(builder, "Siguiente_Jugar2");
Boton_Ayuda_Juego = gtk_builder_get_object(builder, "Boton_Ayuda_Juego");
Boton_Acerca_de_Juego = gtk_builder_get_object(builder,
		"Boton_Acerca_de_Juego");
Boton_Rendirse_Juego = gtk_builder_get_object(builder, "Boton_Rendirse_Juego");
Boton_continuar_ventRendirse = gtk_builder_get_object(builder,
		"Boton_continuar_ventRendirse");
Rendirse_Ventana_Rendirse = gtk_builder_get_object(builder,
		"Rendirse_Ventana_Rendirse");
Boton_Reiniciar_Partida = gtk_builder_get_object(builder,
		"Boton_Reiniciar_Partida");
Boton_continuar_Reiniciar = gtk_builder_get_object(builder,
		"Boton_continuar_Reiniciar");
Boton_Restart = gtk_builder_get_object(builder, "Boton_Restart");
Color_Random = gtk_builder_get_object(builder, "Color_Random");
Nombre_1 = GTK_ENTRY(gtk_builder_get_object(builder, "Nombre_Jugador1"));
Boton_Estadisticas_inic = gtk_builder_get_object(builder,
		"Boton_Estadisticas_inic");
Volver_Estadisticas = gtk_builder_get_object(builder, "Volver_Estadisticas");
Estadisticas_Tablero = gtk_builder_get_object(builder, "Estadisticas_Tablero");
Volver_al_Menu_Perdedor = gtk_builder_get_object(builder,
		"Volver_al_Menu_Perdedor");
Jugar_de_Nuevo_Perdedor = gtk_builder_get_object(builder,
		"Jugar_de_Nuevo_Perdedor");
Volver_al_Menu_Ganador = gtk_builder_get_object(builder,
		"Volver_al_Menu_Ganador");
Jugar_de_Nuevo_Ganador = gtk_builder_get_object(builder,
		"Jugar_de_Nuevo_Ganador");
Capturar2 = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "Capturar2"));
Capturar3 = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "Capturar3"));
Capturar4 = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "Capturar4"));
BotonIrCaptura = gtk_builder_get_object(builder, "BotonIrCaptura");
Arriba = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "Arriba"));
Abajo = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "Abajo"));
Izquierda = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "Izquierda"));
Derecha = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "Derecha"));
Capturar = GTK_BUTTON(gtk_builder_get_object (builder, "Capturar"));

//Asignación de las imágenes de los botones del tablero
int cont_1 = 1;
for (int k = 0; k < 5; k++) {
	for (int j = 0; j < 5; j++) {
		char num_img_1[4];
		char num_img_2[3];
		num_img_1[0] = 'i';
		num_img_2[0] = 'i';
		if (cont_1 < 10) {
			num_img_1[1] = '0' + cont_1;
			num_img_1[2] = '\0';
			cont_1++;
			Dados[k][j] = GTK_WIDGET(
					gtk_builder_get_object(builder, num_img_1));
		} else {
			num_img_2[1] = '0' + (cont_1 / 10);
			num_img_2[2] = '0' + (cont_1 % 10);
			num_img_2[3] = '\0';
			cont_1++;
			Dados[k][j] = GTK_WIDGET(
					gtk_builder_get_object(builder, num_img_2));
		}
	}
}

//Asignacion de las imagenes de los botones del tablero
int cont = 1;
for (int i = 0; i < 5; i++) {
	for (int j = 0; j < 5; j++) {
		char num_boton[3];
		if (cont < 10) {
			num_boton[0] = '0' + cont;
			num_boton[1] = '\0';
			cont++;
		} else {
			num_boton[0] = '0' + (cont / 10);
			num_boton[1] = '0' + (cont % 10);
			num_boton[2] = '\0';
			cont++;
		}
		Botones[i][j] = gtk_builder_get_object(builder, num_boton);
	}
}

//Callbacks que terminan el codigo
g_signal_connect(Ventana_Principal, "destroy", G_CALLBACK (gtk_main_quit),
		NULL);
g_signal_connect(Ventana_Jugar, "destroy", G_CALLBACK (gtk_main_quit), NULL);
g_signal_connect(Ventana_Jugar2, "destroy", G_CALLBACK (gtk_main_quit), NULL);
g_signal_connect(Ventana_Empezar_Partida, "destroy", G_CALLBACK (gtk_main_quit),
		NULL);

g_signal_connect(Boton_Reglas_Menu, "clicked", G_CALLBACK (funcLlamar),
		ventanaReglas1);
g_signal_connect(Volver_A_Menu_Reglas1, "clicked", G_CALLBACK (funcOcultar),
		ventanaReglas1);
g_signal_connect(Boton_Siguiente_Reglas, "clicked",
		G_CALLBACK(funcMostrarOcultar1), NULL);
g_signal_connect(Boton_PaganteReglas1_Reglas2, "clicked",
		G_CALLBACK(funcMostrarOcultar2), NULL);
g_signal_connect(Volver_A_Menu_Reglas2, "clicked", G_CALLBACK (funcOcultar),
		Ventana_Reglas2);
g_signal_connect(Boton_Creditos, "clicked", G_CALLBACK (funcLlamar),
		Ventana_Creditos);
g_signal_connect(Volver_A_Menu_Creditos, "clicked", G_CALLBACK (funcOcultar),
		Ventana_Creditos);
g_signal_connect(Boton_Jugar, "clicked", G_CALLBACK (funcMostrarOcultar3),
		Ventana_Jugar);
g_signal_connect(Volver_al_Menu_Jugar, "clicked",
		G_CALLBACK (funcMostrarOcultar4), NULL);
g_signal_connect(Siguiente_Jugar, "clicked", G_CALLBACK (funcMostrarOcultar5),
		NULL);
g_signal_connect(Volver_al_Menu_Jugar2, "clicked",
		G_CALLBACK (funcMostrarOcultar6), NULL);
g_signal_connect(Boton_Estadisticas_inic, "clicked",
		G_CALLBACK (funcMostrarStats), Estadisticas);
g_signal_connect(Volver_Estadisticas, "clicked", G_CALLBACK (funcOcultar),
		Estadisticas);
g_signal_connect(Estadisticas_Tablero, "clicked", G_CALLBACK (funcMostrarStats),
		Estadisticas);
g_signal_connect(Humano_VS_PC, "toggled", G_CALLBACK (funcBotonesModoJuego1),
		Ventana_Jugar);
g_signal_connect(PC_vs_Humano, "toggled", G_CALLBACK (funcBotonesModoJuego2),
		Ventana_Jugar);
g_signal_connect(Al_Azar, "toggled", G_CALLBACK (funcBotonesModoJuego3),
		Ventana_Jugar);
g_signal_connect(PC_vs_PC, "toggled", G_CALLBACK (funcBotonesModoJuego4),
		Ventana_Jugar);
g_signal_connect(Seleccionar_Red, "toggled", G_CALLBACK (funcColorRed),
		Ventana_Jugar);
g_signal_connect(Seleccionar_Green, "toggled", G_CALLBACK (funcColorGreen),
		Ventana_Jugar);
g_signal_connect(Color_Random, "toggled", G_CALLBACK (funcColorRandom),
		Ventana_Jugar);
g_signal_connect(IA_Aleatoria, "toggled", G_CALLBACK (funcBotonesIA1), NULL);
g_signal_connect(IA_Inteligente, "toggled", G_CALLBACK (funcBotonesIA2), NULL);
g_signal_connect(Siguiente_Jugar2, "clicked", G_CALLBACK(comenzar_juego), NULL);
g_signal_connect(Boton_Ayuda_Juego, "clicked", G_CALLBACK(funcLlamar),
		ventanaReglas1);
g_signal_connect(Boton_Acerca_de_Juego, "clicked", G_CALLBACK(funcLlamar),
		Ventana_Creditos);
g_signal_connect(Boton_Rendirse_Juego, "clicked", G_CALLBACK(funcLlamar),
		Ventana_Rendirse);
g_signal_connect(Boton_continuar_ventRendirse, "clicked",
		G_CALLBACK(funcOcultar), Ventana_Rendirse);
g_signal_connect(Boton_Reiniciar_Partida, "clicked", G_CALLBACK(funcLlamar),
		Ventana_Reiniciar);
g_signal_connect(Boton_continuar_Reiniciar, "clicked", G_CALLBACK(funcOcultar),
		Ventana_Reiniciar);
g_signal_connect(Boton_Restart, "clicked", G_CALLBACK(reiniciar_partida), NULL);
g_signal_connect(Rendirse_Ventana_Rendirse, "clicked",
		G_CALLBACK(funcMostrarOcultar8), NULL);
g_signal_connect(Volver_al_Menu_Perdedor, "clicked",
		G_CALLBACK(funcMostrarOcultar9), Ventana_Principal);
g_signal_connect(Jugar_de_Nuevo_Perdedor, "clicked",
		G_CALLBACK(funcMostrarOcultar9), Ventana_Jugar);
g_signal_connect(Volver_al_Menu_Ganador, "clicked",
		G_CALLBACK (funcMostrarOcultar10), Ventana_Principal);
g_signal_connect(Jugar_de_Nuevo_Ganador, "clicked",
		G_CALLBACK (funcMostrarOcultar10), Ventana_Jugar);
g_signal_connect(Capturar2, "toggled", G_CALLBACK (capturar2), NULL);
g_signal_connect(Capturar3, "toggled", G_CALLBACK (capturar3), NULL);
g_signal_connect(Capturar4, "toggled", G_CALLBACK (capturar4), NULL);
g_signal_connect(BotonIrCaptura, "clicked", G_CALLBACK (funcMostrarOcultar11),
		NULL);

g_signal_connect(Botones[0][0], "clicked", G_CALLBACK(actualizar_boton1), NULL);
g_signal_connect(Botones[0][1], "clicked", G_CALLBACK(actualizar_boton2), NULL);
g_signal_connect(Botones[0][2], "clicked", G_CALLBACK(actualizar_boton3), NULL);
g_signal_connect(Botones[0][3], "clicked", G_CALLBACK(actualizar_boton4), NULL);
g_signal_connect(Botones[0][4], "clicked", G_CALLBACK(actualizar_boton5), NULL);
g_signal_connect(Botones[1][0], "clicked", G_CALLBACK(actualizar_boton6), NULL);
g_signal_connect(Botones[1][1], "clicked", G_CALLBACK(actualizar_boton7), NULL);
g_signal_connect(Botones[1][2], "clicked", G_CALLBACK(actualizar_boton8), NULL);
g_signal_connect(Botones[1][3], "clicked", G_CALLBACK(actualizar_boton9), NULL);
g_signal_connect(Botones[1][4], "clicked", G_CALLBACK(actualizar_boton10),
		NULL);
g_signal_connect(Botones[2][0], "clicked", G_CALLBACK(actualizar_boton11),
		NULL);
g_signal_connect(Botones[2][1], "clicked", G_CALLBACK(actualizar_boton12),
		NULL);
g_signal_connect(Botones[2][2], "clicked", G_CALLBACK(actualizar_boton13),
		NULL);
g_signal_connect(Botones[2][3], "clicked", G_CALLBACK(actualizar_boton14),
		NULL);
g_signal_connect(Botones[2][4], "clicked", G_CALLBACK(actualizar_boton15),
		NULL);
g_signal_connect(Botones[3][0], "clicked", G_CALLBACK(actualizar_boton16),
		NULL);
g_signal_connect(Botones[3][1], "clicked", G_CALLBACK(actualizar_boton17),
		NULL);
g_signal_connect(Botones[3][2], "clicked", G_CALLBACK(actualizar_boton18),
		NULL);
g_signal_connect(Botones[3][3], "clicked", G_CALLBACK(actualizar_boton19),
		NULL);
g_signal_connect(Botones[3][4], "clicked", G_CALLBACK(actualizar_boton20),
		NULL);
g_signal_connect(Botones[4][0], "clicked", G_CALLBACK(actualizar_boton21),
		NULL);
g_signal_connect(Botones[4][1], "clicked", G_CALLBACK(actualizar_boton22),
		NULL);
g_signal_connect(Botones[4][2], "clicked", G_CALLBACK(actualizar_boton23),
		NULL);
g_signal_connect(Botones[4][3], "clicked", G_CALLBACK(actualizar_boton24),
		NULL);
g_signal_connect(Botones[4][4], "clicked", G_CALLBACK(actualizar_boton25),
		NULL);

gtk_widget_show_all(Ventana_Principal);

gtk_main();

return 0;
}
