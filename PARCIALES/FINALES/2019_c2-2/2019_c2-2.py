ROJO = 1
NEGRO = 2
def hay_dos_grupos(grafo):
    '''
    Toma un grafo donde las mariposas son vértices y las comparaciones
    hechas entre ellas son aristas (por ende grafo no dirigido), utilizando
    el concepto de bipartito devuelve True o False en caso de haber dos grupos
    de mariposas, es decir, las mariposas comparadas son distintas en todos los casos.
    Al recorrer el grafo de manera BFS la complejidad es O(V+E) = O(M+D).
    '''
    colores = dict()
    visitados = set()
    predecesor = dict()
    q = Cola()

    inicio = grafo.vertice_aleatorio()
    visitados.add(inicio)
    predecesor[inicio] = None
    colores[inicio] = NEGRO
    q.encolar(inicio)

    while not q.esta_vacia():
        v = q.desencolar()
        for w in grafo.obtener_adyacentes(v):
            if w not in visitados:
                visitados.add(w)
                predecesor[w] = v
                if colores[v] == NEGRO:
                    colores[w] = ROJO
                else:
                    colores[w] = NEGRO
                q.encolar(w)
            else:
                if colores[w] == colores[v]:
                    return False
    return True