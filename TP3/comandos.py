import g_lib as g

#################################################
#               FUNCIONES COMANDOS              #
#################################################

def min_seguimientos(grafo, comandos):
    if len(comandos) != 3:
        return

    res = g.min_seguimientos(grafo, comandos[1], comandos[2])
    g.imprimir_camino(res)

def mas_imp(grafo, comandos):
    if len(comandos) != 2:
        return
        
    k = int(comandos[1])
    k_importantes = g.mas_importantes(grafo, k)
    g.imprimir_vertices(k_importantes)

def persecucion(grafo, comandos):
    if len(comandos) != 3:
        return

    vertices = comandos[1].split(",")
    k = int(comandos[2])

    mejor_camino = g.persecucion_rapida(grafo, vertices, k)
    g.imprimir_camino(mejor_camino)

def comunidades(grafo, comandos):
    if len(comandos) != 2:
        return
    
    n = int(comandos[1])

    comunidades = g.comunidades_n(grafo)
    g.imprimir_comunidades(comunidades, n)            

def divulgar_rumor(grafo, comandos):
    if len(comandos) != 3:
        return
    
    limite = int(comandos[2])

    rumor_1 = g.div_rumor(grafo, comandos[1], limite)
    g.imprimir_vertices(rumor_1) 

def divulgar_ciclo(grafo, comandos):
    if len(comandos) != 3:
        return

    largo = int(comandos[2])

    g.ciclo_largo(grafo, comandos[1], largo)

def cfc(grafo, comandos):
    if len(comandos) != 1:
        return

    componentes = g.componentes_fuertemente_conexas(grafo)
    g.imprimir_componentes(componentes)

