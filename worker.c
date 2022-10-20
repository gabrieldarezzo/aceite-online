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
#define PATH_FILE "storage/persons.csv"
#define LENGHT_PERSONS 1000
#define MAX_NAME_LENGTH_STRING 13

#define PERSON_TYPE_WORKER 1
#define PERSON_TYPE_COSTUMER 3

int countPersons = 0;
int countWorkers = 0;
int countCostumers = 0;

struct Person {
    int id;
    char name[250];
    char cpf[15];
    int person_type;
    char gender[2];
    float salary;
    int is_deleted;
} persons[LENGHT_PERSONS];

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
 * @brief GLOBAL variable persons will be persist on HardDisk in text file with pattern (.csv)
 * @global persons
 */
void savePersonsInHardDisk() {
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
    fprintf(fpt,"id,name,cpf,gender,person_type,salary\n");

    int i; // To Avoid C99 or C11 mode, lets make this....
    for(i = 0; i < LENGHT_PERSONS; i++) {

        if(persons[i].is_deleted || persons[i].id == 0) {
            continue; 
        }

        fprintf(fpt,
            "%d,%s,%s,%s,%d,%.2f\n", 
            persons[i].id,
            persons[i].name,
            persons[i].cpf,
            persons[i].gender,            
            persons[i].person_type,
            persons[i].salary
        );        
    }    
    fclose(fpt);
}



/**
 * @brief Add a person in struct of [GLOBAL] persons
 * 
 * @param personName 
 * @param personCpf 
 * @param personType 
 * @param personGender 
 * @param personSalary 
 */
void addPerson(
        char personName[250],
        char personCpf[14],
        int personType,
        char personGender[1],
        float personSalary
    ) {
    persons[countPersons].id = countPersons + 1;
    strcpy(persons[countPersons].name, personName);
    strcpy(persons[countPersons].cpf, personCpf);
    persons[countPersons].person_type = personType;
    strcpy(persons[countPersons].gender, personGender);
    persons[countPersons].salary = personSalary;
    persons[countPersons].is_deleted = 0;

    if(personType == PERSON_TYPE_COSTUMER) {        
        countCostumers = countCostumers + 1;
    } else {
        countWorkers = countWorkers + 1;
    }
    countPersons = countPersons + 1;

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
        struct Person personTemp;

        const char separator[] = ",";
        char *token;
        
        token = strtok(buf, separator);        
        // Jump line: 'id,name,cpf,gender,person_type,salary'
        if(strcmp(token, "id") == 0) {
            continue;
        }

        personTemp.id = atoi(token); 
        strcpy(personTemp.name, strtok(NULL, separator));
        strcpy(personTemp.cpf, strtok(NULL, separator));
        strcpy(personTemp.gender, strtok(NULL, separator));
        personTemp.person_type = atoi(strtok(NULL, separator));
        personTemp.salary = atof(strtok(NULL, separator));
        
        // Debug Helper        
        // printf("ID: %d\n", personTemp.id );
        // printf("Name: %s\n", personTemp.name );            
        // printf("CPF: %s\n", personTemp.cpf );
        // printf("Gender: %s\n", personTemp.gender );            
        // printf("Ismanager %i\n", personTemp.person_type);
        // printf("Salary: %f\n", personTemp.salary);
        
        addPerson(personTemp.name, personTemp.cpf, personTemp.person_type, personTemp.gender, personTemp.salary);
    }
    return;
}

/**
 * @brief Pass a cpf as a string to delete them from struct Person (Global Person)
 * 
 * @global persons
 * @param cpfPersonToDelete 
 * @return struct Person 
 */
struct Person deletePerson(char *cpfPersonToDelete) {
    struct Person personTemp;

    int i;
    for(i = 0; i < LENGHT_PERSONS; i++) {
        if(strcmp(persons[i].cpf, cpfPersonToDelete) == 0) {

            if(persons[i].person_type == PERSON_TYPE_COSTUMER) {                
                countCostumers = countCostumers - 1;
            } else {
                countWorkers = countWorkers - 1;
            }
            countPersons = countPersons - 1;
            
            persons[i].is_deleted = 1;
            return persons[i];
        }        
    }    
    return personTemp;
}

/**
 * @brief Pass a cpf as a string to update a person from struct Person (Global Person)
 * 
 * @param cpfPersonToUpdate 
 * @param personName 
 * @param workerCpf 
 * @param personType 
 * @param personGender 
 * @param personSalary 
 * @return struct Person 
 */
