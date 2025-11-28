# Trabajo Práctico 2 – Algoritmos y Estructuras de Datos
Este trabajo práctico procesa un archivo de operaciones bursátiles y construye una estructura de datos de **tres niveles** basada en listas enlazadas:

1. **Plazo** (CI, 24Hs, 48Hs, 72Hs)  
   - Contabiliza la cantidad de compras y ventas realizadas.
   - Ordenado de forma ascendente por código de plazo.

2. **Bolsa** (Frankfurt WB, NYSE, Nasdaq, Tokyo SE)  
   - Para cada plazo, agrupa todas las operaciones realizadas en esa bolsa.
   - Calcula el **monto total negociado** (valor absoluto) y el **resultado** (valor con signo).
   - Ordenada alfabéticamente.

3. **Operaciones**  
   - Mantiene el orden original del archivo.
   - Informa: tipo de operación (Compra/Venta), acción y cantidad negociada.

## 📄 **Estructuras utilizadas**
- `Plazo`  
- `Bolsa`  
- `Oper`  
- `Registro` para leer las líneas del archivo  
- Listas enlazadas simples (`Nodo<T>`, `insertar_unico`, `insertar`, `agregar`)

**Integrantes – Grupo 6**

- Nahuel Medina

