/*
 ============================================================================
 Name        : CACHEsym.c
 Author      : jorge
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BIT_0 0
#define BIT_1 1

typedef struct {
	short int ETQ;
	short int Datos[8];
}T_LINEA_CACHE;


//Prototipos

void leerTxt();
void inicializarVariables();
void leerBin();
void resumen();
void comparador(unsigned int addr);

//Variables globales

int tiempoglobal = BIT_0;
int numfallos = BIT_0;
T_LINEA_CACHE array[4];
unsigned char RAM[1024];
char texto[100];
int num_addr = BIT_0;

int main(int argc, char * argv[])
{
	inicializarVariables();
	leerBin();
	leerTxt();
	resumen();

	return 0;
};

//Salida del resumen de los datos de cache

void resumen(){
	printf("\n");
	printf("Resumen:\n");
	printf("=======\n");
	printf("Numero de accesos: %d\n", num_addr);
	printf("Numero de fallos: %d\n", numfallos);
	printf("Tiempo medio: %d\n", tiempoglobal/num_addr);
	printf("Texto: ");
	for(int i = 0 ; i < num_addr ; i++){
	printf("%c ", texto[i]);

	}
	printf("\n");
}

//incializar variables ETQ a FF y Datos a 0

void inicializarVariables(){
	for(int i = 0; i< 4 ; i++){
			array[i].ETQ = 255;
			for(int j = 0 ; j < 8 ; j++){
				array[i].Datos[j] = 0;
			}
		}
}

//Parseo de fichero RAM.bin

void leerBin(){
		FILE *fp;

		fp = fopen("RAM.bin","rb");

		if (fp==NULL)
			{
			perror ("Error al abrir RAM.bin");
					exit(-1);
			}

		fread(RAM,sizeof(RAM),1,fp);

		fclose (fp);
}

//Parseo de fichero acceso_memoria.txt

void leerTxt(){

			FILE *fp;


		 	unsigned int addr;

		 	fp = fopen ( "accesos_memoria.txt", "r" );

		 	if (fp==NULL)
		 	{
		 	   perror ("Error al abrir acceso_memoria.txt");
		 	   exit(-1);
		 	}

		 	while(!feof(fp)){
		 	fscanf(fp, "%X" ,&addr);
		 		comparador(addr);
		 		sleep(2);
		 	}
		 	fclose ( fp );
}

/*Descomposicion de addr en etiqueta, linea y palabra,
comparador de etiqueta con linea cache y
salida de resultados
*/
void comparador(unsigned int addr){

	unsigned int palabra = (addr & 0b0000000111);
	unsigned int linea = (addr & 0b0000011000) >> 3;
	unsigned int etiqueta = (addr & 0b1111100000) >> 5;
	unsigned int n_bloque = (addr & 0b1111111000) >> 3;

	int pin_entrada = BIT_0;

		do {
			if(array[linea].ETQ == etiqueta){
				pin_entrada = BIT_1;
				tiempoglobal = tiempoglobal +10;
				texto[num_addr] = RAM[addr];
				num_addr = num_addr +1;
				printf("T: %d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X\n"
						,tiempoglobal,addr,etiqueta,linea,palabra,array[linea].Datos[palabra]);
				for(int i = 0; i< 4 ; i++){
							printf("ETQ: ");
							printf("%X ",array[i].ETQ);
							printf("Datos ");
							int temporal;
							for (int x = 0; x < 8 / 2; x++) {
								temporal = array[i].Datos[x];
								array[i].Datos[x] = array[i].Datos[8 - x - 1];
								array[i].Datos[8 - x - 1] = temporal;
							}
							for(int j = 0 ; j < 8 ; j++){
								printf("%02X ",array[i].Datos[j]);
							}
							printf("\n");
						}
			}
		if(pin_entrada == 0){
			tiempoglobal = tiempoglobal +10;
			numfallos = numfallos + 1;
			printf("T: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X\n"
					,tiempoglobal,numfallos,addr,etiqueta,linea,palabra,n_bloque);
			printf("Cargando el bloque %02X en la linea %02X\n",n_bloque,linea);
			array[linea].ETQ = etiqueta;
			for(int i = 0 ; i < 8 ; i++){
				array[linea].Datos[i] = RAM[n_bloque*8 + i];
			}
		}
		} while(pin_entrada == 0);
}
