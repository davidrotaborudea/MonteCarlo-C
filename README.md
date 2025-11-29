# Autores

- David Mejia Castaño
- Miguel Angel Puerta Vasquez
- Juan Pablo Zuluaga Jaramillo
- David Stiven Rodríguez Taborda

# Estimación de π con programación paralela (C + pthreads)

Este proyecto implementa la estimación del número π usando el método de Monte Carlo:

- **Versión secuencial** en C.
- **Versión paralela** usando hilos POSIX (**pthreads**).

El objetivo es comparar el **tiempo de ejecución** de ambas versiones y observar el **speedup** que se obtiene al paralelizar el cálculo.

---

## 1. Requisitos y limitaciones

Este proyecto está pensado para ejecutarse en:

- **Linux** (Ubuntu, Debian, Fedora, etc.).
- **macOS** (con las herramientas de desarrollo instaladas).

Necesitas tener instalado:

- `git`
- Un compilador C compatible (por ejemplo `gcc` o `clang`).
- `make`

> ⚠️ **Importante sobre Windows**
>
> El código usa **pthreads**, que es una librería POSIX.  
> Eso **no funciona de forma nativa en Windows** con el compilador estándar de Visual Studio.
>
> Opciones para usarlo en Windows:
> - **WSL / WSL2** (subsistema de Linux para Windows) y seguir las instrucciones como si fuera Linux.
> - O usar un entorno tipo MinGW/Cygwin (no cubierto en este README).

### Cómo instalar las herramientas (ejemplos)

- En **Ubuntu/Debian**:

  ```bash
  sudo apt update
  sudo apt install -y build-essential git


* En **macOS**:

  1. Instala las Xcode Command Line Tools:

     ```bash
     xcode-select --install
     ```
  2. (Opcional) Instala `git` desde Homebrew u otra fuente.

---

## 2. Clonar el repositorio

Abre una terminal y ejecuta:

```bash
git clone https://github.com/davidrotaborudea/MonteCarlo-C.git
cd MonteCarlo-C
```

---

## 3. Estructura básica del proyecto

Una estructura típica es:

```text
.
├─ src/
│  ├─ seq_pi.c            # Versión secuencial
│  ├─ par_pi_pthreads.c   # Versión paralela con pthreads
├─ Makefile
└─ README.md
```

El `Makefile` se encarga de compilar los programas.

---

## 4. Compilar el proyecto con `make`

Desde la raíz del proyecto (la carpeta donde está el `Makefile`):

```bash
make
```

Si todo va bien, esto generará ejecutables como:

* `seq_pi`
* `par_pi_pthreads`

Si quieres limpiar los binarios compilados:

```bash
make clean
```

---

## 5. Ejecutar la versión secuencial

La versión secuencial calcula π usando un solo hilo.

```bash
./seq_pi NUM_MUESTRAS
```

Ejemplo:

```bash
./seq_pi 10000000
```

Salida típica:

```text
Versión secuencial
Muestras totales: 10000000
π ≈ 3.141592
Tiempo total: 0.85 s
```

* `NUM_MUESTRAS` es un `long long` (número grande de iteraciones).
* Valores recomendados para probar en una máquina normal:

  * `1000000` (1 millón)
  * `10000000` (10 millones)
  * `50000000` (50 millones)

---

## 6. Ejecutar la versión paralela (pthreads)

La versión paralela reparte el trabajo entre varios hilos:

```bash
./par_pi_pthreads NUM_MUESTRAS NUM_HILOS
```

* `NUM_MUESTRAS`: número total de puntos aleatorios a generar.
* `NUM_HILOS`: número de hilos a utilizar.

Ejemplos:

```bash
# 10 millones de muestras con 4 hilos
./par_pi_pthreads 10000000 4

# 50 millones de muestras con 8 hilos
./par_pi_pthreads 50000000 8
```

Si no indicas `NUM_HILOS`, el programa intenta usar automáticamente
el número de núcleos disponibles en tu máquina.

Salida típica:

```text
Versión paralela (pthreads)
Muestras totales: 10000000
Hilos: 4
π ≈ 3.141640
Tiempo total: 0.32 s
```

---

## 7. Notas y recomendaciones

* Números de muestras **muy grandes** (decenas de miles de millones) pueden tardar **muchos segundos o minutos**.
* Para una demo en clase, valores como `10 000 000` o `50 000 000` suelen ser un buen balance entre:

  * Tiempo razonable.
  * Diferencia clara entre secuencial y paralelo.
* Si obtienes un error de tipo `Error al reservar memoria`, revisa:

  * Que estés ejecutando el ejecutable correcto.
  * Que `NUM_HILOS` no sea enorme.
  * Que no hayas modificado el código para reservar arrays gigantes (en la versión recomendada solo se usan contadores, no se guardan todos los puntos).

