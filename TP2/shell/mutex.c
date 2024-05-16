#include <xinu.h>
// Estructura para el mutex
typedef struct {
    sid32 sem; // Semáforo binario para controlar el acceso al recurso
} mutex;

// Función para inicializar el mutex
void mutex_init(mutex *mutex) {
    // Inicializar el semáforo binario con un valor inicial de 1 (libre)
    mutex->sem = semcreate(1);
}

// Función para adquirir el mutex (bloqueo)
void mutex_lock(mutex *mutex) {
    // Decrementar el semáforo binario
    wait(mutex->sem);
}

// Función para liberar el mutex (desbloqueo)
void mutex_unlock(mutex *mutex) {
    // Incrementar el semáforo binario
    signal(mutex->sem);
}