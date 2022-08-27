
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Libs to check if file exists 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>






/*
Malloc:
https://www.programiz.com/c-programming/c-structures-pointers

*/

void clearScreen() {
    if (system("clear")) {
        system("cls");
    }
}

void fixAccentuation() {
    #ifdef linux
        //clear
    #else
        system("chcp 65001");
    #endif    
}

struct Worker {
    int id;
    char name[250];
    char cpf[14];
    int is_manager;
    char gender[1];
    float salary;
};


void saveWorkersInHardDisk(struct Worker workers[], int lengthWorkers) {
      struct stat st = {0};
    if (stat("storage", &st) == -1) {
        #ifdef linux
           mkdir("storage", 0700);
        #else
            mkdir("storage");
        #endif   
    }

    FILE *fpt;
    fpt = fopen("storage/workers.csv", "w+");
    
    // Need Header?
    // fprintf(fpt,"ID, Name, Email, Phone Number\n");
    for (int i = 0; i < lengthWorkers; i++) {
        fprintf(fpt,
            "%d,%s,%s,%s,%d,%.2f\n", 
            workers[i].id,
            workers[i].name,
            workers[i].cpf,
            workers[i].gender,            
            workers[i].is_manager,
            workers[i].salary
        );
        printf("%s\n", workers[i].cpf);
    }

    
    fclose(fpt);
}


/**
 * Add a worker in struct of workers, if success, will return int(1)
 * 
 * @param allWorkers 
 * @param index 
 * @param name 
 * @param cpf 
 * @param gender 
 * @param salary 
 * @param is_manager 
 * @return int 
 */
int addWorker(struct Worker allWorkers[], int index, char name[250], char cpf[14], int is_manager, char gender[1], float salary) {
    allWorkers[index].id = index + 1;
    strcpy(allWorkers[index].name, name);
    strcpy(allWorkers[index].cpf, cpf);
    allWorkers[index].is_manager = is_manager;
    strcpy(allWorkers[index].gender, gender);
    allWorkers[index].salary = salary;
    

    return 1;
}


/**
 * Read from a file with csv format, and put in memory
 * Example of .csv => "1,A. CLAUDIA,222.333.666-38,F,0,2540.00"
 * 
 * @param workers 
 * @return int 
 */
int readWorkersFromHardDisk(struct Worker workers[]) {
    FILE *file = fopen("storage/workers.csv", "r");
    char buf[400];
    int id;
    char name[250];
    char aaa[250];
    char cpf[14];
    int is_manager;
    char gender[1];
    float salary;
    const char fields[6] = ";";
    int readWorker = 0;
    while(fgets(buf, sizeof(buf), file))
    {
        const char separator[1] = ",";
        char *token;
        
        token = strtok(buf, separator);
        id = (int) token;

        token = strtok(NULL, separator);        
        strcpy(name, token);
        
        token = strtok(NULL, separator);        
        strcpy(cpf, token);

        token = strtok(NULL, separator);
        strcpy(gender, token);

        token = strtok(NULL, separator);
        is_manager = atoi(token);

        token = strtok(NULL, separator);
        salary = atof(token);

        // // Debug Helper
        // printf("ID: %s\n", id );
        // printf("Name: %s\n", name );            
        // printf("CPF: %s\n", cpf );
        // printf("Gender: %s\n", gender );            
        // printf("Ismanager %i\n", is_manager);
        // printf("Salary: %f\n", salary);

        readWorker = readWorker + addWorker(workers, readWorker, name, cpf, is_manager, gender, salary);
        return readWorker;
    }
}


void displayWorker(struct Worker worker) {    
    printf("| %d\t\t| %s\t\t| R$ %.2f \t\t|\n", worker.id, worker.name, worker.salary);
}

void displayAllWorkers(struct Worker workers[], int lengthWorkers) {
    printf("+---------------+-----------------------+-----------------------+\n");
    printf("| MATRICULA\t| FUNCIONÁRIO(A)\t| SALÁRIO \t\t|\n");
    

    // TODO: Não Rolou kkkcrying :hehe
    // int lengthWorkers = sizeof(workers)/sizeof(workers[0]);      
    for (int i = 0; i < lengthWorkers; i++) {
        displayWorker(workers[i]);
    }
    printf("+---------------+-----------------------+-----------------------+\n");
}

int main(void)
{
    fixAccentuation();
    clearScreen();

    int countWorkers = 0;
    struct Worker workers[20]; // Definimos que pode ter até 20 trabalhadores


    // TODO: Pesquisar como deixa dinamico o incremento do Array, Pointer?!
    
    //countWorkers = countWorkers + addWorker(workers, countWorkers, "A. CLAUDIA", "222.333.666-38",0 ,"F", 2540);
    //countWorkers = countWorkers + addWorker(workers, countWorkers, "DAGOBERTO", "444.555.666-40",1 ,"M", 25000);
    //countWorkers = countWorkers + addWorker(workers, countWorkers, "RODOLFO- UFA", "777.444.666-38",0 ,"M", 850);
    //countWorkers = countWorkers + addWorker(workers, countWorkers, "QUE SUSTO", "666.444.666-38",0 ,"M", 890);
    // saveWorkersInHardDisk(workers, countWorkers);


    countWorkers = readWorkersFromHardDisk(workers);




    displayAllWorkers(workers, countWorkers);
    return 0;
}

