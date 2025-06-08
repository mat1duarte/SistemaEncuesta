#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARCHIVO_CSV "encuestas.csv"
#define ARCHIVO_TEMP "temp.csv"

int generarNuevoId();
void apilar(Encuesta **nv, Encuesta **tope);
void desapilar(Encuesta **ds, Encuesta **tope);
int vaciaP(Encuesta *top);
void altaEncuesta(Encuesta **tope);
void bajaEncuesta(Encuesta **tope, Pregunta *LPG);
void bajaPG(Pregunta *ini, int idencuesta, Respuesta *iniRes);
void BajaRes(int idpregunta, Respuesta *rcRes);
int ExisteIdEnc(int id, Encuesta **tp);

//Funciones de prueba
void listarEncuestas();
void listarPila(Encuesta **tope);
void controlID(int *ID);
void listartodapila(Encuesta **tope);
void listarEncInactivos(Encuesta **tope);


// FunciÛn para generar el prÛximo ID disponible
int generarNuevoId(){
    FILE *archivo = fopen(ARCHIVO_CSV, "r");
    if (archivo == NULL) {
        return 1; // Si el archivo no existe, empezamos con ID 1
    }
    
    int maxId = 0;
    char linea[256];
    
    // Leer el archivo lÌnea por lÌnea
    while (fgets(linea, sizeof(linea), archivo)) {
        int id;
        sscanf(linea, "%d,", &id);
        if (id > maxId) {
            maxId = id;
        }
    }
    
    fclose(archivo);
    return maxId + 1;
}

void apilar(Encuesta **nv, Encuesta **tope){
	(*nv)->sgte = (*tope);
	(*tope) = (*nv);
	(*nv) = NULL;
}

void desapilar(Encuesta **ds, Encuesta **tope){
	(*ds) = (*tope);
	(*tope) = (*tope)->sgte;
	(*ds)->sgte = NULL;
}

int vaciaP(Encuesta *top){
	int vac;
	if(top == NULL){
		vac = 1;
	}else{
		vac = 0;
	}
	return vac;
}

// FunciÛn para dar de alta una nueva encuesta
void altaEncuesta(Encuesta **tope){
    Encuesta nueva;
    FILE *archivo;
    
    // Generar ID autom·ticamente
    nueva.EncuestaId = generarNuevoId();
    
    // Inicializar valores por defecto
    nueva.Activa = 0;
    nueva.Procesada = 0;
    
    // Solicitar datos al usuario
    printf("\n--- Alta de Nueva Encuesta (ID: %d) ---\n", nueva.EncuestaId);
    
    printf("Ingrese la denominacion: ");
    scanf(" %[^\n]", nueva.Denominacion);
    
    printf("Ingrese el mes (1-12): ");
    scanf("%d", &nueva.EncuestaMes);
    
    printf("Ingrese el anio: ");
    scanf("%d", &nueva.EncuestaAnio);
    
    // Abrir archivo en modo append (aÒadir al final)
    archivo = fopen(ARCHIVO_CSV, "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", ARCHIVO_CSV);
        return;
    }
    
    Encuesta *n = (Encuesta*) malloc(sizeof(Encuesta));
    if(n!=NULL){
    	strcpy(n->Denominacion,nueva.Denominacion);
    	n->Activa = nueva.Activa;
    	n->Procesada = nueva.Procesada;
    	n->EncuestaAnio = nueva.EncuestaAnio;
    	n->EncuestaMes = nueva.EncuestaMes;
    	n->EncuestaId = nueva.EncuestaId;
    	n->sgte = NULL;
    	
    	apilar(&n,&(*tope));
	}else{
		printf("No hay espacio en memoria\n");
		return;
	}
		/*else {
			printf("\n---No hay espacio en la memoria----\n");
		}*/
    
    // Escribir los datos en formato CSV
    fprintf(archivo, "%d,%d,%d,%d,%d,%s\n",
		nueva.EncuestaId,
	    nueva.EncuestaMes,
	    nueva.EncuestaAnio,
	    nueva.Procesada,
	    nueva.Activa,
	    nueva.Denominacion);
		
	fclose(archivo);
    printf("Encuesta guardada exitosamente!\n");
}

