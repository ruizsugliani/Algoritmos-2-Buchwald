from grafo import Grafo
from cola import Cola
from pila import Pila
import collections
import math
import heapq

ITERACIONES_PRE_DEF = 15

#################################################
#               IMPRESIONES                     #
#################################################
def imprimir_camino(camino):
    '''
    Recibe un arreglo de vértices e imprime el camino minimo,
    de no ser posible imprime un mensaje que lo indica.
    '''
    if not camino:
        print("Seguimiento imposible")
    else:
        print(" -> ".join(camino))

def imprimir_vertices(vertices):
    '''
    Imprime los vertices pasados por parametro.
    '''
    final = ", ".join(vertices)
    print(f"{final}")

def imprimir_componentes(componentes):
    '''
    Imprime las componentes fuertemente conexas de
    la lista pasada por parámetro.
    '''
    cant_comp = 1
    for componente in componentes:
        print(f"CFC {cant_comp}: ", end="")
        imprimir_vertices(componente)
        cant_comp += 1

def imprimir_comunidades(comunidades, n):
    cont_comu = 1
    for vertices in comunidades.values():
        if len(vertices) >= n:
            print(f"Comunidad {cont_comu}:", end=" ")
            imprimir_vertices(vertices)
            cont_comu += 1

        
#################################################
#               CARGAR GRAFO                    #
#################################################

def cargar_grafo(archivo):
    '''
    Recibe por parámetro un archivo.tvs de dos columnas donde la primer columna
    contiene un vértice A y la segunda un vértice B, procesa el archivo para devolver
    grafo sin peso y dirigido con todos los vértices que contiene el archivo y por 
    cada linea procesada agrega la arista y los vértices en caso de no estar en el grafo.
    '''
    grafo = Grafo(dirigido=True)
    with open(archivo, "r", encoding="utf8") as f:
        for linea in f:
            v, w = linea.split()
            
            if not v in grafo:
                grafo.agregar_vertice(v)
            
            if not w in grafo:
                grafo.agregar_vertice(w)
                
            grafo.agregar_arista(v, w)
    return grafo
    

# A continuación se encuentran las funciones implementadas para el programa AlgoPoli
# seguidas de su correspondiente documentación y como cabecera para estas funciones
# un cartel que indica para qué funcionalidad del programa fue implementada.


#################################################
#           MINIMOS SEGUIMIENTOS                #
#################################################
def min_seguimientos(grafo, origen, destino):
    camino = camino_minimo(grafo, origen, destino)
    return camino

def camino_minimo(grafo, origen, destino):
    '''
    Recorre el GRAFO desde el vértice origen mediante el algoritmo bfs 
    y devuelve un diccionario con los vértices como claves y sus padres como valores.
    '''
    #Inicializo las estructuras a utilizar
    visitados = set()
    padres = {}
    q = Cola()

    #Defino el vertice inicial del recorrido sin padres y de distancia 1, lo encolo y agrego a visitados
    inicio = origen
    padres[inicio] = None
    q.encolar(inicio)
    visitados.add(inicio)
    #Recorro el GRAFO
    while not q.esta_vacia():
        v = q.desencolar()
        if v == destino:
            return reconstruir_camino_minimo(destino, padres)

        for w in grafo.obtener_adyacentes(v):
            if w not in visitados:
                visitados.add(w)
                padres[w] = v
                q.encolar(w)
    
    return None

def reconstruir_camino_minimo(destino, padre):
    '''
    Devuelve una lista con el camino minimo entre 
    el origen y el destino. Si el camino minimo no es posible
    devuelve una lista vacía.
    '''
    res = list()
    
    while padre[destino]:
        res.insert(0, destino)
        destino = padre[destino]
    res.insert(0, destino)
    return res


#################################################
#               MAS IMPORTANTES                 #
#################################################
def mas_importantes(grafo, k):
    scores_inicio = establecer_page_rank(grafo)
    scores_final = iterar_vertices(grafo, scores_inicio, ITERACIONES_PRE_DEF)
    k_importantes = obtener_k_importantes(scores_final, k)
    return k_importantes


def establecer_page_rank(grafo):
    '''
    Establece para todos los vértices del  su score Page Rank inicial.
    Devuelve el diccionario con dichos scores para cada vértice.
    '''
    total_vertices = float(len(grafo.obtener_vertices()))
    proba_inicial = 1/total_vertices
    scores = dict()
    for v in grafo.obtener_vertices():
        scores[str(v)] = proba_inicial
    return scores

