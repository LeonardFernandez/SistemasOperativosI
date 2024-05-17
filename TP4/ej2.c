#include <stdio.h>
#include <semaphore.h>
sem_t sem;
void main(){
    
    sem_init(&sem,1,0);
    int pid=fork();
    if(pid==0){//Ejecuta hijo1
        printf("Proceso uno libera un permiso\n");
        sleep(2);
        sem_post(&sem); //Libero un permiso
    }else{ //Ejecuta padre
        int pid2=fork();
        if(pid2==0){//Ejecuta hijo2
            printf("Proceso dos libera un permiso\n");
            sleep(2);
            sem_post(&sem); //Libero un permiso
        }else{
            int pid3=fork();
            if(pid3==0){
                printf("Proceso tres libera un permiso\n");
                sleep(2);
                sem_post(&sem); //Libero un permiso
            }else{ //Ejecuta padre
                //Se toman 3 permisos
                sem_wait(&sem);
                //sem_wait(&sem);
                //sem_wait(&sem);
                printf("Ejecuta proceso padre");
            }
        }
    }
}