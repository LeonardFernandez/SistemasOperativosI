#include "mutex.h"

/* Función para inicializar el mutex */
void mutex_init(struct mutex *m) {
    m->pid = -1;     // Inicializar el PID del propietario como -1 (sin propietario)
    m->sem = semcreate(1); // Crear un semáforo binario asociado al mutex
}

/* Función para adquirir el mutex (lock) */
void mutex_lock(struct mutex *m) {
    wait(m->sem);                   // Esperar a que el semáforo esté disponible (valor 1)
    m->pid = getpid();        // Establecer el PID del proceso actual como el propietario
}

/* Función para liberar el mutex (unlock) */
void mutex_unlock(struct mutex *m) {
    if (m->pid == getpid()) {  // Verificar que el proceso actual sea el propietario
        m->pid = -1;           // Restablecer el PID del propietario a -1 (sin propietario)
        signal(m->sem);              // Incrementar el semáforo binario (valor 1)
    } else {
        // Mensaje de error: intento de desbloqueo por un proceso no propietario
        printf("Error: intento de desbloqueo por un proceso no propietario\n");
    }
}