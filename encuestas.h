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
int ExisteIdEnc(int id, Encuesta **tp);

//Funciones de prueba
void listarPila(Encuesta **tope);
void controlID(int *ID);
void listartodapila(Encuesta **tope);
void listarEncInactivos(Encuesta **tope);
void listarEncNoProc(Encuesta **tope);
void listarEncActivas(Encuesta **tope);
void ListarEncuestaPro(Encuesta **tope);

// Funci�n para generar el pr�ximo ID disponible
int generarNuevoId(){
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

// Modulo para dar de alta una nueva encuesta
void altaEncuesta(Encuesta **tope){
    Encuesta nueva;
    FILE *archivo;
    char mes[10], anio[10];
	int control=0, controlanio=0;
    // Generar ID autom�ticamente
    nueva.EncuestaId = generarNuevoId();
    
    // Inicializar valores por defecto
    nueva.Activa = 0;
    nueva.Procesada = 0;
    
    // Solicitar datos al usuario
    printf("\n--- Alta de Nueva Encuesta (ID: %d) ---\n", nueva.EncuestaId);
    
    printf("Ingrese la denominacion: ");
    scanf(" %[^\n]", nueva.Denominacion);
    
    fflush(stdin);
    while (control==0){
		control=0;
		printf("Ingrese el mes (01-12): ");
		gets(mes);
		if(strlen(mes)==2){
			if((mes[0]=='0' && (mes[1]>='1' && mes[1]<='9')) || (mes[0]=='1' && (mes[1]>='0' && mes[1]<='2')) /*&& fecha[7]=='/'*/){
				control=1;
			} 
		}
		if (control==0){
			printf("\nerror\n");
		} 
	}
	strcpy(nueva.EncuestaMes, mes);
	
	while (controlanio==0){
		controlanio=0;
		printf("Ingrese el anio: ");
		gets(anio);
		if(strlen(anio)==4){
			if(anio[0]=='2' && anio[1]=='0' && anio[2]>='2' && anio[3]>='5'){
				controlanio=1;
			}
		}
		if (controlanio==0){
			printf("\nerror\n");
		} 
	}
	strcpy(nueva.EncuestaAnio, anio);
    
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
    	strcpy(n->EncuestaAnio, nueva.EncuestaAnio);
    	strcpy(n->EncuestaMes, nueva.EncuestaMes);
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
    fprintf(archivo, "%d,%s,%s,%d,%d,%s\n",
		nueva.EncuestaId,
	    nueva.EncuestaMes,
	    nueva.EncuestaAnio,
	    nueva.Procesada,
	    nueva.Activa,
	    nueva.Denominacion);
		
	fclose(archivo);
    printf("Encuesta guardada exitosamente!\n");
}

// Modulo para listar las encuestas activas y no procesadas
void listarPila(Encuesta **tope){
	
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
		if((p->Activa==1) && (p->Procesada==0)){
			printf("%2d | %2s | %4s | %9s | %6s | %s\n",
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
	        sscanf(linea, "%d,%s,%s,%d,%d,%[^\n]",
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
	        fprintf(archivoTemp, "%d,%s,%s,%d,%d,%s\n",
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

//Modulo para control de ID
void controlID(int *ID){
	int resultado;
    char confirmacion;

    do {
        // Solicita ingreso del ID
        printf("Ingrese el ID: ");
        resultado = scanf("%d", &(*ID));

        // Validaci�n de entrada num�rica
        while (resultado != 1) {
            while (getchar() != '\n'); // Limpia el buffer
            printf("Entrada invalida. Ingrese un numero entero para el ID: ");
            resultado = scanf("%d", &(*ID));
        }

        // Limpia el buffer antes de leer el char
        while (getchar() != '\n');

        // Confirmaci�n del ID
        printf("�Esta seguro de que el ID ingresado es correcto? (s/n): ");
        scanf("%c", &confirmacion);
		
        // Limpia el buffer por si hay m�s caracteres
        while (getchar() != '\n');

    } while (confirmacion == 'n' || confirmacion == 'N');

    printf("ID confirmado: %d\n", (*ID));

    
}

// Modulo para listar todas las encuestas desde la pila
void listartodapila(Encuesta **tope){
	
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
		printf("%2d | %2s | %4s | %9s | %6s | %s\n",
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
    printf("ID | Mes | A�o | Procesada | Activa | Denominacion\n");
    printf("--------------------------------------------------\n");
	while(vaciaP(tp2)!=1){
		desapilar(&p,&tp2);
		if(p->Activa==0){
			printf("%2d | %2s | %4s | %9s | %6s | %s\n",
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

//Modulo para listar encuestas no procesadas
void listarEncNoProc(Encuesta **tope){
	
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
		if((p->Procesada==0)){
			printf("%2d | %2s | %4s | %9s | %6s | %s\n",
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

//Modulo para listar encuestas activas
void listarEncActivas(Encuesta **tope){
	
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
			printf("%2d | %2s | %4s | %9s | %6s | %s\n",
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

void ListarEncuestaPro(Encuesta **tope){
	
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
		if((p->Procesada==1)){
			printf("%2d | %2s | %4s | %9s | %6s | %s\n",
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

