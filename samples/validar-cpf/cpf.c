#include <stdio.h>
#include <string.h>

int main(void) {

    // Alocamos a memoria para uma string de 14 caracteres (que vai ser o CPF + pontuação)
    char cpf[14];
    
    // Imprimir no terminal a string: Escreva o seu nome : 
    printf("Informe seu CPF:\n");
    // Lembra a variavel 'cpf' da linha 6? Vamos esperar o usuario interagir com o I/O via keyboard
    
    // Escrevendo uma string (cpf) e aguardando o retorno do 'carriage', no caso a tecla 'Enter'
    scanf("%s", cpf);
    printf("O CPF informado foi: %s\n", cpf);
    printf("A quantidade de letras são: %lu\n", strlen(cpf));

    // if(???) {
    //     // Aqui vai o bloco quando a condição é verdadeira.
    // } else {
    //     // Aqui vai o bloco quando a condição é falsa.
    // }
    
    return 0;
}