#include <stdio.h>
#include <stdlib.h>
int **a, **b, **c, **d, **e;
int n, m, r;
#define MAXSIZE 10
#define MAXELEMENT 5
struct data {
	int i;
	int j;
};
void initialize() {
	n = 1 + rand() % MAXSIZE;
	m = 1 + rand() % MAXSIZE;
	r = 1 + rand() % MAXSIZE;
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
	d = malloc(n * sizeof(int*));
	for (int i = 0; i < n; ++i) {
		d[i] = malloc(r * sizeof(int));
	}
	e = malloc(n * sizeof(int*));
	for (int i = 0; i < n; ++i) {
		e[i] = malloc(r * sizeof(int));
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
void printMatrix(int **x, int n, int m) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			printf("%d  ", x[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
void elementrunner(void *param) {
	struct data *p = param; // the structure that holds our data
	int sum = 0;
	//Row multiplied by column
	for (int k = 0; k < m; ++k) {
		//printf("in thread:%d %d \n",p->i,p->j);
		sum += (a[p->i][k] * b[k][p->j]);
	}
	//assign the sum to its coordinate
	d[p->i][p->j] = sum;
	//Exit the thread
	pthread_exit(0);
}
void threadedMatMultPerElement() {
	pthread_t id[n * r];
	int cnt = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < r; ++j) {
			struct data *p = (struct data *) malloc(sizeof(struct data));
			p->i = i;
			p->j = j;
			//	printf("in main: %d %d \n",p->i,p->j);
			pthread_create(&id[cnt++], NULL, elementrunner, p);
		}
	}
	for (int i = 0; i < n * m; ++i) {
		pthread_join(id[i], NULL);
	}
}
void rowrunner(int row) {
	for (int j = 0; j < r; ++j) {
		e[row][j] = 0;
		for (int k = 0; k < m; ++k) {
			e[row][j] += (a[row][k] * b[k][j]);
		}
	}

	pthread_exit(0);
}
void threadedMatMultPerRow() {
	pthread_t id[n];
	int cnt = 0;
	for (int i = 0; i < n; ++i) {
		pthread_create(&id[cnt++], NULL, rowrunner, i);
	}
	for (int i = 0; i < n; ++i) {
		pthread_join(id[i], NULL);
	}
}
int main(void) {
	initialize();
	nonThreadedMatMult();
	printMatrix(a, n, m);
	printMatrix(b, m, r);
	printMatrix(c, n, r);
	threadedMatMultPerElement();
	printMatrix(d, n, r);
	threadedMatMultPerRow();
	printMatrix(e, n, r);
	return EXIT_SUCCESS;
}