// FunciÛn para mostrar todas las encuestas
void listarEncuestas(){
    FILE *archivo = fopen(ARCHIVO_CSV, "r");
    if (archivo == NULL) {
        printf("No hay encuestas registradas.\n");
        return;
    }
    
    printf("\n--- Listado de Encuestas ---\n");
    printf("ID | Mes | AÒo | Procesada | Activa | Denominacion\n");
    printf("--------------------------------------------------\n");
    
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
		Encuesta e;
        sscanf(linea, "%d,%d,%d,%d,%d,%[^\n]",
			&e.EncuestaId,
            &e.EncuestaMes,
            &e.EncuestaAnio,
            &e.Procesada,
            &e.Activa,
            e.Denominacion);
        
        printf("%2d | %2d | %4d | %9s | %6s | %s\n",
			e.EncuestaId,
            e.EncuestaMes,
            e.EncuestaAnio,
            e.Procesada ? "SÌ" : "No",
            e.Activa ? "SÌ" : "No",
            e.Denominacion);
    }
    fclose(archivo);
}

// FunciÛn para listar las encuestas activas desde la pila
void listarPila(Encuesta **tope){
	
	Encuesta *p=NULL, *tp2=NULL;
	
	while(vaciaP(*tope)!=1){
		desapilar(&p, &(*tope));
		apilar(&p,&tp2);
	}
	printf("\n--- Listado de Encuestas ---\n");
    printf("ID | Mes | AÒo | Procesada | Activa | Denominacion\n");
    printf("--------------------------------------------------\n");
	while(vaciaP(tp2)!=1){
		desapilar(&p,&tp2);
		if((p->Activa==1) && (p->Procesada==0)){
			printf("%2d | %2d | %4d | %9s | %6s | %s\n",
				p->EncuestaId,
				p->EncuestaMes,
				p->EncuestaAnio,
				p->Procesada ? "Si" : "No",
				p->Activa ? "Si" : "No",
				p->Denominacion);
        }
        apilar(&p,&(*tope));
	}
}

void bajaEncuesta(Encuesta **tope, Pregunta *LPG, Respuesta *LRS){
	int idBaja, encontro=0;
	Encuesta *p=NULL, *tp2=NULL;
	
    printf("\n--- Baja de Encuesta ---\n");
    listarPila(&(*tope));
	controlID(&idBaja);
	encontro = ExisteIdEnc(idBaja, &(*tope));

	if(encontro==0){
		printf("error, no existe el id ingresado\n");
	}else{
		while (!vaciaP(*tope)){
			desapilar(&p, &(*tope));
	    	if(p->EncuestaId == idBaja){
				p->Activa = 0;
	    		bajaPG(LPG, idBaja, LRS);
			}
		apilar(&p, &tp2); // guarda todo temporalmente
		}
		// restaurar la pila original
		while (!vaciaP(tp2)) {
		    desapilar(&p, &tp2);
		    apilar(&p, &(*tope));
		}
	    FILE *archivoOriginal = fopen(ARCHIVO_CSV, "r");
	    FILE *archivoTemp = fopen(ARCHIVO_TEMP, "w");
	    
	    if (archivoOriginal == NULL || archivoTemp == NULL) {
	        printf("Error al abrir archivos\n");
	        return;
	    }
	    
	    char linea[256];
	    int encontrada = 0;
	    
	    while (fgets(linea, sizeof(linea), archivoOriginal)) {
			Encuesta e;
	        sscanf(linea, "%d,%d,%d,%d,%d,%[^\n]",
				&e.EncuestaId,
	            &e.EncuestaMes,
	            &e.EncuestaAnio,
	            &e.Procesada,
	            &e.Activa,
	            e.Denominacion);
	        
	        if (e.EncuestaId == idBaja) {
	            encontrada = 1;
	            e.Activa = 0; // Marcamos como inactiva
	            printf("Encuesta ID %d marcada como inactiva\n", idBaja);
	        }
	        
	        // Escribimos en el archivo temporal (todos los registros)
	        fprintf(archivoTemp, "%d,%d,%d,%d,%d,%s\n",
				e.EncuestaId,
	            e.EncuestaMes,
	            e.EncuestaAnio,
	            e.Procesada,
	            e.Activa,
	            e.Denominacion);
	    }
	    
	    fclose(archivoOriginal);
	    fclose(archivoTemp);
	    
	    // Reemplazamos el archivo original con el temporal
	    remove(ARCHIVO_CSV);
	    rename(ARCHIVO_TEMP, ARCHIVO_CSV);
	    
	    if (!encontrada) {
	        printf("No se encontr√≥ encuesta con ID %d\n", idBaja);
	    }
	}   
}

