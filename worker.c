
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Libs to check if file exists 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// CONTANTS
#define PATH_FILE "storage/workers.csv"

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
    int is_deleted;
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
    fpt = fopen(PATH_FILE, "w+");
    
    // Need Header?
    // fprintf(fpt,"ID, Name, Email, Phone Number\n");
    for (int i = 0; i < lengthWorkers; i++) {

        if(workers[i].is_deleted) {
            continue; 
        }

        fprintf(fpt,
            "%d,%s,%s,%s,%d,%.2f\n", 
            workers[i].id,
            workers[i].name,
            workers[i].cpf,
            workers[i].gender,            
            workers[i].is_manager,
            workers[i].salary
        );        
    }    
    fclose(fpt);
}


/**
 * Add a worker in struct of workers, if success, will return int(1)
 * 
 * @param allWorkers 
 * @param index 
 * @param workerName 
 * @param workerCpf 
 * @param workerIsManager 
 * @param workerGender 
 * @param workerSalary 
 * @return int 
 */
int addWorker(struct Worker allWorkers[], int index, char workerName[250], char workerCpf[14], int workerIsManager, char workerGender[1], float workerSalary) {
    allWorkers[index].id = index + 1;
    strcpy(allWorkers[index].name, workerName);
    strcpy(allWorkers[index].cpf, workerCpf);
    allWorkers[index].is_manager = workerIsManager;
    strcpy(allWorkers[index].gender, workerGender);
    allWorkers[index].salary = workerSalary;
    allWorkers[index].is_deleted = 0;
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
    FILE *file = fopen(PATH_FILE, "r");
    char buf[400];
    int id;
    char name[250];
    char anyVar[1]; // If remove this, will break Name... C-things....
    char cpf[14];
    char anyVar2[1]; // If remove this, will break CPF... C-things....
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
    }

    return readWorker;
}

void displayWorker(struct Worker worker) {
    if(worker.is_deleted) {
        return; 
    }
    printf("|\t\t%d|\t%s\t| %s |GERENTE: %i    |R$ %.2f\t|\n", worker.id, worker.name, worker.cpf, worker.is_manager, worker.salary);
}


int deleteWorker(struct Worker workers[], int lengthWorkers, char *cpfWorkerToDelete) {
    for (int i = 0; i < lengthWorkers; i++) {
        if(strcmp(workers[i].cpf, cpfWorkerToDelete) == 0) {
            workers[i].is_deleted = 1;            
        }        
    }
    return -1;
}

int updateWorker(
    struct Worker allWorkers[],
    int lengthWorkers, 
    char *cpfWorkerToUpdate,
    char workerName[250],
    char workerCpf[14],
    int workerIsManager,
    char workerGender[1],
    float workerSalary
) {
    for (int i = 0; i < lengthWorkers; i++) {
        if(strcmp(allWorkers[i].cpf, cpfWorkerToUpdate) == 0) {
            // workers[i].is_deleted = 1;
            allWorkers[i].id = i + 1;
            strcpy(allWorkers[i].name, workerName);
            strcpy(allWorkers[i].cpf, workerCpf);
            allWorkers[i].is_manager = workerIsManager;
            strcpy(allWorkers[i].gender, workerGender);
            allWorkers[i].salary = workerSalary;
            allWorkers[i].is_deleted = 0;
            return 1;
        }        
    }
}




void displayAllWorkers(struct Worker workers[], int lengthWorkers) {
    printf("+----------------+----------------+-----+-------------------------------+---------------+\n");
    printf("|MATRICULA\t |FUNCIONÁRIO(A)\t|CPF\t\t |GERENTE\t|SALÁRIO\t|\n");
    printf("+----------------+----------------+-----+-------------------------------+---------------+\n");
    
    // TODO: Não Rolou kkkcrying :hehe
    // int lengthWorkers = sizeof(workers)/sizeof(workers[0]);      
    for (int i = 0; i < lengthWorkers; i++) {
        displayWorker(workers[i]);
    }
    printf("+---------------+-----------------------+-------------------------------+---------------+\n");
}

int main(void)
{
    fixAccentuation();
    clearScreen();

    int countWorkers = 0;
    struct Worker workers[20]; // Definimos que pode ter até 20 trabalhadores
    // countWorkers = readWorkersFromHardDisk(workers);
    
    // Massa de Dados. (Simulando Inputs manuais)
    // TODO: Pesquisar como deixa dinamico o incremento do Array, Pointer?!    
    countWorkers = countWorkers + addWorker(workers, countWorkers, "A. CLAUDIA", "222.333.666-01",0 ,"F", 2540);
    countWorkers = countWorkers + addWorker(workers, countWorkers, "DAGOBERTO", "444.555.666-02",1 ,"M", 25000);
    countWorkers = countWorkers + addWorker(workers, countWorkers, "RODOLFO- UFA", "777.444.666-03",0 ,"M", 850);
    countWorkers = countWorkers + addWorker(workers, countWorkers, "QUE SUSTO", "666.444.666-04",0 ,"M", 890);
    countWorkers = countWorkers + addWorker(workers, countWorkers, "SANDRINHA", "222.333.666-38",0 ,"F", 80000);
    

    displayAllWorkers(workers, countWorkers);
    countWorkers = countWorkers + deleteWorker(workers, 4, "222.333.666-38"); // excluir a Sandrinha    
    saveWorkersInHardDisk(workers, countWorkers);


    
    displayAllWorkers(workers, countWorkers);
    updateWorker(workers, countWorkers, "222.333.666-01" ,"CLAUDINHA", "222.333.666-01",0 ,"F", 2540);
    displayAllWorkers(workers, countWorkers);
    // Aumentinho de leve pra Claudia
    updateWorker(workers, countWorkers, "222.333.666-01" ,"CLAUDINHA", "222.333.666-01",0 ,"F", 3000);
    displayAllWorkers(workers, countWorkers);

    return 0;
}