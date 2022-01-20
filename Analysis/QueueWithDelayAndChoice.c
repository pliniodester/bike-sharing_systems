
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STEPS 1000000
#define N 100
#define HISTOGRAM 15
#define LAMBDA 0.8
#define MU 100
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

void initl_vec(long int vec[], int n) {
  int i;
  for(i=0; i<n; i++)
    vec[i] = 0;
}

int sum_int_vec(int vec[], int n) {
  if(n==1)
    return vec[0];
  return (vec[n-1]+sum_int_vec(vec, n-1));
}

float sum_float_vec(float vec[], int n) {
  if(n==1)
    return vec[0];
  return (vec[n-1]+sum_float_vec(vec, n-1));
}

double sum_double_vec (double vec[], int n) {
  if(n==1)
    return vec[0];
  return (vec[n-1]+sum_double_vec(vec, n-1));
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
  int queue[N], vqueue[N], aux_hist[HISTOGRAM];
  long int sum_hist[HISTOGRAM], sum2_hist[HISTOGRAM], avg_qbikes=0, avg_vqbikes=0;
  double avg_hist[HISTOGRAM], dev_hist[HISTOGRAM];
  int i, j;
  float total_time = 0;
  init_vec(queue, N);
  init_vec(vqueue, N);
  init_vec(aux_hist, HISTOGRAM);
  initl_vec(sum_hist, HISTOGRAM);
  initl_vec(sum2_hist, HISTOGRAM);
  
  for(i=0;i<9*(STEPS/10);i++) {
    total_time += next_step(queue, vqueue);
  }
  
  for(i=0;i<STEPS/10;i++) {
    total_time += next_step(queue, vqueue);
    for(j=0; j<N; j++) {
      aux_hist[queue[j]] += 1;
      avg_qbikes += (long int)queue[j];
      avg_vqbikes += (long int)vqueue[j];
    }
    for(j=0;j<HISTOGRAM;j++) {
      sum_hist[j] += aux_hist[j];
      sum2_hist[j] += aux_hist[j]*aux_hist[j];
    }
    init_vec(aux_hist, HISTOGRAM);
  }  

  for(i=0;i<HISTOGRAM;i++) {
    avg_hist[i] = (double)(sum_hist[i]/(STEPS/10))+(double)(sum_hist[i]%(STEPS/10))/(STEPS/10);
    dev_hist[i] = sqrt((double)(sum2_hist[i]/(STEPS/10))+(double)(sum2_hist[i]%(STEPS/10))/(STEPS/10)-avg_hist[i]*avg_hist[i]);
  }
  avg_qbikes = avg_qbikes/(STEPS/10);
  avg_vqbikes = avg_vqbikes/(STEPS/10);

  FILE* data = fopen("QueueWithDelayAndChoice.txt", "w");
  printf("\nAverage histogram for the %d last steps:\n", STEPS/10);
  fprintf(data, "\nAverage histogram for the %d last steps:\n", STEPS/10);
  for(i=0; i<HISTOGRAM; i++) {
    printf("%2d %5.2f %.2f\n", i, avg_hist[i]/N, dev_hist[i]/N);
    fprintf(data, "%d %f %f\n", i, avg_hist[i]/N, dev_hist[i]/N
    );
  }
  printf("\nNumber of Stations = %.0f\n", (float)sum_double_vec(avg_hist, HISTOGRAM));
  fprintf(data, "\nNumber of Stations = %.0f\n", (float)sum_double_vec(avg_hist, HISTOGRAM));
  printf("Lambda = %.1f, Mu = %.1f, Gama = %.1f\n", (float)LAMBDA, (float)MU, (float)GAMA);
  fprintf(data, "Lambda = %.1f, Mu = %.1f, Gama = %.1f\n", (float)LAMBDA, (float)MU, (float)GAMA);
  printf("Total time = %.1f\n", total_time);
  fprintf(data, "Total time = %.1f\n", total_time);
  printf("Number of Steps = %.1e\n\n", (double)STEPS);
  fprintf(data, "Number of Steps = %.1e\n\n", (double)STEPS);
  
  printf("Average time/step = %.1e\n", (double)total_time/STEPS);
  fprintf(data, "Average time/step = %.1e\n", (double)total_time/STEPS);
  printf("Average number of bikes in the virtual queue = %d\n", avg_vqbikes);
  fprintf(data, "Average number of bikes in the virtual queue = %d\n", avg_vqbikes);
  printf("Average number of bikes in the queue = %d\n", avg_qbikes);
  fprintf(data, "Average number of bikes in the queue = %d\n", avg_qbikes);
  fclose(data);
  
  getchar();
  return 0;
}