struct Person updateWorker(    
    char *cpfPersonToUpdate,
    char personName[250],
    char personCpf[14],
    int personType,
    char personGender[1],
    float personSalary
) {
    struct Person personTemp;
    int i;
    for (i = 0; i < LENGHT_PERSONS; i++) {
        if(strcmp(persons[i].cpf, cpfPersonToUpdate) == 0) {            
            persons[i].id = i + 1;
            strcpy(persons[i].name, personName);
            strcpy(persons[i].cpf, personCpf);
            persons[i].person_type = personType;
            strcpy(persons[i].gender, personGender);
            persons[i].salary = personSalary;
            persons[i].is_deleted = 0;
            return persons[i];
        }        
    }
    return personTemp;
}


const char* isManager(int personType) {
    if(personType == 1) {
        return "GERENTE";        
    }
    return "FUNCIONÁRIO";
}

void displayWorker(struct Person person) {
    if(person.is_deleted || person.id == 0 || person.person_type == PERSON_TYPE_COSTUMER) {
        return;
    }
        
    if(strlen(person.name) <= 5) {
        printf("| %d\t\t| %s\t\t\t\t| %s | %s\t |R$ %.2f\t|\n", person.id, cutString(person.name, MAX_NAME_LENGTH_STRING), person.cpf, isManager(person.person_type), person.salary);
    } else {
        printf("| %d\t\t| %s\t\t\t| %s | %s\t |R$ %.2f\t|\n", person.id, cutString(person.name, MAX_NAME_LENGTH_STRING), person.cpf, isManager(person.person_type), person.salary);
    }
}


void displayCostumer(struct Person person) {
    if(person.is_deleted || person.id == 0 || person.person_type != PERSON_TYPE_COSTUMER) {
        return;
    }

    if(strlen(person.name) <= 5) {
        printf("| %d\t\t| %s\t\t\t\t| %s|\n", person.id, cutString(person.name, MAX_NAME_LENGTH_STRING), person.cpf, isManager(person.person_type), person.salary);
    } else {
        printf("| %d\t\t| %s\t\t\t| %s|\n", person.id, cutString(person.name, MAX_NAME_LENGTH_STRING), person.cpf, isManager(person.person_type), person.salary);
    }
}

/**
 * @brief Display all workers using struct Person persons 
 * @global persons
 */
void displayAllWorkers() {
    printf("+---------------+-------------------------------+---------------+---------------+---------------+\n");
    printf("| MATRICULA\t| FUNCIONÁRIO(A)\t\t| CPF\t\t| FUNÇÃO\t| SALÁRIO\t|\n");
    printf("+---------------+-------------------------------+---------------+---------------+---------------+\n");
    int i;
    for (i = 0; i < LENGHT_PERSONS; i++) {
        displayWorker(persons[i]);
    }
    printf("+---------------+-------------------------------+---------------+---------------+---------------+\n");
    printf("Total de Trabalhadores: %d\n", countWorkers);
}


/**
 * @brief Display all costumers using struct Person persons 
 * @global persons
 */
