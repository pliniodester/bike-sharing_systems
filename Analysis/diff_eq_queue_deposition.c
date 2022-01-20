
#include<stdio.h>
#include<stdlib.h>

#define K 30
#define NSTEPS 1000
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
    float sum = y[i][i]/2;
    for(j=i+1;j<2*K+1;j++)
        sum += y[i][j];
    if (x(i,y)>0)
        return sum/x(i,y);
    return 1;
}

float alpha(int i, int j, float y[2*K+1][2*K+1])
{
    return y[i][j]*(a(i,j)+q(i,y));
}

float dy(int i, int j, float y[2*K+1][2*K+1])
{
    return alpha(i-1,j,y)-alpha(i,j,y)+alpha(j+1,i-1,y)-alpha(j,i,y)+alpha(j-1,i,y)-alpha(j,i,y)+alpha(i+1,j-1,y)-alpha(i,j,y);
}

void print_x(float y[2*K+1][2*K+1])
{
    int i;
    for(i=0;i<2*K+1;i++)
        printf("x[%2d] = %f\n", i-K, x(i,y));
}

float sum_dy( float Dy[2*K+1][2*K+1] )
{
    int i, j;
    float sum=0;
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
                sum += Dy[i][j];
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
//        print_x(y);
        printf("sum_dy = %f\n", sum_dy(Dy));
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
                Dy[i][j]=dy(i,j,y);
        for(i=1;i<2*K;i++)
            for(j=1;j<2*K;j++)
                y[i][j]+=dt*Dy[i][j];
    }
    printf("\nIteracao %d:\n",k);
    print_x(y);
    printf("sum_dy = %f\n", sum_dy(Dy));
    
    getchar();
}

