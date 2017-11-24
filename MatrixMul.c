/*
 ============================================================================
 Name        : MatrixMul.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
int **a, **b, **c;
int n, m, r;
#define MAXSIZE 5
#define MAXELEMENT 5
void initialize() {
	n = 1+rand() % MAXSIZE;
	m = 1+rand() % MAXSIZE;
	r= 1+rand() % MAXSIZE;
	a = malloc(n * sizeof(int*));
	for (int i = 0; i < n; ++i) {
		a[i] = malloc(m * sizeof(int));
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			a[i][j] = rand() % MAXELEMENT;
		}
	}
	b = malloc(m * sizeof(int*));
	for (int i = 0; i < m; ++i) {
		b[i] = malloc(r * sizeof(int));
	}
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < r; ++j) {
			b[i][j] = rand() % MAXELEMENT;
		}
	}
	c = malloc(n * sizeof(int*));
	for (int i = 0; i < n; ++i) {
		c[i] = malloc(r * sizeof(int));
	}

}
void nonThreadedMatMult() {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < r; ++j) {
			c[i][j] = 0;
			for (int k = 0; k < m; ++k) {
				c[i][j] += (a[i][k] * b[k][j]);
			}
		}
	}
}
void printMatrix(int **x,int n,int m) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			printf("%d  ", x[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
int main(void) {
	initialize();
	nonThreadedMatMult();
	printf("%d\n",sizeof(b));
	printMatrix(a,n,m);
	printMatrix(b,m,r);
	printMatrix(c,n,r);
	return EXIT_SUCCESS;
}
