
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define N 100
#define K 30

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

float exp_var(float lambda)
{
    float u = (float)((long double)rand()/(RAND_MAX+1));
    if(lambda > 0)
        return -1/lambda*log(1-u);
    return 1000000;
}

float TimeStation(float lambda)
{
    float u = (float)((long double)rand()/(RAND_MAX+1));
    if(lambda > 0)
        return -1/lambda*log(1-u);
    return -1;
}

float TimeVirtualQueue(float gamma)
{

/*
    float u = (float)((long double)rand()/(RAND_MAX+1));
    if(mu > 0)
        return -1/gamma*log(1-u);
    return -1;
*/
    return 1/gamma;
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

int initialise_system(int n, int q[]) {
    int i;
    while(head != NULL)
        delete_end_list();
    set_int_vec(q, n);
    return 0;
}

float next_step(int n, float lambda, float gamma, float mu, float curr_time, int q[]) {
    int i, sum=0, ind;
    float t_aux, t_lambda, t_mu;
        
    for(i=0;i<n;i++)
        if(q[i] != 0)
            sum++;
    
    t_lambda = curr_time + exp_var(n*lambda);
    t_mu = curr_time + exp_var(sum*mu);
    
    if(tail != NULL)
        if(tail->time < t_lambda && tail->time < t_mu)
        {
            q[tail->queue]++;
            t_aux = tail->time;
            delete_end_list();
            printf("G");
            return t_aux;
        }
    if(t_lambda < t_mu || sum == 0)
    {
        add_to_list(TimeVirtualQueue(gamma)+curr_time,f_choice(q, n));
        printf("L");
        return t_lambda;
    }
    ind = rand()%sum;
    for(i=0; ind < 0 ;i++)
    {
        if(q[i]!=0)
            ind--;
    }
    if(ind>n)
        printf("Error: ind > n\n");
    q[i-1]--;
    printf("M");
    return t_mu;
}

void Nvq_yk(long int vq_yk[], int stations[], int n)
{
    int i, sum[n];
    struct bikes_struct *ptr = head;
    for(i=0;i<n;i++)
    {
        sum[i]=0;
    }
    while(ptr!=NULL)
    {
        sum[ptr->queue]++;
        ptr = ptr->next;
    }
    for(i=0;i<n;i++)
    {
        vq_yk[stations[i]] += sum[i];
    }
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

int run_system(int n, int k, float lambda, float gamma, float mu, int steps, int samples)
{
    int i, j, q[n];
    float aux, curr_time=0, step_time;
    
    int hist_sample_yk[k+1][n+1];
    long int hist_sample_vq_yk[k+1];
    float avg_yk[k+1], dev_yk[k+1];
    
    printf("\nN = %d\nLambda = %.2f\nGamma = %.2f\nMu = %.2f\nSteps = %.0e\nSample = %.0e\n\n", n, lambda, gamma, mu, (double)steps, (double)samples);
    
    initialise_system(n, q);
    for(i=0;i<k+1;i++)
        for(j=0;j<n+1;j++)
            hist_sample_yk[i][j]=0;
    for(i=0;i<k+1;i++)
        hist_sample_vq_yk[i]=0;
    
    for(i=0;i<steps;i++)
        curr_time = next_step(n, lambda, gamma, mu, curr_time, q);
    
    for(i=0;i<samples;i++)
    {
        curr_time = next_step(n, lambda, gamma, mu, curr_time, q);
        
        for(j=0;j<k+1;j++)
            hist_sample_yk[j][Ny_k(q, n, j)]++;
        Nvq_yk(hist_sample_vq_yk, q, n);
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
    
    printf("STEPS	%e\n",(double)steps+samples);
    printf("N	%d\n",n);
    printf("Lambda	%f\n",lambda);
    printf("Gamma	%f	DETERMINISTIC\n",lambda);
    printf("Mu	%f\n",mu);    
    
    printf("\nAverage vy_k\n");
    for(i=0;i<6;i++)
    {
        printf("%d	%f\n", i, (float)hist_sample_vq_yk[i]/samples/n);
    }
    
    printf("\nAverage	y_k\n");
    for(i=0;i<6;i++)
    {
        printf("%d	%f\n", i, (float)avg_yk[i]);
    }    

    
    FILE* data = fopen("QueueWithDelay_deterministic.txt", "w");
    fprintf(data, "STEPS	%e\n",(double)steps+samples);
    fprintf(data, "N	%d\n",n);
    fprintf(data, "Lambda	%f\n",lambda);
    fprintf(data, "Gamma	%f	DETERMINISTIC\n",lambda);
    fprintf(data, "Mu	%f\n",mu);    
    
    fprintf(data, "\n\nAverage vy_k\n");
    for(i=0;i<k+1;i++)
    {
        fprintf(data, "%d	%f\n", i, (float)hist_sample_vq_yk[i]/samples/n);
    }
    
    fprintf(data, "\n\nAverage	y_k\n");
    for(i=0;i<k+1;i++)
    {
        fprintf(data, "%d	%f\n", i, (float)avg_yk[i]);
    }    
    fclose(data);
    
    return 0;
}


int main(void)
{
    int n=N, k=K;
    float lambda, gamma, mu;
    srand(time(NULL));
    
    lambda = 0.7;
    gamma = 0.5;
    mu = 1;
    
    run_system(n, k, lambda, gamma, mu, 1000, 10000); //INT_MAX = 2147483647
   
    getchar();
    return 0;
}
