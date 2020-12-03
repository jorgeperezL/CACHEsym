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

typedef struct {
	short int ETQ;
	short int Datos[8];
}T_LINEA_CACHE;



int tiempoglobal = 0;
int numfallos = 0;
//itoa(num,num_2,2);

int main(int argc, char * argv[])
{
	T_LINEA_CACHE array[4];

	for(int i = 0; i< 4 ; i++){
		array[i].ETQ = 255;
		for(int j = 0 ; j < 8 ; j++){
			array[i].Datos[j] = 0;
		}
	}





	return 0;
};


void leerTxt(){
			FILE *fp;

		 	char buffer[100];

		 	fp = fopen ( "accesos_memoria.txt", "r" );

		 	if (fp==NULL)
		 	{
		 	   perror ("Error al abrir acceso_memoria.txt");
		 	   exit(-1);
		 	}

		 	while(!feof(fp)){
		 	fscanf(fp, "%s" ,buffer);
		 	}
		 	fclose ( fp );
}

void leerBin(){

			unsigned char buffer[1024];
			FILE *fp;

			fp = fopen("RAM.bin","rb");

			if (fp==NULL)
			{
				perror ("Error al abrir RAM.bin");
				exit(-1);
			}

			fread(buffer,sizeof(buffer),1,fp);

			fclose (fp);

}

int converBinDec(char bin){
	int dec = 0;

	while (bin != '\n') {
	if (bin == '1') dec = dec * 2 + 1;
	else if (bin == '0') dec *= 2; }

	return dec;
}
