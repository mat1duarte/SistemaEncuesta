#include <stdio.h>
#include <string.h>
#include <stdlib.h>




listarEncuestadores(Encuestador **e, Encuestador **s){
	Encuestador *n=NULL, *e2=NULL, *s2=NULL;
	int idEncuestador=0, existe=0;
	
	while(vaciaC(*s) !=1){
		desencolar(&n, &(*e), &(*s));
		printf("ID: %d %s\n", n->EncuestadorId,n->Nombre);
		encolar(&n, &e2, &s2);
	}
	
	while(vaciaC(*s) != 1){
		desencolar(&n, &e2, &s2);
		encolar(&n, &(*e), &(*s));
	}
	
	do{
		printf("Ingrese id del encuestador a elegir: ");
		scanf("%d", &idEncuestador);
		
		existe = verificar(idEncuestador, &(*e), &(*s));
		
		if(existe=0){
			printf("Este encuestador no existe\n");
		}
	}while(existe !=1);
	
	
	
		
	
}

int verificar(int idE, Encuestador **en, Encuestador **sa){
	Encuestador *nd=NULL, *en2=NULL, *sa2=NULL;
	int ex=0;
	
	while(vaciaC(*s) != 1){
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
