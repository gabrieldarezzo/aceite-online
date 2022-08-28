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
int countWorkers = 0;
int* pCountWorkers = &countWorkers;

struct Worker {
    int id;
    char name[250];
    char cpf[14];
    int is_manager;
    char gender[1];
    float salary;
    int is_deleted;
} workers[LENGHT_WORKER];

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

/**
 * @brief GLOBAL variable workers will be persist on HardDisk in text file with pattern (.csv)
 * @global worker
 */
void saveWorkersInHardDisk() {
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
    
    // Header
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
 * @brief Add a worker in struct of [GLOBAL] workers
 * 
 * @param workerName 
 * @param workerCpf 
 * @param workerIsManager 
 * @param workerGender 
 * @param workerSalary 
 */
void addWorker(
        char workerName[250],
        char workerCpf[14],
        int workerIsManager,
        char workerGender[1],
        float workerSalary
    ) {
    workers[*pCountWorkers].id = *pCountWorkers + 1;
    strcpy(workers[*pCountWorkers].name, workerName);
    strcpy(workers[*pCountWorkers].cpf, workerCpf);
    workers[*pCountWorkers].is_manager = workerIsManager;
    strcpy(workers[*pCountWorkers].gender, workerGender);
    workers[*pCountWorkers].salary = workerSalary;
    workers[*pCountWorkers].is_deleted = 0;
    *pCountWorkers = *pCountWorkers + 1;    
}


/**
 * @brief Read from a file with csv format, and push in memory 
 * Example of line with a valid worker .csv => "1,A. CLAUDIA,222.333.666-01,F,0,2540.00"
 * 
 */
void readWorkersFromHardDisk() {
    FILE *file = fopen(PATH_FILE, "r");
    char buf[400];
    int id;
    char name[250];
    char anyVar[2]; // If remove this, will break Name... C-Stranger-things.... TODO: Tell-me Why?
    char cpf[14];
    char anyVar2[2]; // If remove this, will break CPF... C-Stranger-things.... TODO: Tell-me Why?
    int is_manager;
    char gender[1];
    float salary;
    
    while(fgets(buf, sizeof(buf), file))
    {
        const char separator[1] = ",";
        char *token;
        
        token = strtok(buf, separator);        
        // Jump line: 'id,name,cpf,gender,is_manager,salary'
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
        
        addWorker(name, cpf, is_manager, gender, salary);
    }
    return;
}

/**
 * @brief Pass a cpf as a string to delete them from struct Worker (Global Worker)
 * 
 * @global worker
 * @param cpfWorkerToDelete 
 * @return struct Worker 
 */
struct Worker deleteWorker(char *cpfWorkerToDelete) {
    struct Worker workerTemp;

    for (int i = 0; i < LENGHT_WORKER; i++) {
        if(strcmp(workers[i].cpf, cpfWorkerToDelete) == 0) {
            workers[i].is_deleted = 1;
            return workers[i];
        }        
    }    
    return workerTemp;
}

/**
 * @brief Pass a cpf as a string to update a worker from struct Worker (Global Worker)
 * 
 * @global worker
 * @param allWorkers 
 * @param cpfWorkerToUpdate 
 * @param workerName 
 * @param workerCpf 
 * @param workerIsManager 
 * @param workerGender 
 * @param workerSalary 
 * @return struct Worker 
 */
struct Worker updateWorker(
    struct Worker allWorkers[],    
    char *cpfWorkerToUpdate,
    char workerName[250],
    char workerCpf[14],
    int workerIsManager,
    char workerGender[1],
    float workerSalary
) {
    struct Worker workerTemp;
    for (int i = 0; i < LENGHT_WORKER; i++) {
        if(strcmp(allWorkers[i].cpf, cpfWorkerToUpdate) == 0) {            
            allWorkers[i].id = i + 1;
            strcpy(allWorkers[i].name, workerName);
            strcpy(allWorkers[i].cpf, workerCpf);
            allWorkers[i].is_manager = workerIsManager;
            strcpy(allWorkers[i].gender, workerGender);
            allWorkers[i].salary = workerSalary;
            allWorkers[i].is_deleted = 0;
            return allWorkers[i];
        }        
    }
    return workerTemp;
}

int displayWorker(struct Worker worker) {
    if(worker.is_deleted || worker.id == 0) {
        return 0;
    }
    printf("|\t\t%d|\t%s\t| %s |GERENTE: %i    |R$ %.2f\t|\n", worker.id, worker.name, worker.cpf, worker.is_manager, worker.salary);
    return 1;
}

/**
 * @brief Display all workers using struct Worker workers 
 * @global worker
 */
void displayAllWorkers() {
    printf("+----------------+----------------+-----+----------------+--------------+---------------+\n");
    printf("|MATRICULA\t |FUNCIONÁRIO(A)\t|CPF\t\t |GERENTE\t|SALÁRIO\t|\n");
    printf("+----------------+----------------+-----+----------------+--------------+---------------+\n");
    
    int validWorkers = 0;
    for (int i = 0; i < LENGHT_WORKER; i++) {
        validWorkers+= displayWorker(workers[i]);
    }
    printf("+---------------+-----------------------+----------------+--------------+---------------+\n");
    printf("Total de Trabalhadores: %d\n", validWorkers);
}



/**
 * @brief Request I/O From KeyBoard from User
 * Example of fast input (with space strategy):
 * Gabriel 222.333.666-31 0 M 4000 
 * 
 * @return struct Worker 
 */
struct Worker requestInfoWorker()
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

    workerTemp.id = 1;
    workerTemp.is_deleted = 0;
    
    return workerTemp;
}

