
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define N 100
#define K 30
#define LAMBDA 1
#define MU 0.02

struct bikes_struct
{
    float time;
    int queue;
    struct bikes_struct *prev;
    struct bikes_struct *next;
};

struct bikes_struct *head = NULL;
struct bikes_struct *tail = NULL;


struct bikes_struct* create_list(float time, int queue)
{
    struct bikes_struct *ptr = (struct bikes_struct*)malloc(sizeof(struct bikes_struct));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->time = time;
    ptr->queue = queue;
    ptr->next = NULL;
    ptr->prev = NULL;

    head = tail = ptr;
    return ptr;
}

struct bikes_struct* add_to_list(float time, int queue)
{
    struct bikes_struct *aux1;
    struct bikes_struct *aux2;
    if(NULL == head)
    {
        return (create_list(time, queue));
    }

    struct bikes_struct *ptr = (struct bikes_struct*)malloc(sizeof(struct bikes_struct));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->time = time;
    ptr->queue = queue;
    ptr->next = NULL;
    ptr->prev = NULL;
    aux1 = head;
    if(time > aux1->time)
    {
        head = ptr;
        ptr->next = aux1;
        aux1->prev = ptr;
    }
    else
    {
        aux2 = aux1->next;
        while(aux2 != NULL && time < aux2->time)
        {
            aux1 = aux2;
            aux2 = aux1->next;
        }
        aux1->next = ptr;
        ptr->next = aux2;
        ptr->prev = aux1;
        if(aux2 == NULL)
            tail = ptr;
        else
            aux2->prev = ptr;
    }
    return ptr;
}

int delete_end_list()
{
    struct test_struct *del = tail;
    if(del==NULL)
        return -1;
    if(tail->prev != NULL)
        tail = tail->prev;
    else
    {
        tail = NULL;
        head = NULL;
        free(del);
        del = NULL;
        return 0;
    }
    tail->next = NULL;
    free(del);
    del = NULL;
    return 0;
}

void print_list(void)
{
    struct bikes_struct *ptr = head;

    printf("\n -------Printing list Start------- \n");
    while(ptr != NULL)
    {
        printf("\n [%d] [%f] \n",ptr->queue, ptr->time);
        ptr = ptr->next;
    }
    printf("\n -------Printing list End------- \n");
    
    return;
}


int sum_list(void)
{
    int sum = 0;
    struct bikes_struct *ptr = head;
    while(ptr != NULL)
    {
        sum++;
        ptr = ptr->next;
    }
    return sum;
}

float TimeStation(float lambda)
{
    float u = (float)((long double)rand()/(RAND_MAX+1));
    if(lambda > 0)
        return -1/lambda*log(1-u);
    return -1;
}

float TimeVirtualQueue(float mu)
{

    float u = (float)((long double)rand()/(RAND_MAX+1));
    if(mu > 0)
        return -1/mu*log(1-u);
    return -1;

//    return 1/mu;
}

int f_choice(int vec[], int n)
{
    int i = randN(n), j = randN(n);
    if(vec[i]<vec[j])
        return i;
    else
        return j;
}

int randN(int n)
{
    return rand()%n;
}

int randN_free(int vec[], int n, int k)
{
    int r = randN(n);
    while(vec[r] == k)
    {
        r = randN(n);
    }
    return r;
}

void set_int_vec(int vec[], int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        vec[i] = 0;
    }
}

void set_float_vec(float vec[], int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        vec[i] = 0;
    }
}

int sum_int_vec(int vec[], int n)
{
    if(n==1)
        return vec[0];
    return sum_int_vec(vec, n-1)+vec[n-1];
}

void print_hist(int bs[], int n, int k)
{
    int i, hist[k+1];
    set_int_vec(hist, k+1);
    for(i=0;i<n;i++)
    {
        hist[bs[i]]++;
    }
    printf("\nHistogram:\n");
    for(i=0;i<k+1;i++)
    {
        printf("hist[%2d] = %.2f\n", i, (float)hist[i]/n);
    }
}

int initialise_system(int n, int m, int k, float lambda, float mu, int bs[], float ts[]) {
    int i;
    if(m > n*k)
    {
        printf("WARNING: Number of bikes bigger than number of places\n");
    }
    while(head != NULL)
        delete_end_list();
    for(i=0;i<m;i++)
        add_to_list(TimeVirtualQueue(mu), randN(n));
    set_int_vec(bs, n);
    for(i=0;i<n;i++)
        ts[i]=TimeStation(lambda);
    return 0;
}

