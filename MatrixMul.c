#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int **a, **b, **c, **d, **e;
int n, m, r;
#define MAXSIZE 10
#define MAXELEMENT 5
#define THREADSTACK  655360
struct data {
	int i;
	int j;
};
initializeoutput() {
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
void randominput() {
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
	initializeoutput();
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
	pthread_attr_t attrs;
	pthread_attr_init(&attrs);
	pthread_attr_setstacksize(&attrs, THREADSTACK);
	pthread_t id[n * r];
	int errCode[n * r];
	int cnt = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < r; ++j) {
			struct data *p = (struct data *) malloc(sizeof(struct data));
			p->i = i;
			p->j = j;
			//	printf("in main: %d %d \n", p->i, p->j);
			if ((errCode[cnt] = pthread_create(&id[cnt], &attrs, elementrunner,
					p)) != 0)
				printf("error creating thread %d, error=%d\n", i, errCode[i]);
			/*else
			 printf("created thread %d\n",cnt);*/
			cnt++;
		}
	}
	pthread_attr_destroy(&attrs);
	for (int i = 0; i < n * m; i++) {
		//printf("%d:%d\n", n * m, i);
		if (errCode[i] == 0) {
			errCode[i] = pthread_join(id[i], NULL);
			if (errCode[i] != 0)
				printf("error joining thread %d, error=%d\n", i,
						strerror(errCode[i]));
			/*else{
			 printf("join thread %d\n",i);
			 }*/
		}
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
void readFile(char*s1,char*s2) {
	FILE *fp = fopen(s1, "r");
    if (fp == 0)
       {
           fprintf(stderr, "failed to open %s\n",s1);
           exit(1);
       }
	int i = 0, j = 0;
	size_t count;
	int numbers[1000][1000];
	int MAXLINE=2000000;
	char *line = malloc(MAXLINE);
	int x,offset;
	printf("reading A from :%s\n", s1);
	while (fgets(line, MAXLINE, fp) != NULL) {
		//printf("%s\n", line);
		j=0;
		while (sscanf(line, "%d%n", &numbers[i][j],&offset) ==1){
			j++;
			line+=offset;
		}
		i++;
	}
	n=i;
	m=j;
	printf("%d %d\n",n,m );
	a= malloc(n * sizeof(int*));
	for (int ii = 0; ii < n; ++ii) {
		a[ii] = malloc(m * sizeof(int));
	}		
	for (int ii  = 0; ii< n; ++ii)
	{
			for (int jj = 0; jj < m; ++jj)
			{
				a[ii][jj]=numbers[ii][jj];
			}
	}
	printf("reading B from :%s\n", s2);
	FILE *fp2 = fopen(s2, "r");
    if (fp2 == 0)
       {
           fprintf(stderr, "failed to open %s\n",s2);
           exit(1);
       }
	i = 0;
	j = 0;
	while (fgets(line, 1000, fp2) != NULL) {
		j=0;
		while (sscanf(line, "%d%n", &numbers[i][j],&offset) ==1){
			j++;
			line+=offset;
		}
		i++;
	}
	fclose(fp2);
	r=j;
	b= malloc(m* sizeof(int*));
	for (int ii = 0; ii < m; ++ii) {
		b[ii] = malloc(r * sizeof(int));
	}
	for (int ii  = 0; ii< m; ++ii)
	{
			for (int jj = 0; jj < r; ++jj)
			{
				b[ii][jj]=numbers[ii][jj];
			}
	}
	printf("%d %d\n",m,r );
	initializeoutput();
}
int check(int**x,int**y){
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < r; ++j)
		{
			if(x[i][j]!=y[i][j]){
				return 0;
			}
		}
	}
	return 1;
}
int main(int argc, char *argv[]){
	//freopen("o.txt","w",stdout);
	if(argc!=2){
		randominput();
	}else{
		readFile(argv[0],argv[1]);
	}
	clock_t t;
	t = clock();
	nonThreadedMatMult();
	t = clock() - t;
	double time_taken1 = ((double) t) / CLOCKS_PER_SEC;
	t = clock();
	threadedMatMultPerElement();
	t = clock() - t;
	double time_taken2 = ((double) t) / CLOCKS_PER_SEC;
	t = clock();
	threadedMatMultPerRow();
	t = clock() - t;
	double time_taken3 = ((double) t) / CLOCKS_PER_SEC;
	// printMatrix(c, n, r);
	 if(check(c,d)){
	 	printf("threadedMatMultPerElement:correct \n");
	 }else{
	 	printf("threadedMatMultPerElement:wrong\n");
	 }
	 if(check(c,e)){
	 	printf("threadedMatMultPerRow:correct \n");
	 }else{
	 	printf("threadedMatMultPerRow:wrong\n");
	 }
	printf("nonthreaded took %f seconds to execute \n", time_taken1);
	printf("threaded per element took %f seconds to execute \n", time_taken2);
	printf("threaded per row took %f seconds to execute \n", time_taken3);
	return EXIT_SUCCESS;
}

