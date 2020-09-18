#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void merge(int *, int, int, int);
void mergeSort(int *, int, int);
void printArray(int *, int);

#define MASTER 0
#define SIZE 20

int main(int argc, char** argv)
{
    /********************* Initialize MPI ********************/
    int myid, numprocs, i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    int *inputArray = malloc(SIZE * sizeof(int));
    int size;

    /***** Create data for array *****/
    if (myid == MASTER) {
        for (i=0; i<SIZE; i++) {
            inputArray[i] = rand()%100;
        }

        printf("Given array is:\n");
        printArray(inputArray, SIZE);
        printf("\n");

        /***** Divide the array in chunks *****/
        size = SIZE/numprocs;
    }

    /***** Send size to all processes *****/
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /***** Send each subarray to each process *****/
    int *subArray = malloc(size * sizeof(int));
    MPI_Scatter(inputArray, size, MPI_INT, subArray, size, MPI_INT, MASTER, MPI_COMM_WORLD);

    /***** Show data in each process *****/
    printf("\ndata in process[%d]\n", myid);
    printArray(subArray, size);

    /***** Impliment merge sort on each process *****/
    mergeSort(subArray, 0, size-1);

    /***** Gather the sorted subarrays into one *****/
    int *sortedArray = NULL;

    if (myid == MASTER) {
        sortedArray = malloc(SIZE * sizeof(int));
    }

    MPI_Gather(subArray, size, MPI_INT, sortedArray, size, MPI_INT, MASTER, MPI_COMM_WORLD);

    /***** Impliment merge sort for final array *****/
    if (myid == MASTER) {
        mergeSort(sortedArray, 0, SIZE-1);

        /***** Display the sorted array *****/
        printf("\n\n");
        printf("Sorted array is:\n");
        printArray(sortedArray, SIZE);
    }

    /***** Release memory *****/
    free(inputArray);
    free(subArray);
    free(sortedArray);

    /***** Finalize MPI *****/
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

	return 0;
}

/********************* Merge Function ********************/
void merge(int *arr, int left, int middle, int right) {
	int i, j, k;
	int n1 = middle - left + 1;
	int n2 = right - middle;

	/***** Allocate memory for temporary arrays *****/
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

	/***** Copy data to temp arrays *****/
	for (i=0; i<n1; i++) {
		L[i] = arr[left + i];
	}

	for (j=0; j<n2; j++) {
		R[j] = arr[middle + 1 + j];
	}

    /***** Merge the temp arrays back into arr[] *****/
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

    /***** Copy the remaining elements of L[] *****/
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    /***** Copy the remaining elements of R[] *****/
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    /***** Release memory *****/
    free(L);
    free(R);
}

/********************* Recusive Merge Function ********************/
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

/********************* Print Array Function ********************/
void printArray(int *A, int size) {
    int i;

    for (i=0; i<size; i++) {
        printf("%d\t", A[i]);
    }
    printf("\n");
}
