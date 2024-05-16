#include<xinu.h>
int prioridadScheduler;
int pidScheduler;
void administrarPrioridades(int pidProceso, int msProceso){
    int prioridadProceso=getprio(pidProceso); 
    chprio(pidProceso, prioridadScheduler-1);//Cambia prioridad del proceso a prioriPlanificador-1
    sleepms(msProceso); //Libera CPU
    chprio(pidProceso,prioridadProceso);//Devuelve al proceso prioridad original
}
void high_level_scheduler(int pidA, int msA, int pidB, int msB, int pidC, int msC){
    pidScheduler=getpid();
    prioridadScheduler=getprio(pidScheduler);
    while(1){
        administrarPrioridades(pidA, msA);
        administrarPrioridades(pidB, msB);
        administrarPrioridades(pidC, msC);
    }
}