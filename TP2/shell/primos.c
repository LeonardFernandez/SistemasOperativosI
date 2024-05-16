#include <xinu.h>
#define MAX 5000

int total = 0;
sid32 semaforo,semaforoTotal;
 int encontrar_primos(int from, int to)
 {
         int i, n, primo;
         printf( "\n   " );

          for ( i = from ; i <= to ; i++ ) {
                  primo = 1;
                  n = 2;

                   while ( (n <= i / 2) && primo ) {
                           if ( i % n == 0 )
                                   primo = 0;

                            n++;
                   }

                   if ( primo ){
                        wait(semaforo);
                        printf( "%d  ,", i );
                        total++;
                        signal(semaforo);
                        }
          }
        signal(semaforoTotal);
          
          return 0;
 }

 void controlPlanificador(void){
        int msPA,msPB;
        msPA=2;
        msPB=8;
        int pidA = create(encontrar_primos,2048,5,"procesoAP",2,1,50);
        int pidB = create(encontrar_primos, 4096,5,"procesoB",2,50,100);
        resume(pidA);
        resume(pidB);
        high_level_scheduler2(pidA,msPA,pidB,msPB);
}
 int mainPrimos(void)
 {
        semaforo=semcreate(1);
        //encontrar_primos(1,5000);
        int pidControl=create(controlPlanificador,2048,20,"procesoCntl",0);
        resume(pidControl);
        wait(semaforoTotal);
        wait(semaforoTotal);
        printf("Total : %d \n", total);
        return 0;
 }