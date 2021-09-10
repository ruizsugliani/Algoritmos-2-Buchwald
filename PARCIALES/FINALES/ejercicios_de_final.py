
#########################
#   FINAL 14/12/2018    #
#########################

#1

#2

#4
'''
Sean A y B dos arreglos de enteros sin elementos repetidos, con largos N y M , respectivamente, donde cada uno
representa un conjunto distinto. Nos piden que decidamos si la intersección de A y B es vacía, o no, y nos proponen los
siguiente algoritmos:
'''
# a) Recorrer A, elemento por elemento, y ver si cada elemento de B está en B, o no.
# Esto es hacer O(n) y por cada elemento de A recorrer B, por ende, cada elemento de A implica un O(m) al recorrer B, finalmente es O(n*m)

# b) Ordenar con algún algoritmo eficiente tanto A como B. Intercalar ambos en un único arreglo C. Verificar si C tiene
# elementos repetidos o no, usando un algoritmo similar a búsqueda lineal.
# Al ordenar por ejemplo con counting sort ambos arreglos tenemos un O(n) + O(m), para intercalarlos hay que recorrer ambos
# uno a la vez hasta haber recorrido los dos , teniendo así un O(n) + O(m) nuevamente. Si se recorre ese nuevo arreglo C de
# largo "n+m" para ver si hay elementos repetidos tendremos que recorrerlo "n+m" veces y por ende sería
# --->   2 * O(n + m) + O(n + m)^2 = O(n + m) + O(n + m)^2 = O(n + m)^2 por ende tampoco sería lo mejor.


# c) Ordenar con algún algoritmo eficiente tanto A como B. Intercalar ambos en un único arreglo C. Verificar si C tiene
# elementos repetidos o no, usando un algoritmo similar a búsqueda binaria. Por ejemplo, recorriendo A y buscando
# sus elementos en C verificando si hay repetidos.
# Ya que búsqueda binaria solo puede ser utilizado en arreglos ordenados, esta opción sería descartada porque
# si tengo como arreglos (tras ser ordenados) A = [1, 2, 3] y B = [4, 5, 6] , al intercalar en C me queda:
# C = [1, 4, 2, 5, 3, 6] donde no puedo aplicar busqueda binaria.

# d) Crear un árbol binario de búsqueda balanceado (AVL por ejemplo) tA con los elementos de A. Recorrer B y ver si
# los elementos de B están en tA, o no. 
# Al crear el AVL, como es un ABB que se autovalancea suponiego que se crea con la primitiva que usa heapify y por ende tarda O(n)
# luego al recorrer B tarda O(m) y por cada elemento de B debo hacer la búsqueda en el arbol lo cual tarda O(log m) en cada caso
# y finalmente tardaría ---> O(n) + O(m) * O (log m) = O(n) + O(m log m) = O(n + m log m)

# e) Crear una tabla de hashing cerrado hA e insertar todos los elementos de A en hA. Recorrer los elementos de B, y ver
# si se encuentran en hA, o no.
# Para guardar como claves (con valor NULL) cada elemento de A, debemos recorrer el arreglo y por cada elemento , guardar en el hash -> O(1) para cada elemento.
# Lo cual tarda O(n) para luego recorrer el arreglo B y por cada elemento un O(1) ya que se debe comprobar que corresponda al hash, finalmente sería O(n + m).
# Siendo finalmente la mejor opción.

#5
def encontrar_orden(grafo):
    '''
    Devuelve un orden topológico válido para el grafo 
    dirigido y acíclico pasado por parámetro.
    '''
    #Establesco estructuras iniciales para recorrer tipo  bfs
    q = Cola()
    visitados = set()
    orden = dict()
    res = list()

    #Inicializo a todos los vértices con orden cero para no ignorar luego a los que justamente lo tienen
    for v in grafo.obtener_vertices():
        orden[v] = 0

    #Recorro nuevamente cada vértice y a cada uno de sus adyacentes le subo el orden
    for v in grafo.obtener_vertices():
        for w in grafo.obtener_adyacentes(v):
            orden[w] += 1

    #Agrego a la cola aquellos con orden 0
    for v in grafo.obtener_vertices():
        if orden[v] == 0:
            q.encolar(v)
            visitados.add(v)
            res.append(v)

    while not q.esta_vacia():
        v = q.desencolar()
        for w in grafo.obtener_adyacentes(v):
            if w not in visitados:
                orden[w] -= 1
                if orden[w] == 0:
                    res.append(w)
                    q.encolar(w)
    return res

