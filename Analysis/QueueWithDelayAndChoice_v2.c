
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1000
#define K 15

/*
float rexp(float lambda) {
float u = (float)((long double)rand()/(RAND_MAX+1));
if(lambda > 0)
  return -1/lambda*log(1-u);
return -1;
}
*/

void set_int_vec(int vec[], int n) {
  int i;
  for(i=0; i<n; i++)
    vec[i] = 0;
}

void set_double_vec(double vec[], int n) {
  int i;
  for(i=0; i<n; i++)
    vec[i] = 0;
}

int sum_int_vec(int vec[], int n) {
  if(n==1)
    return vec[0];
  return (vec[n-1]+sum_int_vec(vec, n-1));
}

float sum_float_vec(float vec[], int n) {
  if(n==1)
    return vec[0];
  return (vec[n-1]+sum_float_vec(vec, n-1));
}

double sum_double_vec (double vec[], int n) {
  if(n==1)
    return vec[0];
  return (vec[n-1]+sum_double_vec(vec, n-1));
}

int randN() {
return (int)(((long double)rand()/(RAND_MAX+1))*N);
}

void make_hist(int n, int x[], double hist[K][N+1], float time)
{
    int i, aux[K];
    set_int_vec(aux, K);
    
    for(i=0;i<n;i++)
    {
        if( x[i]<K )
            aux[x[i]]++;
    }
    
    for(i=0;i<K;i++)
    {
        hist[i][aux[i]]+=time;
    }
}

float next_step(int n, int q[], int vq[], double hist[K][N+1], double vhist[K][N+1], float lambda, float gamma, float mu) {
    int i, j, y0=0, vy0=0;
    float aux, dt, u;
  
    for(i=0;i<n;i++)
    {
        if(q[i]==0)
            y0++;
        if(vq[i]==0)
            vy0++;
    }
  
    u = (float)((long double)rand()/(RAND_MAX+1)*( n*lambda+(n-vy0)*gamma+(n-y0)*mu) );
    dt = (float)1/( n*lambda+(n-vy0)*gamma+(n-y0)*mu );
    if(hist != NULL)
        make_hist(n, q, hist, dt);
    if(vhist != NULL)
        make_hist(n, vq, vhist, dt);

    if(u < mu*(n-y0))
    {
        aux = 0;
        for(i=0; aux < u; i++)
        {
            if(q[i]!=0)
                aux += mu;
        }
        i--;
        if(i>=n)
            printf("ERROR: q[%d]--\n", i);
        q[i]--;
        return (double)dt;
    }
    
    if(u < mu*(n-y0)+gamma*(n-vy0))
    {
        aux = mu*(n-y0);
        for(i=0; aux < u; i++)
        {
            if(vq[i]!=0)
                aux += gamma;
        }
        i--;
        if(i>=n)
            printf("ERROR: vq[%d]--\n", i);
        q[i]++;
        vq[i]--;
        return (double)dt;
    }
    
    i = rand()%n;
    j = rand()%n;
    if(q[i] < q[j])
    {
        vq[i]++;
        return (double)dt;
    }
    if(q[i] > q[j])
    {
        vq[j]++;
        return (double)dt;
    }
    if(rand()%2==0)
    {
        vq[i]++;
        return (double)dt;
    }
    else
    {
        vq[j]++;
        return (double)dt;
    }
    printf("ERROR: none of the cases of next step were satisfied\n");
    return NAN;
}

int run_system(int n, float lambda, float gamma, float mu, int steps, int samples)
{
    int i, j, q[n], vq[n];
    float sum=0;
    double Dt, total_time=0;

    double hist[K][n+1], vhist[K][n+1], avg_q[K], avg_vq[K];
    
    set_int_vec(q, n); set_int_vec(vq, n);
    set_double_vec(avg_q, K); set_double_vec(avg_vq, K);

    for(i=0;i<K;i++)
    {
        for(j=0;j<n+1;j++)
        {
            hist[i][j]=0;
            vhist[i][j]=0;
        }
    }
    
    printf("\nN = %d\nlambda = %f\ngamma = %f\nmu = %f\nSteps = %.0e\nSample = %.0e\n\n", n, lambda, gamma, mu, (double)steps, (double)samples);
    
    for(i=0;i<steps;i++)
    {
        Dt = next_step(n, q, vq, NULL, NULL, lambda, gamma, mu);
    }
    
    for(i=0;i<samples;i++)
    {
        Dt = next_step(n, q, vq, hist, vhist, lambda, gamma, mu);
        total_time += Dt;
    }
    
    for(i=0;i<K;i++)
    {
        for(j=0;j<n+1;j++)
        {
            avg_q[i] += j*hist[i][j];
            avg_vq[i] += j*vhist[i][j];
        }
        avg_q[i] = avg_q[i]/total_time;
        avg_vq[i] = avg_vq[i]/total_time;
    }
    
    sum=0;
    for(i=0;i<K;i++)
        sum += avg_q[i];
    printf("Percentage off queue vector = %f\n",1.-(float)(sum/n));
    sum=0;
    for(i=0;i<K;i++)
        sum += avg_vq[i];
    printf("Percentage off virtual queue vector = %f\n",1.-(float)(sum/n));   
    
    printf("*Completed\n\n");

    FILE* data = fopen("Queue_Delay_Simulation.txt", "a");
    fprintf(data, "\nN	%d\nlambda	%f\ngamma	%f\nmu	%f\nSteps	%.0e\nSample	%.0e\n\n", n, lambda, gamma, mu, (double)steps, (double)samples);
    fprintf(data,"k	<q(k)>	<vq(k)>\n");
    for(i=0;i<K;i++)
        fprintf(data, "%d	%e	%e\n", i, avg_q[i]/n, avg_vq[i]/n);
    fprintf(data, "\n\n");
    fclose(data);

    return 0;
}

int main(void)
{
    int n;
    float lambda, gamma, mu;
    srand(time(NULL));

    FILE* data = fopen("Queue_Delay_Simulation.txt", "w");
    fclose(data);
    
    n = N;
    lambda = 0.7;
    gamma = 1;
    mu = 1;
    
    for(gamma = 0.5; gamma<2; gamma+=0.1)
    run_system(n, lambda, gamma, mu, 100000, 1000000);

    getchar();
    return 0;
}
