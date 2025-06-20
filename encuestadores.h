#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void altaEncuestador(Encuestador **e, Encuestador **s);
int selecEncuestadores(Encuestador **e, Encuestador **s);
int verificar(int idE, Encuestador **en, Encuestador **sa);
int vaciaC(Encuestador *s);
void desencolar(Encuestador **ds, Encuestador **e, Encuestador **s);
void encolar(Encuestador **nv, Encuestador **e, Encuestador **s);
void listarEncuestadores(Encuestador **e, Encuestador **s);


void altaEncuestador(Encuestador **e, Encuestador **s){
	Encuestador *n=NULL, *e2=NULL, *s2=NULL, *nd=NULL;
	int ultId=0;
	char nombre[30];
	
	while(vaciaC(*s) != 1){
		desencolar(&n, &(*e), &(*s));
		ultId = n->EncuestadorId;
		encolar(&n,&e2,&s2);
	}
	
	ultId = ultId + 1;
	
	while(vaciaC(s2) != 1){
		desencolar(&n, &e2, &s2);
		encolar(&n, &(*e), &(*s));
	}
	
	printf("Ingrese el nombre del encuestador: ");
	gets(nombre);
	
	nd = (Encuestador*) malloc(sizeof(Encuestador));
	if(nd != NULL){
		nd->EncuestadorId = ultId;
		strcpy(nd->Nombre, nombre);
		nd->sgte = NULL;
		
		encolar(&nd, &(*e), &(*s));
	}else{
		printf("No hay espacio en memoria\n");
	}
	
	
}

int selecEncuestadores(Encuestador **e, Encuestador **s){
	Encuestador *n=NULL, *e2=NULL, *s2=NULL;
	int idEncuestador=0, existe=0;
	
	while(vaciaC(*s) !=1){
		desencolar(&n, &(*e), &(*s));
		printf("ID: %d %s\n", n->EncuestadorId,n->Nombre);
		encolar(&n, &e2, &s2);
	}
	
	while(vaciaC(s2) != 1){
		desencolar(&n, &e2, &s2);
		encolar(&n, &(*e), &(*s));
	}
	
	do{
		printf("Ingrese id del encuestador a elegir: ");
		fflush(stdin);
		scanf("%d", &idEncuestador);
		fflush(stdin);
		
		existe = verificar(idEncuestador, &(*e), &(*s));
		
		if(existe==0){
			printf("Este encuestador no existe, intente nuevamente\n");
		}
	}while(existe !=1);
	
	return idEncuestador;
			
}

int verificar(int idE, Encuestador **en, Encuestador **sa){
	Encuestador *nd=NULL, *en2=NULL, *sa2=NULL;
	int ex=0;
	
	while(vaciaC(*sa) != 1){
		desencolar(&nd, &(*en), &(*sa));
		if(nd->EncuestadorId == idE){
			ex = 1;
		}
		encolar(&nd, &en2, &sa2);
	}
	
	while(vaciaC(sa2) != 1){
		desencolar(&nd, &en2, &sa2);
		encolar(&nd, &(*en), &(*sa));
	}
	
	return ex;
}

int vaciaC(Encuestador *s){
	int vac;
	if(s == NULL){
		vac=1;
	}else{
		vac=0;
	}
	
	return vac;
}

void desencolar(Encuestador **ds, Encuestador **e, Encuestador **s){
	(*ds) = (*s);
	(*s) = (*s)->sgte;
	(*ds)->sgte = NULL;
	if(*s == NULL){
		(*e) = NULL;
	}
}

void encolar(Encuestador **nv, Encuestador **e, Encuestador **s){
	if(*e == NULL){
		(*s) = (*nv);
	}else{
		(*e)->sgte = (*nv);
	}
	(*e) = (*nv);
	(*nv) = NULL;
}

void listarEncuestadores(Encuestador **e, Encuestador **s){
	Encuestador *n = NULL, *e2=NULL, *s2=NULL;
	
	while(vaciaC(*s) != 1){
		desencolar(&n, &(*e),&(*s));
		printf("ID: %d\n", n->EncuestadorId);
		printf("nombre: %s\n", n->Nombre);
		encolar(&n,&e2,&s2);
	}
	
	while(vaciaC(s2) != 1){
		desencolar(&n,&e2,&s2);
		encolar(&n,&(*e),&(*s));
	}
}