int ExisteIdEnc(int id, Encuesta **tp){
	Encuesta *p=NULL, *tp2=NULL;
	int enc=0;
	
	while(vaciaP(*tp) != 1){
		desapilar(&p, &(*tp));
		if(p->EncuestaId == id){
			enc=1;
		}
		apilar(&p, &tp2);
	}
	while(vaciaP(tp2)!=1){
		desapilar(&p, &tp2);
		apilar(&p, &(*tp));
	}
	return enc;
}

//FunciÛn para control de ID
void controlID(int *ID){
	int resultado;
    char confirmacion;

    do {
        // Solicita ingreso del ID
        printf("Ingrese el ID: ");
        resultado = scanf("%d", &(*ID));

        // ValidaciÛn de entrada numÈrica
        while (resultado != 1) {
            while (getchar() != '\n'); // Limpia el buffer
            printf("Entrada inv·lida. Ingrese un n˙mero entero para el ID: ");
            resultado = scanf("%d", &(*ID));
        }

        // Limpia el buffer antes de leer el char
        while (getchar() != '\n');

        // ConfirmaciÛn del ID
        printf("øEst· seguro de que el ID ingresado es correcto? (s/n): ");
        scanf("%c", &confirmacion);
		
        // Limpia el buffer por si hay m·s caracteres
        while (getchar() != '\n');

    } while (confirmacion == 'n' || confirmacion == 'N');

    printf("ID confirmado: %d\n", (*ID));

    
}

// FunciÛn para listar todas las encuestas desde la pila
void listartodapila(Encuesta **tope){
	
	Encuesta *p=NULL, *tp2=NULL;
	
	while(vaciaP(*tope)!=1){
		desapilar(&p, &(*tope));
		apilar(&p,&tp2);
	}
	printf("\n--- Listado de Encuestas ---\n");
    printf("ID | Mes | AÒo | Procesada | Activa | Denominacion\n");
    printf("--------------------------------------------------\n");
	while(vaciaP(tp2)!=1){
		desapilar(&p,&tp2);
		printf("%2d | %2d | %4d | %9s | %6s | %s\n",
        	p->EncuestaId,
            p->EncuestaMes,
            p->EncuestaAnio,
            p->Procesada ? "Si" : "No",
            p->Activa ? "Si" : "No",
            p->Denominacion);
            
        apilar(&p,&(*tope));
	}
}

void listarEncInactivos(Encuesta **tope){
	
	Encuesta *p=NULL, *tp2=NULL;
	
	while(vaciaP(*tope)!=1){
		desapilar(&p, &(*tope));
		apilar(&p,&tp2);
	}
	printf("\n--- Listado de Encuestas ---\n");
    printf("ID | Mes | AÒo | Procesada | Activa | Denominacion\n");
    printf("--------------------------------------------------\n");
	while(vaciaP(tp2)!=1){
		desapilar(&p,&tp2);
		if(p->Activa==0){
			printf("%2d | %2d | %4d | %9s | %6s | %s\n",
            	p->EncuestaId,
            	p->EncuestaMes,
            	p->EncuestaAnio,
            	p->Procesada ? "Si" : "No",
            	p->Activa ? "Si" : "No",
            	p->Denominacion);
        }
        apilar(&p,&(*tope));
	}
}