#########################
#   FINAL 20/12/2018    #
#########################

#1

#2

#3

#4

#5

# a) En cada paso elige un auto que ofrece el máximo valor H[i]/M[i]
# [i] , hasta que se quede sin lugar para otro auto.
# En este caso si se piensa a la cola de K autos como un arreglo, se debe recorrer dicho
# arreglo varias veces encontrando el máximo beneficio hasta que no exista más espacio en la cuadra
# por ende sería O(K^2)

# b) Prueba todas las combinaciones posibles, y elige la que ofrece mayor valor en pesos.
# Esta posibilidad se puede pensar como usar fuerza bruta y por ende, al probar todas las
# combinaciones y elegir la mejor, la complejidad sería exponencial --> O(2^K) y sin duda la menos eficiente

# c) Utilizando un cuaderno cuadriculado, en la casilla c[i][j] anota el valor máximo que puede obtener si la cuadra
# tuviera j metros, tomando en cuenta los primeros i autos.
# Se podria pensar como diccionario de diccionarios, donde la primera clave "i" son la cantidad de autos, la cual tiene como valor
# otro diccionario donde la segunda clave "j" son la cantidad de metros de la cuadra y su valor la cantidad de dinero a ganar
# por ende sería la mejor opción ya que ver estos valores es de tiempo constante y depende de la cantidad de autos que quiera estacionar.

#########################
#   FINAL 18/07/2019    #
#########################
#1

#2

#3
# a) Un heap d-ario en un arreglo se representa igual que un heap binario, los calculos son:
# Pos_padre = (i - 1) / d
# Pos_hijos = d * i + k -> (donde k es el hijo que se busca, por ejemplo en un heap 3-ario tenemos un hijo izquierdo con k =1 , mediano con k = 2 y derecho con k = 3) este parámetro k va del intervalo [1, k]

# b) ¿Cuál es la altura de un heap d-ario con n elementos, en términos de n y d?
# La altura sería calculada como n / d ya que por ejemplo un heap binario de 8 nodos tiene 4 niveles , y un heap 3-ario de 13 nodos --> 13/3 = 

#4
# En este caso el laberinto sería modelado de una forma tal que cada casillero es un vértice
# y cada movimiento a otro vértice tiene como peso al cansancio (el cual varía dependiendo del movimiento).
# Comenzaría a modelarlo buscando el camino minimo por Dijkstra y en el caso de que los pesos sean iguales o en cualquier
# dirección  utilizaría simplemente bfs

def dijkstra(grafo, origen, destino):
    distancias = dict()
    padres = dict()

    for v in grafo.obtener_vertices():
        distancias[v] = 1000000000 #simula ser infinito

    distancias[origen] = 0
    padres[origen] = 0
    q = Heap_min()
    q.encolar(v)

    while not q.esta_vacia():
        v = q.desencolar()
        for w in grafo.obtener_adyacentes(v):
            if distancias[v] + grafo.obtener_peso(v, w) < distancias[w]:
                distancias[w] = distancias[v] + grafo.obtener_peso(v, w)
                padres[w] = v
                q.encolar(w)

    return reconstruir_camino(grafo, origen, destino, distancias, padres)

def reconstruir_camino(destino, padres):
    '''
    Devuelve una lista con el camino minimo entre 
    el origen y el destino. Si el camino minimo no es posible
    devuelve una lista vacía.
    '''
    camino = list()
    while padres[destino]:
        camino.insert(0, destino)
        destino = padres[destino]
    camino.insert(0, destino)
    return camino


#5
def caminos_simples_dirigido(grafo, s, t):
    '''
    Dado un Grafo dirigido y dos vértices s y t, devuelve todos los
    caminos simples para ir desde s a t en el grafo
    '''
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

#########################
#   FINAL 23/07/2019    #
#########################

#########################
#   FINAL 19/12/2019   #
#########################

#########################
#   FINAL 06/02/2020   #
#########################

#########################
#   FINAL 13/02/2020    #
#########################

#########################
#   FINAL 12/03/2021   #
#########################

#########################
#   FINAL 19/03/2021   #
#########################
