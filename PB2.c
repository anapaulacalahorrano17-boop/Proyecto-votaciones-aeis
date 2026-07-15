
 //PROYECTO: Votaciones AEIS - Parte 2 (Sistema de Voto Electrónico)
 // MATERIA: Programación I
 //DESCRIPCIÓN: Manejo de memoria dinámica, archivos, búsqueda y ordenamiento.
 

#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>  
#include <ctype.h>   

/* =======================================================
 * ---ESTRUCTURAS DE DATOS ---
 * ======================================================= */
typedef struct {
    char cedula[11];      
    char nombres[50];     
    int haVotado;         
} Votante;

typedef struct {
    int idLista;          
    char nombre[50];      
    int votos;            
} Candidato;




// Funciones de Memoria y Archivo
int iniciarCandidatos(Candidato **candidatos, int *numCandidatos);
int iniciarPadron(Votante **padron, int *numVotantes);
void guardarPadron(Votante *padron, int numVotantes);
void guardarResultadosCSV(Candidato *candidatos, int numCandidatos);
void generarReportePDF(Candidato *candidatos, int numCandidatos, int totalVotos);

// Funciones de Lógica y Búsqueda
void mostrarMenu();
void limpiarBuffer();
int validarCedulaEcuatoriana(char *cedula);
int busquedaBinariaVotante(Votante *padron, int n, char *cedula);
void procesoVotacion(Votante *padron, int numVotantes, Candidato *candidatos, int numCandidatos);

// Funciones de Ordenamiento y Recursividad
void ordenarVotantesSeleccion(Votante *padron, int n);
void ordenarCandidatosSeleccion(Candidato *candidatos, int n);
int sumarVotosRecursivo(Candidato *candidatos, int n);


