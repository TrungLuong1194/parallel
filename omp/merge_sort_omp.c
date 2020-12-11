#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define SIZE 100000

void merge(int *, int, int, int);
void mergeSort(int *, int, int);
void writeToFile(int *, int, FILE *);


int main(int argc, char** argv) {

    double start, end;
    FILE *fp;

    fp = fopen("output_omp.txt", "a+");

    // Allocate and initialize random data for array
    int *inputArray = malloc(SIZE * sizeof(int));

    for (int i=0; i<SIZE; i++) {
        inputArray[i] = rand() % 1000;
    }

    fprintf(fp, "\n\n");
    fprintf(fp, "Size: %d\n", SIZE);
    fprintf(fp, "\n");

    // fprintf(fp, "Given array is:\n");
    // fprintf(fp, "\n");
    // writeToFile(inputArray, SIZE, fp);
    // fprintf(fp, "\n");

    start = omp_get_wtime();

    // Perform the merge sort
    mergeSort(inputArray, 0, SIZE-1);

    end = omp_get_wtime();

    // fprintf(fp, "Sorted array is:\n");
    // fprintf(fp, "\n");
    // writeToFile(inputArray, SIZE, fp);
    // fprintf(fp, "\n");

    fprintf(fp, "Time to execute: %f\n", end - start);

    // Release memory
    free(inputArray);

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

        #pragma omp parallel sections num_threads(2)
        {

            // Sort first and second halves
            #pragma omp section
            {
                mergeSort(arr, left, middle);
            }

            #pragma omp section
            {
                mergeSort(arr, middle + 1, right);
            }
        }

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
