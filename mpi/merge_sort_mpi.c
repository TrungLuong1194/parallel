#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define SIZE 10000
#define MASTER 0

void merge(int *, int, int, int);
void mergeSort(int *, int, int);
void writeToFile(int *, int, FILE *);

int main(int argc, char** argv) {
    
    double start1, end1;
    clock_t start2, end2;
    FILE *fp;

    fp = fopen("output_mpi.txt", "a+");

    // Allocate and initialize random data for array
    int *inputArray = malloc(SIZE * sizeof(int));

    for (int i=0; i<SIZE; i++) {
        inputArray[i] = rand() % 1000;
    }

    // Initialize the environment MPI
    int proc_id, num_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Divide the array in chunks
    int size = SIZE / num_procs;

    if (proc_id == MASTER) {
        fprintf(fp, "\n\n");
        fprintf(fp, "Size: %d\n", SIZE);
        fprintf(fp, "\n");

        // fprintf(fp, "Given array is:\n");
        // fprintf(fp, "\n");
        // writeToFile(inputArray, SIZE, fp);
        // fprintf(fp, "\n");
    }

    // Send each subarray to each process
    int *subArray = malloc(size * sizeof(int));
    MPI_Scatter(inputArray, size, MPI_INT, subArray, size, MPI_INT, MASTER, MPI_COMM_WORLD);

    start1 = MPI_Wtime();

    // Perform the merge sort on each process
    mergeSort(subArray, 0, size-1);

    end1 = MPI_Wtime();

    // Gather the sorted subarrays into one
    int *sortedArray = NULL;

    if (proc_id == MASTER) {
        sortedArray = malloc(SIZE * sizeof(int));
    }

    MPI_Gather(subArray, size, MPI_INT, sortedArray, size, MPI_INT, MASTER, MPI_COMM_WORLD);

    // Perform the merge sort for final array
    if (proc_id == MASTER) {
        start2 = clock();

        mergeSort(sortedArray, 0, SIZE-1);

        end2 = clock();

        // fprintf(fp, "Sorted array is:\n");
        // fprintf(fp, "\n");
        // writeToFile(sortedArray, SIZE, fp);
        // fprintf(fp, "\n");
    }

    // Release memory
    free(inputArray);
    free(subArray);
    free(sortedArray);

    if (proc_id == MASTER) {
        fprintf(fp, "Time to execute: %f\n", (end1 - start1) + (double)(end2 - start2) / CLOCKS_PER_SEC);
    }

    // Finalize MPI
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

	return 0;
}


void merge(int *arr, int left, int middle, int right) {

    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Allocate memory for temporary arrays
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    // Copy data to temporary arrays
    for (int i=0; i<n1; i++) {
        L[i] = arr[left + i];
    }

    for (int j=0; j<n2; j++) {
        R[j] = arr[middle + 1 + j];
    }

    // Merge the temp arrays back into arr[]
    int i, j, k;

    i = 0; //Initial index of first subarray
    j = 0; //Initial index of second subarray
    k = left; //Initial index of merged array

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }

        k++;
    }

    // Copy the remaining elements of L[]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[]
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // Release memory
    free(L);
    free(R);
}


void mergeSort(int *arr, int left, int right) {

    if (left < right) {
        // Same as (left+right)/2, but avoid overflow for large left and right
        int middle = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        merge(arr, left, middle, right);
    }
}


void writeToFile(int *arr, int size, FILE *fp) {

    for (int i=0; i<size; i++) {
        fprintf(fp, "%5d\t", arr[i]);
        if ((i+1) % 20 == 0) {
            fprintf(fp, "\n");
        }
    }
    fprintf(fp, "\n");
}