void showWorkerMenu() {
    struct Worker workerTemp;
    
    while(1) {
        int choiseWorkerMenu = 0;
        printf("1 - Cadastro novo funcionário\n");
        printf("2 - Listar todos os funcionários\n");        
        printf("3 - Atualizar um funcionário (Por CPF)\n");
        printf("4 - Excluir um funcionários (Por CPF)\n");
        printf("5 - Voltar ao Menu Principal\n");
        printf("Escolha a opção acima: \n");
        scanf("%d", &choiseWorkerMenu);

        switch (choiseWorkerMenu) {
            //Create
            case 1:
                clearScreen();
                workerTemp = requestInfoWorker();
                addWorker(workerTemp.name, workerTemp.cpf, workerTemp.is_manager,workerTemp.gender, workerTemp.salary);
                saveWorkersInHardDisk();
                printf("O Funcionário %s (%s) foi cadastrado com sucesso! \n", workerTemp.name, workerTemp.cpf);
            break;

            //Read
            case 2:
                clearScreen();
                displayAllWorkers();
            break;

            //Update 
            case 3:
                clearScreen();
                displayAllWorkers();                
                printf("Informe o cpf do Funcionário para editar\n");
                char cpfTemp[14];
                scanf("%s", &cpfTemp);
                workerTemp = requestInfoWorker();
                workerTemp = updateWorker(workers, cpfTemp, workerTemp.name, workerTemp.cpf, workerTemp.is_manager ,workerTemp.gender, workerTemp.salary);
                printf("O Funcionário %s (%s) foi atualizado com sucesso! \n", workerTemp.name, workerTemp.cpf);
            break;

            //Delete
            case 4:
                clearScreen();
                displayAllWorkers();                
                printf("Informe o cpf do Funcionário para exluir\n", "cpf");
                scanf("%s",&workerTemp.cpf);
                workerTemp = deleteWorker(workerTemp.cpf);
                printf("O CPF: %s (%s) foi excluido com sucesso! \n", workerTemp.cpf, workerTemp.name);
                saveWorkersInHardDisk();
            break;

            case 5:
                clearScreen();
                printf("Voltando ao Menu Principal\n");
                return;
            break;
        
            default:
                clearScreen();
                printf("Opção errada\n");
            break;
        }
    }    
}

int main(void)
{
    fixAccentuation();
    clearScreen();
    readWorkersFromHardDisk();
    
    //// Examples of simulate inputs manual:    
    // addWorker("A. CLAUDIA", "222.333.666-01", 0, "F", 2540);
    // addWorker("DAGOBERTO", "444.555.666-02", 1, "M", 25000);
    // addWorker("RODOLFO- UFA", "777.444.666-03", 0,"M", 850);
    // addWorker("QUE SUSTO", "666.444.666-04", 0,"M", 890);
    // addWorker("SANDRINHA", "222.333.666-38", 0,"F", 80000);
    // displayAllWorkers();
    showWorkerMenu();
    return 0;
}