int main() 
{
    Votante *padron = NULL;
    Candidato *candidatos = NULL;
    int numVotantes = 0, numCandidatos = 0;
    int opcion;

    // Inicialización y Archivos 
    if (!iniciarCandidatos(&candidatos, &numCandidatos)) {
        printf("Error al asignar memoria para candidatos.\n");
        return 1;
    }
    if (!iniciarPadron(&padron, &numVotantes)) {
        printf("Error al cargar el padron electoral.\n");
        free(candidatos); 
        return 1;
    }

    //Requisito para la búsqueda 
    ordenarVotantesSeleccion(padron, numVotantes);

    printf("=========================================\n");
    printf(" SISTEMA ELECTORAL AEIS INICIADO \n");
    printf("=========================================\n");

    do {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) {
            limpiarBuffer();
            opcion = -1; 
        }

        switch (opcion) {
            case 1:
                procesoVotacion(padron, numVotantes, candidatos, numCandidatos);
                break;
            case 2:
                printf("\n--- RESULTADOS EN VIVO ---\n");
        
                int totalVotos = sumarVotosRecursivo(candidatos, numCandidatos);
                printf("Total de sufragios registrados: %d\n", totalVotos);
                for(int i=0; i<numCandidatos; i++) {
                    printf("Lista %d (%s): %d votos\n", candidatos[i].idLista, candidatos[i].nombre, candidatos[i].votos);
                }
                break;
            case 3:
                printf("\nFinalizando elecciones y generando reportes...\n");
                
                
                ordenarCandidatosSeleccion(candidatos, numCandidatos);
                int totalFinal = sumarVotosRecursivo(candidatos, numCandidatos);
                
        
                guardarPadron(padron, numVotantes);
                guardarResultadosCSV(candidatos, numCandidatos);
                generarReportePDF(candidatos, numCandidatos, totalFinal);
                
                printf("Reportes generados con exito ('resultados.csv', 'Reporte_Electoral.pdf').\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 3);

    //Prevención de Memory Leaks 
    free(padron);
    free(candidatos);
    
    printf("Sistema apagado de forma segura.\n");
    return 0;
}


//ARCHIVOS Y MEMORIA 
 
int iniciarCandidatos(Candidato **candidatos, int *numCandidatos) {
    *numCandidatos = 3; 
    *candidatos = (Candidato *)malloc((*numCandidatos) * sizeof(Candidato));
    if (*candidatos == NULL) return 0;

    (*candidatos)[0].idLista = 1; strcpy((*candidatos)[0].nombre, "Innovacion Estudiantil"); (*candidatos)[0].votos = 0;
    (*candidatos)[1].idLista = 2; strcpy((*candidatos)[1].nombre, "Fuerza Universitaria");   (*candidatos)[1].votos = 0;
    (*candidatos)[2].idLista = 0; strcpy((*candidatos)[2].nombre, "Nulo / Blanco");          (*candidatos)[2].votos = 0;
    return 1;
}

int iniciarPadron(Votante **padron, int *numVotantes) {
    FILE *archivo = fopen("padron.txt", "r");
    
    if (archivo == NULL) {
        *numVotantes = 4;
        *padron = (Votante *)malloc((*numVotantes) * sizeof(Votante));
        if (*padron == NULL) return 0;
        strcpy((*padron)[0].cedula, "1712345678"); strcpy((*padron)[0].nombres, "Juan Perez"); (*padron)[0].haVotado = 0;
        strcpy((*padron)[1].cedula, "1723456789"); strcpy((*padron)[1].nombres, "Maria Lopez"); (*padron)[1].haVotado = 0;
        strcpy((*padron)[2].cedula, "1734567890"); strcpy((*padron)[2].nombres, "Carlos Ruiz"); (*padron)[2].haVotado = 0;
        strcpy((*padron)[3].cedula, "1714264724"); strcpy((*padron)[3].nombres, "Ana Viteri");  (*padron)[3].haVotado = 0;
        guardarPadron(*padron, *numVotantes); 
        return 1;
    }
    
    char buffer[150];
    *numVotantes = 0;
    while (fgets(buffer, sizeof(buffer), archivo)) { (*numVotantes)++; }
    rewind(archivo); 
    
    *padron = (Votante *)malloc((*numVotantes) * sizeof(Votante));
    if (*padron == NULL) { fclose(archivo); return 0; }
    
    int i = 0;
    while (fscanf(archivo, "%10s %d %[^\n]", (*padron)[i].cedula, &((*padron)[i].haVotado), (*padron)[i].nombres) == 3) {
        i++;
    }
    fclose(archivo);
    return 1;
}

void guardarPadron(Votante *padron, int numVotantes) {
    FILE *archivo = fopen("padron.txt", "w"); 
    if (archivo == NULL) return;
    for (int i = 0; i < numVotantes; i++) {
        fprintf(archivo, "%s %d %s\n", padron[i].cedula, padron[i].haVotado, padron[i].nombres);
    }
    fclose(archivo);
}

void guardarResultadosCSV(Candidato *candidatos, int numCandidatos) {
    FILE *archivo = fopen("resultados.csv", "w");
    if (archivo == NULL) return;
    fprintf(archivo, "ID_Lista,Nombre_Candidato,Votos\n"); 
    for (int i = 0; i < numCandidatos; i++) {
        fprintf(archivo, "%d,%s,%d\n", candidatos[i].idLista, candidatos[i].nombre, candidatos[i].votos);
    }
    fclose(archivo);
}

void generarReportePDF(Candidato *candidatos, int numCandidatos, int totalVotos) {
    FILE *archivo = fopen("Reporte_Electoral.pdf", "w"); 
    if (archivo == NULL) return;
    fprintf(archivo, "=========================================\n");
    fprintf(archivo, "      ACTA OFICIAL DE VOTACIONES AEIS    \n");
    fprintf(archivo, "=========================================\n\n");
    for (int i = 0; i < numCandidatos; i++) {
        float porcentaje = (totalVotos > 0) ? ((float)candidatos[i].votos / totalVotos) * 100 : 0.0;
        fprintf(archivo, "Lista %d: %s \n\t -> Votos obtenidos: %d (%.2f%%)\n\n", 
                candidatos[i].idLista, candidatos[i].nombre, candidatos[i].votos, porcentaje);
    }
    fclose(archivo);
}



//LÓGICA Y BÚSQUEDA 
 
void mostrarMenu() {
    printf("\n--- MENU PRINCIPAL ---\n");
    printf("1. Ingresar a Sufragar (Votar)\n");
    printf("2. Ver Resultados en Vivo\n");
    printf("3. Finalizar Elecciones y Generar Reportes\n");
    printf("Elija una opcion: ");
}

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
}

// Filtro matemático Modulo 10 
int validarCedulaEcuatoriana(char *cedula) {
    if (strlen(cedula) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (!isdigit(cedula[i])) return 0; 
    }

    int provincia = (cedula[0] - '0') * 10 + (cedula[1] - '0');
    if (provincia < 1 || provincia > 24) return 0; 

    int tercerDigito = cedula[2] - '0';
    if (tercerDigito >= 6) return 0; 

    int sumaTotal = 0;
    for (int i = 0; i < 9; i++) {
        int digito = cedula[i] - '0';
        if (i % 2 == 0) { 
            digito = digito * 2;
            if (digito > 9) digito = digito - 9;
        }
        sumaTotal += digito;
    }

    int verificadorCalculado = 10 - (sumaTotal % 10);
    if (verificadorCalculado == 10) verificadorCalculado = 0;

    int verificadorReal = cedula[9] - '0';
    return (verificadorCalculado == verificadorReal); 
}

// Implementación de Búsqueda Binaria
int busquedaBinariaVotante(Votante *padron, int n, char *cedula) {
    int inicio = 0, fin = n - 1;
    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        int cmp = strcmp(padron[medio].cedula, cedula);
        
        if (cmp == 0) return medio; 
        
        if (cmp < 0) inicio = medio + 1;
        else fin = medio - 1;
    }
    return -1; 
}

