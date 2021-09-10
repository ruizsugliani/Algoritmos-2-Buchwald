#2
def dijkstra(grafo, inicio):
    visitados = set()
    padre = dict()
    dist = dict()
    heap = Heap(min)

    #Establesco en infinito la distancia a todos los vertices -> O(v)
    for v in grafo.obtener_vertices():
        dist[v] = inf

    visitados.add(inicio)
    padre[inicio] = None
    dist[inicio] = 0
    heap.encolar((inicio, 0))

    while not heap.esta_vacio():
        v = heap.desencolar()
        for w in grafo.obtener_adyacentes(v):
            if w not in visitados:
                nueva_dist = grafo.obtener_peso(v, w) + dist[v]
                if nueva_dist < dist[w]:
                    dist[w] = nueva_dist
                    padre[w] = v
                    heap.encolar((w, dist[w])) #O(E log E) = O(E log V)
    return padre, dist

# En caso de existir más caminos minimos hacia un vértice, eso lo determinará una de las aristas de entrada al mismo
# además de la que ya se encuentra en el camino minimo devuelto por los diccionarios, lo cual podemos averiguar inicializando 
# un diccionario "cant_cm" donde al encontrar una arista que conduzca a un vértice X y el peso de esta arista más el de la distancia
# del vertice predecesor sea igual al que ya existe en el dict dist[w], por ende el dict "cant_cm" sumara 1 unidad.    

#3