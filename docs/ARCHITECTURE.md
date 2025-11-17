# Arquitetura do Sistema Resource-Monitor

## 1. Visão Geral

O `resource-monitor` é uma ferramenta de linha de comando (CLI) escrita em C (padrão C17), projetada para analisar, monitorar e controlar os recursos de processos no sistema operacional Linux.

A arquitetura é modular, com cada componente principal (Profiler, Analyzer, Manager) expondo uma API pública através de seus arquivos de cabeçalho (`.h`) na pasta `include/`.

O ponto de entrada `src/main.c` atua como o orquestrador, interpretando os comandos do usuário e chamando os módulos apropriados para interagir diretamente com as interfaces do kernel.

## 2. Decisão de Design Crítica: cgroup v2 (Ubuntu 24.04)

Este projeto foi desenvolvido e testado no **Ubuntu 24.04 LTS**, que utiliza a hierarquia **cgroup v2** por padrão. Esta decisão impacta fundamentalmente o design:

1.  **Hierarquia Unificada:** Não existem mais montagens separadas para `cpu`, `memory`, `blkio`. Todos os controladores residem em uma única árvore em `/sys/fs/cgroup`.
2.  **Ativação de Controlador:** Os controladores (`cpu`, `memory`, `io`) devem ser ativados **antes** da criação do grupo, escrevendo no arquivo `cgroup.subtree_control` do diretório pai.
3.  **Nomes de Arquivo V2:** O código usa os nomes de arquivo modernos do v2 (ex: `cpu.max`, `memory.max`, `cpu.stat`, `io.stat`).

## 3. Fontes de Dados do Kernel

O sistema interage diretamente com duas interfaces principais do kernel, sem bibliotecas externas:

* **/proc (Procfs):** Usado para *leitura* e *profiling*. É a fonte de dados para os módulos **Resource Profiler** (Aluno 1/2) e **Namespace Analyzer** (Aluno 3).
* **/sys/fs/cgroup (Cgroupfs):** Usado para *leitura* e *escrita*. É a fonte de dados para o **Control Group Manager** (Aluno 4) para aplicar limites e coletar métricas de *grupos*.

## 4. Componentes Principais e APIs

### 4.1. CLI (main.c)
* **Responsável:** Aluno 1 (Integração).
* **Função:** Ponto de entrada. Interpreta `argc` e `argv` para determinar qual ação executar (ex: "profile", "limit", "analyze").
* **Interações:** É o único módulo que chama as funções dos outros três componentes.

### 4.2. Resource Profiler (monitor.h)
* **Responsável:** Aluno 1 (CPU/Mem) e Aluno 2 (I/O/Rede).
* **Função:** Coletar métricas detalhadas de um processo específico (por PID).
* **API Exposta:**
    * `CpuMonitorState`, `MemorySample`, `IoSample` (Structs de dados).
    * `cpu_monitor_init` / `cpu_monitor_sample`: Coleta CPU%, threads, etc. (Fonte: `/proc/[pid]/stat`, `/proc/stat`).
    * `memory_monitor_sample`: Coleta RSS, VSZ, Page Faults. (Fonte: `/proc/[pid]/status`, `/proc/[pid]/statm`).
    * `io_monitor_init` / `io_monitor_sample`: Coleta I/O de disco e rede, e calcula taxas. (Fonte: `/proc/[pid]/io`, `/proc/[pid]/net/dev`).
    * `..._csv_write`: Funções auxiliares para exportar dados.

### 4.3. Namespace Analyzer (namespace.h)
* **Responsável:** Aluno 3.
* **Função:** Analisar o isolamento de processos via namespaces.
* **API Exposta:**
    * `NamespaceInfo` (Struct de dados).
    * `list_process_namespaces(pid_t)`: Lista os namespaces de um processo.
    * `compare_namespaces(pid_t, pid_t)`: Compara os inodes de namespace de dois processos.
    * `list_namespace_members(...)`, `measure_namespace_overhead(...)`, `generate_namespace_report(...)`: Funções para experimentos e relatórios.
* **Lógica Principal:** A implementação deve usar `readlink` para ler os links simbólicos em `/proc/[pid]/ns/` (ex: `ns/pid`, `ns/net`) e comparar seus valores de inode.

### 4.4. Control Group Manager (cgroup.h)
* **Responsável:** Aluno 4.
* **Função:** Criar grupos, mover processos para grupos, aplicar limites e ler métricas de *grupos* inteiros.
* **API Exposta (`cgroup.h`):**
    * `cgroup_create(...)`: Cria o diretório do grupo (ex: `mkdir /sys/fs/cgroup/MEU_GRUPO`).
    * `cgroup_move_pid(...)`: Escreve o PID no arquivo `cgroup.procs`.
    * `cgroup_set_memory_limit(...)`: Escreve o limite em bytes no arquivo `memory.max`.
    * `cgroup_set_cpu_limit(...)`: Escreve a quota e o período no arquivo `cpu.max`.
    * `cgroup_get_memory_usage(...)`: Lê o uso atual de `memory.current`.
    * `cgroup_get_cpu_usage(...)`: Lê e "parseia" `usage_usec` de `cpu.stat`.
    * `cgroup_get_io_stats(...)`: Lê e "parseia" `rbytes` e `wbytes` de `io.stat`.

## 5. Fluxos de Dados (Exemplos)

> **Fluxo 1: Profiling (Alunos 1/2)**
> 1.  **[Usuário]** Digita: `./resource-monitor profile 1234 --interval 1`
> 2.  **[main.c]** Chama `cpu_monitor_init()` e `io_monitor_init()` para o PID 1234.
> 3.  **[main.c]** Entra em um loop `while(true)`.
> 4.  Dentro do loop:
>     * `sleep(1)`.
>     * `cpu_monitor_sample()` é chamado -> **[monitor.c]** lê `/proc/1234/stat`.
>     * `memory_monitor_sample()` é chamado -> **[monitor.c]** lê `/proc/1234/status`.
>     * `io_monitor_sample()` é chamado -> **[monitor.c]** lê `/proc/1234/io` e `/proc/1234/net/dev`.
>     * `cpu_sample_csv_write()` (e outros) são chamados para salvar os dados.

> **Fluxo 2: Limitação de CPU (Alunos 1 e 4)**
> 1.  **[Usuário]** Digita: `./resource-monitor limit-cpu meu_grupo 0.5 1234`
> 2.  **[main.c]** (Aluno 1) Interpreta os argumentos.
> 3.  **[main.c]** Executa `system("echo '+cpu +io +memory' | sudo tee /sys/fs/cgroup/cgroup.subtree_control")` para preparar o kernel.
> 4.  **[main.c]** Chama `cgroup_create("cpu", "meu_grupo")`.
> 5.  **[cgroup_manager.c]** (Aluno 4) Executa `mkdir("/sys/fs/cgroup/meu_grupo")`.
> 6.  **[main.c]** Chama `cgroup_set_cpu_limit("meu_grupo", 0.5, 100000)`.
> 7.  **[cgroup_manager.c]** (Aluno 4) Escreve "50000 100000" em `/sys/fs/cgroup/meu_grupo/cpu.max`.
> 8.  **[main.c]** Chama `cgroup_move_pid("cpu", "meu_grupo", 1234)`.
> 9.  **[cgroup_manager.c]** (Aluno 4) Escreve "1234" em `/sys/fs/cgroup/meu_grupo/cgroup.procs`.
> 10. **[main.c]** Imprime "Sucesso" para o usuário.