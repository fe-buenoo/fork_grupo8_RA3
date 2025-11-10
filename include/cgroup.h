#ifndef CGROUP_H
#define CGROUP_H

#include <sys/types.h> // Para pid_t

/**
 * @brief Cria um novo cgroup com o nome fornecido.
 * @param controller O controlador (ex: "cpu", "memory").
 * @param group_name O nome do cgroup a ser criado.
 * @return 0 em sucesso, -1 em falha.
 */
int cgroup_create(const char *controller, const char *group_name);

/**
 * @brief Move um processo (PID) para um cgroup específico.
 * @param controller O controlador (ex: "cpu", "memory").
 * @param group_name O nome do cgroup.
 * @param pid O ID do processo a ser movido.
 * @return 0 em sucesso, -1 em falha.
 */
int cgroup_move_pid(const char *controller, const char *group_name, pid_t pid);

/**
 * @brief Define o limite de memória para um cgroup (cgroup v1).
 * @param group_name O nome do cgroup.
 * @param limit_bytes O limite em bytes (ex: 100 * 1024 * 1024 para 100MB).
 * @return 0 em sucesso, -1 em falha.
 */
int cgroup_set_memory_limit(const char *group_name, long long limit_bytes);

/**
 * @brief Define o limite de CPU para um cgroup (cgroup v1).
 * @param group_name O nome do cgroup.
 * @param cores A fração de cores (ex: 0.5 para 50% de um core).
 * @param period_us O período do CFS (geralmente 100000).
 * @return 0 em sucesso, -1 em falha.
 */
int cgroup_set_cpu_limit(const char *group_name, double cores, long period_us);

/**
 * @brief Lê o uso atual de memória de um cgroup (cgroup v1).
 * @param group_name O nome do cgroup.
 * @return O uso de memória em bytes, ou -1 em caso de falha.
 */
long long cgroup_get_memory_usage(const char *group_name);

/**
 * @brief Lê o uso total de CPU (em nanosegundos) de um cgroup (cgroup v1).
 * @param group_name O nome do cgroup.
 * @return O uso de CPU em nanosegundos, ou -1 em caso de falha.
 */
long long cgroup_get_cpu_usage(const char *group_name);

#endif // CGROUP_H