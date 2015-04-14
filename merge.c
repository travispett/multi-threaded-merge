#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ARRAY_LEN 100

struct mystruct1 {
	int *arr1;
	int size1;
};

struct mystruct2 {
	int *arr2;
	int size2;
};

//Merge two sorted arrays, A with a integers and 
//B with b integers, into a sorted array C.
void merge (int *A, int a, int *B, int b, int *C) {
	int i,j,k;
	i = 0; 
	j = 0;
  	k = 0;
  	while (i < a && j < b) {
    		if (A[i] <= B[j]) {
	  	/* copy A[i] to C[k] and move the pointer i and k forward */
	  		C[k] = A[i];
	  		i++;
	  		k++;
    		} else {
      	/* copy B[j] to C[k] and move the pointer j and k forward */
      			C[k] = B[j];
      			j++;
      			k++;
    		}
  	}
  	/* move the remaining elements in A into C */
  	while (i < a) {
    		C[k]= A[i];
    		i++;
    		k++;
  	}
  	/* move the remaining elements in B into C */
  	while (j < b)  {
    		C[k]= B[j];
    		j++;
    		k++;
  	}
}  

//Sort array A with n integers, using merge-sort algorithm.
void * merge_sort(void *args) {
	struct mystruct1 *s1 = args;
	struct mystruct2 *s2 = args;
 	
	int *A = s1->arr1;
	int n = s1->size1;

	int i;
  	int *A1, *A2;
  	int n1, n2;
	pthread_t tid1, tid2;

  
	if (n >= 2) {
  		n1 = n / 2;   //the number of elements in A1
  		n2 = n - n1;  //the number of elements in A2
  		A1 = (int*)malloc(sizeof(int) * n1);
  		A2 = (int*)malloc(sizeof(int) * n2);
  
  		// move the first n/2 elements to A1
  		for (i =0; i < n1; i++) {
  			A1[i] = A[i];
  		}
  		// move the rest to A2
  		for (i = 0; i < n2; i++) {
  			A2[i] = A[i+n1];
  		}

		s1->arr1 = A1;
		s1->size1 = n1;
  		pthread_create(&tid1, NULL, merge_sort, s1);
		pthread_join(tid1, NULL);

		s2->arr2 = A2;
		s2->size2 = n2;
		pthread_create(&tid2, NULL, merge_sort, s2);
		pthread_join(tid2, NULL);	

  		merge(A1, n1, A2, n2, A);
  		free(A1);
  		free(A2);
	}
}

int main(int argc, char *argv[])
{
	FILE *input, *output;
  	int i, n, err;
  	int *A;
  	char c;
	pthread_t tid;

	if (argc != 3) {
		printf("usage: %s input_file output_file\n", argv[0]);
		exit(0);
	}
	
	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");

	if (input == NULL || output == NULL) {
		printf("file could not be opened\n");
		exit(0);
	}
	
  	A = (int *)malloc(sizeof(int) * 100);
	int counter = 0;
	while((fscanf(input, "%d", &(A[counter])) != EOF) && (counter < 100))
		counter++;
	n = counter; 

  	/* print the original array */
  	printf("************** Result **************\n");
  	printf("The input array is: ");
  	for (i = 0; i < n; i++) {
    		printf("%d ", A[i]);
  	}
  	printf("\n");

	struct mystruct1 *s = malloc(sizeof(struct mystruct1));
	s->arr1 = A;
	s->size1 = n;
  	
	pthread_create(&tid, NULL, merge_sort, s);
	pthread_join(tid, NULL);

  	// print the sorted array
  	printf("The sorted array is: ");
  	for (i = 0; i < n; i++) {
    		printf("%d ", A[i]);
  		fprintf(output, "%d ", A[i]);
	}

  	printf("\n");
  	free(A);
	fclose(input);
	fclose(output);
} 
