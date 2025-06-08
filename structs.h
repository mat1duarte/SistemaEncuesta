#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Encuesta{
	int EncuestaId, EncuestaMes, EncuestaAnio, Procesada, Activa;
	char Denominacion[50];
	struct Encuesta *sgte;
}Encuesta;

typedef struct Pregunta{
	int PreguntaId, EncuestaId, Activa;
	char Pregunta[100];
	float  Ponderacion;
	struct Pregunta *sgte;
}Pregunta;

typedef struct Respuesta{
	int RespuestaId, PreguntaId, RespuestaNro, Activa;
	char Respuesta[50];
	float  Ponderacion;
	struct Respuesta *sgte;
}Respuesta;

typedef struct Encuestador{
	int EncuestadorId;
	char Nombre[30];
	struct Encuestador *sgte;
}Encuestador;

typedef struct Participaciones{
	int IdEncuesta, IdPregunta, IdRespuesta, IdEncuestador, IdEncRespondida;
	char FechaRealizo[20];
	struct Participaciones *sgte;
}Participaciones;


