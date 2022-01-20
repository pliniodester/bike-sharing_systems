
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STEPS 100000
#define SAMPLE 10000
#define N 100 //number of stations
#define M 600  //number of bikes
#define K 10  //capacity of each station
#define LAMBDA 1  //rate of people per station
#define MU 100  //1/mu = average trip time
#define GAMA 0.1  //1/gama = average delay time to get to the chosen station
#define INF 1000000
#define R 0 //rate of people who search another station (not empty)

float rexp(float lambda) {
float u = (float)((long double)rand()/(RAND_MAX+1));
if(lambda > 0)
  return -1/lambda*log(1-u);
return INF;
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

int randN() {
return (int)(((long double)rand()/(RAND_MAX+1))*N);
}

float next_step(int s[], int vq[]) {
  int i, j, aux, ind = -1;
  float t_min, t_aux;
  
  
  aux = 0;
  for(i=0;i<N;i++)
    aux += s[i]+vq[i];
  t_min = rexp((float)(M-aux)*MU);
  t_aux = t_min;
  
  aux = 0;
  for(i=0;i<N;i++)
    if(s[i]==0)
      aux += 1;
  for(i=0; i<N; i++){
    if(s[i]>0) {
      t_aux = rexp((float)LAMBDA*(float)N/(N-R*aux));
      if(t_aux < t_min) {
        t_min = t_aux;
        ind = i;
      }
    }
    if(vq[i]>0) {
      t_aux = rexp((float)vq[i]*GAMA);
      if (t_aux < t_min) {
        t_min = t_aux;
        ind = N + i;
      }
    }
  }

  if(ind == -1) {
    i = randN();
    j = randN();
    if(s[i]==K && s[j]==K) {
      printf("Treat case when both chosen stations are full.\n"); //take this out, because the person takes the queue
    }
    else {
      if(s[i] < s[j])
        vq[i] = vq[i]+1;
      else
        vq[j] = vq[j]+1;
    }
  }
  else {
    if(ind < N) {
      s[ind] = s[ind]-1;
    }
    else { //question here!! choose another randomly
      vq[ind%N] = vq[ind%N]-1;
      if(s[ind%N]<K)
        s[ind%N] = s[ind%N]+1;
    }
  }
  if(t_min==-1)
    printf("Erro: t_min = -1");
  return t_min;
}

main() {
  srand(time(0));
  int station[N], vqueue[N], aux_hist[K+1];
  long int sum_hist[K+1], sum2_hist[K+1], sum_sbikes=0, sum_vqbikes=0, sum_rbikes=0;
  double sum2_sbikes=0, sum2_vqbikes=0, sum2_rbikes=0;
  float avg_hist[K+1], dev_hist[K+1], avg_sbikes=0, avg_vqbikes=0, avg_rbikes=0, dev_sbikes=0, dev_vqbikes=0, dev_rbikes=0;
  int i, j, aux;
  float total_time = 0;
  init_vec(station, N);
  init_vec(vqueue, N);
  init_vec(aux_hist, K+1);
  initl_vec(sum_hist, K+1);
  initl_vec(sum2_hist, K+1);
  
  for(i=0;i<(STEPS-SAMPLE);i++) {
    total_time += next_step(station, vqueue);
  }
  
  for(i=0;i<SAMPLE;i++) {
    total_time += next_step(station, vqueue);
    sum_sbikes += (long int)sum_int_vec(station, N);
    sum_vqbikes += (long int)sum_int_vec(vqueue, N);
    sum_rbikes += (long int)(M-sum_int_vec(vqueue, N)-sum_int_vec(station, N));
    sum2_sbikes += (double)sum_int_vec(station, N)*sum_int_vec(station, N);
    sum2_vqbikes += (double)sum_int_vec(vqueue, N)*sum_int_vec(vqueue, N);
    sum2_rbikes += (double)(M-sum_int_vec(vqueue, N)-sum_int_vec(station, N))*(M-sum_int_vec(vqueue, N)-sum_int_vec(station, N));
    for(j=0; j<N; j++) {
      aux_hist[station[j]] += 1;
    }
    for(j=0;j<K+1;j++) {
      sum_hist[j] += (long int)aux_hist[j];
      sum2_hist[j] += (long int)aux_hist[j]*aux_hist[j];
    }
    init_vec(aux_hist, K+1);
  }

  for(i=0;i<K+1;i++) {
    avg_hist[i] = (float)(sum_hist[i]/SAMPLE)+(float)(sum_hist[i]%SAMPLE)/SAMPLE;
    dev_hist[i] = sqrt((float)(sum2_hist[i]/SAMPLE)+(float)(sum2_hist[i]%SAMPLE)/SAMPLE-avg_hist[i]*avg_hist[i]);
  }
  avg_sbikes = (float)(sum_sbikes/SAMPLE)+(float)(sum_sbikes%SAMPLE)/SAMPLE;
  dev_sbikes = sqrt((float)(sum2_sbikes/SAMPLE-avg_sbikes*avg_sbikes));
  avg_vqbikes = (float)(sum_vqbikes/SAMPLE)+(float)(sum_vqbikes%SAMPLE)/SAMPLE;
  dev_vqbikes = sqrt((float)(sum2_vqbikes/SAMPLE-avg_vqbikes*avg_vqbikes));
  avg_rbikes = (float)(sum_rbikes/SAMPLE)+(float)(sum_rbikes%SAMPLE)/SAMPLE;
  dev_rbikes = sqrt((float)(sum2_rbikes/SAMPLE-avg_rbikes*avg_rbikes));

  FILE* data = fopen("Velib.txt", "w");
  printf("\nAverage histogram for the %d last steps:\n", SAMPLE);
  fprintf(data, "\nAverage histogram for the %d last steps:\n", SAMPLE);
  for(i=0; i<K+1; i++) {
    printf("%2d %5.2f %.2f\n", i, avg_hist[i]/N, dev_hist[i]/N);
    fprintf(data, "%d %f %f\n", i, avg_hist[i]/N, dev_hist[i]/N);
  }
  printf("\nNumber of Stations = %.0f\n", sum_float_vec(avg_hist, K+1));
  fprintf(data, "\nNumber of Stations = %.0f\n", sum_float_vec(avg_hist, K+1));
  printf("Total time = %.1f\n", total_time);
  fprintf(data, "Total time = %.1f\n", total_time);
  printf("Number of Steps = %.1e\n\n", (double)STEPS);
  fprintf(data, "Number of Steps = %.1e\n\n", (double)STEPS);
  
  printf("Statistics over the last %d steps:\n", SAMPLE);
  printf("Average time/step = %.1e\n", (double)total_time/STEPS); //make a correction here
  fprintf(data, "Average time/step = %.1e\n", (double)total_time/STEPS);
  printf("Average number of running bikes = %.2f (%.2f)\n", avg_rbikes, dev_rbikes);
  fprintf(data, "Average number of running bikes = %f (%f)\n", avg_rbikes, dev_rbikes);
  printf("Average number of bikes in the virtual queue = %.2f (%.2f)\n", avg_vqbikes, dev_vqbikes);
  fprintf(data, "Average number of bikes in the virtual queue = %f (%f)\n", avg_vqbikes, dev_vqbikes);
  printf("Average number of bikes in the stations = %.2f (%.2f)\n", avg_sbikes, dev_sbikes);
  fprintf(data, "Average number of bikes in the stations = %f (%f)\n", avg_sbikes, dev_sbikes);
  fclose(data);
  
  getchar();
  return 0;
}



