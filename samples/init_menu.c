
#include <stdio.h>
#include <stdlib.h>

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



void showCostumerMenu() {
     while(1) {
        int choiseCostumerMenu = 0;
        
        printf("1 - Cadastro novo cliente\n");
        printf("2 - Listar Todos os Clientes\n");
        printf("3 - Listar um cliente (Por CPF)\n");
        printf("4 - Atualizar um Cliente (Por CPF)\n");
        printf("4 - Excluir um Cliente (Por CPF)\n");
        printf("5 - Voltar ao Menu Principal\n");
        printf("Escolha a opção acima: \n");
        scanf("%d", &choiseCostumerMenu);

        switch (choiseCostumerMenu) {
            case 1:
                clearScreen();
                printf("!!!Cadastrandro um cliente!!!\n");
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
    
    while(1) {
        int choise = 0;
        
        printf("1 - Para cadastro de Clientes\n");
        printf("2 - Para cadastro de Perfil\n");
        printf("3 - Para cadastro de Funcionário\n");
        printf("4 - Relatórios em TXT\n");
        printf("5 - Encerrar o programa\n");
        printf("Escolha a opção acima: \n");
        scanf("%d", &choise);

        switch (choise) {
            case 1:
                clearScreen();
                showCostumerMenu();
            break;

            case 2:                
                //showProfileMenu();
            break;

            case 3:
                //showProfileMenu();
            break;

            case 4:
                //showReportMenu();
            break;

            case 5:
                printf("Thau thau (͡ ° ͜ʖ ͡ °)\n");
                return 0;
            break;
        
            default:
                clearScreen();
                printf("Opção errada\n");
            break;
        }
    }
    return 0;
}

