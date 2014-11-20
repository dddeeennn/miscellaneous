#include <omp.h>
#include <stdio.h>
#include <Windows.h>


void computePi(long n, int threads);
void computePi2(long n, int threads);

int main (){

	computePi2(100,1);
	computePi2(1000000000,1);

	for(int i=2;i<13;i=i+2){
		computePi2(100,i);
		computePi2(1000000000,i);
	}

	getchar();
	return 0;
	//	n			threads		time 
	//100			1			0.007124 sec.

}

void computePi2(long n, int threads){

	
	long int i = 0;

	double pi = 0;
	double x = 0;

	omp_set_num_threads(threads);

double start, stop;
	start = omp_get_wtime();
#pragma omp parallel private(x)
	{
#pragma omp for
		for(i=1; i<n; i++) {
			x = (i+0.5)/n;
#pragma omp critical 
			pi+=4/(1+x*x);	
		}
	}
	stop = omp_get_wtime();
	printf("%d\t \t %d\t   %f sec.\n",n,threads, stop-start);
}

void computePi(long n, int threads){
	printf("compute %d in %d treads \n",n,threads);

	double start, stop;
	start = omp_get_wtime();

	long int i = 0;

	double  k = 0;

	double pi = 0;
	double x = 0;

	omp_set_num_threads(threads);

#pragma omp parallel reduction(+:pi) private(x,k)
	{
		k=0;
#pragma omp for
		for(i=1; i<n; i++) {
			x = (i+0.5)/n;
			pi+=4/(1+x*x);	
		}
	}
	//printf("%0.10f \n",pi/n);

	stop = omp_get_wtime();
	printf("%d\t %d\t \t %f sec.\n",n,threads, stop-start);
}


//lr 2
//#pragma omp parallel
//		{
//			printf("hello, world!");		
//			getchar();
//		}


// lr 3
//int count = 4;
//
//		omp_set_num_threads(count);
//		#pragma omp parallel
//		{
//
//			printf("I am %d thread from %d threads!\n", 
//			omp_get_thread_num(),omp_get_num_threads());		
//			getchar();
//
//		}


//lr 4
//int rank;

//omp_set_num_threads(2);
//#pragma omp parallel private(rank)
//{
//	rank = omp_get_thread_num();
//	printf("I am %d thread.\n", rank);
//	Sleep(100);
//	getchar();

//}

// lr 5
//int main (){
//
//	int n = 4;
//
//	int sum=0;
//
//	omp_set_num_threads(2);
//	#pragma omp parallel reduction(+:sum)
//	{
//		if (omp_get_thread_num() == 0) { 
//			for(int i = 0;i<=n/2;i++)
//			{
//				sum +=i; 
//			}
//			printf("%d \n",sum);
//		}
//		else
//		{
//			for(int i = n/2+1;i<=n;i++)
//			{
//				sum +=i; 
//			}
//				printf("%d \n",sum);
//		}
//	}
//		printf("%d \n",sum);
//		getchar();
//	return 0;
//}

// lr 5
//int main (){
//
//	int n = 4;
//
//	int k = 2;
//
//	int sum=0;
//
//	omp_set_num_threads(k);
//	
//	#pragma omp parallel reduction(+:sum)
//	{
//		for(int i =omp_get_thread_num()*n/k+1 ;i<=omp_get_thread_num()*n/k+n/k;i++)
//		{
//			sum +=i; 
//		}
//		printf("%d \n",sum);
//	}
//
//	printf("%d \n",sum);
//	getchar();
//	return 0;
//}

//
// lr 6
//int main (){
//
//	int n = 2;
//
//	int k = 2;
//
//	int sum=0;
//
//	omp_set_num_threads(k);
//	
//	#pragma omp for
//	for(int i=1; i<=n; i++) {
//		sum+=i;	
//	}
//
//	printf("%d \n",sum);
//	getchar();
//	return 0;
//}


// lr 7
//int n = 4;
//
//int k = 3;
//
//int sum=0;
//
//omp_set_num_threads(k);
//
//#pragma omp parallel reduction(+:sum)
//{
//
//	#pragma omp for schedule(static)
//	for(int i=1; i<=n; i++) {
//		sum+=i;	
//		printf("%d: calculation of the iteration number <%d>;. \n", omp_get_thread_num(),i);
//	}
//}
//printf("%d \n",sum);
//getchar();
//return 0;

