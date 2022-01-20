
#include<stdio.h>
#include<stdlib.h>

#define K 30
#define NSTEPS 10000
#define STEP 0.1

void initialize(long double y[2*K+1][2*K+1])
{
    int i, j;
    for(i=0;i<2*K+1;i++)
        for(j=0;j<2*K+1;j++)
            y[i][j]=0;
    y[K][K]=1;
}

long double a(int i, int j)
{
    if(i<j)
        return 1.;
    if(i==j)
        return 0.5;
    return 0.;
}

long double x(int i, long double y[2*K+1][2*K+1])
{
    int j;
    long double sum = 0;
    for(j=0;j<2*K+1;j++)
    {
        sum += y[i][j];
    }
    return sum;
}

long double q_p(int i, long double y[2*K+1][2*K+1])
{
    int j;
    long double sum = y[K][i]/2;
    for(j=K+1;j<2*K+1;j++)
        sum += y[j][i];
    if (x(i,y)>0)
        return sum/x(i,y);
    return 1;
}

long double q_n(int i, long double y[2*K+1][2*K+1])
{
    int j;
    long double sum = y[i][K]/2;
    for(j=0;j<K;j++)
        sum += y[i][j];
    if (x(i,y)>0)
        return sum/x(i,y);
    return 1;
}

long double dy(int i, int j, long double y[2*K+1][2*K+1])
{
    return y[i-1][j]*(a(i-1,K)+q_p(i-1, y)) + y[i+1][j-1]*(a(j-1,K)+a(K,i+1)) + 
           y[i][j+1]*(a(K,j+1)+q_n(j+1,y)) - y[i][j]*(2+q_p(i,y)+q_n(j,y));
}

void print_x(long double y[2*K+1][2*K+1])
{
    int i;
    for(i=0;i<2*K+1;i++)
        printf("x[%2d] = %e\n", i-K, (double)x(i,y));
}

void print_y(long double y[2*K+1][2*K+1])
{
    int i, j;
    printf("");
    for(i=0;i<2*4+1;i++)
        printf("   %3d", i-4);
    for(i=0;i<2*4+1;i++)
    {
        printf("\n%2d ", i-K);
        for(j=0;j<2*4+1;j++)
            printf("%.3e ", (double)y[i][j]);
    }
    printf("\n");
}

void fprint_x(long double y[2*K+1][2*K+1])
{
    FILE* data = fopen("diff_eq_queue_deposition_new_pairapp.txt", "w");
    int i;
    for(i=0;i<2*K+1;i++)
        fprintf(data, "%d	%e\n", i-K, (double)x(i,y));
    fprintf(data, "\n");
    fclose(data);
}

void fprint_y(long double y[2*K+1][2*K+1])
{
    int i, j;
    FILE* data = fopen("diff_eq_queue_deposition_new_pairapp.txt", "a");
    fprintf(data, "\n");
    for(i=0;i<2*K+1;i++)
        fprintf(data, "	%d", i-K);
    for(i=0;i<2*K+1;i++)
    {
        fprintf(data, "\n%d	", i-K);
        for(j=0;j<2*K+1;j++)
            fprintf(data, "%e	", (double)y[i][j]);
    }
    fprintf(data, "\n");
    fclose(data);
}

long double sum_vec( long double vec[2*K+1][2*K+1] )
{
    int i, j;
    long double sum=0;
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
                sum += vec[i][j];
    return sum;
}

int main(void)
{
    int i, j, k=0, aux=1;
    long double y[2*K+1][2*K+1], Dy[2*K+1][2*K+1];
    long double dt = STEP;
    
    initialize(y);
    initialize(Dy);

    for(k=0; k<NSTEPS && aux>0 ;k++)
    {
        aux = 0;
        if(k%10==0)
            printf("x[0] = %e\n", (double)x(K,y));
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
            {
                Dy[i][j]=dy(i,j,y);
                if(Dy[i][j]!=0)
                    aux++;
            }
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
                y[i][j]+=dt*Dy[i][j];
    }
    printf("N = %d\n", k);
    fprint_x(y);
    fprint_y(y);
    fprint_y(Dy);
    
    getchar();
}

