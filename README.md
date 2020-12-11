# parallel

#### Run Serial
- gcc -o merge_sort merge_sort.c
- ./merge_sort

#### Run MPI
- mpicc -o merge_sort_mpi merge_sort_mpi.c
- mpirun -np 4 ./merge_sort_mpi

#### Run OMP
- gcc -o merge_sort_omp merge_sort_omp.c -fopenmp
- export OMP_NUM_THREADS=2
- ./merge_sort_omp

#### Merge Sort
- https://en.wikipedia.org/wiki/Merge_sort
- https://www.geeksforgeeks.org/merge-sort/
- https://www.christianbaun.de/CGC18/Skript/MPI_TASK_2_Presentation.pdf
- https://idre.ucla.edu/sites/default/files/intro-openmp-2013-02-11.pdf
- https://docs.microsoft.com/en-us/cpp/parallel/openmp/reference/openmp-directives?view=msvc-160#sections-openmp
- http://jakascorner.com/blog/2016/05/omp-sections.html
