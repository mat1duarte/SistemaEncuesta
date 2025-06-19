#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void cargarParticipaciones(Encuesta **tope, Pregunta *iniP, Respuesta *iniR, Encuestador **Ent, Encuestador **Sal){
	int idEnc=0, existe=0, res=0;
	char fecha[20];
	
	printf("Ingrese la fecha en que se realizo: ");
	gets(fecha);
	
	listarEncuestadores(&(*Ent), &(*Sal));
	
	listarEncActivas(&(*tope));
	controlID(&idEnc);
	
	while(iniP != NULL){
		if(iniP->EncuestaId == idEnc){
			printf("\n%s\n",ini->,iniP->Pregunta);
				mostrarRespuestas(iniP->PreguntaId, iniR);
				
				do{
					existe = 0;
					res = 0;
				
					printf("\nIngrese el numero de respuesta: ");
					scanf("%d", &res);
					comprobarRes(&existe, iniP->PreguntaId, iniR, res, &idRes);
				}while(existe!=1);
				
				
				
				listarEncuestadores();
		}
		iniP = iniP->sgte;
	}
	
}

void mostrarRespuesta(int idPreg, Respuesta *rcRes){
	Respuesta *aux=NULL;
	
	if(rcRes != NULL){
		if(idPreg == rcRes->PreguntaId){
			printf("%d: %s\n",rcRes->RespuestaNro, rcRes->Respuesta);
		}
		aux = rcRes->sgte;
		while(aux != rcRes){
			if(idPreg == aux->PreguntaId){
				printf("%d: %s\n",aux->RespuestaNro, aux->Respuesta);
			}
			aux = aux->sgte;
		}
	}else{
		printf("No hay respuestas existentes\n");
	}
	
	
}

void comprobarRes(int *ex, int idPreg, Respuesta *rcRes, int res, int *idResp){
	Respuesta *aux = NULL;
	
	if((idPreg == rcRes->PreguntaId) && (res == rcRes->RespuestaNro)){
		*ex = 1;
		idResp = rcRes->RespuestaId;
	}
	
	aux = rcRes->sgte;
	while(aux != rcRes){
		if((idPreg == aux->PreguntaId) && (res == aux->RespuestaNro)){
			*ex = 1;
			idResp = rcRes->RespuestaId;
		}
		aux = aux->sgte;
	}
	
	
}