void procesoVotacion(Votante *padron, int numVotantes, Candidato *candidatos, int numCandidatos) {
    char cedulaIngresada[15];
    printf("\n--- ZONA DE SUFRAGIO ---\nIngrese su numero de cedula: ");
    scanf("%14s", cedulaIngresada);
    limpiarBuffer();

    if (validarCedulaEcuatoriana(cedulaIngresada) == 0) {
        printf("ALERTA DE SEGURIDAD: La cedula '%s' no existe o es matematicamente invalida.\n", cedulaIngresada);
        return; 
    }

    int idxVotante = busquedaBinariaVotante(padron, numVotantes, cedulaIngresada);

    if (idxVotante == -1) {
        printf("ERROR: La cedula SI es valida legalmente, pero NO consta en el padron electoral.\n");
        return;
    }
    if (padron[idxVotante].haVotado == 1) {
        printf("ERROR FRAUDE ELECTORAL: El elector ya ejercio su voto previamente.\n");
        return;
    }

    printf("\nBienvenido/a %s.\nPapeleta Virtual:\n", padron[idxVotante].nombres);
    for (int i = 0; i < numCandidatos; i++) {
        printf("Opcion [%d] -> Lista %d: %s\n", i + 1, candidatos[i].idLista, candidatos[i].nombre);
    }

    int voto;
    printf("Elija una opcion (1-%d): ", numCandidatos);
    if (scanf("%d", &voto) != 1) { limpiarBuffer(); voto = -1; }

    if (voto >= 1 && voto <= numCandidatos) {
        candidatos[voto - 1].votos++;
        padron[idxVotante].haVotado = 1; 
        printf("Su voto ha sido procesado con exito.\n");
    } else {
        printf("Voto invalido. Voto Nulo registrado por defecto.\n");
        for(int i=0; i<numCandidatos; i++) {
             if(candidatos[i].idLista == 0) { candidatos[i].votos++; break; }
        }
        padron[idxVotante].haVotado = 1;
    }
}




 // ORDENAMIENTO/RECURSIVIDAD 
void ordenarVotantesSeleccion(Votante *padron, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i; 
        for (int j = i + 1; j < n; j++) {
            if (strcmp(padron[j].cedula, padron[min_idx].cedula) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Votante temp = padron[i];
            padron[i] = padron[min_idx];
            padron[min_idx] = temp;
        }
    }
}

void ordenarCandidatosSeleccion(Candidato *candidatos, int n) {
    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (candidatos[j].votos > candidatos[max_idx].votos) {
                max_idx = j; 
            }
        }
        if (max_idx != i) {
            Candidato temp = candidatos[i];
            candidatos[i] = candidatos[max_idx];
            candidatos[max_idx] = temp;
        }
    }
}

int sumarVotosRecursivo(Candidato *candidatos, int n) {
    if (n <= 0) return 0; 
    return candidatos[n - 1].votos + sumarVotosRecursivo(candidatos, n - 1);
}
