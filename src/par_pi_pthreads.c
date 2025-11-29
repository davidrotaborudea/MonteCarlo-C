#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

double now_seconds(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
}

static inline double lcg_rand01(unsigned int *state) {
    *state = (*state * 1103515245u + 12345u);
    unsigned int x = (*state >> 8) & 0x00FFFFFFu;
    return (double)x / (double)0x01000000u;
}

typedef struct {
    long long num_samples;
    unsigned int seed;
    long long inside;
} ThreadData;

void *worker(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    long long inside = 0;
    unsigned int state = data->seed;

    for (long long i = 0; i < data->num_samples; ++i) {
        double x = lcg_rand01(&state);
        double y = lcg_rand01(&state);
        if (x * x + y * y <= 1.0) {
            inside++;
        }
    }

    data->inside = inside;
    return NULL;
}

int main(int argc, char *argv[]) {
    long long num_samples = 1000000;
    int num_threads = 0;

    if (argc >= 2) {
        num_samples = atoll(argv[1]);
        if (num_samples <= 0) {
            fprintf(stderr, "num_samples debe ser > 0\n");
            return 1;
        }
    }

    if (argc >= 3) {
        num_threads = atoi(argv[2]);
    }

    if (num_threads <= 0) {
        long nprocs = sysconf(_SC_NPROCESSORS_ONLN);
        if (nprocs < 1) nprocs = 2;
        num_threads = (int)nprocs;
    }

    if (num_samples < num_threads) {
        num_threads = (int)num_samples;
    }

    if (num_threads <= 0) {
        num_threads = 1;
    }

    printf("Versión paralela (pthreads)\n");
    printf("Muestras totales: %lld\n", num_samples);
    printf("Hilos: %d\n", num_threads);

    pthread_t *threads = malloc((size_t)num_threads * sizeof(pthread_t));
    ThreadData *data = malloc((size_t)num_threads * sizeof(ThreadData));

    if (!threads || !data) {
        fprintf(stderr, "Error al reservar memoria\n");
        free(threads);
        free(data);
        return 1;
    }

    long long base = num_samples / num_threads;
    long long remainder = num_samples % num_threads;

    double t0 = now_seconds();

    for (int i = 0; i < num_threads; ++i) {
        data[i].num_samples = base + (i < remainder ? 1 : 0);
        data[i].inside = 0;

        unsigned int seed = (unsigned int)time(NULL);
        seed ^= (unsigned int)(i * 2654435761u);
        data[i].seed = seed;

        int rc = pthread_create(&threads[i], NULL, worker, &data[i]);
        if (rc != 0) {
            fprintf(stderr, "Error al crear hilo %d\n", i);
            num_threads = i;
            break;
        }
    }

    long long inside_total = 0;
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
        inside_total += data[i].inside;
    }

    double t1 = now_seconds();
    double elapsed = t1 - t0;

    double pi_est = 4.0 * (double)inside_total / (double)num_samples;

    printf("π ≈ %.6f\n", pi_est);
    printf("Tiempo total: %.6f s\n", elapsed);

    free(threads);
    free(data);

    return 0;
}
