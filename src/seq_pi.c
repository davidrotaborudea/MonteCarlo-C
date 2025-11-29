// src/seq_pi.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/**
 * Devuelve el tiempo actual en segundos (double).
 */
double now_seconds(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
}

/**
 * Generador simple de números pseudoaleatorios [0, 1),
 * basado en un LCG. Cada hilo / ejecución tendrá su propio estado.
 */
static inline double lcg_rand01(unsigned int *state) {
    *state = (*state * 1103515245u + 12345u);
    unsigned int x = (*state >> 8) & 0x00FFFFFFu; // 24 bits "decentes"
    return (double)x / (double)0x01000000u;       // dividir por 2^24
}

/**
 * Estima pi usando Monte Carlo de forma secuencial.
 */
double estimate_pi_sequential(long long num_samples) {
    long long inside = 0;
    unsigned int seed = (unsigned int)time(NULL);

    for (long long i = 0; i < num_samples; ++i) {
        double x = lcg_rand01(&seed);
        double y = lcg_rand01(&seed);
        if (x * x + y * y <= 1.0) {
            inside++;
        }
    }

    return 4.0 * (double)inside / (double)num_samples;
}

int main(int argc, char *argv[]) {
    long long num_samples = 1000000; // valor por defecto

    if (argc >= 2) {
        num_samples = atoll(argv[1]);
        if (num_samples <= 0) {
            fprintf(stderr, "Uso: %s [num_samples > 0]\n", argv[0]);
            return 1;
        }
    }

    printf("Versión secuencial\n");
    printf("Muestras: %lld\n", num_samples);

    double t0 = now_seconds();
    double pi_est = estimate_pi_sequential(num_samples);
    double t1 = now_seconds();

    double elapsed = t1 - t0;

    printf("π ≈ %.6f\n", pi_est);
    printf("Tiempo: %.6f s\n", elapsed);

    return 0;
}
