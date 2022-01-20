
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define N 1000
#define K 15

float TimeArrival(float lambda)
{
    float u = (float)((long double)rand()/(RAND_MAX+1));
    if(lambda > 0)
        return -1/lambda*log(1-u);
    return -1;
}

void set_int_vec(int vec[], int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        vec[i] = 0;
    }
}

void set_float_vec(float vec[], int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        vec[i] = 0;
    }
}

int sum_int_vec(int vec[], int n)
{
    if(n==1)
        return vec[0];
    return sum_int_vec(vec, n-1)+vec[n-1];
}

int initialise_system(int n, int delta_x[]) {
    int i;
    for(i=0;i<n;i++)
    {
        delta_x[i] = 0;
    }
    return 0;
}

int next_step(int n, int dx[])
{
    int aux = rand()%n;
    
    if(dx[aux]>0)
    {
        dx[aux]--;
        dx[(aux+1)%n]++;
        return -1;
    }
    if(dx[aux]<0)
    {
        dx[aux]++;
        dx[(aux+n-1)%n]--;
        return 1;
    }
    if(rand()%2==0)
    {
        dx[aux]--;
        dx[(aux+1)%n]++;
    }
    else
    {
        dx[aux]++;
        dx[(aux+n-1)%n]--;
    }
    return 0;
}

void make_hist(int n, int dx[], int histp[K][N], int histn[K][N])
{
    int i, auxp[K], auxn[K];
    set_int_vec(auxp, K);
    set_int_vec(auxn, K);
    
    for(i=0;i<n;i++)
    {
        if(dx[i]>=0 && dx[i]<K)
            auxp[dx[i]]++;
        if(dx[i]<=0 && dx[i]>-K)
            auxn[-dx[i]]++;
//        if(dx[i]>=K || dx[i]<=-K)
//            printf("Warning! dx[%d]>=K\n", i);
    }
    
    for(i=0;i<K;i++)
    {
        histp[i][auxp[i]]++;
        histn[i][auxn[i]]++;
    }
    
}

void print_hist(int n, int histp[K][N], int histn[K][N], int samples)
{
    int i, j;
    for(i=-K+1;i<0;i++)
    {
        printf("\n[%3d] ", i);
        for(j=0;j<14&&j<n;j++)
        {
            printf("%.2f ", (float)histn[-i][j]/samples);
        }
    }
    for(i=0;i<K;i++)
    {
        printf("\n[%3d] ", i);
        for(j=0;j<14&&j<n;j++)
        {
            printf("%.2f ", (float)histp[i][j]/samples);
        }
    }
}

int run_detailed_system(int n, int steps, int samples)
{
    int i, j, aux=0, sum=0, delta_x[n];
    int histp[K][n], histn[K][n];
    int avgp_x[K], avgn_x[K];
    
    set_int_vec(delta_x, n);
    set_int_vec(avgp_x, K);
    set_int_vec(avgn_x, K);
    for(i=0;i<K;i++)
    {
        for(j=0;j<n;j++)
        {
            histp[i][j]=0;
            histn[i][j]=0;
        }
    }
    
    printf("\nN = %d\nSteps = %.0e\nSample = %.0e\n\n", n, (double)steps, (double)samples);
    
    for(i=0;i<steps;i++)
        aux += next_step(n, delta_x);
    
    for(i=0;i<samples;i++)
    {
        aux += next_step(n, delta_x);
        make_hist(n, delta_x, histp, histn);
    }
    
    for(i=0;i<n;i++)
        sum += delta_x[i];
    if(sum!=0)
        printf("Sum of delta_x != 0\n");
    
    for(i=0;i<K;i++)
    {
        for(j=0;j<n;j++)
        {
            avgp_x[i] += j*histp[i][j];
            avgn_x[i] += j*histn[i][j];
        }
    }
    
    for(i=-K+1;i<0;i++)
        printf("<x>[%3d] = %f\n", i, (float)avgn_x[-i]/samples/n);
    for(i=0;i<K;i++)
        printf("<x>[%3d] = %f\n", i, (float)avgp_x[i]/samples/n);

    FILE* data = fopen("QueueDeposition_Detailed.txt", "w");
    fprintf(data, "\nN	%d\nSteps	%.0e\nSample	%.0e\n\n", n, (double)steps, (double)samples);
    for(i=-K+1;i<0;i++)
        fprintf(data, "%d	%f\n", i, (float)avgn_x[-i]/samples/n);
    for(i=0;i<K;i++)
        fprintf(data, "%d	%f\n", i, (float)avgp_x[i]/samples/n);
    fclose(data);

    return 0;
}


int run_system(int n, int steps, int samples)
{
    int i, aux=0, delta_x[n];
    
    printf("\nN = %d\nSteps = %.0e\nSample = %.0e\n\n", n, (double)steps, (double)samples);
    
    set_int_vec(delta_x, n);
    
    for(i=0;i<steps;i++)
        aux += next_step(n, delta_x);
    
    for(i=0;i<samples;i++)
    {
        aux += next_step(n, delta_x);
    }
    
    
    return 0;
}


int main(void)
{
    int m;
    float mu;
    srand(time(NULL));

    run_detailed_system(1000, 100000, 1000000);
       
    getchar();
    return 0;
}
