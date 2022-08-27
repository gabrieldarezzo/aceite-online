
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// CONTANTS
#define LENGHT_WORKER 20

struct Worker {
    int id;
    char name[250];    
} workers[LENGHT_WORKER];

void clearArrStructWorkers(struct Worker *workers) {
    for(int i = 0; i < LENGHT_WORKER; i++) {
        workers[i].id = 0;
        strcpy(workers[i].name, "");        
    }
}

void infoOfStruct(struct Worker *workers) {
    printf("Tamanho da Struct Total Worker: %i\n", sizeof(workers)); // Will Returna 4960
    printf("Tamanho de 1 Struct Worker: %i\n", sizeof(workers[0])); // Will Return 248    
    printf("Total de Workers: %i\n",  sizeof(workers) / sizeof(workers[0])); // Will Return 20 (4960 / 248 = 20)
}

void addWorker(struct Worker *workers) {
    workers[2].id = 3;
    strcpy(workers[2].name, "Roberta");
}

/**
 * @brief Will filter only structs already set (different of zero); an will print
 * 
 * @param workers 
 */
void displayAllNames(struct Worker *workers) {
    for(int i = 0; i < LENGHT_WORKER; i++) {
        if(workers[i].id != 0) {
            printf("Nome: %s\n", workers[i].name);
        }        
    }
}

int main(void)
{
    //clearArrStructWorkers(workers);

    workers[0].id = 1;
    strcpy(workers[0].name, "Gabriel");
    

    workers[1].id = 2;
    strcpy(workers[1].name, "Robert");
    
    addWorker(workers);

    workers[3].id = 4;
    strcpy(workers[3].name, "Rod");

    infoOfStruct(workers);

    printf("===============\nOutside of Function\n===============\n", sizeof(workers[0]));
    printf("Tamanho de 1 Struct Worker: %i\n", sizeof(workers[0])); // Will Return 248
    printf("Tamanho da Struct Total Worker: %i\n", sizeof(workers)); // Will Returna 4 de onde veio esse 4?
    printf("Total de Workers: %i\n",  sizeof(workers) / sizeof(workers[0])); // Will Return 0

    displayAllNames(workers);
    return 0;
}

