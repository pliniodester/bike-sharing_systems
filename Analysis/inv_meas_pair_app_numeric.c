
#include<stdio.h>
#include<stdlib.h>

#define K 20
#define rho 0.1
#define d 2.

#define NSTEPS 10000
#define STEP 0.01

void initialize(float y[K+1][K+1])
{
    int i, j;
    for(i=0;i<K+1;i++)
        for(j=0;j<K+1;j++)
            y[i][j]=0;
    y[0][0]=1;
}

float a(int i, int j)
{
    if(i<j)
        return 1.;
    if(i==j)
        return 0.5;
    return 0.;
}

float x(int i, float y[K+1][K+1])
{
    int j;
    float sum = 0;
    for(j=0;j<K+1;j++)
    {
        sum += y[i][j];
    }
    return sum;
}

float q(int i, float y[K+1][K+1])
{
    int j;
    float sum = y[i][i]/2;
    for(j=i+1;j<K+1;j++)
        sum += y[i][j];
    if (x(i,y)>0)
        return sum/x(i,y);
    return 1;
}

float ind(int i)
{
    if(i>0)
        return 1;
    return 0;
}

float dy(int i, int j, float y[K+1][K+1])
{
    float aux = y[i+1][j]+y[j+1][i];
    if(i>0)
        aux += -y[i][j];
    if(j>0)
        aux += -y[i][j];
    aux += -2*rho/d*y[i][j];
    if(i>0)
        aux += 2*rho/d*(a(i-1,j)*y[i-1][j]);
    if(j>0)
        aux += 2*rho/d*(a(j-1,i)*y[j-1][i]);
    aux+= 2*rho*(d-1)/d*(-(q(i,y)+q(j,y))*y[i][j]);
    if(i>0)
        aux+= 2*rho*(d-1)/d*(q(i-1,y)*y[i-1][j]);
    if(j>0)
        aux+= 2*rho*(d-1)/d*(q(j-1,y)*y[j-1][i]);
    return aux;
}

//y[i+1][j]*(a(i-1,K)+q(i-1, y)) - y[i][j]*(a(i,K)+q(i,y)) + y[i+1][j-1]*(a(j-1,K)+a(K,i+1)) - y[i][j]*(a(j,K)+a(K,i));

void print_x(float y[K+1][K+1])
{
    int i;
    for(i=0;i<K+1;i++)
        printf("x[%2d] = %f\n", i, x(i,y));
}

void print_y(float y[K+1][K+1])
{
    int i, j;
    printf("");
    for(i=0;i<2*4+1;i++)
        printf("   %3d", i);
    for(i=0;i<2*4+1;i++)
    {
        printf("\n%2d ", i);
        for(j=0;j<2*4+1;j++)
            printf("%.3f ", y[i][j]);
    }
    printf("\n");
}

void fprint_x(float y[K+1][K+1])
{
    FILE* data = fopen("inv_meas_papp_x.txt", "w");
    int i;
    for(i=0;i<2*K+1;i++)
        fprintf(data, "%d	%f\n", i-K, x(i,y));
    fprintf(data, "\n");
    fclose(data);
}

void fprint_y(float y[K+1][K+1])
{
    int i, j;
    FILE* data = fopen("inv_meas_papp_y.txt", "a");
    for(i=0;i<K+1;i++)
        fprintf(data, "	%d", i-K);
    for(i=0;i<K+1;i++)
    {
        fprintf(data, "\n%d	", i-K);
        for(j=0;j<K+1;j++)
            fprintf(data, "%f	", y[i][j]);
    }
    fprintf(data, "\n");
    fclose(data);
}

float sum_vec( float vec[K+1][K+1] )
{
    int i, j;
    float sum=0;
        for(i=1;i<K;i++)
            for(j=1;j<K;j++)
                sum += vec[i][j];
    return sum;
}

int main(void)
{
    int i, j, k=0;
    float y[K+1][K+1], Dy[K+1][K+1];
    float dt = STEP;
    
    initialize(y);
    initialize(Dy);
    
    for(k=0;k<NSTEPS;k++)
    {
        if(k%100==0)
            printf("x[0] = %f\n", x(0,y));
        for(i=1;i<K;i++)
            for(j=1;j<K;j++)
                Dy[i][j]=dy(i,j,y);
        for(i=1;i<K;i++)
            for(j=1;j<K;j++)
                y[i][j]+=dt*Dy[i][j];
    }
    fprint_x(y);
    fprint_y(y);
    fprint_y(Dy);
    
    getchar();
}

