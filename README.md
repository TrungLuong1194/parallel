# parallel

#### Run Serial
- gcc -o merge_sort_original merge_sort_original.c
- ./merge_sort_original

#### Run MPI
- mpicc -o merge_sort_mpi merge_sort_mpi.c
- mpirun -np 4 ./merge_sort_mpi

#### Merge Sort
- https://en.wikipedia.org/wiki/Merge_sort
- https://www.geeksforgeeks.org/merge-sort/
