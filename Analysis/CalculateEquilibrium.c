
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define C 3
#define K 100
#define S 10
#define LAMBDA 1
#define MU 0.1
#define DELTA 0.000001

void initialise(float ykl[][C+1], float yl[] ,int s, int k, int c)
{
     int i,j;
     for(i=0;i<k+1;i++)
     {
         for(j=0;j<c+1;j++)
         {
             ykl[i][j]=0;
         }
     }
     for(i=0;i<c+1;i++)
         yl[i]=0;
     ykl[s][0]=1;
     yl[0]=1;;
}

float fkl(float y[][C+1], float yl[], int k, int l, float lambda, float mu, int k_max, int c)
{
      int i;
      float dykl, Rc, pl;
      
      Rc=0;
      for(i=0;i<c;i++)
          Rc += i*y[i][c];
      Rc = Rc/(1-yl[c]);
      
      pl=yl[l];
      for(i=l+1;i<c;i++)
          pl += 2*yl[i];
      
      if(k<k_max && l>0 && l<c)
      {
          dykl = mu*((k+1)*y[k+1][l-1]-k*y[k][l]+Rc*(y[k][l-1]-y[k][l]))+lambda*(y[k][l+1]-y[k][l]);
          if(k>0)
              dykl += (1-yl[0])*pl*(y[k-1][l]-y[k][l]);
          else
              dykl += (1-yl[0])*pl*(-y[k][l]);
          return dykl;
      }
      if(k<k_max && l==0)
      {
          dykl = mu*(-k*y[k][l]+Rc*(-y[k][l]))+lambda*(y[k][l+1]);
          if(k>0)
              dykl += (1-yl[0])*pl*(y[k-1][l]-y[k][l]);
          else
              dykl += (1-yl[0])*pl*(-y[k][l]);
          return dykl;
      }
      if(k<k_max && l==c)
      {
          dykl = mu*((k+1)*y[k+1][l]+(k+1)*y[k+1][l-1]-k*y[k][l]+Rc*(y[k][l-1]))+lambda*(-y[k][l]);
          if(k>0)
              dykl += (1-yl[0])*pl*(y[k-1][l]-y[k][l]);
          else
              dykl += (1-yl[0])*pl*(-y[k][l]);
          return dykl;
      }
      if(k==k_max)
      {
          dykl = lambda *(1-yl[0])*pl * y[k-1][l];
          if(dykl>0) printf("y[K_max][%d]!=0", l);
          return dykl;
      }
      printf("\nERROR fykl[%d][%d]\n", k, l);
      return NAN;
}



int main(void)
{
    int i, j, m;
    float sum, DY;
    float ykl[K+1][C+1], yl[C+1], dykl[K+1][C+1];
    initialise(ykl, yl, S, K, C);
    
    for(m=0;m<100000;m++)
    {
        for(j=0;j<C+1;j++)
            for(i=0;i<K+1;i++)
                dykl[i][j]=fkl(ykl, yl, i, j, LAMBDA, MU, K, C);
        for(j=0;j<C+1;j++)
            for(i=0;i<K+1;i++)
                ykl[i][j] += DELTA*dykl[i][j];
        for(j=0;j<C+1;j++)
        {
            yl[j]=0;
            for(i=0;i<K+1;i++)
                yl[j]+= ykl[i][j];
        }
    }
    
    for(j=0;j<C+1;j++)
    {
        printf("\n%2d ", j);
        for(i=0;i<30;i++)
            printf("%.2f ",ykl[i][j]);
    }
    printf("\n\n");
    for(j=0;j<C+1;j++)
    {
        printf("%2d %f\n", j, yl[j]);
    }
    printf("\n");
    
    sum = 0;
    for(j=0;j<C+1;j++)
    {
        sum += yl[j];
    }
    printf("SUM[y[l],l] = %f\n", sum);
    
    sum = 0;
    for(j=0;j<C+1;j++)
    {
        for(i=0;i<K+1;i++)
            sum += (i+j)*ykl[i][j];
    }
    printf("s = %f\n", sum);
    
    DY=0;
    for(j=0;j<C+1;j++)
    {
        sum=0;
        for(i=0;i<K+1;i++)
            sum += dykl[i][j];
        DY += sum*sum;
    }
    DY = sqrt(DY);
    printf("DY = %f\n", DY);
    
    getchar();
    return 0;
}
