//Control se encarga de crear y matar los procesos
#include <xinu.h>
extern int galaga(void);
extern int display();
pid32 pidDisplay,pidGalaga,pidControl;
void procesoControl(){  
    pidControl=currpid;
    pidDisplay=create(display,2048,20,"proceso2",0);
    pidGalaga=create(galaga,2048,20,"proceso1",0);
    resume(pidDisplay);
    resume(pidGalaga);
    receive();
    kill(pidDisplay);
    kill(pidGalaga);
}