Resource Monitor — Namespaces, Cgroups e Profiling de Recursos

Este projeto implementa um sistema completo de monitoramento, análise e limitação de recursos no Linux, utilizando as interfaces do kernel:

- /proc
- /sys/fs/cgroup
- Namespaces (CLONE_NEW*)
- Control Groups (cgroup v1)
- Syscalls de baixo nível (stat, clone, sched…)

O objetivo é demonstrar como containers utilizam namespaces para isolamento e cgroups para controle e contabilização de recursos.

Estrutura do Projeto:

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

Requisitos: 

- GCC
- Linux Kernel 5.x ou superior
- Cgroup v1 habilitado
- Permissões de root para operações envolvendo cgroups


Contribuição dos Alunos:

- Aluno 1 – Resource Profiler + Integração (Felipe Simionato Bueno)

Coleta de CPU e memória

Integração dos módulos

Makefile

- Aluno 2 – Resource Profiler + Testes (Vinicius Pelissari Jordani)

Coleta de I/O e rede

Criação de workloads

Testes automatizados

- Aluno 3 – Namespace Analyzer (Kevin Mitsuo Lohmann Abe)

Implementação completa do módulo de namespaces

Listagem e comparação de namespaces

Identificação de membros por namespace

Medição de overhead via clone()

Geração de relatórios CSV

Execução e documentação dos experimentos de isolamento

- Aluno 4 – Control Groups (João Barowski)

Implementação do Cgroup Manager

Criação, configuração e leitura de cgroups

Experimentos de limitação de CPU e memória
