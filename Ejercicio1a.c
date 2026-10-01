/* Ejercicio 1, caso 1: dos temperaturas por proceso con MPI_Gather. */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    float temperaturas_locales[2];
    float temperaturas_recibidas[8];  /* 4 procesos x 2 mediciones */

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0) {
            fprintf(stderr, "Este programa requiere exactamente 4 procesos.\n");
        }
        MPI_Finalize();
        return 1;
    }

    /* El indice de cada fila coincide con el rank del proceso. */
    const float mediciones[4][2] = {
        {24.5f, 25.0f},
        {26.1f, 26.4f},
        {23.8f, 24.1f},
        {27.0f, 27.3f}
    };
    temperaturas_locales[0] = mediciones[rank][0];
    temperaturas_locales[1] = mediciones[rank][1];

    printf("Proceso %d: temperaturas registradas = %.1f C, %.1f C\n",
           rank, temperaturas_locales[0], temperaturas_locales[1]);

    /* Una sola llamada reúne dos valores consecutivos de cada proceso. */
    MPI_Gather(temperaturas_locales, 2, MPI_FLOAT,
               temperaturas_recibidas, 2, MPI_FLOAT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nOficina Central: temperaturas recibidas\n");
        for (int i = 0; i < size; i++) {
            printf("Proceso %d: %.1f C, %.1f C\n", i,
                   temperaturas_recibidas[2 * i],
                   temperaturas_recibidas[2 * i + 1]);
        }
    }

    MPI_Finalize();
    return 0;
}