// lr 8
//int main (){
//
//	long int i = 0;
//
//	long n = 1000000000;
//
//	double  k = 0;
//
//	double pi = 0;
//	double x = 0;
//
//	omp_set_num_threads(8);
//
//	#pragma omp parallel reduction(+:pi) private(x,k)
//	{
//		k=0;
//		#pragma omp for
//		for(i=1; i<n; i++) {
//			x = (i+0.5)/n;
//			pi+=4/(1+x*x);	
//		}
//	}
//	printf("%0.10f",pi/n);
//	getchar();
//	return 0;
//}

//lr 9
//	int i,j,k;
//	const int dim =3 ;
//	int threadCount =dim ;
//
//
//	int m1[dim][dim];
//	int m2[dim][dim];
//	int result[dim][dim];
//
//	m1[0][0] = 1;
//	m1[0][1] = 3;
//	m1[0][2] = 2;
//	m1[1][0] = 4;
//	m1[1][1] = 8;
//	m1[1][2] = 3;
//	m1[2][0] = 5;
//	m1[2][1] = 2;
//	m1[2][2] = 1;
//
//	m2[0][0] = 5;
//	m2[0][1] = 4;
//	m2[0][2] = 1;
//	m2[1][0] = 2;
//	m2[1][1] = 3;
//	m2[1][2] = 0;
//	m2[2][0] = 4;
//	m2[2][1] = 6;
//	m2[2][2] = 2;
//
//
//	omp_set_num_threads(threadCount);
//#pragma omp parallel  
//	{
//#pragma omp for private(i,j,k)
//		for(i=0; i<dim;i++)
//			for(j = 0;j<dim;j++)
//			{
//				result[i][j] = 0;
//
//				for(k=0;k<dim;k++)
//					result[i][j] = result[i][j]+m1[i][k]*m2[k][j];
//
//				printf("I am %d thread from %d threads! Result is %d, i = %d , j = %d \n", omp_get_thread_num(),omp_get_num_threads(),result[i][j],i,j);	
//
//			}
//	}
//
//	for(int i = 0;i<dim;i++)
//	{
//		for(int j = 0;j<dim;j++)
//			printf("%d \t",result[i][j]);
//		printf("\n");
//	}
//
//	getchar();
//	return 0;

//  lr 10
//omp_set_num_threads(4);
//
//	#pragma omp parallel
//	{
//		#pragma omp sections
//		{
//			#pragma omp section 
//			{ 
//				printf("%d thread came in section [1] \n", omp_get_thread_num()); 
//				Sleep(1000);
//			}
//			#pragma omp section 
//			{ 
//				printf("%d thread came in section [2] \n", omp_get_thread_num()); 
//				Sleep(1000);
//			} 
//			#pragma omp section 
//			{ 
//				printf("%d thread came in section [3] \n", omp_get_thread_num());
//				Sleep(1000);
//			} 
//		}
//		printf("%d: parallel region \n",omp_get_thread_num());
//	}
//	getchar();
//	return 0;


//lr 11
/*
int n = 10;

int k = 8;

int sum=0;

omp_set_num_threads(k);

#pragma omp parallel
{
printf("%d thread run \n",omp_get_thread_num());

#pragma omp for
for(int i=1; i<=n; i++) {
#pragma omp atomic
sum+=i;	
}
}
printf("%d \n",sum);
getchar();
return 0;*/

//lr 12
/*long int i = 0;

long n = 1000000;

double pi = 0;
double x = 0;

omp_set_num_threads(8);

#pragma omp parallel private(x)
{
#pragma omp for
for(i=1; i<n; i++) {
x = (i+0.5)/n;
#pragma omp critical 
pi+=4/(1+x*x);	
}
}
printf("%0.10f",pi/n);
getchar();
return 0;*/


