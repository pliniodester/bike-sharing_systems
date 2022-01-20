
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

#define N 2
#define K 31

/*
float exp_time(float lambda)
{
    float u = (float)((long double)rand()/(RAND_MAX+1));
    if(lambda > 0)
        return -1/lambda*log(1-u);
    return 100000;
}
*/

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

void set_double_vec(double vec[], int n)
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

int initialise_system(int n, int x[]) {
    int i;
    for(i=0;i<n;i++)
    {
        x[i] = 0;
    }
    return 0;
}

double next_step(int n, int x[], float rho)
{
    int i, y0=0;
    float aux;
    float u;
    
    for(i=0;i<n;i++)
        if(x[i]==0)
            y0++;
    
    
    u = (float)((long double)rand()/(RAND_MAX+1)*(n*rho+n-y0));
    if (u < n-y0)
    {
        aux = 0;
        for(i=0; aux < u; i++)
        {
            if(x[i]!=0)
                aux++;
        }
        i--;
        if(i>=n)
            printf("ERROR: x[%d]--\n", i);
        x[i]--;
        return (double)1;
    }
    i = rand()%n;
    if(x[i] < x[(i+1)%n])
    {
        if(x[i]<K-1)
            x[i]++;
        return (double)1/rho;
    }
    if(x[i] > x[(i+1)%n])
    {
        if(x[(i+1)%n] < K-1)
            x[(i+1)%n]++;
        return (double)1/rho;
    }
    if(rand()%2==0)
    {
        if(x[i] < K-1)
            x[i]++;
        return (double)1/rho;
    }
    else
    {
        if(x[(i+1)%n] < K-1)
            x[(i+1)%n]++;
        return (double)1/rho;
    }
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

void count_yij(int n, int x[], double yij[K][K], float time)
{
    int i;
    for(i=0;i<n;i++)
    {
        if(x[i]<K && x[(i+1)%n]<K)
            yij[x[i]][x[(i+1)%n]]+=(double)time;
    }
}

double next_step_with_hist(int n, int x[], float rho, double hist[K][N+1], double yij[K][K])
{
    int i, y0=0;
    float aux;
    float u, dt;
    
    for(i=0;i<n;i++)
        if(x[i]==0)
            y0++;
    
    
    u = (float)((long double)rand()/(RAND_MAX+1)*(n*rho+n-y0));
    dt = (float)1/(n*rho+n-y0);
    
    make_hist(n, x, hist, dt);
    if(yij != NULL)
        count_yij(n, x, yij, dt);
    
    if (u < n-y0)
    {
        aux = 0;
        for(i=0; aux < u; i++)
        {
            if(x[i]!=0)
                aux++;
        }
        i--;
        if(i>=n)
            printf("ERROR: x[%d]--\n", i);
        x[i]--;
        return (double)dt;
    }
    i = rand()%n;
    if(x[i] < x[(i+1)%n])
    {
        if(x[i] < K-1)
            x[i]++;
        return (double)dt;
    }
    if(x[i] > x[(i+1)%n])
    {
        if(x[(i+1)%n] < K-1)
            x[(i+1)%n]++;
        return (double)dt;
    }
    if(rand()%2==0)
    {
        if(x[i] < K-1)
            x[i]++;
        return (double)dt;
    }
    else
    {
        if(x[(i+1)%n] < K-1)
            x[(i+1)%n]++;
        return (double)dt;
    }
}

void print_x(int n, int x[])
{
    int i;
    for(i=0; i<n; i++)
        printf("x[%2d] = %d\n", i, x[i]);
    printf("\n");
}

void print_hist(int n, double hist[K][N+1], double total_time)
{
    int i, j;
    for(i=0;i<K;i++)
    {
        printf("\n[%3d] ", i);
        for(j=0;j<14&&j<n+1;j++)
        {
            printf("%.2f ", (float)hist[i][j]/total_time);
        }
    }
}

int run_detailed_system(int n, float rho, int steps, int samples)
{
    int i, j, x[n];
    float sum=0;
    double Dt, total_time=0;

    double hist[K][n+1], avg_x[K], dev_x[K];
    double yij[K][K];
    
    set_int_vec(x, n); set_double_vec(avg_x, K);
    set_double_vec(dev_x, K);
    
    for(i=0;i<K;i++)
        for(j=0;j<K;j++)
            yij[i][j]=0;

    for(i=0;i<K;i++)
    {
        for(j=0;j<n+1;j++)
        {
            hist[i][j]=0;
        }
    }
    
    printf("\nN = %d\nrho = %f\nSteps = %.0e\nSample = %.0e\n\n", n, rho, (double)steps, (double)samples);
    
    for(i=0;i<steps;i++)
    {
        Dt = next_step(n, x, rho);
    }
    
    for(i=0;i<samples;i++)
    {
        Dt = next_step_with_hist(n, x, rho, hist, yij);
        total_time += Dt;
    }
    
    for(i=0;i<K;i++)
    {
        for(j=0;j<n+1;j++)
        {
            avg_x[i] += j*hist[i][j];
        }
        avg_x[i] = avg_x[i]/total_time;
    }
    for(i=0;i<K;i++)
    {
        for(j=0;j<n;j++)
        {
            dev_x[i] += (j-avg_x[i])*(j-avg_x[i])*hist[i][j]/total_time;
        }
    }
    for(i=0;i<K;i++)
    {
        dev_x[i] = sqrt(dev_x[i])/n;
    }
    
    sum=0;
    for(i=0;i<K;i++)
    {
        printf("<x>[%3d] = %f (%f)\n", i, (float)(avg_x[i]/n), (float)dev_x[i]);
        sum += avg_x[i];
    }

    printf("\nPercentage off vector = %f (because of K)\n",1-(float)(sum/n));
    printf("Total Time = %e\n", total_time);

    FILE* data = fopen("QueueGast_Detailed.txt", "a");
    fprintf(data, "\nN	%d\nrho	%f\nSteps	%.0e\nSample	%.0e\n\n", n, rho, (double)steps, (double)samples);
    fprintf(data,"k	<y(k)>	SD[y(k)]\n");
    for(i=0;i<K;i++)
        fprintf(data, "%d	%e	%e\n", i, avg_x[i]/n, dev_x[i]);
    fprintf(data, "\n");
    fprintf(data,"y_ij");
    for(i=0;i<K;i++)
        fprintf(data,"	%d", i);
    for(i=0;i<K;i++)
    {
        fprintf(data,"\n%d	", i);
        for(j=0;j<K;j++)
        {
            fprintf(data,"%e	", (double)yij[i][j]/total_time/n);
        }
    }
    fprintf(data, "\n\n");
    fclose(data);

    return 0;
}


int run_system(int n, float rho, int steps, int samples)
{
    int i, j, x[n];
    float sum=0;
    double Dt, total_time=0;

    double hist[K][n+1], avg_x[K], dev_x[K];
    
    set_int_vec(x, n); set_double_vec(avg_x, K);
    set_double_vec(dev_x, K);

    for(i=0;i<K;i++)
    {
        for(j=0;j<n+1;j++)
        {
            hist[i][j]=0;
        }
    }
    
    printf("\nN = %d\nrho = %f\nSteps = %.0e\nSample = %.0e\n\n", n, rho, (double)steps, (double)samples);
    
    for(i=0;i<steps;i++)
    {
        Dt = next_step(n, x, rho);
    }
    
    for(i=0;i<samples;i++)
    {
        Dt = next_step_with_hist(n, x, rho, hist, NULL);
        total_time += Dt;
    }
    
    for(i=0;i<K;i++)
    {
        for(j=0;j<n+1;j++)
        {
            avg_x[i] += j*hist[i][j];
        }
        avg_x[i] = avg_x[i]/total_time;
    }
    for(i=0;i<K;i++)
    {
        for(j=0;j<n;j++)
        {
            dev_x[i] += (j-avg_x[i])*(j-avg_x[i])*hist[i][j]/total_time;
        }
    }
    for(i=0;i<K;i++)
    {
        dev_x[i] = sqrt(dev_x[i])/n;
    }
    
    sum=0;
    for(i=0;i<K;i++)
        sum += avg_x[i];
    printf("Percentage off vector = %f\n",1.-(float)(sum/n));
    printf("*Completed\n\n");
/*
    FILE* data = fopen("QueueGast.txt", "a");
    fprintf(data, "\nN	%d\nrho	%f\nSteps	%.0e\nSample	%.0e\n\n", n, rho, (double)steps, (double)samples);
    fprintf(data,"k	<y(k)>	SD[y(k)]\n");
    for(i=0;i<K;i++)
        fprintf(data, "%d	%e	%e\n", i, avg_x[i]/n, dev_x[i]);
    fprintf(data, "\n\n");
    fclose(data);
*/
    sum=0;
    for(i=0;i<K;i++)
        sum += i*avg_x[i]/n;
    FILE* data = fopen("QueueGast.txt", "a");
    fprintf(data, "%f	%f\n", rho, sum);
    fclose(data);

    return 0;
}


int main(void)
{
    float rho;
    srand(time(NULL));

    FILE* data = fopen("QueueGast.txt", "w");
    fclose(data);
    data = fopen("QueueGast_Detailed.txt", "w");
    fclose(data);
    
    rho=1;
    run_system(N, rho, 100000, 1000000);

    getchar();
    return 0;
}
