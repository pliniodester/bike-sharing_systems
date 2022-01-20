
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define N 1000
#define K 15

float exp_time(float lambda)
{
    float u = (float)((long double)rand()/(RAND_MAX+1));
    if(lambda > 0)
        return -1/lambda*log(1-u);
    return 100000;
}

float time_arrival(int i, int dx[], float dB, int n)
{
    if( dx[i]>0&&dx[(i+n-1)%n]<0 )
        return exp_time(1-4*dB);
    if( (dx[i]==0&&dx[(i+n-1)%n]<0)||(dx[i]>0&&dx[(i+n-1)%n]==0) )
        return exp_time(1-3*dB);
    if( (dx[i]==0&&dx[(i+n-1)%n]==0)||(dx[i]>0&&dx[(i+n-1)%n]>0)||(dx[i]<0&&dx[(i+n-1)%n]<0) )
        return exp_time(1-2*dB);
    if( (dx[i]==0&&dx[(i+n-1)%n]>0)||(dx[i]<0&&dx[(i+n-1)%n]==0) )
        return exp_time(1-dB);
    if( dx[i]<0&&dx[(i+n-1)%n]>0 )
        return exp_time(1);
    return NAN;
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

int next_step(int n, int dx[], float dB)
{
    int i, ind;
    float min_t, aux_t;
    
    ind = 0;
    min_t = time_arrival(0, dx, dB, n);
    
    for(i=1;i<n;i++)
    {
        aux_t = time_arrival(i, dx, dB, n);
        if(aux_t < min_t)
        {
            min_t = aux_t;
            ind = i;
        }
    }
    dx[ind]++;
    dx[(ind+n-1)%n]--;
    return i;
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

int run_detailed_system(int n, float dB, int steps, int samples)
{
    int i, j, aux=0, sum=0, delta_x[n];
    
    int histp[K][n], histn[K][n];
    int avgp_x[K], avgn_x[K];
    float devp_x[K], devn_x[K];
    
    set_int_vec(delta_x, n); set_int_vec(avgp_x, K); set_int_vec(avgn_x, K);
    set_float_vec(devp_x, K); set_float_vec(devn_x, K);
    
    for(i=0;i<K;i++)
    {
        for(j=0;j<n;j++)
        {
            histp[i][j]=0;
            histn[i][j]=0;
        }
    }
    
    printf("\nN = %d\nr = %.2f\nSteps = %.0e\nSample = %.0e\n\n", n, (1-3*dB)/(1-dB), (double)steps, (double)samples);
    
    for(i=0;i<steps;i++)
        aux += next_step(n, delta_x, dB);
    
    for(i=0;i<samples;i++)
    {
        aux += next_step(n, delta_x, dB);
        make_hist(n, delta_x, histp, histn);
    }
    
    for(i=0;i<n;i++)
        sum += delta_x[i];
    if(sum!=0)
        printf("WARNING: Sum of delta_x = %d\n\n", sum);
    
    for(i=0;i<K;i++)
    {
        for(j=0;j<n;j++)
        {
            avgp_x[i] += j*histp[i][j];
            avgn_x[i] += j*histn[i][j];
        }
    }
    for(i=0;i<K;i++)
    {
        for(j=0;j<n;j++)
        {
            devp_x[i] += (j-(float)avgp_x[i]/samples)*(j-(float)avgp_x[i]/samples)*(float)histp[i][j]/samples;
            devn_x[i] += (j-(float)avgn_x[i]/samples)*(j-(float)avgn_x[i]/samples)*(float)histn[i][j]/samples;
        }
    }
    
    for(i=0;i<K;i++)
    {
        devp_x[i] = sqrt(devp_x[i])/n;
        devn_x[i] = sqrt(devn_x[i])/n;
    }
    
    sum=0;
    for(i=-K+1;i<0;i++)
    {
        printf("<x>[%3d] = %.3f (%.3f)\n", i, (float)avgn_x[-i]/samples/n, devn_x[-i]);
        sum += avgn_x[-i];
    }
    for(i=0;i<K;i++)
    {
        printf("<x>[%3d] = %.3f (%.3f)\n", i, (float)avgp_x[i]/samples/n, devp_x[i]);
        sum += avgp_x[i];
    }

    printf("\nPercentage off vector = %.3f (because of K)\n",(samples-(float)sum/n)/samples);

    FILE* data = fopen("QueueDeposition_Detailed.txt", "w");
    fprintf(data, "\nN	%d\nr	%f\nSteps	%.0e\nSample	%.0e\n\n", n, (1-3*dB)/(1-dB), (double)steps, (double)samples);
    fprintf(data,"k	<x(k)>	SD[x(k)]\n");
    for(i=-K+1;i<0;i++)
        fprintf(data, "%d	%f	%f\n", i, (float)avgn_x[-i]/samples/n, devn_x[-i]);
    for(i=0;i<K;i++)
        fprintf(data, "%d	%f	%f\n", i, (float)avgp_x[i]/samples/n, devp_x[i]);
    fclose(data);

    return 0;
}


int run_system(int n, float dB, int steps, int samples)
{
    int i, j, aux=0, sum=0, delta_x[n];
    
    int histp[K][n], histn[K][n];
    int avgp_x[K], avgn_x[K];
    float devp_x[K], devn_x[K];
    
    set_int_vec(delta_x, n); set_int_vec(avgp_x, K); set_int_vec(avgn_x, K);
    set_float_vec(devp_x, K); set_float_vec(devn_x, K);
    
    for(i=0;i<K;i++)
    {
        for(j=0;j<n;j++)
        {
            histp[i][j]=0;
            histn[i][j]=0;
        }
    }
    
    printf("\nN = %d\nr = %.2f\nSteps = %.0e\nSample = %.0e\n", n, (1-3*dB)/(1-dB), (double)steps, (double)samples);
    
    for(i=0;i<steps;i++)
        aux += next_step(n, delta_x, dB);
    
    for(i=0;i<samples;i++)
    {
        aux += next_step(n, delta_x, dB);
        make_hist(n, delta_x, histp, histn);
    }
    
    for(i=0;i<n;i++)
        sum += delta_x[i];
    if(sum!=0)
        printf("WARNING: Sum of delta_x = %d\n\n", sum);
    
    for(i=0;i<K;i++)
    {
        for(j=0;j<n;j++)
        {
            avgp_x[i] += j*histp[i][j];
            avgn_x[i] += j*histn[i][j];
        }
    }
    for(i=0;i<K;i++)
    {
        for(j=0;j<n;j++)
        {
            devp_x[i] += (j-(float)avgp_x[i]/samples)*(j-(float)avgp_x[i]/samples)*(float)histp[i][j]/samples;
            devn_x[i] += (j-(float)avgn_x[i]/samples)*(j-(float)avgn_x[i]/samples)*(float)histn[i][j]/samples;
        }
    }
    for(i=0;i<K;i++)
    {
        devp_x[i] = sqrt(devp_x[i])/n;
        devn_x[i] = sqrt(devn_x[i])/n;
    }

    FILE* data = fopen("QueueDeposition.txt", "a");
    fprintf(data, "\nN	%d\nr	%f\nSteps	%.0e\nSample	%.0e\n\n", n, (1-3*dB)/(1-dB), (double)steps, (double)samples);
    fprintf(data,"k	<x(k)>	SD[x(k)]\n");
    for(i=-K+1;i<0;i++)
        fprintf(data, "%d	%f	%f\n", i, (float)avgn_x[-i]/samples/n, devn_x[-i]);
    for(i=0;i<K;i++)
        fprintf(data, "%d	%f	%f\n", i, (float)avgp_x[i]/samples/n, devp_x[i]);
    fclose(data);
    
    printf("*Completed*\n\n");
    
    return 0;
}


int main(void)
{
    float r;
    srand(time(NULL));

    r=1./3;
        run_system(N, (1-r)/(3-r), 100000, 1000000);

    getchar();
    return 0;
}