def page_rank_entrantes(grafo, v, scores):
    '''
    Calcula la sumatoria de los scores en Page Rank de los vertices 
    con arista dirigida al pasado por parámetro y lo devuelve.
    '''
    res = 0
    entrantes = grafo.obtener_entrantes(v)
    for e in entrantes:
        res += (scores[str(e)] / len(grafo.obtener_adyacentes(e)))
    return res

def page_rank(grafo, v, scores):
    '''
    Calcula el nuevo score de Page rank para el vértice pasado por parámetro y lo devuelve.
    '''
    total_vertices = float(len(grafo.obtener_vertices()))
    sumatoria = page_rank_entrantes(grafo, v, scores)
    res = ( float((1 - 0.85) / total_vertices )) + ( 0.85 * sumatoria )
    return res

def iterar_vertices(grafo, scores, repeticiones):
    '''
    Recorre los vertices aplicando PR a cada uno la cantidad de repeticiones deseada.
    Hace uso de un wrapper.
    '''
    vertices = grafo.obtener_vertices()
    contador = 0
    return _iterar_vertices(grafo, scores, repeticiones, contador, vertices)

def _iterar_vertices(grafo, scores, repeticiones, contador, vertices):
    if contador == repeticiones:
        return scores
    for v in vertices:
        scores[str(v)] = page_rank(grafo, v, scores)
    return _iterar_vertices(grafo, scores, repeticiones, contador + 1, vertices)

def obtener_k_importantes(scores, k):
    '''
    Obtiene los K vértices más importantes.
    '''
    importantes = list()
    top_k = sorted(scores.items(), key=lambda x : x[1], reverse = True)[:k]
    for v in top_k:
        importantes.append(v[0])
    return importantes

#################################################
#               PERSECUCIÓN RÁPIDA              #
#################################################
def persecucion_rapida(grafo, vertices, k):
    k_importantes = mas_importantes(grafo, k)
    mejor_camino = camino_mas_corto(grafo, vertices, k_importantes)
    return mejor_camino

def camino_mas_corto(grafo, vertices, k_importantes):
    '''
    Obtener cuál es el camino más corto para llegar desde alguno de los
    vértices pasados por parámetro, a alguno de los K vértices más importantes.
    '''
    res = None
    for v in vertices:
        for imp in k_importantes:
            cam_min = camino_minimo(grafo, v, imp)
            
            #No hay camino posible.
            if len(cam_min) == 0:
                continue
            
            #El camino actual es mas corto al mejor camino de los anteriores vistos.
            if res is None or len(cam_min) < len(res):
                res = cam_min
            
            #Los caminos tienen el mismo largo pero el camino actual llega a un vértice más importante que el anterior.
            if len(cam_min) == len(res) and k_importantes.index(cam_min[-1]) < k_importantes.index(res[-1]):
                res = cam_min
    return res

#################################################
#               COMUNIDADES                     #
#################################################
def comunidades_n(grafo):
    etiquetas = establecer_etiqueta(grafo)
    parcial = repetir_recorrido(grafo, etiquetas)
    comunidades = detectar_comunidades(parcial)
    return comunidades

def establecer_etiqueta(grafo):
    '''
    Para cada vértice, se establece una etiqueta igual a su número identificatorio.
    '''
    etiqueta = dict()
    for v in grafo.obtener_vertices():
        etiqueta[v] = int(v)
    return etiqueta

def max_freq(grafo, v, etiquetas):
    '''
    Devuelve la etiqueta que aparece más frecuentemente entre todos 
    los vertices que tienen como adyacente a 'v'. En caso de empate, 
    devuelve cualquiera de las etiquetas.
    '''
    entrantes = grafo.obtener_entrantes(v)
    frecuencias = dict()
    
    for w in entrantes:
        clave = etiquetas[w]
        frecuencias[clave] = frecuencias.get(clave, 0) + 1
    
    return max(frecuencias.items(), key=lambda x : x[1])[0]

def dfs_modificador(grafo, etiquetas):
    #Estructuras a utilizar
    visitados = set()
    p = Pila()
    #Establecemos el vértice inicial
    inicio = grafo.vertice_aleatorio()
    visitados.add(inicio)
    p.apilar(inicio)
    #Establecemos la nueva etiqueta del inicio
    etiquetas[inicio] = max_freq(grafo, inicio, etiquetas)
    while not p.esta_vacia():
        v = p.desapilar()
        for w in grafo.obtener_adyacentes(str(v)):
            if w not in visitados:
                visitados.add(w)
                etiquetas[v] = max_freq(grafo, v, etiquetas)
                p.apilar(w)
    return etiquetas

