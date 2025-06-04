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
void bajaEncuesta(Encuesta **tope);


//Funciones de prueba
void listarEncuestas();
void listarPila(Encuesta **tope);
void controlID(int *ID);
void listartodapila(Encuesta **tope);

// Funci�n para generar el pr�ximo ID disponible
int generarNuevoId() {
    FILE *archivo = fopen(ARCHIVO_CSV, "r");
    if (archivo == NULL) {
        return 1; // Si el archivo no existe, empezamos con ID 1
    }
    
    int maxId = 0;
    char linea[256];
    
    // Leer el archivo l�nea por l�nea
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

// Funci�n para dar de alta una nueva encuesta
void altaEncuesta(Encuesta **tope) {
    Encuesta nueva;
    FILE *archivo;
    
    // Generar ID autom�ticamente
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
    
    // Abrir archivo en modo append (a�adir al final)
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

// Funci�n para mostrar todas las encuestas
void listarEncuestas() {
    FILE *archivo = fopen(ARCHIVO_CSV, "r");
    if (archivo == NULL) {
        printf("No hay encuestas registradas.\n");
        return;
    }
    
    printf("\n--- Listado de Encuestas ---\n");
    printf("ID | Mes | A�o | Procesada | Activa | Denominacion\n");
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
               e.Procesada ? "S�" : "No",
               e.Activa ? "S�" : "No",
               e.Denominacion);
    }
    
    fclose(archivo);
}

// Funci�n para listar las encuestas activas desde la pila
void listarPila(Encuesta **tope) {
	
	Encuesta *p=NULL, *tp2=NULL;
	
	while(vaciaP(*tope)!=1){
		desapilar(&p, &(*tope));
		apilar(&p,&tp2);
	}
	printf("\n--- Listado de Encuestas ---\n");
    printf("ID | Mes | A�o | Procesada | Activa | Denominacion\n");
    printf("--------------------------------------------------\n");
	while(vaciaP(tp2)!=1){
		desapilar(&p,&tp2);
		if(p->Activa==1){
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

void bajaEncuesta(Encuesta **tope){
	int idBaja;
	Encuesta *p=NULL, *tp2=NULL;
    printf("\n--- Baja de Encuesta ---\n");
    listarPila(&(*tope));
	controlID(&idBaja);
    while (!vaciaP(*tope)){
    	desapilar(&p, &(*tope));
    	if(p->EncuestaId == idBaja){
    		p->Activa = 0;
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
        printf("No se encontró encuesta con ID %d\n", idBaja);
    }
}

//Funci�n para control de ID
void controlID(int *ID){
	int resultado;
    char confirmacion;

    do {
        // Solicita ingreso del ID
        printf("Ingrese el ID: ");
        resultado = scanf("%d", ID);

        // Validaci�n de entrada num�rica
        while (resultado != 1) {
            while (getchar() != '\n'); // Limpia el buffer
            printf("Entrada inv�lida. Ingrese un n�mero entero para el ID: ");
            resultado = scanf("%d", ID);
        }

        // Limpia el buffer antes de leer el char
        while (getchar() != '\n');

        // Confirmaci�n del ID
        printf("�Est� seguro de que el ID ingresado es correcto? (s/n): ");
        scanf("%c", &confirmacion);

        // Limpia el buffer por si hay m�s caracteres
        while (getchar() != '\n');

    } while (confirmacion == 'n' || confirmacion == 'N');

    printf("ID confirmado: %d\n", ID);

    
}



// Funci�n para listar todas las encuestas desde la pila
void listartodapila(Encuesta **tope) {
	
	Encuesta *p=NULL, *tp2=NULL;
	
	while(vaciaP(*tope)!=1){
		desapilar(&p, &(*tope));
		apilar(&p,&tp2);
	}
	printf("\n--- Listado de Encuestas ---\n");
    printf("ID | Mes | A�o | Procesada | Activa | Denominacion\n");
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


