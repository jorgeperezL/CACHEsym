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

typedef struct {
	short int ETQ;
	short int Datos[8];
}T_LINEA_CACHE;


int converBinDec(char * bin);
char * converHexBin(char * hex);
void leerTxt();
void inicializarVariables();
void leerBin();
char * converHexBin(char * hex);

int tiempoglobal = 0;
int numfallos = 0;
T_LINEA_CACHE array[4];
unsigned char RAM[1024];

int main(int argc, char * argv[])
{
	inicializarVariables();
	leerBin();
	leerTxt();

	return 0;
};

void inicializarVariables(){
	for(int i = 0; i< 4 ; i++){
			array[i].ETQ = 255;
			for(int j = 0 ; j < 8 ; j++){
				array[i].Datos[j] = 0;
			}
		}
}

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

void leerTxt(){

			FILE *fp;

		 	char buffer[10];
		 	char num_2[30];

		 	fp = fopen ( "accesos_memoria.txt", "r" );

		 	if (fp==NULL)
		 	{
		 	   perror ("Error al abrir acceso_memoria.txt");
		 	   exit(-1);
		 	}

		 	while(!feof(fp)){
		 	fscanf(fp, "%s" ,buffer);
		 		char * bin = converHexBin(buffer);
		 		comparador(bin);
		 		printf("%s\n",bin);

		 		//sleep(2);
		 	}
		 	fclose ( fp );
}

void comparador(char * bin){
	char etq[5];
	char linea[2];
	char palabra[3];
	char n_bloque[7];

	for(int i = 0 ; i < 5 ; i++){
		etq[i] = bin[6+i];
	}

	for(int i = 0 ; i < 2 ; i++){
		linea[i] = bin[11+i];
	}

	for(int i = 0 ; i < 3 ; i++){
		palabra[i] = bin[13+i];
	}

	for(int i = 0 ; i < 7 ; i++){
		n_bloque[i]=bin[6+i];
	}

	int etq_dec = converBinDec(etq);
	int linea_dec = converBinDec(linea);
	int hex_dec = converBinDec(bin);
	int palabra_dec = converBinDec(palabra);
	int n_bloque_dec = converBinDec(n_bloque);


	int pin_entrada = 0;

	do {

		if(array[linea_dec].ETQ == etq_dec){
			pin_entrada = 1;
			tiempoglobal = tiempoglobal +10;
			int dato = array[linea_dec].Datos[palabra_dec];
			printf("T: %d, Acierto de CACHE, ADDR %04X ETQ %X \
					linea %02X palabra %02X DATO %02X",tiempoglobal,hex_dec,etq_dec,linea_dec,palabra_dec,dato);

			for(int i = 0; i< 4 ; i++){
						printf("ETQ: ");
						printf("%d ",array[i].ETQ);
						printf("Datos ");
						for(int j = 0 ; j < 8 ; j++){
							printf("%d ",array[i].Datos[j]);
						}
					}
		}
	if(pin_entrada == 0){
		tiempoglobal = tiempoglobal +10;
		numfallos = numfallos + 1;
		printf("“T: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X \
			palabra %02X bloque %02X",tiempoglobal,numfallos,hex_dec,etq_dec,linea_dec,palabra_dec,n_bloque_dec);
		printf("“Cargando el bloque %02X en la linea %02X ",n_bloque_dec,linea_dec);
		array[linea_dec].ETQ = etq_dec;
		for(int i = 0 ; i < 8 ; i++){
			array[linea_dec].Datos[i] = RAM[n_bloque_dec*8 + i];
		}
	}
	} while(pin_entrada == 0);
}

int converBinDec(char * bin){
	int dec = 0;
	while (bin != '\n') {
	if (bin == '1') dec = dec * 2 + 1;
	else if (bin == '0') dec *= 2; }
	return dec;
}

char * converHexBin(char * hex){

	char * bin = (char*)malloc(17*sizeof(char));
	bin[0] = '\0';
	    for(int i=0; hex[i]!='\0'; i++)
	    {

	    	switch(hex[i])
	        {
	            case '0':
	                strcat(bin, "0000");
	                break;
	            case '1':
	                strcat(bin, "0001");
	                break;
	            case '2':
	                strcat(bin, "0010");
	                break;
	            case '3':
	                strcat(bin, "0011");
	                break;
	            case '4':
	                strcat(bin, "0100");
	                break;
	            case '5':
	                strcat(bin, "0101");
	                break;
	            case '6':
	                strcat(bin, "0110");
	                break;
	            case '7':
	                strcat(bin, "0111");
	                break;
	            case '8':
	                strcat(bin, "1000");
	                break;
	            case '9':
	                strcat(bin, "1001");
	                break;
	            case 'a':
	            case 'A':
	                strcat(bin, "1010");
	                break;
	            case 'b':
	            case 'B':
	                strcat(bin, "1011");
	                break;
	            case 'c':
	            case 'C':
	                strcat(bin, "1100");
	                break;
	            case 'd':
	            case 'D':
	                strcat(bin, "1101");
	                break;
	            case 'e':
	            case 'E':
	                strcat(bin, "1110");
	                break;
	            case 'f':
	            case 'F':
	                strcat(bin, "1111");
	                break;
	            default:
	                printf("Error en hexadecimal");
	        }
	    }
	    bin[16] = '\0';
	    return bin;
}
