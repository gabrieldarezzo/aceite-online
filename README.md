Execute o comando
```shell
gcc -o binary menu.c


menu.exe
```



https://app.diagrams.net/#G1qRgqKxPBZyYuL1YDv5Tw2RoON5Od_gRE

Consumer (Cliente)
```markdown
struct Consumer
---------------------------------------------
id: int
name: char[250]
cpf: char[11]
gender: char[1]
```

Document (Documents)
```markdown
struct Documents
---------------------------------------------
id: int
consumer_id: int
path_file: char[250]
```

Worker (Worker)
```markdown
struct Worker
---------------------------------------------
id: int
name: char[250]
cpf: char[11]
gender: char[1]
salary: float
is_manager: int
```



