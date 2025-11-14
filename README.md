Resource Monitor — Namespaces, Cgroups e Profiling de Recursos

Este projeto implementa um sistema completo de monitoramento, análise e limitação de recursos no Linux, utilizando as interfaces do kernel:

- /proc
- /sys/fs/cgroup
- Namespaces (CLONE_NEW*)
- Control Groups (cgroup v1)
- Syscalls de baixo nível (stat, clone, sched…)

O objetivo é demonstrar como containers utilizam namespaces para isolamento e cgroups para controle e contabilização de recursos.

Este trabalho segue as especificações da Atividade Avaliativa RA3.

🧩 Estrutura do Projeto
resource-monitor/
├── README.md
├── Makefile
├── docs/
│   └── ARCHITECTURE.md
├── include/
│   ├── monitor.h
│   ├── namespace.h
│   └── cgroup.h
├── src/
│   ├── cpu_monitor.c
│   ├── memory_monitor.c
│   ├── io_monitor.c
│   ├── namespace_analyzer.c
│   ├── cgroup_manager.c
│   └── main.c
└── tests/
    ├── test_cpu.c
    ├── test_memory.c
    └── test_io.c

⚙️ Compilação

Use: make

O binário será criado como: ./monitor

Requisitos: 

GCC
Linux Kernel 5.x ou superior
Cgroup v1 habilitado
Permissões de root para operações envolvendo cgroups

▶️ Uso Geral

O formato geral é:

./monitor <comando> [argumentos...]

O sistema possui três módulos principais:

Resource Profiler

Namespace Analyzer (Aluno 3)

Control Group Manager

🟦 Comandos do Namespace Analyzer (Aluno 3)
🔹 Listar namespaces de um processo
./monitor ns_list <pid>

🔹 Comparar namespaces entre dois processos
./monitor ns_compare <pid1> <pid2>

🔹 Listar processos que compartilham um namespace
./monitor ns_members <type> <inode>

📊 Experimentos do Aluno 3

O Namespace Analyzer foi utilizado para realizar os seguintes experimentos:

✔ 1. Overhead de criação de namespaces

Coletado via:

./monitor ns_overhead

✔ 2. Verificação de isolamento entre namespaces

Usando ferramentas como unshare:

unshare -p bash
unshare -n bash
unshare -m bash


Testando isolamento de:

PID

NET

MNT

IPC

USER

UTS

✔ 3. Relatório de namespaces do sistema

Gerado usando:

./monitor ns_report ns.csv

👤 Contribuição dos Alunos

Aluno 1 – Resource Profiler + Integração (Felipe Simionato Bueno)

Coleta de CPU e memória

Integração dos módulos

Makefile

Aluno 2 – Resource Profiler + Testes (Vinicius Pelissari Jordani)

Coleta de I/O e rede

Criação de workloads

Testes automatizados

Aluno 3 – Namespace Analyzer (Kevin Mitsuo Lohmann Abe)

Implementação completa do módulo de namespaces

Listagem e comparação de namespaces

Identificação de membros por namespace

Medição de overhead via clone()

Geração de relatórios CSV

Execução e documentação dos experimentos de isolamento

Aluno 4 – Control Groups (João Barowski)

Implementação do Cgroup Manager

Criação, configuração e leitura de cgroups

Experimentos de limitação de CPU e memória
