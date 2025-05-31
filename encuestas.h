#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARCHIVO_CSV "encuestas.csv"

int generarNuevoId();
void apilar(Encuesta **nv, Encuesta **tope);
void desapilar(Encuesta **ds, Encuesta **tope);
int vaciaP(Encuesta *top);
void altaEncuesta(Encuesta **tope);

//Funciones de prueba
void listarEncuestas();
void listarPila(Encuesta **tope);


// Función para generar el próximo ID disponible
int generarNuevoId() {
    FILE *archivo = fopen(ARCHIVO_CSV, "r");
    if (archivo == NULL) {
        return 1; // Si el archivo no existe, empezamos con ID 1
    }
    
    int maxId = 0;
    char linea[256];
    
    // Leer el archivo línea por línea
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

// Función para dar de alta una nueva encuesta
void altaEncuesta(Encuesta **tope) {
    Encuesta nueva;
    FILE *archivo;
    
    // Generar ID automáticamente
    nueva.EncuestaId = generarNuevoId();
    
    // Inicializar valores por defecto
    nueva.Activa = 1;
    nueva.Procesada = 0;
    
    // Solicitar datos al usuario
    printf("\n--- Alta de Nueva Encuesta (ID: %d) ---\n", nueva.EncuestaId);
    
    printf("Ingrese la denominacion: ");
    scanf(" %[^\n]", nueva.Denominacion);
    
    printf("Ingrese el mes (1-12): ");
    scanf("%d", &nueva.EncuestaMes);
    
    printf("Ingrese el anio: ");
    scanf("%d", &nueva.EncuestaAnio);
    
    // Abrir archivo en modo append (añadir al final)
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
	}
    
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

// Función para mostrar todas las encuestas
void listarEncuestas() {
    FILE *archivo = fopen(ARCHIVO_CSV, "r");
    if (archivo == NULL) {
        printf("No hay encuestas registradas.\n");
        return;
    }
    
    printf("\n--- Listado de Encuestas ---\n");
    printf("ID | Mes | Año | Procesada | Activa | Denominacion\n");
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
               e.Procesada ? "Sí" : "No",
               e.Activa ? "Sí" : "No",
               e.Denominacion);
    }
    
    fclose(archivo);
}

void listarPila(Encuesta **tope) {
	
	Encuesta *p=NULL, *tp2=NULL;
	
	while(vaciaP(*tope)!=1){
		desapilar(&p, &(*tope));
		apilar(&p,&tp2);
	}
	printf("\n--- Listado de Encuestas ---\n");
    printf("ID | Mes | Año | Procesada | Activa | Denominacion\n");
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
