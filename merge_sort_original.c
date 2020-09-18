#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void merge(int *, int, int, int);
void mergeSort(int *, int, int);
void printArray(int *, int);

#define SIZE 200

int main(int argc, char** argv)
{
    int i;

    /***** Start time *****/
    struct timeval tvalBefore, tvalAfter;
    gettimeofday(&tvalBefore, NULL);

    /***** Create data for array *****/
    int *inputArray = malloc(SIZE * sizeof(int));

    for (i=0; i<SIZE; i++) {
        inputArray[i] = rand()%100;
    }

    printf("Given array is:\n");
    printArray(inputArray, SIZE);
    printf("\n");

    /***** Impliment merge sort on each process *****/
    mergeSort(inputArray, 0, SIZE-1);

    /***** Display the sorted array *****/
    printf("Sorted array is:\n");
    printArray(inputArray, SIZE);

    /***** End time *****/
    gettimeofday(&tvalAfter, NULL);
    long tm = (tvalAfter.tv_sec - tvalBefore.tv_sec) * 1000000L + tvalAfter.tv_usec - tvalBefore.tv_usec;

    printf("\nSerial execution time: %ld microseconds\n", tm);

    /***** Release memory *****/
    free(inputArray);

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
