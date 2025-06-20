#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs.h"
#include "encuestas.h"
#include "Preguntas.h"
#include "respuestas.h"
#include "encuestadores.h"
#include "Participaciones.h"


void menuEncuestador(Encuestador **Entrada, Encuestador **Salida);
void menuEncuesta(Encuesta **top, Pregunta *LPreg, Respuesta *LRes);
void menuPregunta(Encuesta **top, Pregunta **LPreg, Respuesta **LRespuesta);
void menuRespuesta(Encuesta **top, Pregunta *LPreg, Respuesta **LRes);
void menuProcesos(Encuesta **top, Pregunta *LPreg, Respuesta *LRes, Encuestador **Entrada, Encuestador **Salida, Participaciones **LPart);


int main(){
	
	Encuesta *tp=NULL;
	Pregunta *LPregunta=NULL;
	Respuesta *LRespuesta=NULL;
	Encuestador *E=NULL, *S=NULL;
	Participaciones *LPart = NULL;
	
	char op;
	int band=1;
	
	while(band!=0){
		printf("\n-------------------------------------------------------------------------------\n");
		printf("\t\t\t\tSistemaEncuesta\n");
		printf("-------------------------------------------------------------------------------\n");
	
		printf("0- Salir.\n");
		printf("1- Encuestador.\n");
		printf("2- Encuesta.\n");
		printf("3- Pregunta.\n");
		printf("4- Respuesta.\n");
		printf("5- Procesos\n");
	
		fflush(stdin);
		scanf("%c", &op);
		printf("\n");
		fflush(stdin);
	
		system("cls");
	
		switch(op){	
		case '0':
				band=0;
				printf("\n-------------------------------------------------------------------------------");
				puts("\n\t\t\t\tSaliendo...");
				printf("-------------------------------------------------------------------------------\n\n");
			break;
			
			case '1':
				menuEncuestador(&E,&S);
			break;
			
			case '2':
				menuEncuesta(&tp, LPregunta, LRespuesta);
			break;
		
			case '3':
				menuPregunta(&tp, &LPregunta,&LRespuesta);
			break;
		
			case '4':
				menuRespuesta(&tp, LPregunta, &LRespuesta);
			break;
			
			case '5':
				menuProcesos(&tp, LPregunta, LRespuesta, &E, &S, &LPart);
			break;
			
			default:
			printf("\n-------------------------------------------------------------------------------");
			puts("\n\t\t\tOpcion no valida, intente nuevo.");
			printf("-------------------------------------------------------------------------------\n\n");
		}
	}
	
	return 0;
}

void menuEncuestador(Encuestador **Entrada, Encuestador **Salida){
	char op;
	int band = 1;
	
	while(band!=0){
		printf("\n-------------------------------------------------------------------------------\n");
		printf("\t\t\t\tSistemaEncuesta Encuestas\n");
		printf("-------------------------------------------------------------------------------\n");
		printf("1- Alta Encuestador.\n");
		printf("2- listar Encuestadores.\n");
		printf("0- Volver.\n");
			
		fflush(stdin);
		scanf("%c", &op);
		printf("\n");
		fflush(stdin);
		
		switch(op){
			case '0':
				band=0;
				system("cls");
				printf("\n-------------------------------------------------------------------------------");
				puts("\n\t\t\t\tVolviendo al Menu.");
				printf("-------------------------------------------------------------------------------\n\n");
			break;
			
			case '1':
				system("cls");				
				//Alta Encuestadores
				altaEncuestador(&(*Entrada), &(*Salida));
			break;
			
			case '2':
				system("cls");
				//Listar cola encuestadores
				listarEncuestadores(&(*Entrada), &(*Salida));
			break;
			
			default:
			system("cls");
			printf("\n-------------------------------------------------------------------------------");
			puts("\n\t\t\tOpcion no valida, intente nuevo.");
			printf("-------------------------------------------------------------------------------\n\n");
		}
	}
}

void menuEncuesta(Encuesta **top, Pregunta *LPreg, Respuesta *LRes){
	char op;
	int band = 1;
	
	while(band!=0){
		printf("\n-------------------------------------------------------------------------------\n");
		printf("\t\t\t\tSistemaEncuesta Encuestas\n");
		printf("-------------------------------------------------------------------------------\n");
		printf("1- Alta Encuesta.\n");
		printf("2- Baja Encuesta.\n");
		printf("3- Listar pila Encuestas\n");
		printf("0- Volver.\n");
			
		fflush(stdin);
		scanf("%c", &op);
		printf("\n");
		fflush(stdin);
		
		switch(op){
			case '0':
				band=0;
				system("cls");
				printf("\n-------------------------------------------------------------------------------");
				puts("\n\t\t\t\tVolviendo al Menu.");
				printf("-------------------------------------------------------------------------------\n\n");
			break;
			
			case '1':
				system("cls");				
				//Alta Encuesta
				altaEncuesta(&(*top));
			break;
			
			case '2':
				system("cls");
				//Baja Encuesta
				bajaEncuesta(&(*top));
			break;
			
			case '3':
				//Listar pila Encuesta
				listartodapila(&(*top));
			break;
			
			default:
			system("cls");
			printf("\n-------------------------------------------------------------------------------");
			puts("\n\t\t\tOpcion no valida, intente nuevo.");
			printf("-------------------------------------------------------------------------------\n\n");
		}
	}
}

