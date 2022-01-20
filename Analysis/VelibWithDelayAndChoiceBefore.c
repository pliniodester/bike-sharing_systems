
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define STEPS 10000000
#define SAMPLE 9000000
#define N 100 //number of stations
//#define M 600  //number of bikes
#define K 30  //capacity of each station
#define LAMBDA 1  //rate of people per station
#define MIN_TRIP 5
#define MAX_TRIP 50
#define STEP_TRIP 5
#define INF 1000000 //a big number
// #define R 0 //rate of people who search another non-empty station

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

void initl_vec(long long int vec[], int n) {
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

void fill_queue_rand(int q[], int n, int k, int m) {
  int i;
  for(i=0; i<m; i++)
    q[randN()]++;
}

float next_step(int s[], int vq[], float lambda, float mu) {
  int i, j, aux, flag, ind;
  float t_min = INF, t_aux;
  
  //decide the next event (the one with t_min)
  for(i=0; i<N; i++){
    if(s[i]>0) {
      t_aux = rexp(lambda); //time to someone get a bike
      if(t_aux < t_min) {
        t_min = t_aux;
        ind = i;
      }
    }
    if(vq[i]>0) {
      t_aux = rexp((float)vq[i]*mu); //time to someone return a bike
      if (t_aux < t_min) {
        t_min = t_aux;
        ind = N + i;
      }
    }
  }
  
  //execute event
  if(ind < N) { //someone get a bike and decide which station to go
    s[ind]--;
    i = randN();
    j = randN();
    if(s[i] < s[j])
      vq[i]++;
    else
      vq[j]++;
  }
  else { //someone arrives in a station
    ind = ind - N;
    vq[ind]--;
    if(s[ind] < K) {//station is not full
      s[ind]++;
    }
    else { //station is full
      flag = 0;
      while(flag == 0) { //searches a non-full station
        aux = randN();
        if (s[aux] < K) {
        s[aux]++;
        flag = 1;
        }
      }
    }
  }
  if(ind < 0 || ind >= N)
    printf("ERRO ind = %d\n", ind);
  return t_min;
}

main() {
  srand(time(0));
  int station[N], vqueue[N], aux_hist[K+1];
  long long int sum_hist[K+1], sum2_hist[K+1], sum_sbikes=0, sum_vqbikes=0, sum_rbikes=0;
  double sum2_sbikes=0, sum2_vqbikes=0;
  float total_time, mu, avg_hist[K+1], dev_hist[K+1], avg_sbikes=0, avg_vqbikes=0, dev_sbikes=0, dev_vqbikes=0;
  int i, j, aux;
  
  for( mu = (float)1/MIN_TRIP; mu>=(float)1/MAX_TRIP; mu = mu/(1+mu*STEP_TRIP) ) {
  
    total_time = 0;
    init_vec(station, N);
    init_vec(vqueue, N);
    init_vec(aux_hist, K+1);
    initl_vec(sum_hist, K+1);
    initl_vec(sum2_hist, K+1);
    sum_sbikes=0; sum_vqbikes=0; sum_rbikes=0;
    sum2_sbikes=0; sum2_vqbikes=0;
    avg_sbikes=0; avg_vqbikes=0; dev_sbikes=0; dev_vqbikes=0;
  
    fill_queue_rand(vqueue, N, K, N*K/2 + N*LAMBDA/mu );
  
    for(i=0;i<(STEPS-SAMPLE);i++) {
      total_time += next_step(station, vqueue, LAMBDA, mu);
    }
  
    for(i=0;i<SAMPLE;i++) {
      total_time += next_step(station, vqueue, LAMBDA, mu);
      sum_sbikes += (long long int)sum_int_vec(station, N);
      sum_vqbikes += (long long int)sum_int_vec(vqueue, N);
      sum2_sbikes += (double)sum_int_vec(station, N)*sum_int_vec(station, N);
      sum2_vqbikes += (double)sum_int_vec(vqueue, N)*sum_int_vec(vqueue, N);
      for(j=0; j<N; j++) {
        aux_hist[station[j]]++;
      }
      for(j=0;j<K+1;j++) {
        sum_hist[j] += (long int)aux_hist[j];
        sum2_hist[j] += (long int)aux_hist[j]*aux_hist[j];
      }
      init_vec(aux_hist, K+1);
    }

    for(i=0;i<K+1;i++) {
      avg_hist[i] = (float)((double)sum_hist[i]/SAMPLE/N);
      dev_hist[i] = (sqrt((float)( (double)sum2_hist[i]/SAMPLE/N/N )-avg_hist[i]*avg_hist[i]));
    }
    avg_sbikes = (float)(sum_sbikes/SAMPLE)+(float)(sum_sbikes%SAMPLE)/SAMPLE;
    dev_sbikes = sqrt((float)(sum2_sbikes/SAMPLE-avg_sbikes*avg_sbikes));
    avg_vqbikes = (float)(sum_vqbikes/SAMPLE)+(float)(sum_vqbikes%SAMPLE)/SAMPLE;
    dev_vqbikes = sqrt((float)(sum2_vqbikes/SAMPLE-avg_vqbikes*avg_vqbikes));
    
    printf("%5.2f %.3f (%.3f) ", 1/mu, avg_hist[0]+avg_hist[K], sqrt(dev_hist[0]*dev_hist[0]+dev_hist[K]*dev_hist[K]));
    printf("sbikes=%4.0f(%4.1f) vqbikes=%4.0f(%4.1f)\n", avg_sbikes, dev_sbikes, avg_vqbikes, dev_vqbikes);

/*
//    FILE* data = fopen("VelibWithDelayAndChoiceBefore.txt", "w");
    printf("\nAverage histogram for the %d last steps:\n", SAMPLE);
//    fprintf(data, "\nAverage histogram for the %d last steps:\n", SAMPLE);
    for(i=0; i<K+1; i++) {
      printf("%2d %5.2f %.2f\n", i, avg_hist[i], dev_hist[i]);
//      fprintf(data, "%d %f %f\n", i, avg_hist[i]/N, dev_hist[i]/N);
    }
    printf("\nNumber of Stations = %.0f\n", sum_float_vec(avg_hist, K+1));
//    fprintf(data, "\nNumber of Stations = %.0f\n", sum_float_vec(avg_hist, K+1));
    printf("Total time = %.1f\n", total_time);
//    fprintf(data, "Total time = %.1f\n", total_time);
    printf("Number of Steps = %.1e\n\n", (double)STEPS);
//    fprintf(data, "Number of Steps = %.1e\n\n", (double)STEPS);
  
    printf("Statistics over the last %d steps:\n", SAMPLE);
    printf("Average time/step = %.1e\n", (double)total_time/STEPS);
//    fprintf(data, "Average time/step = %.1e\n", (double)total_time/STEPS);
    printf("Average number of lost bikes = %.2f (%.2f)\n", avg_rbikes, dev_rbikes);
//    fprintf(data, "Average number of lost bikes = %f (%f)\n", avg_rbikes, dev_rbikes);
    printf("Average number of bikes in the virtual queue = %.2f (%.2f)\n", avg_vqbikes, dev_vqbikes);
//    fprintf(data, "Average number of bikes in the virtual queue = %f (%f)\n", avg_vqbikes, dev_vqbikes);
    printf("Average number of bikes in the stations = %.2f (%.2f)\n", avg_sbikes, dev_sbikes);
//    fprintf(data, "Average number of bikes in the stations = %f (%f)\n", avg_sbikes, dev_sbikes);
//    fclose(data);
*/
  }
  
  getchar();
  return 0;
}



