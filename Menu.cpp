#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs.h"
#include "encuestas.h"
#include "Preguntas.h"
#include "respuestas.h"

void menuEncuesta(Encuesta **top, Pregunta *LPreg);
void menuPregunta(Encuesta **top, Pregunta **LPreg);
void menuRespuesta(Encuesta **top, Pregunta *LPreg, Respuesta **LRes);


int main(){
	
	Encuesta *tp=NULL;
	Pregunta *LPregunta=NULL;
	Respuesta *LRespuesta=NULL;
	char op;
	int band=1;
	
	while(band!=0){
		printf("\n-------------------------------------------------------------------------------\n");
		printf("\t\t\t\tSistemaEncuesta\n");
		printf("-------------------------------------------------------------------------------\n");
	
		printf("0- Salir.\n");
		printf("1- Encuesta.\n");
		printf("2- Pregunta.\n");
		printf("3- Respuesta.\n");
	
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
				menuEncuesta(&tp, LPregunta);
			break;
		
			case '2':
				menuPregunta(&tp, &LPregunta);
			break;
		
			case '3':
				menuRespuesta(&tp, LPregunta, &LRespuesta);
			break;
			default:
				printf("\n-------------------------------------------------------------------------------");
				puts("\n\t\t\tOpcion no valida, intente nuevo.");
				printf("-------------------------------------------------------------------------------\n\n");
		}
	}
	
	return 0;
}

void menuEncuesta(Encuesta **top, Pregunta *LPreg){
	char op;
	int band = 1;
	
	while(band!=0){
		printf("\n-------------------------------------------------------------------------------\n");
		printf("\t\t\t\tSistemaEncuesta Encuestas\n");
		printf("-------------------------------------------------------------------------------\n");
		printf("1- Alta Encuesta.\n");
		printf("2- Baja Encuesta.\n");
		printf("3- Listar Encuestas.csv.\n");
		printf("4- Listar pila Encuestas\n");
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
				bajaEncuesta(&(*top), LPreg);
			break;
			
			
			case '3':
				system("cls");
				//Listar Encuesta (archivo csv)
				listarEncuestas();
				
			break;
			
			case '4':
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

void menuPregunta(Encuesta **top, Pregunta **LPreg){
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
				
			break;
			
			case '3':
				system("cls");
				//Listar Preguntas
				
			break;

			
			default:
				system("cls");
				printf("\n-------------------------------------------------------------------------------");
				puts("\n\t\t\tOpcion no valida, intente nuevo.");
				printf("-------------------------------------------------------------------------------\n\n");
		}
	}
}

void menuRespuesta(){
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
				
			break;
			
			
			case '3':
				system("cls");
				//Listar Respuestas
				
			break;

			
			default:
				system("cls");
				printf("\n-------------------------------------------------------------------------------");
				puts("\n\t\t\tOpcion no valida, intente nuevo.");
				printf("-------------------------------------------------------------------------------\n\n");
		}
	}
}
