#!/usr/bin/python3

from grafo import Grafo
import sys
import g_lib as g
import comandos as c

COMANDOS = {
    "min_seguimientos": c.min_seguimientos,
    "mas_imp": c.mas_imp,
    "persecucion": c.persecucion,
    "comunidades": c.comunidades,
    "divulgar": c.divulgar_rumor,
    "divulgar_ciclo": c.divulgar_ciclo,
    "cfc": c.cfc
}

#################################################
#            EJECUCIÓN DE COMANDOS              #
#################################################

def esperar_comando(grafo):
    for line in sys.stdin:
        if not line or line == "\n":
            break

        comandos = line.split() 
        
        accion = comandos[0] 
        if accion in COMANDOS:
            COMANDOS[accion](grafo, comandos)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Cantidad de parametros invalida")
    else:    
        grafo = g.cargar_grafo(sys.argv[1])
        esperar_comando(grafo)