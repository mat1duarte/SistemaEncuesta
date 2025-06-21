#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void cargarParticipaciones(Encuesta **tope, Pregunta *iniP, Respuesta *iniR, Encuestador **Ent, Encuestador **Sal, Participaciones **RPar);
int obtenerUltimoId(Participaciones *rcPar);
void mostrarRespuestas(int idPreg, Respuesta *rcRes);
void comprobarRes(int *ex, int idPreg, Respuesta *rcRes, int res, int *idResp);
Participaciones* insParticipaciones(Participaciones *nd, Participaciones *r);
void listarTodasPart(Participaciones *rcPar);
void recorrerIRD (Participaciones *r, int *id);
void listarArbol(Participaciones *r);


void cargarParticipaciones(Encuesta **tope, Pregunta *iniP, Respuesta *iniR, Encuestador **Ent, Encuestador **Sal, Participaciones **RPar){
	int idEnc=0, existe=0, res=0, encuestadorId=0, idPart=0, idRes=0;
	char fecha[20];
	Encuesta *p=NULL, *tp2=NULL;
	
	printf("Ingrese la fecha de hoy (YYYYMMDD): ");
	gets(fecha);
	
	encuestadorId = selecEncuestadores(&(*Ent), &(*Sal));
	
	idPart = obtenerUltimoId(*RPar);
	
	idPart = idPart + 1;
	
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
						n->izq = NULL;
						n->der = NULL;
						
						*RPar = insParticipaciones(n,*RPar);
						
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


int obtenerUltimoId(Participaciones *rPar){
	int ultimoId =0;
	
	if (rPar != NULL){
		recorrerIRD (rPar, &ultimoId);
	}
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

	if (r != NULL){
		if (r->IdEncuesta == nd->IdEncuesta){
			if (r->IdEncRespondida == nd->IdEncRespondida){
				if (nd->IdPregunta < r->IdPregunta){
					r->izq = insParticipaciones (nd, r->izq);	
				}else{
					r->der = insParticipaciones (nd, r->der);
				}
			}else{
				if (nd->IdEncRespondida < r->IdEncRespondida){
					r->izq = insParticipaciones (nd, r->izq); 
				}else{
					r->der = insParticipaciones (nd, r->der);
				}
			}
		}else{
			if (nd->IdEncuesta < r->IdEncuesta){
					r->izq = insParticipaciones (nd, r->izq); 
			}else{
					r->der = insParticipaciones (nd, r->der);
			}
		}		
	}else{
		r = nd;
	}
	return (r);
}

void recorrerIRD (Participaciones *r, int *id){
	
	if (r!= NULL){
		recorrerIRD (r->izq, &(*id));
		*id = r->IdEncRespondida;
		recorrerIRD (r->der, &(*id));
	}
}

// PARA LISTAR LAS PARTICIPACIONES DEL ARBOL
void listarArbol(Participaciones *r){
	if(r != NULL){
		listarArbol(r->izq);
		printf("\nidencuesta: %d\n",r->IdEncuesta);
		printf("idpregunta: %d\n",r->IdPregunta);
		printf("idrespuesta: %d\n",r->IdRespuesta);
		printf("idencuestador: %d\n",r->IdEncuestador);
		printf("idencuestarespondida. %d\n", r->IdEncRespondida);
		printf("fecha: %s\n",r->FechaRealizo);
		listarArbol(r->der);
	}
}





