#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Mostrarparticipaciones(Encuesta **tope, Pregunta *iniP, Respuesta *iniR, Encuestador **Ent, Encuestador **Sal, Participaciones *R);
int verificar (Encuesta **tope, int ID);
void recorrerIRD (Participaciones *r, int idEncuRes, int iden, Pregunta *INIP, Respuesta *INIR, Encuestador **ENT, Encuestador **SAL, Encuesta **tope);
void mostrarPregunta(Pregunta *inip, int idpregunta);
void mostrarRespuesta (Respuesta *inir, int idpregunta, int idrespuesta);
void mostrarEncuestador(Encuestador **ent, Encuestador **sal, int idencuestador);
void mostrarDenominacion (Encuesta **Tope, int idencuesta);

void Mostrarparticipaciones(Encuesta **tope, Pregunta *iniP, Respuesta *iniR, Encuestador **Ent, Encuestador **Sal, Participaciones *R){
	int IDenc=0, correcto=0, idencResp=0;
	
	ListarEncuestaPro(&(*tope));
	
	do{
    	printf ("\nIngrese ID encuesta: ");
    	scanf("%d",&IDenc);
	    correcto = verificar (&(*tope), IDenc);
	    if (correcto == 0) {
			printf("\n Error, encuesta no disponible");
		}
	} while (correcto != 1);
	
	recorrerIRD (R, idencResp, IDenc, iniP, iniR, &(*Ent), &(*Sal), &(*tope));
}

int verificar (Encuesta **tope, int ID){

    int corr = 0;
    Encuesta *aux=NULL, *TP2=NULL;

    while (vaciaP(*tope)!=1){
        desapilar (&aux, &(*tope));
        if (aux->EncuestaId == ID) {
            corr = 1;
    	}
        apilar (&aux, &TP2);
	}
    
    while (vaciaP(TP2)!=1) {
        desapilar (&aux, &TP2);
        apilar (&aux, &(*tope));
	}
    return (corr);
}

void recorrerIRD (Participaciones *r, int idEncuRes, int iden, Pregunta *INIP, Respuesta *INIR, Encuestador **ENT, Encuestador **SAL, Encuesta **tope){
	
	if (r != NULL){
		recorrerIRD (r->izq, idEncuRes, iden, INIP, INIR, &(*ENT), &(*SAL), &(*tope));
		if (iden == r->IdEncuesta){
			if (idEncuRes == r->IdEncRespondida){
				mostrarPregunta (INIP, r->IdPregunta);
				mostrarRespuesta(INIR, r->IdPregunta, r->IdRespuesta);
			}else{
				idEncuRes = idEncuRes + 1; 
				mostrarEncuestador(&(*ENT), &(*SAL), r->IdEncuestador);
				printf("\nFecha: %s", r->FechaRealizo);
				mostrarDenominacion (&(*tope), r->IdEncuesta);
				mostrarPregunta (INIP, r->IdPregunta);
				mostrarRespuesta(INIR, r->IdPregunta, r->IdRespuesta); 
			}
		}
		recorrerIRD (r->der, idEncuRes, iden, INIP, INIR, &(*ENT), &(*SAL), &(*tope));
	}
}

void mostrarPregunta(Pregunta *inip, int idpregunta){
	while (inip != NULL){
		if (inip->PreguntaId == idpregunta){
			printf("\n %s", inip->Pregunta);
		}
		inip = inip->sgte;
	}	
}

void mostrarRespuesta (Respuesta *inir, int idpregunta, int idrespuesta){
	Respuesta *aux=NULL;
	if (inir->PreguntaId == idpregunta){
		if (inir->RespuestaId == idrespuesta){
			printf("\n %s <----- Respuesta Elegida", inir->Respuesta);
		}else{
			printf("\n %d. %s", inir->RespuestaNro, inir->Respuesta);
		}	
	}
	aux = inir->sgte;
	while (aux != inir){
		if (aux->PreguntaId == idpregunta){
			if (aux->RespuestaId == idrespuesta){
				printf("\n %d. %s <----- Respuesta Elegida",aux->RespuestaNro, aux->Respuesta);
			}else{
				printf("\n %d. %s", aux->RespuestaNro, aux->Respuesta);
			}
		}
		aux = aux->sgte;
	}
}

void mostrarEncuestador(Encuestador **ent, Encuestador **sal, int idencuestador){
	Encuestador *aux=NULL, *ent1=NULL, *sal1=NULL;
	
	while (vaciaC(*sal)!=1){
		desencolar(&aux, &(*ent), &(*sal));
		if (aux->EncuestadorId == idencuestador){
			printf("\n %s", aux->Nombre);
		}
		encolar(&aux, &ent1, &sal1);
	}
	while (vaciaC(sal1)!=1){
		desencolar (&aux, &ent1, &sal1);
		encolar (&aux, &(*ent), &(*sal));
	}
}

void mostrarDenominacion (Encuesta **Tope, int idencuesta){
	Encuesta *aux=NULL, *tope2=NULL;
	
	while (vaciaP(*Tope)!=1){
		desapilar(&aux, &(*Tope));
		if (aux->EncuestaId == idencuesta){
			printf("\n %s", aux->Denominacion);
		}
		apilar(&aux, &tope2);
	}
	
	while (vaciaP(tope2)!=1){
		desapilar(&aux, &tope2);
		apilar(&aux, &(*Tope));
	}
}
	

