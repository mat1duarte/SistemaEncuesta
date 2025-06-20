#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cargarParticipaciones(Encuesta **tope, Pregunta *iniP, Respuesta *iniR, Encuestador **Ent, Encuestador **Sal, Participaciones **iniPar);
int obtenerUltimoId(Participaciones *rcPar);
void mostrarRespuestas(int idPreg, Respuesta *rcRes);
void comprobarRes(int *ex, int idPreg, Respuesta *rcRes, int res, int *idResp);
Participaciones* insParticipaciones(Participaciones *nd, Participaciones *r);
void listarTodasPart(Participaciones *rcPar);


void cargarParticipaciones(Encuesta **tope, Pregunta *iniP, Respuesta *iniR, Encuestador **Ent, Encuestador **Sal, Participaciones **iniPar){
	int idEnc=0, existe=0, res=0, encuestadorId=0, idPart=0, idRes=0;
	char fecha[20];
	Encuesta *p=NULL, *tp2=NULL;
	
	printf("Ingrese la fecha de hoy (YYYYMMDD): ");
	gets(fecha);
	
	encuestadorId = selecEncuestadores(&(*Ent), &(*Sal));
	
	idPart = obtenerUltimoId(*iniPar);
	
	listarEncActivas(&(*tope));
	controlID(&idEnc);
	
	while(vaciaP(*tope) != 1){
		desapilar(&p, &(*tope));
		if(p->EncuestaId == idEnc){
			printf("Encuesta: %s\n", p->Denominacion);
		}
		apilar(&p,&tp2);
	}
	while(vaciaP(tp2) != 1){
		desapilar(&p,&tp2);
		apilar(&p,&(*tope));
	}
	
	while(iniP != NULL){
		if(iniP->EncuestaId == idEnc){
			printf("\n%s\n",iniP->Pregunta);
				mostrarRespuestas(iniP->PreguntaId, iniR);
				
				do{
					existe = 0;
					res = 0;
				
					printf("\nIngrese el numero de respuesta: ");
					scanf("%d", &res);
					comprobarRes(&existe, iniP->PreguntaId, iniR, res, &idRes);
				}while(existe!=1);
	
				Participaciones *n = (Participaciones*) malloc(sizeof(Participaciones));
					if(n != NULL){
						n->IdEncuesta = idEnc;
						n->IdPregunta = iniP->PreguntaId;
						n->IdRespuesta = idRes;
						n->IdEncuestador = encuestadorId;
						n->IdEncRespondida = idPart;
						strcpy(n->FechaRealizo,fecha);
						n->sgte = NULL;
						
						*iniPar = insParticipaciones(n,*iniPar);
						
						while(vaciaP(*tope) != 1){
							desapilar(&p,&(*tope));
							if(p->EncuestaId == idEnc){
								p->Procesada = 1; //Encuesta procesada con exito
							}
							apilar(&p,&tp2);
						}
						while(vaciaP(tp2) != 1){
							desapilar(&p,&tp2);
							apilar(&p,&(*tope));
						}
					}
		}
		iniP = iniP->sgte;
	}
	
	
	
}

int obtenerUltimoId(Participaciones *rcPar){
	int ultimoId =0;
	
	while(rcPar != NULL){
		ultimoId = rcPar->IdEncRespondida;
		rcPar = rcPar->sgte;
	}
	
	ultimoId = ultimoId + 1;
	
	return ultimoId;
}

void mostrarRespuestas(int idPreg, Respuesta *rcRes){
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
		*idResp = rcRes->RespuestaId;
	}
	
	aux = rcRes->sgte;
	while(aux != rcRes){
		if((idPreg == aux->PreguntaId) && (res == aux->RespuestaNro)){
			*ex = 1;
			*idResp = aux->RespuestaId;
		}
		aux = aux->sgte;
	}	
}

Participaciones* insParticipaciones(Participaciones *nd, Participaciones *r){
	if(r != NULL){
		r->sgte = insParticipaciones(nd,r->sgte);
	}else{
		r = nd;
	}
	
	return(r);
}

void listarTodasPart(Participaciones *rcPar){
	while(rcPar != NULL){
		printf("\nidEncuesta: %d\n", rcPar->IdEncuesta);
		printf("idPregunta: %d\n", rcPar->IdPregunta);
		printf("idRespuesta: %d\n", rcPar->IdRespuesta);
		printf("fecha: %s\n", rcPar->FechaRealizo);
		printf("id encuestador: %d\n", rcPar->IdEncuestador);
		printf("id enc respondida: %d\n", rcPar->IdEncRespondida);
		
		rcPar = rcPar->sgte;
	}
}