float next_step(int n, int m, int k, float lambda, float mu, float curr_time, int bs[], float ts[]) {
    int i, ind = -1;
    float t_min;
    
    if(tail != NULL)
        t_min = tail->time;
    else
        t_min = ts[0]+0.1;
    for(i=0;i<N;i++)
    {
        if(ts[i] < t_min)
        {
            t_min = ts[i];
            ind = i;
        }
    }
    
    if(ind == -1)
    {
        if(bs[tail->queue]<k)
            bs[tail->queue]++;
        else
        {
            add_to_list(TimeVirtualQueue(mu)+curr_time, f_choice(bs, n));
        }
        delete_end_list();
    }
    else
    {
        if(bs[ind]>0)
        {
            bs[ind]--;
            add_to_list(TimeVirtualQueue(mu)+curr_time, f_choice(bs, n));
        }
        ts[ind] = curr_time + TimeStation(lambda);
    }
    return t_min;
}

int Ny_k (int vec[], int n , int k)
{
      int i, sum=0;
      for(i=0;i<n;i++)
      {
          if(vec[i]==k)
              sum++;
      }
      return sum;
}

int run_detailed_system(int n, int m, int k, float lambda, float mu, int steps, int samples)
{
    int i, j, bike_station[n];
    float aux, curr_time=0, time_station[n], step_time;
    
    int hist_sample_yk[k+1][n+1];
    float avg_yk[k+1], dev_yk[k+1];
    float avg_sbikes=0, dev_sbikes=0;
    
    printf("\nN = %d\nM = %d\nK = %d\nLambda = %.2f\nMu = %.2f\nSteps = %.0e\nSample = %.0e\n\n", n, m, k, lambda, mu, (double)steps, (double)samples);
    
    initialise_system(n, m, k, lambda, mu, bike_station, time_station);
    for(i=0;i<k+1;i++)
        for(j=0;j<n+1;j++)
            hist_sample_yk[i][j]=0;
    
    for(i=0;i<steps;i++)
        curr_time = next_step(n, m, k, lambda, mu, curr_time, bike_station, time_station);
    
    step_time = curr_time;
    printf("\nTime before sample: %.f\n", step_time);
    
    for(i=0;i<samples;i++)
    {
        curr_time = next_step(n, m, k, lambda, mu, curr_time, bike_station, time_station);
        
        for(j=0;j<k+1;j++)
            hist_sample_yk [j][Ny_k(bike_station, n, j)]++;
    }
    
    for(i=0;i<k+1;i++)
    {
    avg_yk[i] = 0;
        for(j=1;j<n+1;j++)
        {
            avg_yk[i] += j*hist_sample_yk[i][j];
        }
    avg_yk[i] = avg_yk[i]/samples/n;
    }
    for(i=0;i<k+1;i++)
    {
    dev_yk[i] = 0;
        for(j=0;j<n+1;j++)
        {
            dev_yk[i] += (float)hist_sample_yk[i][j]*(n*avg_yk[i]-j)*(n*avg_yk[i]-j);
        }
    dev_yk[i] = sqrt(dev_yk[i]/samples)/n;
    }
   
    for(i=1;i<k+1;i++)
        avg_sbikes += i*avg_yk[i];
   
    printf("Time after sample: %.f\n", curr_time);
    printf("Average step time: %.2e\n", (double)(curr_time-step_time)/samples);
    printf("Average proportion of problematic stations: %.4f(%.4f)\n", avg_yk[0]+avg_yk[k], sqrt(dev_yk[0]*dev_yk[0]+dev_yk[k]*dev_yk[k]));
    printf("Average number of bikes per station: %.2f\n", avg_sbikes);
    printf("Average number of bikes per virtual queue: %.2f\n", (float)m/n-avg_sbikes);
    
/*
    printf("\nHistogram of the Ny_k samples\n");
    printf("       ");
    for(i=0;i<14;i++)
        printf("   %2d", i);
    for(i=0;i<k+1;i++)
    {
        printf("\n #Ny[%2d]", i);
        for(j=0;j<n&&j<14;j++)
            printf(" %.2f", (float)hist_sample_yk[i][j]/samples);
    }
*/
    FILE* data = fopen("VelibDelayChoice.txt", "w");
    fprintf(data, "\nHistogram of the Ny_k samples\n");
    fprintf(data, " -");
    for(i=0;i<n+1;i++)
        fprintf(data, " %d", i);
    for(i=0;i<k+1;i++)
    {
        fprintf(data, "\n #Ny[%d]", i);
        for(j=0;j<n+1;j++)
            fprintf(data, " %f", (float)hist_sample_yk[i][j]/samples);
    }
    fclose(data);
    
    return 0;
}


