#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void altaPregunta(Encuesta **tope, Pregunta **ini);
void cargaPreguntas(int encId, Pregunta **inicio);
float obtenerSumaPonderacion(int idE, Pregunta *rc);
int generarIdPregunta(Pregunta *rc);
float leerPonderacionValida(float sumaPond);
Pregunta* insertarPreguntas(Pregunta *nd, Pregunta *r);

void bajaPregunta(Encuesta **tope, Pregunta **iniP, Respuesta **iniR);
int recorrerLESPreg(int idEncuesta, Pregunta ** iniPr, Respuesta **iniResp);
void borrarPregunta(int idPREG, Pregunta **iniPREG);
void buscarBorrarP(int idP, Pregunta **rc, Pregunta **ant);
int RecorrerLCRes(int idPreg, Respuesta **iniRes);
void borrarRespuesta(int idPrg, Respuesta **iniRs);
void buscarBorrarR(int idP, Respuesta **rc, Respuesta **ant, int *encontrado);


void ListarLESPreguntas(Pregunta *rc);



void altaPregunta(Encuesta **tope, Pregunta **ini){
	
	int idEnc=0;
	
	listarEncInactivos(&(*tope));
	controlID(&idEnc);
	
	cargaPreguntas(idEnc, &(*ini));
}

void cargaPreguntas(int encId, Pregunta **inicio){
	
    float sumaPonderaciones = 0.0f;
    int auxId; 
    char continuar;
    
    sumaPonderaciones = obtenerSumaPonderacion(encId, *inicio);
    if(sumaPonderaciones>=1){
    	printf("No es posible seguir agregando preguntas a la encuesta ID: %d", encId);
    	return;
	}else{
		printf("\n--- Alta de Preguntas para Encuesta ID: %d ---\n", encId);
    
    	do {
        	// Variables para nueva pregunta
        	char textoPregunta[100];
        	float pond;
        
        	auxId = generarIdPregunta(*inicio);
        	// Ingreso del texto de la pregunta
        	printf("\nPregunta ID: %d\n", auxId);
        	printf("Ingrese el texto de la pregunta: ");
        	scanf(" %[^\n]", textoPregunta);
        	fflush(stdin);
        
        	pond = leerPonderacionValida(sumaPonderaciones);  
        
        	// Verificar que no exceda el límite al sumar
        	while (sumaPonderaciones + pond > 1.0f) {
            	printf("Error: La suma total no puede exceder 1.0 (actual: %.2f)\n", sumaPonderaciones);
            	pond = leerPonderacionValida(sumaPonderaciones);
        	}
        
	        // Actualizar suma
	        sumaPonderaciones += pond;
	        printf("Ponderacion aceptada. Suma total: %.2f\n", sumaPonderaciones);
	        
	        // Insercion en LES Preguntas
	        Pregunta *n = (Pregunta*) malloc(sizeof(Pregunta));
	        if(n!=NULL){
	        	n->PreguntaId = auxId;
				n->EncuestaId =  encId;
				n->Ponderacion = pond;
				n->Activa = 0;
				strcpy(n->Pregunta,textoPregunta);
				n->sgte = NULL;
				(*inicio) = insertarPreguntas(n, *inicio);
			}else{
				printf("No hay espacio en memoria\n");
				return;
			}
	        
	        // Preguntar si desea continuar (si no ha llegado a 1.0)
	        if (sumaPonderaciones < 1.0f) {
    			do {
        			printf("\n¿Desea agregar otra pregunta? (s/n): ");
        			scanf(" %c", &continuar);
        			while (getchar() != '\n'); // Limpiar el búfer
        			continuar = tolower(continuar);
   				} while (continuar != 's' && continuar != 'n');
			} else {
    			printf("\n¡Suma de ponderaciones completada (1.0)!\n");
    			continuar = 'n';
			}
	        
	    } while ((continuar == 's' || continuar == 'S') && sumaPonderaciones < 1.0f);
	    
	    // Validación final
	    if (sumaPonderaciones != 1.0f) {
	        printf("\n¡Atencion! Suma total de ponderaciones: %.2f (debe ser exactamente 1.0)\n", sumaPonderaciones);
	    } else {
	        printf("\n¡Preguntas registradas correctamente!\n");
	    }
	}
    
}

float obtenerSumaPonderacion(int idE, Pregunta *rc){
	float aux=0.0f;
	
	while(rc!=NULL){
		if(rc->EncuestaId == idE ){
			aux = aux + rc->Ponderacion;
		}
		rc = rc->sgte;
	}
	return aux;
}

int generarIdPregunta(Pregunta *rc){
    int maxId = 0;
    
    // Buscar el ID más alto en la lista existente
    while (rc != NULL) {
        maxId = rc->PreguntaId;
        rc = rc->sgte;
    }
    return maxId + 1; // Devolver el siguiente ID disponible
}