//lr 13-1
//int i,j,k;
//	int max=10,min=-10;
//	const long n = 100;
//	int threadCount = 12 ;
//
//	
//	/*allocate dynamic memory for a[][]*/
//	int** a  = (int**) calloc(n,sizeof(int*));
//
//	for(int i =0; i<n;i++)
//		a[i] = (int*) calloc(n,sizeof(int));
//
//	for(int i =0;i<n;i++){
//		for(int j = 0;j<n;j++){
//			a[i][j]= (int) rand()%(max-min+1)+min;
//		}
//	}
//
//	printf("allocate memory for a array \n");
//
//	/*allocate dynamic memory for b[][]*/
//	int** b  = (int**) calloc(n,sizeof(int*));
//
//	for(int i =0; i<n;i++)
//		b[i] = (int*) calloc(n,sizeof(int));
//
//	for(int i =0;i<n;i++){
//		for(int j = 0;j<n;j++){
//			b[i][j]= (int) rand()%(max-min+1)+min;
//		}
//	}
//
//	
//	printf("allocate memory for b array \n");
//
//
//	/*allocate dynamic memory for c[][]*/
//	int** c  = (int**) calloc(n,sizeof(int*));
//
//	for(int i =0; i<n;i++)
//		c[i] = (int*) calloc(n,sizeof(int));
//
//	for(int i =0;i<n;i++){
//		for(int j = 0;j<n;j++){
//			c[i][j]= (int) rand()%(max-min+1)+min;
//		}
//	}
//	
//	
//	printf("allocate memory for c array \n");
//
//
//	/*output array a*/
//	/*for(int i =0;i<n;i++){
//		for(int j = 0;j<n;j++)
//		{
//			printf("%d \t",a[i][j]);
//		}
//		printf("\n");
//	}
//
//	printf("\n");*/
//
//	/*output array b*/
//	/*for(int i =0;i<n;i++){
//		for(int j = 0;j<n;j++)
//		{
//			printf("%d \t",b[i][j]);
//		}
//		printf("\n");
//	}*/
//	
//
//
//	double start, stop;
//	start = omp_get_wtime();
//
//
//	omp_set_num_threads(threadCount);
//#pragma omp parallel  
//	{
//#pragma omp for private(i,j,k)
//		for(i=0; i<n;i++)
//			for(j = 0;j<n;j++)
//			{
//				c[i][j] = 0;
//
//				for(k=0;k<n;k++)
//					c[i][j] = c[i][j]+a[i][k]*b[k][j];
//
//				//	printf("I am %d thread from %d threads! Result is %d, i = %d , j = %d \n", omp_get_thread_num(),omp_get_num_threads(),result[i][j],i,j);	
//
//			}
//	}
//
//	//for(int i = 0;i<dim;i++)
//	//{
//	//	for(int j = 0;j<dim;j++)
//	//		printf("%d \t",result[i][j]);
//	//	printf("\n");
//	//}
//	//
//	stop = omp_get_wtime();
//	printf("Computation time: %f sec.\n", stop-start);
//
//	getchar();
//	return 0;
//	//	n		threads		time 
//	//100		1			0.013675 sec.
//	//1000		1			49.987936 sec.	
//	//100		2			0.011937 sec.
//	//1000		2			26.537658 sec.
//	//100		4			0.010481 sec.
//	//1000		4			28.709030 sec.
//	//100		6			0.079155 sec.
//	//1000		6			27.915684 sec.
//	//100		8			0.096677 sec.
//	//1000		8			29.590833 sec.
//	//100		10			0.012850 sec.
//	//1000		10			26.669813 sec.
//	//100		12			0.039347 sec.
//	//1000		12			27.368623 sec.

//------------------------------------------
//100			 1               0.000053 sec.
//1000000000     1               30.486168 sec.
//100			 2               0.000904 sec.
//1000000000     2               16.295353 sec.
//100			 4               0.002429 sec.
//1000000000     4               17.366511 sec.
//100		     6               0.004737 sec.
//1000000000     6               15.812984 sec.
//100			 8               0.034517 sec.
//1000000000     8               15.786773 sec.
//100		     10              0.012908 sec.
//1000000000     10              16.363775 sec.
//100		     12              0.011784 sec.
//1000000000     12              16.357616 sec.

//-------------------------------------------------
//100              1         0.000062 sec.
//1000000000       1         112.796286 sec.
//100              2         0.000959 sec.
//1000000000       2         168.243689 sec.
//100              4         0.004170 sec.
//1000000000       4         172.010659 sec.
//100              6         0.067751 sec.
//1000000000       6         182.199439 sec.
//100              8         0.008566 sec.
//1000000000       8         181.105283 sec.
//100              10        0.012208 sec.
//1000000000       10        182.710204 sec.
//100              12        0.012790 sec.
//1000000000       12        176.871631 sec.