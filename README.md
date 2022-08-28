# Aceite Online
## O que é?
Um sistema desenvolvido na linguagem C.  
Possibilitando o operador:
- Listar / Cadastrar / Atualizar / Excluir => Funcionario (Caso gerente)
- Listar / Cadastrar / Atualizar / Excluir => Clientes
- Listar custo mensal da empresa


A brincaideora aqui usou alguns conceitos do C: 
- Functions 
- Pointers and Refferences
- Correção de acentuação no Windows (setLocale is Sux!)
- Structs 
- I/O Files Persistir em (.csv) 
- Structs 


https://docs.google.com/spreadsheets/d/1eMlOSFpCK09SJYl4bYadGosuIpcc6fSVmUAPV1VW3GI/edit?usp=sharing



### Copilar e Executar ?!  
Execute o comando
```shell
gcc -o binary.exe worker.c && binary.exe
```


### Copilar pra valer ?!
Execute o comando
```shell
gcc -o binary.exe worker.c && dist/aceite-online.exe
```

![Terminal](docs/out_worker.png)    


![Fluxograma](docs/fluxograma_fluxo.png)    
![Fluxograma](docs/fluxograma_menus.png)    


O restante está aqui:  
https://app.diagrams.net/#G1qRgqKxPBZyYuL1YDv5Tw2RoON5Od_gRE   
Ou uma versão estatica: `docs/fluxo_drawio.xml`  


Planilha de dados (storage/workers.csv)
https://docs.google.com/spreadsheets/d/1eMlOSFpCK09SJYl4bYadGosuIpcc6fSVmUAPV1VW3GI/edit?usp=sharing
![Terminal](docs/workers_csv)    


## Dicas:
In VSCode:    
Utilizar Doxygen (Extensão)   

---------------------------------------------   
Estruturas utilizadas:

Worker (Worker)
```markdown
struct Worker
---------------------------------------------
int id;
char name[250];
char cpf[14];
int is_manager;
char gender[1];
float salary;
int is_deleted;
```


Consumer (Cliente)
```markdown
struct Consumer
---------------------------------------------
id: int;
name: char[250];
cpf: char[14];
gender: char[1];
```

Document (Documents)
```markdown
struct Documents
---------------------------------------------
id: int
consumer_id: int
path_file: char[250]
```


Topicos para estudar:  
- Malloc: https://www.programiz.com/c-programming/c-structures-pointers
- Retornar o indice em outro escopo, 
Ex: Passei na função:
```c
void showWorkersInDisplay(struct Worker workers[]) {
    lengthWorkers ?
    // // int lengthWorkers = sizeof(workers)/sizeof(workers[0]);
}

```