float leerPonderacionValida(float sumaPond){
    char buffer[100];
    float ponderacion = 0.0f;
    int intentos = 0;
    const int max_intentos = 10;

    do {
    	printf("Ingrese la ponderacion (suma actual: %.2f) (0.0 a 1.0, usar punto decimal): ", sumaPond);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error de lectura. Intente nuevamente.\n");
            continue;
        }

        // Verificar si se ingresó una coma
        char *comma_pos = strchr(buffer, ',');
        if (comma_pos != NULL) {
            printf("Error: Use punto decimal (.) en lugar de coma (,).\n");
            intentos++;
            continue;
        }

        // Verificar caracteres no válidos
        int caracteres_validos = 1;
        for (int i = 0; buffer[i] != '\0' && buffer[i] != '\n'; i++) {
        	if (!isdigit(buffer[i]) && buffer[i] != '.' && buffer[i] != '-') {
                caracteres_validos = 0;
                break;
            }
        }

        if (!caracteres_validos) {
            printf("Error: Solo se permiten numeros y punto decimal.\n");
            intentos++;
            continue;
        }

        // Convertir a float
        char *endptr;
        ponderacion = strtof(buffer, &endptr);

        // Verificar conversión exitosa
        if (endptr == buffer) {
            printf("Error: No se ingreso un numero valido.\n");
            intentos++;
            continue;
        }

        // Validar rango
        if (ponderacion <= 0.0f || ponderacion > 1.0f) {
            printf("Error: La ponderacion debe estar entre 0.0 y 1.0\n");
            intentos++;
            continue;
        }
        
        break; // Salir si todo está correcto

    } while (intentos < max_intentos);

    if (intentos >= max_intentos) {
        printf("Demasiados intentos fallidos. Usando valor por defecto 0.0\n");
        return 0.0f;
    }

    return ponderacion;
}

Pregunta* insertarPreguntas(Pregunta *nd, Pregunta *r){
	
	if(r!=NULL){
		r->sgte = insertarPreguntas(nd, r->sgte);
	}else{
		r = nd;
	}
	return r;
}

void bajaPregunta(Encuesta **tope, Pregunta **iniP, Respuesta **iniR){
	int idEnc, fin=0;
	Pregunta *rcPr = NULL;
	
	listarEncNoProc(&(*tope));
  	controlID(&idEnc);
  	
  		do{
  			fin = recorrerLESPreg(idEnc, &(*iniP), &(*iniR));
		  }while(fin != 1);
}

int recorrerLESPreg(int idEncuesta, Pregunta ** iniPr, Respuesta **iniResp){
	Pregunta *rcPr=NULL;
	int finalizar=0;
	
	rcPr = *iniPr;
	while(rcPr != NULL){
		if(rcPr->EncuestaId == idEncuesta){		
			do{
				finalizar = RecorrerLCRes(rcPr->PreguntaId, &(*iniResp));
			}while(finalizar != 1);
					 
				borrarPregunta(rcPr->PreguntaId, &(*iniPr));
				return 0;
		}
	rcPr = rcPr->sgte;
	}
	
	return 1;
}

void borrarPregunta(int idPREG, Pregunta **iniPREG){
	Pregunta *ant=NULL, *bor=NULL;
	
	bor = *iniPREG;
	buscarBorrarP(idPREG, &bor, &ant);
	if(bor != NULL){
		if(ant == NULL){
			*iniPREG = (*iniPREG)->sgte;
		}else{
			ant->sgte = bor->sgte;
		}
		bor->sgte = NULL;
		free(bor);
	}else{
		printf("EL idPregunta %d (nodo) no esta en la lista\n", idPREG);
	}
}

void buscarBorrarP(int idP, Pregunta **rc, Pregunta **ant){
	int encontrado;
	
	*ant = NULL;
	encontrado = 0;
	
	while((*rc != NULL) && (!encontrado)){
		if((*rc)->PreguntaId == idP){
			encontrado = 1;
		}else{
			*ant = *rc;
			*rc = (*rc)->sgte;
		}
	}
}

int RecorrerLCRes(int idPreg, Respuesta **iniRes){
	if(*iniRes == NULL) return 1;
	
	Respuesta *aux=NULL, *rcRes=NULL;
	
		aux = *iniRes;
		rcRes = *iniRes;
	
		if(rcRes->PreguntaId == idPreg){
			borrarRespuesta(idPreg, &(*iniRes));
			return 0;
		}
		aux = rcRes->sgte;
		while(aux != rcRes){
			if(aux->PreguntaId == idPreg){
				borrarRespuesta(idPreg, &(*iniRes));
				return 0;
			}
			aux = aux->sgte;
		}
		
		return 1;
}

void borrarRespuesta(int idPrg, Respuesta **iniRs){
	if(*iniRs == NULL) return; 
	
	Respuesta *ant=NULL, *bor=NULL;
	int aBorrar;
	
	if(*iniRs != NULL){
		bor = *iniRs;
		aBorrar = 0;
		buscarBorrarR(idPrg, &bor, &ant, &aBorrar);
		if((bor == *iniRs) && ((*iniRs)->PreguntaId == idPrg)){
			*iniRs = (*iniRs)->sgte;
			aBorrar = 1;
		}
		if(aBorrar == 1){
			ant->sgte = bor->sgte;
			bor->sgte = NULL;
			
			free(bor);
		}else{
			printf("EL idPregunta: %d (nodo )no esta en la lista\n", idPrg);
		}
	}
}

void buscarBorrarR(int idP, Respuesta **rc, Respuesta **ant, int *encontrado){
	Respuesta *aux=NULL;
	
	*ant = *rc;
	aux = *rc;
	*rc = (*rc)->sgte;
	while((*rc != aux) && (!(*encontrado))){
		if((*rc)->PreguntaId == idP){
			*encontrado = 1;
		}else{
			*ant = *rc;
			*rc = (*rc)->sgte;
		}
	} 
}

void ListarLESPreguntas(Pregunta *rc){
	
	if(rc==NULL){
		printf("No hay preguntas existentes\n");
	}else{
		while(rc!=NULL){
			printf("EncuestaId: %d \n",rc->EncuestaId);
			printf("PreguntaId: %d \n",rc->PreguntaId);
			printf("Pregunta: %s \n",rc->Pregunta);
			printf("Ponderacion: %f \n", rc->Ponderacion);
			rc=rc->sgte;
		}
	}	
}





