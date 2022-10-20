#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Libs to check if file exists 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Add to use in Dev-C++ (bug in MingW)
#include <locale.h>

// CONTANTS
#define PATH_FILE "storage/workers.csv"
#define LENGHT_PERSONS 1000
#define MAX_NAME_LENGTH_STRING 13

#define PERSON_TYPE_WORKER 1
#define PERSON_TYPE_COSTUMER 3

int countWorkers = 0;
int countCostumers = 0;

struct Worker {
    int id;
    char name[250];
    char cpf[15];
    int is_manager;
    char gender[2];
    float salary;
    int is_deleted;
} workers[LENGHT_PERSONS];

void clearScreen() {
    #ifdef __APPLE__
        system("clear");
    #elif linux
        system("clear");
    #else // Windows
        system("cls");
    #endif
}


/**
 * @brief Return piece of string
 * Example: cutString("ABCD", 2); // => AB
 * 
 * @param stringToCut 
 * @param lengthToCut 
 * @return const char* 
 */
const char* cutString(char *stringToCut, unsigned lengthToCut) {
    char *subtext = NULL;
    subtext = (char*) malloc((lengthToCut+1)*sizeof(char));
    memcpy(subtext, &stringToCut[0], lengthToCut);
    subtext[lengthToCut] = '\0';
    return subtext;
}

/**
 * @brief GLOBAL variable workers will be persist on HardDisk in text file with pattern (.csv)
 * @global worker
 */
