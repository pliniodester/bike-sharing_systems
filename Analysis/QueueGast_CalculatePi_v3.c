
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define N 6
#define K 6

void initialize(float y[], int n[])
{
    int j, max;
    max = pow(K/2+K%2+1,N);
    for(j=1; j<max; j++)
    {
        y[j]=0;
    }
    for(j=0; j<N; j++)
        n[j]=0;
    y[0]=1;
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

float a(int i, int j)
{
    if(i<j)
        return 1.;
    if(i==j)
        return 0.5;
    return 0.;
}

float gi(int i, int n[N])
{
    return a(n[i]-1,n[(i-1+N)%N])+a(n[i]-1,n[(i+1)%N]);
}

int pos(int n[])
{
    int j, aux=1, pos=0;
    for(j=0;j<N;j++)
    {
        if(n[j]>K/2+K%2)
            printf("Error: Function pos\n");
        pos += n[j]*aux;
        aux *= K/2+K%2+1;
    }
    return pos;
}

float yn_n(int n[], float y[])
{
    int j, aux=1, pos=0;
    for(j=0;j<N;j++)
    {
        if(n[j]>K/2+K%2)
            return 0;
        pos += n[j]*aux;
        aux *= K/2+K%2+1;
    }
    return y[pos];
}

float yn_plus_ei(int i, int n[], float y[])
{
    int j, aux=1, pos=0;
    n[i]++;
    for(j=0;j<N;j++)
    {
        if(n[j]>K/2+K%2)
            return 0;
        pos += n[j]*aux;
        aux *= K/2+K%2+1;
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
        if(n[j]>K/2+K%2)
            return 0;
        pos += n[j]*aux;
        aux *= K/2+K%2+1;
    }
    n[i]++;
    return y[pos];
}

float calculate_yn(int k, int n[N], float y[])
{
      int i;
      float sum=0;
      sum += -N*k*yn_n(n, y);
      for(i=0;i<N;i++)
      {
          if(n[i]<K/2+K%2)
              sum += yn_plus_ei(i, n, y);
          if(n[i]>0)
              sum += k*gi(i, n)*yn_less_ei(i, n, y);
      }
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

void calculate_k(int k, int n[], float y[])
{
    int j, n_ei, sum;
    bool flag;
    
    for(n_ei=k; n_ei>0; n_ei--)
    {
        for(j=0;j<N;j++)
            n[j]=0;
        while(n[N-1] < n_ei && n[N-1] <= k/2+k%2)
        {
            flag = true;
            for(j=0;flag && j<N;j++)
            {
                if(n[j] < n_ei  && n[j] <= k/2+k%2)
                {
                    n[j]++;
                    flag = false;
                }
                else
                    n[j]=0;
            }
            sum=0;
            for(j=0;j<N;j++)
            {
                sum += n[j];
                if(n[j]>K/2+K%2)
                    sum = n_ei+1;
            }
            if(sum == n_ei)
            {
                calculate_yn(k, n, y);
            }
        }
    }
    for(j=0;j<N;j++)
        n[j]=0;
    y[0] = 0;
    while(n[N-1] <= k/2+k%2)
    {
        flag = true;
        for(j=0;flag && j<N;j++)
        {
            if(n[j] <= k/2+k%2)
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
    
    while(n[N-1] <= k/2+k%2)
    {
        flag = true;
        for(j=1;flag && j<N;j++)
        {
            if(n[j] <= k/2+k%2)
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

int main(void)
{
    int i, k, n[N];
    float y[(int)pow(K/2+K%2+1,N)];
    initialize(y, n);
    
    for(k=1; k<K; k++)
    {
        calculate_k(k, n, y);
        for(i=0; i <= k/2+k%2; i++)
            printf("Pi(%d)[%d] = %f\n", k, i, calculate_pi(i,k,y));
        printf("\n");
    }
    
    printf("%f\n", y[0]);
    
    for(i=0;i<N;i++)
        n[i]=0;
    n[0] = k/2;
    n[1] = (k-2)/2;
    printf("%f\n", y[pos(n)]);
    
    getchar();
    return 0;
}
