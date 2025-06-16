#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void altaRespuesta(Encuesta **tope, Pregunta *iniP, Respuesta **iniR);
void CargaRespuestas(int idEnc, Pregunta *rc, Respuesta **iniR);
void incrementoResid(int *respid, Respuesta *iniR);
Respuesta* borrarlista(Respuesta *iniTmp);
void ActualizarIdRes(Respuesta *rcTmp, Respuesta *iniRes);
Respuesta* cargarListaTemp(Respuesta *rcTemp, Respuesta **iniRS);
void insertarLRes(Respuesta **nodotemp, Respuesta **iniR);
Respuesta* buscarant(int Respid, Respuesta *rc);

void bajaRespuestas(Encuesta **tope, Pregunta *iniP, Respuesta **iniR);
void listarPreguntas(Pregunta *ini, int idencu);
void ListarLcRespuestas(Respuesta *);
void VerificarEncuesta(int idEcta, Encuesta **tpEnc, Pregunta *rcP, Respuesta *rcRsp);
void evaluarBanderas(int *cont, int bandP, int bandR);
void ActivarEncuesta(Encuesta **tpE, int idEncuesta);

void bajaRespuestas(Encuesta **tope, Pregunta *iniP, Respuesta **iniR){ 
  int idEnc=0,idpreg = 0, finalizar=0;
  
  listarEncNoProc(&(*tope));
  controlID(&idEnc);
  
  listarPreguntas(iniP,idEnc);
  printf("\nIngrese el ID de la pregunta cuyas respuestas desea eliminar: ");
  controlID(&idpreg);
  
  do{
		finalizar = RecorrerLCRes(idpreg, &(*iniR));
	}while(finalizar != 1);
}

void listarPreguntas(Pregunta *ini, int idencu){
	
	while(ini != NULL){
		if(ini->EncuestaId == idencu){
			printf("\nID: %d\n%s",ini->PreguntaId,ini->Pregunta);
		}
		ini = ini->sgte;
	}
}

void altaRespuesta(Encuesta **tope, Pregunta *iniP, Respuesta **iniR){ 
  int idEnc=0;
  
  listarEncInactivos(&(*tope));
  controlID(&idEnc);
  
  CargaRespuestas(idEnc,iniP,&(*iniR));
  VerificarEncuesta(idEnc, &(*tope), iniP, *iniR);
}

void CargaRespuestas(int idEnc, Pregunta *rc, Respuesta **iniR){ 
	Pregunta *aux=NULL;
	Respuesta *nodotemp=NULL, *initemp=NULL;
	int controlistadopreg=0, idpregunta,cantidadres=0, confirmado=0,respuestabucle=0, respnro=0,pond=0;
	char respuesta[50];
	
	aux=rc;
  
	while(rc!=NULL) { 
    	if (rc->EncuestaId==idEnc) { 
			controlistadopreg=1;
			rc=NULL;
		}else{
			rc=(rc)->sgte;
		} 
  	}
  
	if (controlistadopreg==0) { 
		printf("\n----- La encuesta aun no tiene preguntas -----\n");
	}else{ 
		rc=aux;
		aux=NULL;
	 
	 	printf("\n----- Listado de preguntas encuesta id --> %d",rc->EncuestaId);
		while(rc!=NULL) {
			confirmado=0;
			respnro=0;
	  
			if (rc->EncuestaId==idEnc) { 
				printf("\n%s\n",rc->Pregunta);
				pond=0;
				while (confirmado!=1) {
					strcpy(respuesta,"1");  
					while(strcmp(respuesta,"0")!=0) { 
						nodotemp=(Respuesta*) malloc(sizeof(Respuesta));
						if (nodotemp!=NULL) { 
							nodotemp->Activa=0;
				            nodotemp->PreguntaId=rc->PreguntaId;  
				            nodotemp->RespuestaId = 0;
				            respnro++;
				            nodotemp->RespuestaNro=respnro; 
               
				            if (respnro<=2) { 
				              printf("\nIngrese la respuesta nro %d: ",nodotemp->RespuestaNro);
				              gets(respuesta);
							  fflush(stdin);
							} 
						    if (respnro>2) { 
							   printf("\nIngrese la respuesta nro %d o 0 para finalizar: ",nodotemp->RespuestaNro);
							   gets(respuesta);
							   fflush(stdin);
							}
							strcpy(nodotemp->Respuesta,respuesta); 
			
							if(strcmp(respuesta,"0")!=0){
								printf("\nCargue ponderacion: ");
								scanf("%f",&nodotemp->Ponderacion);
								fflush(stdin);
								
								if (nodotemp->Ponderacion==1) { 
									pond=1;
								}
								
					            nodotemp->sgte=NULL;
								insertarLRes(&nodotemp,&initemp);
								
							}else{
								free(nodotemp);
							}
			
						}else{ 
							printf("\n----No hay espacio en memoria----\n");
						}
					}
					if (pond==1) {
						ActualizarIdRes(initemp, *iniR); 
						initemp = cargarListaTemp(initemp, &(*iniR)); //Funcion para cargar nodos de lista temporal a lista circular de Repuestas	
						confirmado=1;
						pond=0;
					}else{ 
						printf("\nal menos una ponderacion debe ser uno, cargue nuevamente\n");
						initemp=borrarlista(initemp); //liberar lista temporal 
					
						respnro=0;
					} 
				}		  
	  		} 
			rc=rc->sgte;
		}
	}
}