void displayAllCostumers() {
    printf("+---------------+-------------------------------+---------------+\n");
    printf("| CADASTRO\t| NOME(A)\t\t\t| CPF\t\t|\n");
    printf("+---------------+-------------------------------+---------------+\n");
    int i;
    for (i = 0; i < LENGHT_PERSONS; i++) {
        displayCostumer(persons[i]);
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
 * @return struct Person 
 */
struct Person requestInfoPerson(int typePerson)
{
    char aliasPerson[250];
    
    if(typePerson == PERSON_TYPE_COSTUMER) {        
        strcpy(aliasPerson, "Cliente");        
    } else {
        strcpy(aliasPerson, "Funcionário");
    }
    
    struct Person personTemp;
    printf("Informe o %s do %s:\n", "nome", aliasPerson);  
    // Pega a variavel com espaço, Ex: "Gabriel Darezzo"
    // fgets(personTemp.name, 250, stdin);
    // if ((strlen(personTemp.name) > 0) && (personTemp.name[strlen (personTemp.name) - 1] == '\n')) {
    //     personTemp.name[strlen (personTemp.name) - 1] = '\0';
    // }
    scanf("%s",personTemp.name);
    
    printf("Informe o cpf do %s (%s) com pontuação do (Ex: 222.333.666-33):\n", aliasPerson, personTemp.name);
    scanf("%s",personTemp.cpf);


    if(typePerson == PERSON_TYPE_COSTUMER) {
        personTemp.person_type = PERSON_TYPE_COSTUMER;
    } else {
        printf("Informe se '%s' é GERENTE, (Ex: '1' = GERENTE | '0' = FUNCIONÁRIO):\n", personTemp.name);
        scanf("%i",&personTemp.person_type);
    }

    printf("Informe o %s: (%s) do %s (Ex: 'M' = Masculino | 'F' = Feminino):\n", "sexo", aliasPerson, personTemp.name);    
    scanf("%s",personTemp.gender);
    
    if(typePerson == PERSON_TYPE_COSTUMER) {        
        personTemp.salary = 0;
    } else {
        printf("Informe o salário do (%s)  (Apenas os números, Ex: Para informar 'R$ 1.280,78' preencha dessa forma: '1280.78'):\n", personTemp.name);    
        scanf("%f",&personTemp.salary);
    }

    personTemp.id = 1;
    personTemp.is_deleted = 0;
    
    return personTemp;
}

void showWorkerMenu() {
    struct Person personTemp;
    
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
                personTemp = requestInfoPerson(PERSON_TYPE_WORKER);
                addPerson(personTemp.name, personTemp.cpf, personTemp.person_type,personTemp.gender, personTemp.salary);
                savePersonsInHardDisk();
                printf("O Funcionário %s (%s) foi cadastrado com sucesso! \n", personTemp.name, personTemp.cpf);
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
                scanf("%s", cpfTemp);
                personTemp = requestInfoPerson(PERSON_TYPE_WORKER);
                personTemp = updateWorker(cpfTemp, personTemp.name, personTemp.cpf, personTemp.person_type ,personTemp.gender, personTemp.salary);
                savePersonsInHardDisk();
                printf("O Funcionário %s (%s) foi atualizado com sucesso! \n", personTemp.name, personTemp.cpf);
            break;

            //Delete
            case 4:
                clearScreen();
                displayAllWorkers();                
                printf("Informe o cpf do Funcionário para exluir\n");
                scanf("%s",personTemp.cpf);
                personTemp = deletePerson(personTemp.cpf);
                printf("O CPF: %s (%s) foi excluido com sucesso! \n", personTemp.cpf, personTemp.name);
                savePersonsInHardDisk();
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
    struct Person personTemp;
    
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
                personTemp = requestInfoPerson(PERSON_TYPE_COSTUMER);
                addPerson(personTemp.name, personTemp.cpf, personTemp.person_type,personTemp.gender, personTemp.salary);
                savePersonsInHardDisk();
                printf("O Cliente %s (%s) foi cadastrado com sucesso! \n", personTemp.name, personTemp.cpf);
            break;

            //Read
            case 2:
                clearScreen();
                displayAllCostumers();
            break;

            //Update 
            case 3:
                clearScreen();
                displayAllCostumers();                
                printf("Informe o cpf do Cliente para editar\n");
                char cpfTemp[14];
                scanf("%s", cpfTemp);
                personTemp = requestInfoPerson(PERSON_TYPE_COSTUMER);
                personTemp = updateWorker(cpfTemp, personTemp.name, personTemp.cpf, personTemp.person_type ,personTemp.gender, personTemp.salary);
                savePersonsInHardDisk();
                printf("O Cliente %s (%s) foi atualizado com sucesso! \n", personTemp.name, personTemp.cpf);
            break;

            //Delete
            case 4:
                clearScreen();
                displayAllCostumers();                
                printf("Informe o cpf do Cliente para exluir\n");
                scanf("%s",personTemp.cpf);
                personTemp = deletePerson(personTemp.cpf);
                printf("Cliente CPF: %s (%s) foi excluido com sucesso! \n", personTemp.cpf, personTemp.name);
                savePersonsInHardDisk();
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
    while(1) {
        int choiseWorkerMenu = 0;
        printf("1 - Listar menu de Funcionários\n");
        printf("2 - Listar menu do Cliente\n");                
        printf("3 - Exibir Relatório\n");
        
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
                // TODO: Implement sum of salary?
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
            system("chcp 65001");
            // setlocale(LC_ALL, "Portuguese");
        }
    #endif  

    
    clearScreen();    
    readPersonsFromHardDisk();
    showIntro();
    showIntroMenu();
    
    return EXIT_SUCCESS;
}