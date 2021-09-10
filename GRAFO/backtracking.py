from cola import Cola
from pila import Pila
from grafo import Grafo

#Ejercicio backtracking 1
def caminos_simples(grafo, v, w):
    '''
    Recibe un grafo, un vértice v y otro w, y determina (utilizando backtracking) 
    la cantidad de caminos simples diferentes que hay desde v a w.
    '''
    visitados = set()
    return _caminos_simples(grafo, v, w, visitados)

def _caminos_simples(grafo, v, w , visitados):
    if v == w:
        return 1
    
    visitados.add(v)
    contador = 0
    for ady in grafo.adyacentes(v):
        if ady not in visitados:
            contador += _caminos_simples(grafo, ady, w, visitados)

    visitados.remove(v)
    return contador

#Ejercicio backtracking 2
def camino_simple_k(grafo, v, w, k):
    '''
    Recibe un grafo, un vértice v y otro w y un valor K, y determine (utilizando backtracking) 
    la cantidad de caminos simples diferentes que hay desde v a w, siempre y cuando el peso del camino sea menor a K.
    '''
    visitados = set()
    peso = 0
    return _camino_simple_k(grafo, v, w, k, peso, visitados)

def _camino_simple_k(grafo, v, w, k, peso, visitados):
    if v == w:
        return 1

    contador = 0
    visitados.add(v)
    for ady in grafo.adyacentes(v):
        peso += grafo.peso_arista(v, ady)
        if ady not in visitados and peso <= k:
            contador += _camino_simple_k(grafo, ady, w, k, peso, visitados)
        peso -= grafo.peso_arista(v, ady)
    visitados.remove(v)
    return contador

#Ejercicio backtracking 3
def caminos_posibles(grafo, v, w):
    visitados = set()
    caminos = list()
    res = ""
    return _caminos_posibles(grafo, v, w, visitados, caminos, res)

def _caminos_posibles(grafo, v, w, visitados, caminos, res):
    visitados.add(v)
    res += v
    if v == w:
        caminos.append(res)
    for ady in grafo.adyacentes(v):
        if ady not in visitados:
            _caminos_posibles(grafo, ady, w, visitados, caminos, res)
    visitados.remove(v)
    res = res[:-1]
    return caminos

#Con estas 2 funciones encuentro el camino mas corto o mas largo posible por backtracking
def camino_minimo(grafo, v, w):
    return min(caminos_posibles(grafo, v, w))

def camino_maximo(grafo, v, w):
    return max(caminos_posibles(grafo, v, w))

#Ejercicio backtracking 4
def camino_largo(grafo, v, w, k):
    '''
    Recibe un grafo no pesado y por backtracking devuelve
    un booleano en cuanto a si existe un camino simple de
    longitud K de V a W.
    '''
    visitados = set()
    largo = 0
    return encontrar_camino(grafo, v, w, k, largo, visitados)

def encontrar_camino(grafo, v, w, k, largo, visitados):
    if v == w and largo == k:
        return True

    visitados.add(v)
    largo += 1
    for ady in grafo.adyacentes(v):
        if ady not in visitados:
           if encontrar_camino(grafo, ady, w, k, largo, visitados):
               return True
    visitados.remove(v)
    largo -= 1
    return False

def caminos_simples_dirigido(grafo, s, t):
    visitados = set()
    caminos = list()
    cam_act = ""
    return _caminos_simples_dirigido(grafo, visitados, caminos, cam_act, s, t)

def _caminos_simples_dirigido(grafo, visitados, caminos, cam_act, s, t):
    visitados.add(s)
    cam_act += s

    if s == t:
        caminos.append(cam_act)

    for ady in grafo.obtener_adyacentes(s):
        if ady not in visitados:
            _caminos_simples_dirigido(grafo, visitados, caminos, cam_act, ady, t)
        
    visitados.remove(s)
    cam_act = cam_act[:-1]
    return caminos

g = Grafo(True)
g.agregar_vertice('A')
g.agregar_vertice('B')
g.agregar_vertice('C')
g.agregar_vertice('D')
g.agregar_vertice('E')
g.agregar_vertice('S')

g.agregar_arista('A', 'B', 1)
g.agregar_arista('A', 'C', 1)
g.agregar_arista('C', 'D', 1)
g.agregar_arista('C', 'B', 1)
g.agregar_arista('C', 'E', 1)
g.agregar_arista('D', 'B', 1)
g.agregar_arista('E', 'S', 1)
g.agregar_arista('S', 'B', 1)

print(caminos_simples_dirigido(g, 'S', 'A'))