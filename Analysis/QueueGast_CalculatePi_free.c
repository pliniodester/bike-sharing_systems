
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define N 3

void initialize(float y[], float py[], int n[])
{
    int j, max;
    max = pow(N,N);
    for(j=0; j<max; j++)
    {
        y[j]=0;
        py[j]=0;
    }
    for(j=0; j<N; j++)
        n[j]=0;
    py[0]=1;
}

float n_nonzero(int n[N])
{
    float sum = 0;
    int i;
    for(i=0;i<N;i++)
        if(n[i]!=0)
            sum++;
    return sum;
}

float gi(int i, int n[N])
{
/*
    float aux=0;        
    if(n[i]-1<n[(i-1+N)%N]-1)
        aux++;
    if(n[i]-1<=n[(i+1)%N]+1)
        aux++;
    return aux;
*/
    return 1;
}

int pos(int n[])
{
    int j, aux=1, pos=0;
    for(j=0;j<N;j++)
    {
        pos += n[j]*aux;
        aux *= N;
    }
    return pos;
}

float yn_n(int n[], float y[])
{
    int j, aux=1, pos=0;
    for(j=0;j<N;j++)
    {
        pos += n[j]*aux;
        aux *= N;
    }
    return y[pos];
}

float yn_plus_ei(int i, int n[], float y[])
{
    int j, aux=1, pos=0;
    n[i]++;
    for(j=0;j<N;j++)
    {
        pos += n[j]*aux;
        aux *= N;
    }
    n[i]--;
    return y[pos];
}

float yn_less_ei(int i, int n[], float y[])
{
    int j, aux=1, pos=0;
    n[i]--;
    for(j=0;j<N;j++)
    {
        pos += n[j]*aux;
        aux *= N;
    }
    n[i]++;
    return y[pos];
}

float calculate_yn(int k, int n[N], float y[], float py[])
{
      int i;
      float sum=0;
      for(i=0;i<N;i++)
      {
          sum += yn_plus_ei(i, n, y);
          if(n[i]>0)
              sum += k*gi(i, n)*yn_less_ei(i, n, py);
      }
      sum += -N*k*yn_n(n, py);
      sum = sum/n_nonzero(n);
      y[pos(n)] = sum;
      return sum;
}

void print_n(int n[])
{
    int j;
    for(j=0;j<N;j++)
        printf("n[%d] = %d\n",j, n[j]);
    printf("\n");
}

void calculate_k(int k, int n[], float y[], float py[])
{
    int j, n_ei, sum;
    bool flag;
    
    for(n_ei=k; n_ei>0; n_ei--)
    {
        for(j=1;j<N;j++)
            n[j]=0;
        n[0]=0;
        while(n[N-1] < n_ei)
        {
            flag = true;
            for(j=0;flag && j<N;j++)
            {
                if(n[j]<n_ei)
                {
                    n[j]++;
                    flag = false;
                }
                else
                    n[j]=0;
            }
            sum=0;
            for(j=0;j<N;j++)
                sum += n[j];
            if(sum == n_ei)
            {
                calculate_yn(k, n, y, py);
            }
        }
    }
    for(j=1;j<N;j++)
        n[j]=0;
    while(n[N-1]<k)
    {
        flag = true;
        for(j=0;flag && j<N;j++)
        {
            if(n[j]<k)
            {
                n[j]++;
                flag = false;
            }
            else
                n[j]=0;
    }
    y[0] += -yn_n(n, y);
    }
}

float calculate_pi(int i, int k, float y[])
{
    int j, n[N];
    float pi;
    bool flag;

    for(j=1;j<N;j++)
        n[j]=0;
    n[0]=i;
    pi = yn_n(n, y);
    
    while(n[N-1]<k)
    {
        flag = true;
        for(j=1;flag && j<N;j++)
        {
            if(n[j]<k)
            {
                n[j]++;
                flag = false;
            }
            else
                n[j]=0;
        }
        pi += yn_n(n, y);
    }
    return pi;    
}

void copy_vecs(int k, float y[], float py[])
{
    int j, n[N];
    bool flag;
    
    for(j=0;j<N;j++)
        n[j]=0;
    
    py[pos(n)] = y[pos(n)];
    y[pos(n)] = 0;
    while(n[N-1]<k)
    {
        flag = true;
        for(j=0;flag && j<N;j++)
        {
            if(n[j]<k)
            {
                n[j]++;
                flag = false;
            }
            else
                n[j]=0;
        }
        py[pos(n)] = y[pos(n)];
        y[pos(n)] = 0;
    }
}

int main(void)
{
    int i, k, n[N];
    float y[(int)pow(N,N)];
    float py[(int)pow(N,N)];
    initialize(y, py, n);
    
    for(k=1; k<N; k++)
    {
        calculate_k(k, n, y, py);
        for(i=0; i<=k; i++)
            printf("Pi(%d)[%d] = %f\n", k, i, calculate_pi(i,k,y));
        printf("\n");
        copy_vecs(k, y, py);
    }
    
    printf("%f\n", py[0]);
    
    getchar();
    return 0;
}
