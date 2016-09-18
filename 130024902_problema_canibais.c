/**
* UNIVERSIDADE DE BRASILIA
* Programção Concorrente 2/2016
* Douglas Shiro Yokoyama 13/0024902
* Problema dos Canibais - Estudo Dirigido 4
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_C 5
#define TRUE 1

pthread_mutex_t acordar = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t porcao = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cozinhar = PTHREAD_COND_INITIALIZER;
pthread_cond_t comida = PTHREAD_COND_INITIALIZER;

void * canibais(void * arg);
void * cozinheiro(void * arg);

int porcoes = 10;

int main(){
    pthread_t can[QTD_C];
    pthread_t coz;

    int i;
    int *id;

    for (i = 0; i < QTD_C; i++){
        id = (int*)malloc(sizeof(int));
        *id = i;
        pthread_create(&can[i], NULL, canibais, (void*) (id));
    }

    pthread_create(&coz, NULL, cozinheiro, (void*) (id));

    pthread_join(can[0], NULL);
return 0;
}

void * canibais(void * arg){
    int i = *((int*) arg);
    while(TRUE){
        pthread_mutex_lock(&acordar);
        if(porcoes < 1){
            pthread_cond_signal(&cozinhar);
            pthread_cond_wait(&comida, &acordar);
        }
        porcoes-=1;
        printf("Canibal %d pegou uma porcao\nporcao: %d\n", i, porcoes);
        sleep(1);
        pthread_mutex_unlock(&acordar);


        pthread_mutex_lock(&porcao);
        sleep(1);
        pthread_mutex_unlock(&porcao);
    }
}

void * cozinheiro(void * arg){
    while(TRUE){
        pthread_mutex_lock(&acordar);
        while(porcoes > 0){
            pthread_cond_broadcast(&comida);
            pthread_cond_wait(&cozinhar, &acordar);
        }
        pthread_mutex_unlock(&acordar);

        printf("------ COZINHEIRO ------\n");
        printf("Preparando comida ");
        sleep(5);
        porcoes = 10;
        printf("\n%d porcoes preparadas\n", porcoes);
    }
    pthread_exit(0);
}
