#include<xinu.h>
void ahorcado(){
	char palabra[]="testeando";
    int c=0;
	int cantLetras=0;
	int intentos=0;
	int limiteIntentos=7;
	int i = 0;
	/*Calculo cantLetras*/
	while(palabra[cantLetras]!='\0'){
		cantLetras++;
	}

	char palabraOutput[cantLetras+1];
	palabraOutput[cantLetras]='\0';
	
	while(i<cantLetras){
	  palabraOutput[i]='_';
	  i++;
	}
	
	/* Decirle al sistema que el modo input es RAW */
	control(CONSOLE, TC_MODER,0,0);
	
	while(1) {
		printf("\r                                                          ");
		printf("\r %s c = %c  ingrese una letra (0 para salir): ",palabraOutput, c);
		c = getchar();

		if (c == '0' || intentos>=limiteIntentos){
			break;
		}else{
		  int estaContenida=0;
		  for(i=0; i<cantLetras; i++){
		    if(palabra[i]==c){
		      palabraOutput[i]=c;
		      estaContenida=1; /*letra encontrada*/
		    }
		  }
		  if(!estaContenida){
		    intentos++;
		  }
		}
		
	}
	if(intentos>=limiteIntentos){
	  printf("\n Perdiste!, Limite de intentos alcanzados");
	}
	
	control(CONSOLE,TC_MODEC,0,0); //Consola tty modo cooked
}