void menuPregunta(Encuesta **top, Pregunta **LPreg,Respuesta **LRespuesta){
	char op;
	int band = 1;
	
	while(band!=0){
		printf("\n-------------------------------------------------------------------------------\n");
		printf("\t\t\t\tSistemaEncuesta Preguntas\n");
		printf("-------------------------------------------------------------------------------\n");
		printf("1- Alta Preguntas.\n");
		printf("2- Baja Preguntas.\n");
		printf("3- Listar Preguntas.\n");
		printf("0- Volver.\n");
		
		fflush(stdin);
		scanf("%c", &op);
		printf("\n");
		fflush(stdin);
		
		switch(op){
			case '0':
				band=0;
				system("cls");
				printf("\n-------------------------------------------------------------------------------");
				puts("\n\t\t\t\tVolviendo al Menu.");
				printf("-------------------------------------------------------------------------------\n\n");
			break;
			
			case '1':
				system("cls");				
				//Alta Preguntas
				altaPregunta(&(*top), &(*LPreg));
			break;
			
			case '2':
				system("cls");
				//Baja Preguntas
				bajaPregunta(&(*top),&(*LPreg),&(*LRespuesta));
			break;
			
			case '3':
				system("cls");
				//Listar Preguntas
				ListarLESPreguntas(*LPreg);
			break;

			
			default:
			system("cls");
			printf("\n-------------------------------------------------------------------------------");
			puts("\n\t\t\tOpcion no valida, intente nuevo.");
			printf("-------------------------------------------------------------------------------\n\n");
		}
	}
}

void menuRespuesta(Encuesta **top, Pregunta *LPreg, Respuesta **LRes){
	char op;
	int band = 1;
	
	while(band!=0){
		printf("\n-------------------------------------------------------------------------------\n");
		printf("\t\t\t\tSistemaEncuesta Respuestas\n");
		printf("-------------------------------------------------------------------------------\n");
		printf("1- Alta Respuestas.\n");
		printf("2- Baja Respuestas.\n");
		printf("3- Listar Respuestas.\n");
		printf("0- Volver.\n");
		
		fflush(stdin);
		scanf("%c", &op);
		printf("\n");
		fflush(stdin);
		
		switch(op){
			case '0':
				band=0;
				system("cls");
				printf("\n-------------------------------------------------------------------------------");
				puts("\n\t\t\t\tVolviendo al Menu.");
				printf("-------------------------------------------------------------------------------\n\n");
			break;
			
			case '1':
				system("cls");				
				//Alta Respuestas
			    altaRespuesta(&(*top), LPreg, &(*LRes));
			break;
			
			case '2':
				system("cls");
				//Baja Respuestas
				bajaRespuestas(&(*top),LPreg,&(*LRes));
			break;
			
			
			case '3':
				system("cls");
				//Listar Respuestas
				ListarLcRespuestas(*LRes);
				
			break;

			
			default:
			system("cls");
			printf("\n-------------------------------------------------------------------------------");
			puts("\n\t\t\tOpcion no valida, intente nuevo.");
			printf("-------------------------------------------------------------------------------\n\n");
		}
	}
}

void menuProcesos(Encuesta **top, Pregunta *LPreg, Respuesta *LRes, Encuestador **Entrada, Encuestador **Salida, Participaciones **Lpar){
	char op;
	int band = 1;
	
	while(band!=0){
		printf("\n-------------------------------------------------------------------------------\n");
		printf("\t\t\t\tSistemaEncuesta Respuestas\n");
		printf("-------------------------------------------------------------------------------\n");
		printf("1- Carga Participaciones.\n");
		printf("2- Calcular ponderacion de una encuesta.\n");
		printf("3- Listar participaciones a una encuesta.\n");
		printf("4- Listar TODAS participaciones a una encuesta.\n");
		printf("0- Volver.\n");
		
		fflush(stdin);
		scanf("%c", &op);
		printf("\n");
		fflush(stdin);
		
		switch(op){
			case '0':
				band=0;
				system("cls");
				printf("\n-------------------------------------------------------------------------------");
				puts("\n\t\t\t\tVolviendo al Menu.");
				printf("-------------------------------------------------------------------------------\n\n");
			break;
			
			case '1':
				system("cls");				
				//Cargar participacion a una encuesta
			    cargarParticipaciones(&(*top), LPreg, LRes, &(*Entrada), &(*Salida), &(*Lpar));
			break;
			
			case '2':
				system("cls");
				//Calcular ponderacion de una encuesta x
				
			break;
			
			
			case '3':
				system("cls");
				//Listar participaciones de una encuesta x
					
			break;
			
			case '4':
				system("cls");
				//Listar todas las participaciones 
				listarTodasPart(*Lpar);
			break;

			
			default:
			system("cls");
			printf("\n-------------------------------------------------------------------------------");
			puts("\n\t\t\tOpcion no valida, intente nuevo.");
			printf("-------------------------------------------------------------------------------\n\n");
		}
	}
}
