
#include<stdio.h>
#include<stdlib.h>

#define K 20
#define NSTEPS 10000
#define STEP 0.01

void initialize(float y[2*K+1][2*K+1])
{
    int i, j;
    for(i=0;i<2*K+1;i++)
        for(j=0;j<2*K+1;j++)
            y[i][j]=0;
/*    for(i=1;i<2*K;i++)
        for(j=1;j<2*K;j++)
            y[i][j]=1./(2*K-1)/(2*K-1);
*/
    y[K][K]=1;
}

float a(int i, int j)
{
    if(i<j)
        return 1.;
    if(i==j)
        return 0.5;
    return 0.;
}

float x(int i, float y[2*K+1][2*K+1])
{
    int j;
    float sum = 0;
    for(j=0;j<2*K+1;j++)
    {
        sum += y[i][j];
    }
    return sum;
}

float q(int i, float y[2*K+1][2*K+1])
{
    int j;
    float sum = y[K][i]/2;
    for(j=K+1;j<2*K+1;j++)
        sum += y[j][i];
    if (x(i,y)>0)
        return sum/x(i,y);
    return 1;
}

float dy(int i, int j, float y[2*K+1][2*K+1])
{
    return y[i-1][j]*(a(i-1,K)+q(i-1, y)) - y[i][j]*(a(i,K)+q(i,y)) + y[i+1][j-1]*(a(j-1,K)+a(K,i+1)) - y[i][j]*(a(j,K)+a(K,i)) +
           y[j-1][i]*(a(j-1,K)+q(j-1, y)) - y[j][i]*(a(j,K)+q(j,y)) + y[j+1][i-1]*(a(i-1,K)+a(K,j+1)) - y[j][i]*(a(i,K)+a(K,j)) +
           y[i+1][j-1]*(a(K,i+1)+a(j-1,K)) - y[i][j]*(a(K,i)+a(j,K)) + y[i][j+1]*(a(K,j+1)+1-q(j+1,y)) - y[i][j]*(a(K,j)+1-q(j,y)) +
           y[j+1][i-1]*(a(K,j+1)+a(i-1,K)) - y[j][i]*(a(K,j)+a(i,K)) + y[j][i+1]*(a(K,i+1)+1-q(i+1,y)) - y[j][i]*(a(K,i)+1-q(i,y));
}

void print_x(float y[2*K+1][2*K+1])
{
    int i;
    for(i=0;i<2*K+1;i++)
        printf("x[%2d] = %f\n", i-K, x(i,y));
}

void print_y(float y[2*K+1][2*K+1])
{
    int i, j;
    printf("");
    for(i=0;i<2*4+1;i++)
        printf("   %3d", i-4);
    for(i=0;i<2*4+1;i++)
    {
        printf("\n%2d ", i-K);
        for(j=0;j<2*4+1;j++)
            printf("%.3f ", y[i][j]);
    }
    printf("\n");
}

void fprint_x(float y[2*K+1][2*K+1])
{
    FILE* data = fopen("diff_eq_queue_deposition.txt", "w");
    int i;
    for(i=0;i<2*K+1;i++)
        fprintf(data, "%d	%f\n", i-K, x(i,y));
    fprintf(data, "\n");
    fclose(data);
}

void fprint_y(float y[2*K+1][2*K+1])
{
    int i, j;
    FILE* data = fopen("diff_eq_queue_deposition.txt", "a");
    for(i=0;i<2*K+1;i++)
        fprintf(data, "	%d", i-K);
    for(i=0;i<2*K+1;i++)
    {
        fprintf(data, "\n%d	", i-K);
        for(j=0;j<2*K+1;j++)
            fprintf(data, "%f	", y[i][j]);
    }
    fprintf(data, "\n");
    fclose(data);
}

float sum_vec( float vec[2*K+1][2*K+1] )
{
    int i, j;
    float sum=0;
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
                sum += vec[i][j];
    return sum;
}

int main(void)
{
    int i, j, k=0;
    float y[2*K+1][2*K+1], Dy[2*K+1][2*K+1];
    float dt = STEP;
    
    initialize(y);
    initialize(Dy);
    
    for(k=0;k<NSTEPS;k++)
    {
        if(k%10==0)
            printf("x[0] = %f\n", x(K,y));
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
                Dy[i][j]=dy(i,j,y);
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
                y[i][j]+=dt*Dy[i][j];
    }
    fprint_x(y);
    fprint_y(y);
    fprint_y(Dy);
    
    getchar();
}

