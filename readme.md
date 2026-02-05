# Trabajo Práctico N°2 – Algoritmos y Estructuras de Datos

Este trabajo práctico procesa un archivo binario (`Datos.bin`) que contiene las operaciones bursátiles realizadas por un broker financiero durante una jornada.  
A partir de la lectura del archivo, se construye una **estructura de datos jerárquica de tres niveles**, utilizando **listas enlazadas simples**, y luego se emite un listado ordenado según los criterios indicados en la consigna.

## Funcionalidad del sistema

- Lectura del archivo binario de operaciones bursátiles.
- Visualización de todos los registros leídos.
- Construcción de una estructura de datos de tres niveles:
  
### Nivel 1 – Plazo
- CI (0), 24Hs (1), 48Hs (2), 72Hs (3).
- Ordenado en forma ascendente por código de plazo.
- Para cada plazo se informa:
  - Cantidad total de compras.
  - Cantidad total de ventas.

### Nivel 2 – Bolsa
- Para cada plazo, se agrupan las operaciones por bolsa.
- Las bolsas se listan en orden alfabético.
- Para cada bolsa se informa:
  - Monto total negociado (valor absoluto).
  - Resultado total (valor con signo).

### Nivel 3 – Operaciones
- Se listan todas las operaciones correspondientes a cada bolsa y plazo.
- Se mantiene el orden original en el que aparecen en el archivo.
- Para cada operación se informa:
  - Tipo de operación (Compra o Venta).
  - Acción negociada.
  - Cantidad de acciones.

## Estructuras utilizadas

- `Registro`: estructura utilizada para leer los datos desde el archivo binario.
- `Plazo`: representa el primer nivel de agrupación.
- `Bolsa`: representa el segundo nivel de agrupación.
- `Operacion`: representa cada operación individual.
- Listas enlazadas simples genéricas (`Nodo<T>`), con funciones:
  - `insertar_unico`
  - `agregar`

## Integrantes – Grupo 6

- Nahuel Medina