void incrementoResid(int *resId, Respuesta *iniR){
	Respuesta *aux=NULL;
  
	if(iniR==NULL){
		*resId = 0;
 	}else{
  		*resId = iniR->RespuestaId;
	  	aux = iniR->sgte;
	  	while(aux!=iniR) { 
	  		*resId = aux->RespuestaId;
			aux = aux->sgte;
		}
  	}
	(*resId)++;
}

Respuesta* borrarlista(Respuesta *iniTmp){
    if (iniTmp == NULL) {
        printf("No hay nodos en la lista circular\n");
        return NULL;
    }

    Respuesta *aux = iniTmp->sgte;
    Respuesta *sig;

    while (aux != iniTmp) {
        sig = aux->sgte;
        free(aux);
        aux = sig;
    }

    free(iniTmp); // finalmente se libera el nodo inicial
    return NULL;
}


void ActualizarIdRes(Respuesta *rcTmp, Respuesta *iniRes){
	int idx;
	Respuesta *aux=NULL;
	
	incrementoResid(&idx, iniRes);
	rcTmp->RespuestaId = idx;
	aux = rcTmp->sgte;
	while(aux != rcTmp){
		idx++;
		aux->RespuestaId = idx;
		aux = aux->sgte;
	}
}

Respuesta* cargarListaTemp(Respuesta *rcTemp, Respuesta **iniRS){
	Respuesta *aux=NULL, *bor=NULL, *ant=NULL;
	
	if(rcTemp!=NULL){
		ant = rcTemp;
		rcTemp = rcTemp->sgte;
		while(rcTemp != ant){
			aux = rcTemp;
			rcTemp = rcTemp->sgte;
			ant->sgte = rcTemp;
			aux->sgte = NULL;
			insertarLRes(&aux, &(*iniRS));
		}
		if(rcTemp == ant){
			rcTemp->sgte = NULL;
			insertarLRes(&rcTemp, &(*iniRS));
		}
	}else{
		printf("No hay nodos en la lista temporal\n");
	}
	
	return (NULL);
}
 
void insertarLRes(Respuesta **nodotemp, Respuesta **iniR){
	Respuesta *ant;
	
  	if (*iniR==NULL) { 
    	*iniR = *nodotemp;
    	(*iniR)->sgte= *nodotemp;
  	}else{ 
    	ant = buscarant((*nodotemp)->RespuestaId, *iniR);
     	(*nodotemp)->sgte = ant->sgte;
     	ant->sgte = *nodotemp;
      	if ((*nodotemp)->RespuestaId < (*iniR)->RespuestaId) { 
        	*iniR= *nodotemp;
	  	}
   	}
  	*nodotemp=NULL;
} 