int run_system(int n, int m, int k, float lambda, float mu, int steps, int samples)
{
    int i, j, bike_station[n];
    float aux, curr_time = 0, time_station[n], step_time;
    
    int hist_sample_yk[k+1][n+1];
    double avg_yk[k+1], dev_yk[k+1];
    float avg_sbikes=0;
    
    initialise_system(n, m, k, lambda, mu, bike_station, time_station);
    for(i=0;i<k+1;i++)
        for(j=0;j<n+1;j++)
            hist_sample_yk[i][j]=0;
    
    for(i=0;i<steps;i++)
    {
        curr_time = next_step(n, m, k, lambda, mu, curr_time, bike_station, time_station);
    }
    
    step_time = curr_time;
    
    for(i=0;i<samples;i++)
    {
        curr_time = next_step(n, m, k, lambda, mu, curr_time, bike_station, time_station);
        for(j=0;j<k+1;j++)
            hist_sample_yk [j][Ny_k(bike_station, n, j)]++;
    }

    for(i=0;i<k+1;i++)
    {
    avg_yk[i] = 0;
        for(j=1;j<n+1;j++)
        {
            avg_yk[i] += (double)j*hist_sample_yk[i][j];
        }
    avg_yk[i] = avg_yk[i]/samples/n;
    }
    for(i=0;i<k+1;i++)
    {
    dev_yk[i] = 0;
        for(j=0;j<n+1;j++)
        {
            dev_yk[i] += (double)hist_sample_yk[i][j]*(n*avg_yk[i]-j)*(n*avg_yk[i]-j);
        }
    dev_yk[i] = sqrt(dev_yk[i]/samples)/n;
    }
    
    for(i=1;i<k+1;i++)
        avg_sbikes += i*avg_yk[i];
    
    printf("%d %.1e %2.1f %.2f %.4f(%.4f) ", n, (double)steps+(double)samples, (float)lambda/mu, (float)m/n, avg_yk[0]+avg_yk[k], sqrt(dev_yk[0]*dev_yk[0]+dev_yk[k]*dev_yk[k]));
    printf("%.2f %.2f\n",avg_sbikes, (float)m/n-avg_sbikes);
    
    FILE* data = fopen("VelibDelayChoiceBeforeNotDetailed.txt", "a");
    fprintf(data, "%e %d %d %f %f %e %e ", (double)steps+(double)samples, n, k, (float)lambda/mu, (float)m/n, avg_yk[0]+avg_yk[k], sqrt(dev_yk[0]*dev_yk[0]+dev_yk[k]*dev_yk[k]));
    fprintf(data, "%f %f\n",avg_sbikes, (float)m/n-avg_sbikes);
    fclose(data);

    return 0;
}


int main(void)
{
    int k;
    float mu;
    srand(time(NULL));

//  run_detailed_system(N, M, K, LAMBDA, MU, STEPS, SAMPLES);
//    run_detailed_system(N, N*K/2+N*LAMBDA/MU, K, LAMBDA, MU, 1000*N, 100000); //INT_MAX = 2147483647
    
    FILE* data = fopen("VelibDelayChoiceBeforeNotDetailed.txt", "w");
    fprintf(data, "STEPS N K lambda/mu s y0+yK var(y0,yK) StationBikes StreetBikes\n");
    fclose(data);
    
//    for(mu=0.0675; mu<3; mu=mu/(1-mu*0.5))
//        run_system(N, N*K/2+N*LAMBDA/mu, K, LAMBDA, mu, 500000, 1000000);
    run_detailed_system(100, 10*100, 3, 1, 0.1, 500000, 1000000);
   
    getchar();
    return 0;
}
