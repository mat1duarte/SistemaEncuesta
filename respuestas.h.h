#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void altaRespuesta(Encuesta **tope, Pregunta *iniP, Respuesta **iniR);
void listarEncInactivos(Encuesta **tope);
void listarPreguntasEnc(int idEnc, Pregunta *rc, Respuesta **iniR);
void incrementoRespnro(int *respid, Respuesta *iniR);
Respuesta borrarlista(Respuesta *initemp);
void insertarLRes(Respuesta **nodotemp, Respuesta **iniR);

void altaRespuesta(Encuesta **tope, Pregunta *iniP, Respuesta **iniR) { 
  int idEnc=0;
  listarEncInactivos(&(*tope));
  controlID(&idEnc);
  
  listarPreguntasEnc(idEnc,iniP,&(*iniR));
}

void listarPreguntasEnc(int idEnc, Pregunta *rc, Respuesta **iniR) { 
  Pregunta *aux;
  Respuesta *nodotemp=NULL, *initemp=NULL;
  aux=rc;
  int controlistadopreg=0, idpregunta,cantidadres=0, confirmado=0,respuestabucle=0, respid=0, respnro=0,pond=0;
  char respuesta[100];
  while(rc!=NULL) { 
    if (rc->EncuestaId==idEnc) { 
      controlistadopreg=1;
      rc=NULL;
	} 
    rc=(rc)->sgte;
  }
  
  if (controlistadopreg==0) { 
    printf("\n----- La encuesta aun no tiene preguntas -----\n");
  }
   else { 
     rc=aux;
	 aux=NULL;
     printf("\n----- Listado de preguntas encuesta id --> %d",rc->EncuestaId);
     while(rc!=NULL) {
	 confirmado=0; 
      if (rc->EncuestaId==idEnc) { 
          printf("\n%s\n",rc->Pregunta);
        while (confirmado!=1) {
		strcpy(respuesta,"1");  
          while(strcmp(respuesta,"0")!=0) { 
          nodotemp=(struct Respuesta*) malloc(sizeof(struct Respuesta));
          if (nodotemp!=NULL) { 
            printf("\nCargue la respuesta: \n");
            nodotemp->Activa=0;
            nodotemp->PreguntaId=rc->PreguntaId;
            incrementoResid(&respid,*iniR);
            nodotemp->RespuestaId=respid;
            nodotemp->RespuestaNro=respnro++;
            
            
            
            if (respnro<=2) { 
              printf("\nIngrese la respuesta nro %d: ",nodotemp->RespuestaNro);
              gets(respuesta);
			} 
		    if (respnro>2) { 
			   printf("\nIngrese la respuesta nro %d o 0 para finalizar: ",nodotemp->RespuestaNro);
			   gets(respuesta);
			}
			strcpy(nodotemp->Respuesta,respuesta); 
			printf("\nCargue ponderacion: ");
			scanf("%f",&nodotemp->Ponderacion);
			if (nodotemp->Ponderacion==1) { 
			  pond=1;
			}
			
            nodotemp->sgte=NULL;
            if (strcmp(respuesta,"0")!=0) {
              insertar(&nodotemp,&initemp);
			}
			 else { 
			   free(nodotemp);
			 }
            
		  }
		   else { 
		     printf("\n----No hay espacio en memoria----\n");
		   }
		}
		if (pond==1) { 
		  //carga lista circular
		  insertarLRes(&nodotemp,&(*iniR));
		  confirmado=1;
		} 
		 else { 
		   printf("\nal menos una ponderacion debe ser uno, cargue nuevamente\n")
		   initemp=borrarlista(initemp)//liberar lista temporal  
		 } 
	  }  
         
	  } 
      rc=rc->sgte;
    }
     
     
     
    
   }
}

void incrementoResid(int *respid, Respuesta *iniR) { 
  Respuesta *aux;
  
  respid=iniR->RespuestaId
  aux=iniR->sgte;
  while(aux!=iniR) { 
    respid=aux->RespuestaId;
    aux=(aux)->sgte;
  }
   respid++;
}

Respuesta borrarlista(Respuesta *initemp) { 
  Respuesta *aux=NULL;
  
  while(initemp!=NULL) { 
    aux=initemp;
    initemp=initemp->sgte;
    aux->sgte=NULL;
    free(aux);
  }
  
  return(NULL);
} 
 
void insertarLRes(Respuesta **nodotemp, Respuesta **iniR){ // completar insertar y buscar borrar
  Respuesta *ant;
  
  if (iniR!=NULL) { 
    iniR=nodotemp;
    iniR->sgte=nodotemp;
  } 
   else { 
     ant=buscarant(nodotemp->RespuestaId,iniR);
     nodotemp->
   }
  
}
