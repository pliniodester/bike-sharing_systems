
#include<stdio.h>
#include<stdlib.h>

#define K 5
#define NSTEPS 100
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
           y[j-1][i]*(a(j-1,K)+q(j-1, y)) - y[j][i]*(a(j,K)+q(j,y)) + y[j+1][i-1]*(a(i-1,K)+a(K,j+1)) - y[j][i]*(a(i,K)+a(K,j));
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
//        printf("\nIteracao %d:\n",k);
//        print_y(y);
        printf("sum_dy = %f\n", sum_vec(Dy));
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
                Dy[i][j]=dy(i,j,y);
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
                y[i][j]+=dt*Dy[i][j];
    }
    printf("\nIteracao %d:\n",k);
    print_x(y);
    printf("sum_dy = %f\n", sum_vec(Dy));
    
    getchar();
}

