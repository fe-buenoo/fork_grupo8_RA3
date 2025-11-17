#!/usr/bin/env python3
"""
visualize.py - Visualização simples de arquivos CSV gerados pelo resource-monitor.

Uso:
    python3 visualize.py namespace_report.csv
"""

import sys
import pandas as pd

try:
    import matplotlib.pyplot as plt
    HAS_MPL = True
except ImportError:
    HAS_MPL = False

def main():
    if len(sys.argv) < 2:
        print("Uso: python3 visualize.py <arquivo.csv>")
        sys.exit(1)

    path = sys.argv[1]
    df = pd.read_csv(path)
    print("\nPrimeiras linhas do CSV:\n")
    print(df.head())

    # Se tiver coluna pid_count (caso do namespace_report.csv), faz um gráfico simples
    if HAS_MPL and {"inode", "pid_count"}.issubset(df.columns):
        plt.figure()
        df.plot(kind="bar", x="inode", y="pid_count", legend=False)
        plt.title("Processos por namespace (inode)")
        plt.xlabel("inode")
        plt.ylabel("pid_count")
        plt.tight_layout()
        plt.show()
    else:
        if not HAS_MPL:
            print("\nmatplotlib não encontrado. Mostrando apenas a tabela no terminal.")
        else:
            print("\nCSV não possui colunas 'inode' e 'pid_count'. Nenhum gráfico gerado.")

if __name__ == "__main__":
    main()
