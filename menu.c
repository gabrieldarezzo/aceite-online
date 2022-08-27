
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    // Fix accent in Windos
    system("chcp 65001");
    system("cls");
    
    while(1) {
        int choise = 0;
        
        printf("1 - Para cadastro de clientes\n");
        printf("2 - Para cadastro de Perfil\n");
        printf("3 - Para cadastro de Funcionário\n");
        printf("4 - Relatórios em TXT\n");
        printf("5 - Quit\n");
        printf("Escolha a opção acima: \n");
        scanf("%d", &choise);

        switch (choise) {
            case 1:
                printf("Novo Menu");
            break;

            case 5:
                exit(0);
            break;
        
            default:
            break;
        }
    }
   

        
    return 0;
}