def repetir_recorrido(grafo, etiquetas):
    contador = 0
    while contador < ITERACIONES_PRE_DEF:
        dfs_modificador(grafo, etiquetas)
        contador +=1
    return etiquetas

def detectar_comunidades(etiquetas):
    '''
    Recibe un etiquetas con vertices como claves y sus 
    comunidades como valor, devuelve todas aquellas con al menos
    N integrantes.
    '''
    comunidades = {}
    for valor,clave in etiquetas.items():
        comunidades[clave] = comunidades.get(clave, list())
        comunidades[clave].append(str(valor))
    return comunidades


#################################################
#               DIVULGAR RUMOR                  #
#################################################

def div_rumor(grafo, origen, limite):
    '''
    Recorrido dfs, devuelve una cadena con los vértices en el rango 
    de distancia 0 a limite.
    '''
    visitados = dict()
    distancia = dict()
    res = []
    q = Cola()

    visitados[origen] = True
    distancia[origen] = 0
    q.encolar(origen)

    while not q.esta_vacia():
        v = q.desencolar()
        
        if distancia[v] == limite:
            break

        for w in grafo.obtener_adyacentes(str(v)):
            if w not in visitados:
                visitados[w] = True
                distancia[w] = distancia[v] + 1
                q.encolar(w)
                res.append(str(w))
            
    return res

#################################################
#               CICLO DE LARGO 'N'              #
#################################################

def ciclo_largo(grafo, origen, largo):
    '''
    Imprime un ciclo del largo pasado por parámetro en caso de existir
    y en caso contrario imprime un mensaje indicando que no existe un ciclo 
    de ese largo posible.
    '''
    visitados = set()
    ciclo = list()
    destino = origen
    contador = 0
    if not _encontrar_ciclo(grafo, origen, destino, largo, visitados, ciclo, contador):
        print("No se encontro recorrido")

def _encontrar_ciclo(grafo, actual, destino, largo, visitados, ciclo, contador):
    '''
    Wrapper que hace uso de backtracking.
    '''
    ciclo.append(actual)
    visitados.add(actual)
    contador += 1
    adyacentes_actual = grafo.obtener_adyacentes(actual)

    if contador == largo:
        if destino in adyacentes_actual:
            ciclo.append(destino)
            imprimir_camino(ciclo)
            return True

    for w in adyacentes_actual:
        if contador == largo:
            break
        if w not in visitados and contador < largo:
            if _encontrar_ciclo(grafo, w, destino, largo, visitados, ciclo, contador):
                return True
    visitados.remove(actual)
    ciclo.pop()
    contador -= 1
    return False

#################################################
#       COMPONENTES FUERTEMENTE CONEXAS         #
#################################################

def componentes_fuertemente_conexas(grafo):
    '''
    Devuelve una lista con las componentes fuertemente
    conexas del GRAFO, hace uso de una función auxiliar.
    '''
    actual = grafo.vertice_aleatorio()
    apilados = set()
    visitados = set()
    orden = dict()
    mb = dict()
    res = list()
    pila = collections.deque()
    orden_global = 0
    _componentes_fuertemente_conexas(grafo, actual, visitados, apilados, orden, pila, mb, orden_global, res)
    return res

def _componentes_fuertemente_conexas(grafo, actual, visitados, apilados, orden, pila, mb, orden_global, componentes):
    visitados.add(actual)
    orden[actual] = orden_global
    mb[actual] = orden[actual]
    orden_global += 1
    pila.appendleft(actual)
    apilados.add(actual)

    for w in grafo.obtener_adyacentes(str(actual)):
        if w not in visitados:
            _componentes_fuertemente_conexas(grafo, w, visitados, apilados, orden, pila, mb, orden_global, componentes)

        if w in apilados:
            mb[actual] = min(mb[actual], mb[w])


    if orden[actual] == mb[actual] and len(pila) > 0:
        nueva_componente = []
        while True:
            w = pila.popleft()
            apilados.remove(w)
            nueva_componente.append(str(w))
            if w == actual:
                break

        componentes.append(nueva_componente)