void saveWorkersInHardDisk() {
    struct stat st = {0};
    if (stat("storage", &st) == -1) {
        #ifdef __APPLE__
            mkdir("storage", 0700);
        #elif linux
            mkdir("storage", 0700);
        #else
            mkdir("storage");
        #endif
    }

    FILE *fpt;
    fpt = fopen(PATH_FILE, "w+");
    
    // Header
    fprintf(fpt,"id,name,cpf,gender,is_manager,salary\n");

    int i; // To Avoid C99 or C11 mode, lets make this....
    for(i = 0; i < LENGHT_PERSONS; i++) {

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
    workers[countWorkers].id = countWorkers + 1;
    strcpy(workers[countWorkers].name, workerName);
    strcpy(workers[countWorkers].cpf, workerCpf);
    workers[countWorkers].is_manager = workerIsManager;
    strcpy(workers[countWorkers].gender, workerGender);
    workers[countWorkers].salary = workerSalary;
    workers[countWorkers].is_deleted = 0;

    if(workerIsManager == PERSON_TYPE_COSTUMER) {        
        countCostumers = countCostumers + 1;
    } else {
        countWorkers = countWorkers + 1;
    }

}


/**
 * @brief Read from a file with csv format, and push in memory 
 * Example of line with a valid worker .csv => "1,A. CLAUDIA,222.333.666-01,F,0,2540.00"
 * 
 */
void readPersonsFromHardDisk() {
    FILE *file = fopen(PATH_FILE, "r");
    char buf[400];
   
    
    while(fgets(buf, sizeof(buf), file))
    {    
        struct Worker workerTemp;

        const char separator[] = ",";
        char *token;
        
        token = strtok(buf, separator);        
        // Jump line: 'id,name,cpf,gender,is_manager,salary'
        if(strcmp(token, "id") == 0) {
            continue;
        }

        workerTemp.id = atoi(token); 
        strcpy(workerTemp.name, strtok(NULL, separator));
        strcpy(workerTemp.cpf, strtok(NULL, separator));
        strcpy(workerTemp.gender, strtok(NULL, separator));
        workerTemp.is_manager = atoi(strtok(NULL, separator));
        workerTemp.salary = atof(strtok(NULL, separator));
        
        // Debug Helper        
        // printf("ID: %d\n", workerTemp.id );
        // printf("Name: %s\n", workerTemp.name );            
        // printf("CPF: %s\n", workerTemp.cpf );
        // printf("Gender: %s\n", workerTemp.gender );            
        // printf("Ismanager %i\n", workerTemp.is_manager);
        // printf("Salary: %f\n", workerTemp.salary);
        
        addWorker(workerTemp.name, workerTemp.cpf, workerTemp.is_manager, workerTemp.gender, workerTemp.salary);
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

    int i;
    for(i = 0; i < LENGHT_PERSONS; i++) {
        if(strcmp(workers[i].cpf, cpfWorkerToDelete) == 0) {
            countWorkers = countWorkers - 1;    
            workers[i].is_deleted = 1;
            return workers[i];
        }        
    }    
    return workerTemp;
}

/**
 * @brief Pass a cpf as a string to update a worker from struct Worker (Global Worker)
 * 
 * @param cpfWorkerToUpdate 
 * @param workerName 
 * @param workerCpf 
 * @param workerIsManager 
 * @param workerGender 
 * @param workerSalary 
 * @return struct Worker 
 */
struct Worker updateWorker(    
    char *cpfWorkerToUpdate,
    char workerName[250],
    char workerCpf[14],
    int workerIsManager,
    char workerGender[1],
    float workerSalary
) {
    struct Worker workerTemp;
    int i;
    for (i = 0; i < LENGHT_PERSONS; i++) {
        if(strcmp(workers[i].cpf, cpfWorkerToUpdate) == 0) {            
            workers[i].id = i + 1;
            strcpy(workers[i].name, workerName);
            strcpy(workers[i].cpf, workerCpf);
            workers[i].is_manager = workerIsManager;
            strcpy(workers[i].gender, workerGender);
            workers[i].salary = workerSalary;
            workers[i].is_deleted = 0;
            return workers[i];
        }        
    }
    return workerTemp;
}


const char* isManager(int isManager) {
    if(isManager != 0) {
        return "FUNCIONÁRIO";
    }
    return "GERENTE";
}

void displayWorker(struct Worker worker) {
    if(worker.is_deleted || worker.id == 0 || worker.is_manager == PERSON_TYPE_COSTUMER) {
        return;
    }
        
    if(strlen(worker.name) <= 5) {
        printf("| %d\t\t| %s\t\t\t\t| %s | %s\t |R$ %.2f\t|\n", worker.id, cutString(worker.name, MAX_NAME_LENGTH_STRING), worker.cpf, isManager(worker.is_manager), worker.salary);
    } else {
        printf("| %d\t\t| %s\t\t\t| %s | %s\t |R$ %.2f\t|\n", worker.id, cutString(worker.name, MAX_NAME_LENGTH_STRING), worker.cpf, isManager(worker.is_manager), worker.salary);
    }
}


void displayCostumer(struct Worker worker) {
    if(worker.is_deleted || worker.id == 0 || worker.is_manager != PERSON_TYPE_COSTUMER) {
        return;
    }

    if(strlen(worker.name) <= 5) {
        printf("| %d\t\t| %s\t\t\t\t| %s|\n", worker.id, cutString(worker.name, MAX_NAME_LENGTH_STRING), worker.cpf, isManager(worker.is_manager), worker.salary);
    } else {
        printf("| %d\t\t| %s\t\t\t| %s|\n", worker.id, cutString(worker.name, MAX_NAME_LENGTH_STRING), worker.cpf, isManager(worker.is_manager), worker.salary);
    }
}

/**
 * @brief Display all workers using struct Worker workers 
 * @global worker
 */
void displayAllWorkers() {
    printf("+---------------+-------------------------------+---------------+---------------+---------------+\n");
    printf("| MATRICULA\t| FUNCIONÁRIO(A)\t\t| CPF\t\t| FUNÇÃO\t| SALÁRIO\t|\n");
    printf("+---------------+-------------------------------+---------------+---------------+---------------+\n");
    int i;
    for (i = 0; i < LENGHT_PERSONS; i++) {
        displayWorker(workers[i]);
    }
    printf("+---------------+-------------------------------+---------------+---------------+---------------+\n");
    printf("Total de Trabalhadores: %d\n", countWorkers);
}


/**
 * @brief Display all costumers using struct Worker workers 
 * @global worker
 */
void displayAllCostumers() {
    printf("+---------------+-------------------------------+---------------+\n");
    printf("| CADASTRO\t| NOME(A)\t\t\t| CPF\t\t|\n");
    printf("+---------------+-------------------------------+---------------+\n");
    int i;
    for (i = 0; i < LENGHT_PERSONS; i++) {
        displayCostumer(workers[i]);
    }
    printf("+---------------+-------------------------------+---------------+\n");
    printf("Total de Clientes: %d\n", countCostumers);
}



/**
 * @brief Request From I/O via KeyBoard a person, can be a Costumer or a Worker
 * Example of fast input (with space strategy):
 * 
 * Worker:
 * Gabriel 222.333.666-31 0 M 4000
 * 
 * Costumer:
 * Gabriel 222.333.666-33 M
 * 
 * @return struct Worker 
 */
struct Worker requestInfoPerson(int typePerson)
{
    char aliasPerson[250];
    
    if(typePerson == PERSON_TYPE_COSTUMER) {        
        strcpy(aliasPerson, "Cliente");        
    } else {
        strcpy(aliasPerson, "Funcionário");
    }
    
    struct Worker workerTemp;
    printf("Informe o %s do %s:\n", "nome", aliasPerson);  
    // Pega a variavel com espaço, Ex: "Gabriel Darezzo"
    // fgets(workerTemp.name, 250, stdin);
    // if ((strlen(workerTemp.name) > 0) && (workerTemp.name[strlen (workerTemp.name) - 1] == '\n')) {
    //     workerTemp.name[strlen (workerTemp.name) - 1] = '\0';
    // }
    scanf("%s",workerTemp.name);
    
    printf("Informe o %s do %s:\n", "cpf", aliasPerson);    
    scanf("%s",workerTemp.cpf);


    if(typePerson == 3) {
        workerTemp.is_manager = 3;
    } else {
        printf("Informe se o %s é %s:\n", aliasPerson, "gerente (0 | 1)");
        scanf("%i",&workerTemp.is_manager);
    }

    printf("Informe o %s do %s %s:\n", "sexo", aliasPerson, "('M' | 'F')");    
    scanf("%s",workerTemp.gender);
    
    if(typePerson == 3) {        
        workerTemp.salary = 0;
    } else {
        printf("Informe o %s do %s:\n", "salário", aliasPerson);    
        scanf("%f",&workerTemp.salary);
    }

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
                workerTemp = requestInfoPerson(PERSON_TYPE_WORKER);
                addWorker(workerTemp.name, workerTemp.cpf, workerTemp.is_manager,workerTemp.gender, workerTemp.salary);
                saveWorkersInHardDisk();
                printf("O Funcionário %s (%s) foi cadastrado com sucesso! \n", workerTemp.name, workerTemp.cpf);
            break;

            //Read
            case 2:
                clearScreen();
                displayAllWorkers(PERSON_TYPE_WORKER);
            break;

            //Update 
            case 3:
                clearScreen();
                displayAllWorkers();                
                printf("Informe o cpf do Funcionário para editar\n");
                char cpfTemp[14];
                scanf("%s", cpfTemp);
                workerTemp = requestInfoPerson(PERSON_TYPE_WORKER);
                workerTemp = updateWorker(cpfTemp, workerTemp.name, workerTemp.cpf, workerTemp.is_manager ,workerTemp.gender, workerTemp.salary);
                saveWorkersInHardDisk();
                printf("O Funcionário %s (%s) foi atualizado com sucesso! \n", workerTemp.name, workerTemp.cpf);
            break;

            //Delete
            case 4:
                clearScreen();
                displayAllWorkers();                
                printf("Informe o cpf do Funcionário para exluir\n");
                scanf("%s",workerTemp.cpf);
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

void showCostumerMenu() {
    struct Worker workerTemp;
    
    while(1) {
        int choiseWorkerMenu = 0;
        printf("1 - Cadastro novo cliente\n");
        printf("2 - Listar todos os cliente\n");        
        printf("3 - Atualizar um cliente (Por CPF)\n");
        printf("4 - Excluir um cliente (Por CPF)\n");
        printf("5 - Voltar ao Menu Principal\n");
        printf("Escolha a opção acima: \n");
        scanf("%d", &choiseWorkerMenu);

        switch (choiseWorkerMenu) {
            //Create
            case 1:
                clearScreen();
                workerTemp = requestInfoPerson(PERSON_TYPE_COSTUMER);
                addWorker(workerTemp.name, workerTemp.cpf, workerTemp.is_manager,workerTemp.gender, workerTemp.salary);
                saveWorkersInHardDisk();
                printf("O Cliente %s (%s) foi cadastrado com sucesso! \n", workerTemp.name, workerTemp.cpf);
            break;

            //Read
            case 2:
                clearScreen();
                displayAllCostumers();
            break;

            //Update 
            case 3:
                // clearScreen();
                // displayAllWorkers();                
                // printf("Informe o cpf do Funcionário para editar\n");
                // char cpfTemp[14];
                // scanf("%s", cpfTemp);
                // workerTemp = requestInfoPerson();
                // workerTemp = updateWorker(cpfTemp, workerTemp.name, workerTemp.cpf, workerTemp.is_manager ,workerTemp.gender, workerTemp.salary);
                // saveWorkersInHardDisk();
                // printf("O Funcionário %s (%s) foi atualizado com sucesso! \n", workerTemp.name, workerTemp.cpf);
            break;

            //Delete
            case 4:
                // clearScreen();
                // displayAllWorkers();                
                // printf("Informe o cpf do Funcionário para exluir\n");
                // scanf("%s",workerTemp.cpf);
                // workerTemp = deleteWorker(workerTemp.cpf);
                // printf("O CPF: %s (%s) foi excluido com sucesso! \n", workerTemp.cpf, workerTemp.name);
                // saveWorkersInHardDisk();
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


void showIntroMenu() {
    struct Worker workerTemp;
    
    while(1) {
        int choiseWorkerMenu = 0;
        printf("1 - Listar menu de Funcionários\n");
        printf("2 - Listar menu do Cliente\n");                
        printf("3 - Tela de Relatório\n");
        
        printf("Escolha a opção de menu acima: (1, 2, 3)\n");
        scanf("%d", &choiseWorkerMenu);

        switch (choiseWorkerMenu) {
            case 1:
                clearScreen();
                showWorkerMenu();                
            break;

            case 2:
                clearScreen();
                showCostumerMenu();                
            break;

            case 3:
                // TODO: Falta Fazer
                return;
            break;
        
            default:
                clearScreen();
                printf("Opção errada\n");
            break;
        }
    }    
}


void showIntro() {
    printf("\\\\            //\n");
    printf(" \\\\          // \n");
    printf("  \\\\________//\n");
    printf("   |        |\n");
    printf("   |  BOX   |\n");
    printf("   |  DADOS |\n");
    printf("   |________|\n");
    printf("\n");
    printf("\n");
    printf("Seus dados, seguros!\n\n");
}

int main(int argc, char *argv[]) {

    #ifdef linux

    #else
        if(argv[1] && strcmp(argv[1], "1") == 0) {
            // call with argument, binary.exe 1
            system("chcp 65001");
        } else {
            setlocale(LC_ALL, "Portuguese");
        }
    #endif  

    
    clearScreen();    
    readPersonsFromHardDisk();
    
    //// Examples of simulate inputs manual:    
    // addWorker("A. CLAUDIA", "222.333.666-01", 0, "F", 2540);
    // addWorker("DAGOBERTO", "444.555.666-02", 1, "M", 25000);
    // addWorker("RODOLFO- UFA", "777.444.666-03", 0,"M", 850);
    // addWorker("QUE SUSTO", "666.444.666-04", 0,"M", 890);
    // addWorker("SANDRINHA", "222.333.666-38", 0,"F", 80000);
    // displayAllWorkers();
    showIntro();
    showIntroMenu();
    
    return EXIT_SUCCESS;
}