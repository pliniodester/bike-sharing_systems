
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STEPS 100000
#define N 100
#define LAMBDA 0.8
#define MU 1
#define GAMA 1

float rexp(float lambda) {
float u = (float)((long double)rand()/(RAND_MAX+1));
if(lambda > 0)
  return -1/lambda*log(1-u);
return -1;
}

void init_vec(int vec[], int n) {
  int i;
  for(i=0; i<n; i++)
    vec[i] = 0;
}

int count_vec(int vec[], int n) {
  if(n==1)
    return vec[0];
  return (vec[n-1]+count_vec(vec, n-1));
}

int randN() {
return (int)(((long double)rand()/(RAND_MAX+1))*N);
}

float next_step(int q[], int vq[]) {
  int i, j, flag = -1;
  float t_min = rexp(N*LAMBDA);
  float t_aux;
  
  for (i=0; i<N; i++){
    if(q[i]>0) {
      t_aux = rexp(GAMA);
      if(t_aux < t_min) {
        t_min = t_aux;
        flag = i;
      }
    }
    if(vq[i]>0) {
      t_aux = rexp(vq[i]*MU);
      if (t_aux < t_min) {
        t_min = t_aux;
        flag = N + i;
      }
    }
  }

  if(flag == -1) {
    i = randN();
    j = randN();
    if(q[i] < q[j])
      vq[i] = vq[i]+1;
    else
      vq[j] = vq[j]+1;
  }
  else {
    if(flag < N) {
      q[flag] = q[flag]-1;
    }
    else {
      vq[flag%N] = vq[flag%N]-1;
      q[flag%N] = q[flag%N]+1;
    }
  }
  return t_min;
}

main() {
  srand(time(0));
  int queue[N], vqueue[N];
  int i, max = 0;
  float total_time = 0;
  init_vec(queue, N);
  init_vec(vqueue, N);
  
  for(i=0;i<9/10*STEPS;i++) {
    total_time += next_step(queue, vqueue);
  }
  for(i=0;i<STEPS/10;i++) {
    total_time += next_step(queue, vqueue);
  }  
/*
  for(i=0; i<N; i++) {
    printf("%d %d\n",vqueue[i], queue[i]);
  }
*/

  for(i=0; i<N; i++) {
    if(max<queue[i])
      max=queue[i];
  }
  
  int histogram[max+1];
  init_vec(histogram, max+1);
  
  for(i=0; i<N; i++) {
  histogram[queue[i]] += 1;
  }
  
  FILE* data = fopen("data.txt", "w");
  printf("\nHistogram:\n");
  for(i=0; i<max+1; i++) {
    printf("%d %d\n", i, histogram[i]);
    fprintf(data, "%d %d\n", i, histogram[i]);
  }
  printf("\nTotal time = %f\n", total_time);
  fprintf(data, "\nTotal time = %f\n", total_time);
  printf("Number of Steps = %.1e\n", (double)STEPS);
  printf("Average time/step = %.1e\n", (double)total_time/STEPS);
  printf("Number of Stations = %d\n", count_vec(histogram, max+1));
  printf("Number of Bikes in the virtual queue = %d\n", count_vec(vqueue, N));
  printf("Number of Bikes in the queue = %d\n", count_vec(queue, N));
  fclose(data);
  
  getchar();
  return 0;
}