Respuesta* buscarant(int Respid, Respuesta *rc){
	Respuesta *ant=NULL, *aux=NULL;
  
 	ant=rc;
  	aux=rc->sgte;
  
  	if (rc->RespuestaId>Respid) { 
    	while(aux!=rc) { 
      		ant=aux;
      		aux=aux->sgte;
		}
  	}else{ 
    	while(aux!=rc) { 
    		if (aux->RespuestaId>Respid) {
       	 		aux=rc;
	   		}else{ 
	      		ant=aux;
	      		aux=aux->sgte;
			}
	 	}
   	}
   	return(ant);
}

void ListarLcRespuestas(Respuesta *LCR){
	Respuesta *aux=NULL;
	if(LCR!= NULL){
		printf("\nPreguntaId: %d\n", LCR->PreguntaId);
		printf("RespuestaId: %d\n",LCR->RespuestaId);
		printf("Nro Respuesta: %d\n", LCR->RespuestaNro);
		printf("Respuesta: %s\n", LCR->Respuesta);
		printf("Ponderacion: %.2f\n",LCR->Ponderacion);
		printf("Activa: %d\n", LCR->Activa);
		
		aux = LCR->sgte;
		while(aux != LCR){
			printf("\nPreguntaId: %d\n", aux->PreguntaId);
			printf("RespuestaId: %d\n",aux->RespuestaId);
			printf("Nro Respuesta: %d\n", aux->RespuestaNro);
			printf("Respuesta: %s\n", aux->Respuesta);
			printf("Ponderacion: %.2f\n",aux->Ponderacion);
			printf("Activa: %d\n", aux->Activa);
			aux = aux->sgte;
		}
	}else{
		printf("No hay respuestas existentes\n");
	}
	
}

void VerificarEncuesta(int idEcta, Encuesta **tpEnc, Pregunta *rcP, Respuesta *rcRsp){
	int contPreg=0, bandPreg=0, bandRes=0, contFinal=0;
	Pregunta *auxPreg=NULL;
	Respuesta *auxRes=NULL, *aux=NULL;
	
	auxPreg = rcP;
	while(rcP != NULL){
		if(rcP->EncuestaId == idEcta){
			contPreg= contPreg +1;
		}
		rcP = rcP->sgte;
	}
	
	rcP = auxPreg;
	auxPreg = NULL;
	
	while(rcP != NULL){
		bandPreg=0;
		bandRes=0;
		
		if(rcP->EncuestaId == idEcta){
			bandPreg=1;
			
			auxRes = rcRsp;
			if(rcRsp->PreguntaId == rcP->PreguntaId){
				bandRes = 1;
			}
			aux = rcRsp->sgte;
			while(aux != rcRsp){
				if(aux->PreguntaId == rcP->PreguntaId){
					bandRes = 1;
				}
				aux = aux->sgte;
			}
			
			evaluarBanderas(&contFinal,bandPreg,bandRes);
			
			rcRsp = auxRes;
			auxRes = NULL;	
		}
		rcP = rcP->sgte;
	}
	
	if(contPreg == contFinal){
		//ActivarEncuesta
		ActivarEncuesta(&(*tpEnc), idEcta);
	}else{
		printf("La encuesta está incompleta\n");
	}
}

void evaluarBanderas(int *cont, int bandP, int bandR){
	
	if(bandP==1 && bandR==1){
		*cont = *cont + 1;
	}
}

void ActivarEncuesta(Encuesta **tpE, int idEncuesta){
	Encuesta *p=NULL, *tp2=NULL;
	
	while(vaciaP(*tpE) != 1){
		desapilar(&p,&(*tpE));
		if(p->EncuestaId == idEncuesta){
			p->Activa = 1; //Activa Encuesta dada de alta
		}
		apilar(&p,&tp2);	
	}
	
	while(vaciaP(tp2) != 1){
		desapilar(&p,&tp2);
		apilar(&p,&(*tpE));
	}
}
