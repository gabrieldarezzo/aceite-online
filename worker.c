
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
#define LENGHT_WORKER 50

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
} workers[LENGHT_WORKER];

void saveWorkersInHardDisk(struct Worker workers[]) {
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
    fprintf(fpt,"id,name,cpf,gender,is_manager,salary\n");
    for (int i = 0; i < LENGHT_WORKER; i++) {

        if(workers[i].is_deleted || workers[i].id == 0) {
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
int addWorker(struct Worker allWorkers[], int* countWorkers, char workerName[250], char workerCpf[14], int workerIsManager, char workerGender[1], float workerSalary) {
    allWorkers[*countWorkers].id = *countWorkers + 1;
    strcpy(allWorkers[*countWorkers].name, workerName);
    strcpy(allWorkers[*countWorkers].cpf, workerCpf);
    allWorkers[*countWorkers].is_manager = workerIsManager;
    strcpy(allWorkers[*countWorkers].gender, workerGender);
    allWorkers[*countWorkers].salary = workerSalary;
    allWorkers[*countWorkers].is_deleted = 0;

    *countWorkers = *countWorkers + 1;
    return 1;
}


/**
 * Read from a file with csv format, and put in memory
 * Example of .csv => "1,A. CLAUDIA,222.333.666-38,F,0,2540.00"
 * 
 * @param workers 
 * @return int 
 */
void readWorkersFromHardDisk(struct Worker workers[], int* pCountWorkers) {
    FILE *file = fopen(PATH_FILE, "r");
    char buf[400];
    int id;
    char name[250];
    char anyVar[2]; // If remove this, will break Name... C-things....
    char cpf[14];
    char anyVar2[2]; // If remove this, will break CPF... C-things....
    int is_manager;
    char gender[1];
    float salary;
    const char fields[6] = ";";
    
    while(fgets(buf, sizeof(buf), file))
    {
        const char separator[1] = ",";
        char *token;
        
        token = strtok(buf, separator);        
        // Pula iteração do header 'id,name,cpf,gender,is_manager,salary'
        if(strcmp(token, "id") == 0) {
            continue;
        }
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
        
        addWorker(workers, pCountWorkers, name, cpf, is_manager, gender, salary);
    }

    return;
}

int displayWorker(struct Worker worker) {
    if(worker.is_deleted || worker.id == 0) {
        return 0;
    }
    printf("|\t\t%d|\t%s\t| %s |GERENTE: %i    |R$ %.2f\t|\n", worker.id, worker.name, worker.cpf, worker.is_manager, worker.salary);
    return 1;
}

int deleteWorker(struct Worker workers[], char *cpfWorkerToDelete) {
    for (int i = 0; i < LENGHT_WORKER; i++) {
        if(strcmp(workers[i].cpf, cpfWorkerToDelete) == 0) {
            workers[i].is_deleted = 1;            
        }        
    }
    return -1;
}

int updateWorker(
    struct Worker allWorkers[],    
    char *cpfWorkerToUpdate,
    char workerName[250],
    char workerCpf[14],
    int workerIsManager,
    char workerGender[1],
    float workerSalary
) {
    for (int i = 0; i < LENGHT_WORKER; i++) {
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

void displayAllWorkers(struct Worker workers[]) {
    printf("+----------------+----------------+-----+----------------+--------------+---------------+\n");
    printf("|MATRICULA\t |FUNCIONÁRIO(A)\t|CPF\t\t |GERENTE\t|SALÁRIO\t|\n");
    printf("+----------------+----------------+-----+----------------+--------------+---------------+\n");
    
    // TODO: Não Rolou kkkcrying :hehe
    // int lengthWorkers = sizeof(workers)/sizeof(workers[0]);      

    int validWorkers = 0;
    for (int i = 0; i < LENGHT_WORKER; i++) {
        validWorkers+= displayWorker(workers[i]);
    }
    printf("+---------------+-----------------------+----------------+--------------+---------------+\n");
    printf("Total de Trabalhadores: %d\n", validWorkers);
}


/**
 * @brief 
 * Example:
 * Gabriel 222.333.666-31 0 M 4000 
 * 
 * @param pCountWorkers 
 */
void requestAndSaveNewWorker(int* pCountWorkers)
{
    struct Worker workerTemp;

    printf("Informe o %s do %s\n", "nome", "Funcionário:");  
    // Pega a variavel com espaço, Ex: "Gabriel Darezzo"
    // fgets(workerTemp.name, 250, stdin);
    // if ((strlen(workerTemp.name) > 0) && (workerTemp.name[strlen (workerTemp.name) - 1] == '\n')) {
    //     workerTemp.name[strlen (workerTemp.name) - 1] = '\0';
    // }
    scanf("%s",&workerTemp.name);
    
    printf("Informe o %s do %s\n", "cpf", "Funcionário:");    
    scanf("%s",&workerTemp.cpf);

    printf("Informe se o %s é %s\n", "Funcionário", "gerente (0 | 1) :");
    scanf("%i",&workerTemp.is_manager);

    printf("Informe o %s do %s\n", "sexo", "Funcionário ('M' | 'F'):");    
    scanf("%s",&workerTemp.gender);

    printf("Informe o %s do %s\n", "salário", "Funcionário:");    
    scanf("%f",&workerTemp.salary);

    // Indiferente
    // workerTemp.id = 1;
    // workerTemp.is_deleted = 0;

    addWorker(workers, pCountWorkers, workerTemp.name, workerTemp.cpf, workerTemp.is_manager,workerTemp.gender, workerTemp.salary);
    saveWorkersInHardDisk(workers);
}

int main(void)
{
    fixAccentuation();
    clearScreen();

    int countWorkers = 0;
    int* pCountWorkers = &countWorkers;


    readWorkersFromHardDisk(workers, pCountWorkers);
    
    // Massa de Dados. (Simulando Inputs manuais)    
    // addWorker(workers, pCountWorkers, "A. CLAUDIA", "222.333.666-01", 0, "F", 2540);
    // addWorker(workers, pCountWorkers, "DAGOBERTO", "444.555.666-02", 1, "M", 25000);
    // addWorker(workers, pCountWorkers, "RODOLFO- UFA", "777.444.666-03", 0,"M", 850);
    // addWorker(workers, pCountWorkers, "QUE SUSTO", "666.444.666-04", 0,"M", 890);
    // addWorker(workers, pCountWorkers, "SANDRINHA", "222.333.666-38", 0,"F", 80000);
    
    //displayAllWorkers(workers);

    // deleteWorker(workers, "222.333.666-38"); // Excluir a Sandrinha

    // Persiste em Disco (só de zoas)
    // saveWorkersInHardDisk(workers);
    
    // Aumentinho de leve pra Claudia
    // updateWorker(workers, "222.333.666-01" ,"CLAUDINHA", "222.333.666-01",0 ,"F", 3000);

    // Exibe de novo essa budega
    // displayAllWorkers(workers);

    // Persiste em Disco (só de novo)
    // saveWorkersInHardDisk(workers);


    
    //requestAndSaveNewWorker(pCountWorkers);
    displayAllWorkers(workers);


    return 0;
}