void bajaPG(Pregunta *ini, int idencuesta, Respuesta *iniRes){
	while (ini!= NULL){
		if (ini->EncuestaId == idencuesta){
			BajaRes(ini->PreguntaId, iniRes);
			ini->Activa = 0;
		}
		ini = ini->sgte;
	}
	
	FILE *archivo = fopen("preguntas.csv", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de preguntas\n");
        return;
    }

    // Creamos un archivo temporal
    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL) {
        printf("Error al crear archivo temporal\n");
        fclose(archivo);
        return;
    }

    char linea[256];
    // Leemos la cabecera (si existe) y la copiamos
    if (fgets(linea, sizeof(linea), archivo)) {
    	fprintf(temp, "%s", linea);
    }

    // Procesamos cada l√≠nea del archivo
    while (fgets(linea, sizeof(linea), archivo)) {
    	int preguntaId, encuestaId, ponderacion, activa;
        char pregunta[100];

        // Parseamos la l√≠nea
        if (sscanf(linea, "%d,%d,%d,%d,%[^\n]", &preguntaId, &encuestaId, &ponderacion, &activa, pregunta) == 5) {
            
            // Si es de la encuesta a dar de baja, marcamos como inactiva
            if (encuestaId == idencuesta) {
                activa = 0;
            }
            
            // Escribimos la l√≠nea (modificada o no) en el temporal
            fprintf(temp, "%d,%d,%d,%d,%s\n", preguntaId, encuestaId, ponderacion, activa, pregunta);
        }
    }

    fclose(archivo);
    fclose(temp);

    // Reemplazamos el archivo original
    remove("preguntas.csv");
    rename("temp.csv", "preguntas.csv");
}

void BajaRes(int idpregunta, Respuesta *rcRes){
	Respuesta *aux;
	
	if(rcRes->PreguntaId == idpregunta){
		rcRes->Activa = 0;
	}
	aux = rcRes->sgte;
	while (aux != rcRes){
		if(aux->PreguntaId == idpregunta){
			aux->Activa = 0;
		}			
		aux = aux->sgte;
	}
	// Ahora actualizamos el archivo
    FILE *archivoOriginal, *archivoTemporal;
    char linea[200];
    char tempFilename[] = "temp_preguntas.csv";
    
    archivoOriginal = fopen("preguntas.csv", "r");
    if (archivoOriginal == NULL) {
        perror("Error al abrir el archivo original");
        return;
    }
    
    archivoTemporal = fopen(tempFilename, "w");
    if (archivoTemporal == NULL) {
    	perror("Error al crear archivo temporal");
        fclose(archivoOriginal);
        return;
    }
    
    // Procesamos cada l√≠nea del archivo original
    while (fgets(linea, sizeof(linea), archivoOriginal)) {
        int currentPreguntaId, currentRespuestaId, nro, ponderacion, activa;
        char respuesta[50];
        
        // Parseamos la l√≠nea (asumiendo formato: id_pregunta,id_respuesta,nro,respuesta,ponderacion,activa)
        if (sscanf(linea, "%d,%d,%d,%[^,],%d,%d", &currentPreguntaId, &currentRespuestaId, &nro, respuesta, &ponderacion, &activa) == 6) {
            
            // Si es una respuesta de la pregunta que nos interesa, la desactivamos
        	if (currentPreguntaId == idpregunta) {
            	activa = 0;
            }
            
            // Escribimos la l√≠nea (modificada o no) en el temporal
            fprintf(archivoTemporal, "%d,%d,%d,%s,%d,%d\n", currentPreguntaId, currentRespuestaId, nro, respuesta, ponderacion, activa);
        } else {
            // Si no pudo parsearse, copiamos la l√≠nea tal cual (podr√≠an ser encabezados)
            fprintf(archivoTemporal, "%s", linea);
        }
    }
    
    fclose(archivoOriginal);
    fclose(archivoTemporal);
    
    // Reemplazamos el archivo original con el temporal
    remove("preguntas.csv");
    rename(tempFilename, "preguntas.csv");
}

