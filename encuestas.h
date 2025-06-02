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
void modificarEncuesta(Encuesta **tope);


//Funciones de prueba
void listarEncuestas();
void listarPila(Encuesta **tope);
void controlID(int *ID);
void listartodapila(Encuesta **tope);
void actualizarCSV(Encuesta *tope);

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
        printf("No se encontrÃ³ encuesta con ID %d\n", idBaja);
    }
}

void controlID(int *ID){
	int resultado;
    char confirmacion;

    do {
        // Solicita ingreso del ID
        printf("Ingrese el ID: ");
        resultado = scanf("%d", ID);

        // Validación de entrada numérica
        while (resultado != 1) {
            while (getchar() != '\n'); // Limpia el buffer
            printf("Entrada inválida. Ingrese un número entero para el ID: ");
            resultado = scanf("%d", ID);
        }

        // Limpia el buffer antes de leer el char
        while (getchar() != '\n');

        // Confirmación del ID
        printf("¿Está seguro de que el ID ingresado es correcto? (s/n): ");
        scanf("%c", &confirmacion);

        // Limpia el buffer por si hay más caracteres
        while (getchar() != '\n');

    } while (confirmacion == 'n' || confirmacion == 'N');

    printf("ID confirmado: %d\n", ID);

    
}

void modificarEncuesta(Encuesta **tope) {
    if (vaciaP(*tope)) {
        printf("La pila de encuestas estÃ¡ vacÃ­a\n");
        return;
    }

    int idModificar;
    
    printf("\n--- Modificar Encuesta ---\n");
  
    listartodapila(&(*tope));
    controlID(&idModificar);

    // Pila auxiliar para buscar la encuesta
    Encuesta *pilaAux = NULL;
    Encuesta *encuestaModificar = NULL;
    int encontrada = 0;

    // Buscar la encuesta en la pila
    while (!vaciaP(*tope) && !encontrada) {
        Encuesta *actual;
        desapilar(&actual, tope);
        
        if (actual->EncuestaId == idModificar) {
            encuestaModificar = actual;
            encontrada = 1;
        } else {
            apilar(&actual, &pilaAux);
        }
    }

    // Si no se encontrÃ³, restaurar la pila y salir
    if (!encontrada) {
        printf("No se encontrÃ³ encuesta con ID %d\n", idModificar);
        // Restaurar la pila original
        while (!vaciaP(pilaAux)) {
            Encuesta *temp;
            desapilar(&temp, &pilaAux);
            apilar(&temp, tope);
        }
        return;
    }

    // MenÃº de modificaciÃ³n interactivo
    int opcion;
    do {
        printf("\nEncuesta ID: %d\n", encuestaModificar->EncuestaId);
        printf("1. Mes (Actual: %d)\n", encuestaModificar->EncuestaMes);
        printf("2. AÃ±o (Actual: %d)\n", encuestaModificar->EncuestaAnio);
        printf("3. Procesada (Actual: %s)\n", encuestaModificar->Procesada ? "SÃ­" : "No");
        printf("4. Activa (Actual: %s)\n", encuestaModificar->Activa ? "SÃ­" : "No");
        printf("5. DenominaciÃ³n (Actual: %s)\n", encuestaModificar->Denominacion);
        printf("0. Terminar modificaciones\n");
        printf("Seleccione campo a modificar: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                printf("Nuevo mes (1-12): ");
                scanf("%d", &encuestaModificar->EncuestaMes);
                break;
            case 2:
                printf("Nuevo aÃ±o: ");
                scanf("%d", &encuestaModificar->EncuestaAnio);
                break;
            case 3:
                printf("Â¿Procesada? (1=SÃ­, 0=No): ");
                scanf("%d", &encuestaModificar->Procesada);
                break;
            case 4:
                printf("Â¿Activa? (1=SÃ­, 0=No): ");
                scanf("%d", &encuestaModificar->Activa);
                break;
            case 5:
                printf("Nueva denominaciÃ³n: ");
                scanf(" %[^\n]", encuestaModificar->Denominacion);
                break;
            case 0:
                printf("Guardando cambios...\n");
                break;
            default:
                printf("OpciÃ³n no vÃ¡lida\n");
        }
    } while (opcion != 0);

    // Volver a apilar la encuesta modificada
    apilar(&encuestaModificar, tope);

    // Restaurar el resto de la pila
    while (!vaciaP(pilaAux)) {
        Encuesta *temp;
        desapilar(&temp, &pilaAux);
        apilar(&temp, tope);
    }

    // Actualizar el archivo CSV
    actualizarCSV(*tope);
    printf("Encuesta modificada exitosamente!\n");
}

void listartodapila(Encuesta **tope) {
	
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

void actualizarCSV(Encuesta *tope) {
    // Primero invertimos la pila para guardar en orden correcto
    Encuesta *pilaInvertida = NULL;
    Encuesta *aux;
    
    while (!vaciaP(tope)) {
        desapilar(&aux, &tope);
        apilar(&aux, &pilaInvertida);
    }
    
    // Ahora escribimos al archivo
    FILE *archivo = fopen(ARCHIVO_CSV, "w");
    if (archivo == NULL) {
        printf("Error al abrir archivo para escritura\n");
        return;
    }
    
    while (!vaciaP(pilaInvertida)) {
        desapilar(&aux, &pilaInvertida);
        fprintf(archivo, "%d,%d,%d,%d,%d,%s\n",
                aux->EncuestaId,
                aux->EncuestaMes,
                aux->EncuestaAnio,
                aux->Procesada,
                aux->Activa,
                aux->Denominacion);
        apilar(&aux, &tope); // Volvemos a construir la pila original
    }
    
    fclose(archivo);
}
