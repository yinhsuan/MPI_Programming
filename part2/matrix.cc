#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0

// Read size of matrix_a and matrix_b (n, m, l) and whole data of matrixes from stdin
//
// n_ptr:     pointer to n
// m_ptr:     pointer to m
// l_ptr:     pointer to l
// a_mat_ptr: pointer to matrix a (a should be a continuous memory space for placing n * m elements of int)
// b_mat_ptr: pointer to matrix b (b should be a continuous memory space for placing m * l elements of int)
void construct_matrices(int *n_ptr, int *m_ptr, int *l_ptr,
                        int **a_mat_ptr, int **b_mat_ptr) {
	int world_rank, world_size;
    int *ptr;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == MASTER) {
        scanf("%d %d %d", n_ptr, m_ptr, l_ptr);
	}
	MPI_Bcast(n_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(m_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(l_ptr, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int n = *n_ptr;
	int m = *m_ptr;
	int l = *l_ptr;

	*a_mat_ptr = (int*)calloc(n * m, sizeof(int));
	*b_mat_ptr = (int*)calloc(m * l, sizeof(int));

	if (world_rank == MASTER) {
		for (int i=0; i<(n * m); i++) {
			ptr = *a_mat_ptr + i;
			scanf("%d", ptr);
		}

		for (int i=0; i<(m * l); i++) {
			ptr = *b_mat_ptr + i;
			scanf("%d", ptr);
		}
	}
	MPI_Bcast(*a_mat_ptr, (n * m), MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(*b_mat_ptr, (m * l), MPI_INT, 0, MPI_COMM_WORLD);
    
}

// Just matrix multiplication (your should output the result in this function)
// 
// n:     row number of matrix a
// m:     col number of matrix a / row number of matrix b
// l:     col number of matrix b
// a_mat: a continuous memory placing n * m elements of int
// b_mat: a continuous memory placing m * l elements of int
void matrix_multiply(const int n, const int m, const int l,
                     const int *a_mat, const int *b_mat) {
    int world_rank, world_size;
    int start_row, end_row;
    int sum;
    int *c, *ans;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int count = n / world_size;
	
    c = (int*) calloc(n * l, sizeof(int));
	ans = (int*) calloc(n * l, sizeof(int));

	for (int i=0; i<(n * l); i++) {
		c[i] = 0;
		ans[i] = 0;
	}

    start_row = count * world_rank;
    end_row = (world_rank == world_size-1) ? n : (start_row + count);

    for (int i=start_row; i<end_row; i++) {
        for (int j=0; j<l; j++) {
            sum = 0;
            for (int k=0; k<m; k++) {
                sum += a_mat[i*m + k] * b_mat[j + k*l];
            }
            c[i*l + j] = sum;
        }
    }
    MPI_Reduce(c, ans, (n * l), MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(world_rank == MASTER){
        for (int i=0; i<n; i++) {
            for (int j=0; j<l; j++) {
                if (j) {
                    printf(" ");
                }
                printf("%d", ans[i*l + j]);
            }
			printf(" ");
            printf("\n");
        }
	}
	free(c);
	free(ans);
}

// Remember to release your allocated memory
void destruct_matrices(int *a_mat, int *b_mat) {
	free(a_mat);
	free(b_mat);
}
