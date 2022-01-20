
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define N 1500

struct bikes_struct
{
    time_t arr_time;
    int id;
    struct bikes_struct *prev;
    struct bikes_struct *next;
};

struct bikes_struct *head = NULL;
struct bikes_struct *tail = NULL;


struct bikes_struct* create_list(time_t arr_time, int id)
{
    struct bikes_struct *ptr = (struct bikes_struct*)malloc(sizeof(struct bikes_struct));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->arr_time = arr_time;
    ptr->id = id;
    ptr->next = NULL;
    ptr->prev = NULL;

    head = tail = ptr;
    return ptr;
}

struct bikes_struct* add_to_list(time_t arr_time, int id)
{
    struct bikes_struct *aux1;
    struct bikes_struct *aux2;
    if(NULL == head)
    {
        return (create_list(arr_time, id));
    }

    struct bikes_struct *ptr = (struct bikes_struct*)malloc(sizeof(struct bikes_struct));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->arr_time = arr_time;
    ptr->id = id;
    ptr->next = NULL;
    ptr->prev = NULL;
    aux1 = head;
    if( difftime(arr_time, aux1->arr_time) > 0 )
    {
        head = ptr;
        ptr->next = aux1;
        aux1->prev = ptr;
    }
    else
    {
        aux2 = aux1->next;
        while(aux2 != NULL && difftime(arr_time, aux2->arr_time) < 0)
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

typedef struct state_station {
    int id;
    int freebk;
    int freebs;
    struct tm lastCheck;
}state;

typedef struct bike_trip {
    int reason;
    struct tm out_date;
    int out_station;
    int out_bikestand;
    struct tm in_date;
    int in_station;
    int in_bikestand;
    double trip_time;
}trip;

char getcc(FILE *file)
{
    char c;
    c=getc(file);
    while(c==0)
        c=getc(file);
    return c;
}

int ToNumber(char c)
{
    if( (c >= '0') && (c <= '9'))
        return (int)(c-'0');
    printf("ERROR_04: Treated '%c' as a number.\n", c);
    return NAN;
}

int read_line_states(FILE *file, state **stations)
{
    int i=0, c;
    while((c=getcc(file))!='\n' && c!=EOF)
    {
        if(c=='{' && stations[i]==NULL)
            stations[i] = malloc(sizeof(state));
        if(c=='n' && (c=getcc(file))=='b' && (c=getcc(file))=='"' && (c=getcc(file))==':' )
        {
            stations[i]->id = ToNumber(getcc(file));
            while((c=getcc(file))!=',')
                stations[i]->id = stations[i]->id*10+ToNumber(c);
        }
        if(c=='b')
        {
            if( (c=getcc(file))=='k' && (c=getcc(file))=='"' && (c=getcc(file))==':')
            {
                stations[i]->freebk = ToNumber(getcc(file));
                while((c=getcc(file))!=',')
                    stations[i]->freebk = stations[i]->freebk*10+ToNumber(c);
            }
            else if( c=='s' && (c=getc(file))=='"' && (c=getcc(file))==':' )
            {
                stations[i]->freebs = ToNumber(getcc(file));
                while((c=getcc(file))!=',')
                    stations[i]->freebs = stations[i]->freebs*10+ToNumber(c);
            }
        } 
        if(c=='c' && (c=getcc(file))=='k' && (c=getcc(file))=='"' && (c=getcc(file))==':' && (c=getcc(file))=='"')
        {
            stations[i]->lastCheck.tm_year = ToNumber(getcc(file));
            while((c=getcc(file))!='-')
                stations[i]->lastCheck.tm_year = stations[i]->lastCheck.tm_year*10+ToNumber(c);
            stations[i]->lastCheck.tm_year += -1900;
            stations[i]->lastCheck.tm_mon = ToNumber(getcc(file));
            while((c=getcc(file))!='-')
                stations[i]->lastCheck.tm_mon = stations[i]->lastCheck.tm_mon*10+ToNumber(c);
            stations[i]->lastCheck.tm_mon += -1;
            stations[i]->lastCheck.tm_mday = ToNumber(getcc(file));
            while((c=getcc(file))!='T')
                stations[i]->lastCheck.tm_mday = stations[i]->lastCheck.tm_mday*10+ToNumber(c);
            stations[i]->lastCheck.tm_hour = ToNumber(getcc(file));
            stations[i]->lastCheck.tm_hour = stations[i]->lastCheck.tm_hour*10+ToNumber(getcc(file));
            if((c=getcc(file))!=':')
                printf("ERROR_01: reading time at station %d, found '%c' instead of ':'.\n", stations[i]->id, c);
            stations[i]->lastCheck.tm_min = ToNumber(getcc(file));
            stations[i]->lastCheck.tm_min = stations[i]->lastCheck.tm_min*10+ToNumber(getcc(file));
            if((c=getcc(file))!=':')
                printf("ERROR_02: reading time at station %d, found '%c' instead of ':'.\n", stations[i]->id, c);
            stations[i]->lastCheck.tm_sec = ToNumber(getcc(file));
            stations[i]->lastCheck.tm_sec = stations[i]->lastCheck.tm_sec*10+ToNumber(getcc(file));
            if((c=getcc(file))!='Z')
                printf("ERROR_03: reading time at station %d, found '%c' instead of ':'.\n", stations[i]->id, c);
            mktime(&(stations[i]->lastCheck));
        }
        if(c=='}')
        {
            if(i>N)
               printf("ERROR: counting stations\n");
            i++;
        }
    }
    if(c=='\n')
        return 1;
    return 0;
}

print_state(int n, state **station, FILE *s_results)
{
    char buff[70];
    strftime(buff, sizeof buff, "%d/%m/%y %H:%M", &(station[n]->lastCheck) );
    fprintf(s_results, "%d	%s	%d	%d\n", station[n]->id, buff, station[n]->freebk, station[n]->freebs);
}

void putOUTdate(int n, int d, trip *b_trip)
{
    switch(d)
    {
        case 0:
             b_trip->out_date.tm_year = 1000*n - 1000;
             break;
        case 1:
             b_trip->out_date.tm_year += 100*n - 900;
             break;
        case 2:
             b_trip->out_date.tm_year += 10*n;
             break;
        case 3:
             b_trip->out_date.tm_year += n;
             break;
        case 4:
             b_trip->out_date.tm_mon = 10*n;
             break;
        case 5:
             b_trip->out_date.tm_mon += n - 1;
             break;
        case 6:
             b_trip->out_date.tm_mday = 10*n;
             break;
        case 7:
             b_trip->out_date.tm_mday += n;
             break;
        case 8:
             b_trip->out_date.tm_hour = 10*n;
             break;
        case 9:
             b_trip->out_date.tm_hour += n;
             break;
        case 10:
             b_trip->out_date.tm_min = 10*n;
             break;
        case 11:
             b_trip->out_date.tm_min += n;
             break;
        case 12:
             b_trip->out_date.tm_sec = 10*n;
             break;
        case 13:
             b_trip->out_date.tm_sec += n;
             break;
        default:
             printf("\nError in calctime\n");
             break;
    }
}

void putINdate(int n, int d, trip *b_trip)
{
    switch(d)
    {
        case 0:
             b_trip->in_date.tm_year = 1000*n - 1000;
             break;
        case 1:
             b_trip->in_date.tm_year += 100*n - 900;
             break;
        case 2:
             b_trip->in_date.tm_year += 10*n;
             break;
        case 3:
             b_trip->in_date.tm_year += n;
             break;
        case 4:
             b_trip->in_date.tm_mon = 10*n;
             break;
        case 5:
             b_trip->in_date.tm_mon += n - 1;
             break;
        case 6:
             b_trip->in_date.tm_mday = 10*n;
             break;
        case 7:
             b_trip->in_date.tm_mday += n;
             break;
        case 8:
             b_trip->in_date.tm_hour = 10*n;
             break;
        case 9:
             b_trip->in_date.tm_hour += n;
             break;
        case 10:
             b_trip->in_date.tm_min = 10*n;
             break;
        case 11:
             b_trip->in_date.tm_min += n;
             break;
        case 12:
             b_trip->in_date.tm_sec = 10*n;
             break;
        case 13:
             b_trip->in_date.tm_sec += n;
             break;
        default:
             printf("\nError in calctime\n");
             break;
    }
}

void count(int c, int count_inf, int count_c, trip *b_trip)
{
    switch(count_inf)
    {
        case 0:
            b_trip->reason = (int)(c-'0');
            break;
        case 1:
            putOUTdate((int)(c-'0'), count_c, b_trip);
            break;
        case 2:
            b_trip->out_station = b_trip->out_station*10+(int)(c-'0');
            break;
        case 3:
            b_trip->out_bikestand = b_trip->out_bikestand*10+(int)(c-'0');
            break;
        case 4:
            putINdate((int)(c-'0'), count_c, b_trip);
            break;
        case 5:
            b_trip->in_station = b_trip->in_station*10+(int)(c-'0');
            break;
        case 6:
            b_trip->in_bikestand = b_trip->in_bikestand*10+(int)(c-'0');
            break;
        default:
            printf("Error in count.\n");
            break;
    }
}

int read_line(FILE *file, trip *b_trip )
{
     int i, c, count_inf=0, count_c=0;
     time_t aux_time;
     b_trip->out_station = 0;
     b_trip->out_bikestand = 0;
     b_trip->in_station = 0;
     b_trip->in_bikestand = 0;

     while((c = getc(file)) != '\n' && c != EOF)
     {
            switch(c)
            {
                case ';':
                    count_inf++;
                    count_c=0;
                    break;
                default:
                    count(c, count_inf, count_c, b_trip);
                    count_c++;
                    break;
            }
     }
     b_trip->out_date.tm_isdst = -1;
     b_trip->in_date.tm_isdst = -1;
     aux_time = mktime(&(b_trip->out_date));
     b_trip->trip_time = difftime( mktime(&(b_trip->in_date)), aux_time );
     if(c == '\n')
         return 1;
     return 0;
}

print_trip(trip *b_trip)
{
    printf("%d, ", b_trip->reason);
    printf("(%d,%d), ", b_trip->out_station, b_trip->in_station);
    printf("%.2f min\n", b_trip->trip_time/60);
}

void print_date(struct tm *date, FILE *results)
{
    char buff[70];
    strftime(buff, sizeof buff, "%d/%m/%y %H:%M", date );
    fprintf(results, "%s", buff);
}

print_station(int n, trip *b_trip, FILE *results)
{
    char buff[70];
    if(b_trip->out_station == n)
    {
        strftime(buff, sizeof buff, "%d/%m/%y %H:%M", &(b_trip->out_date) );
        fprintf(results, "%s	%d	%d\n", buff, -1, b_trip->out_bikestand);
    }
    if(b_trip->in_station == n)
    {
        strftime(buff, sizeof buff, "%d/%m/%y %H:%M", &(b_trip->in_date) );
        fprintf(results, "%s	%d	%d\n", buff, 1, b_trip->in_bikestand);
    }
}

float eval_perf(state **station)
{
    int i, j=0;
    float perf=0;
    for(i=0;station[i]!=NULL;i++)
    {
        if(station[i]->freebk+station[i]->freebs > 0)
            perf += ((float)(station[i]->freebk-station[i]->freebs)/(station[i]->freebk+station[i]->freebs))*((float)(station[i]->freebk-station[i]->freebs)/(station[i]->freebk+station[i]->freebs));
        else
            j++;
    }
    return sqrt(perf/(i-j));
}

void make_rel_vec(state **current, int rel_vec[])
{
    int i;
    for(i=0;i<44103;i++)
    {
        rel_vec[i]=-1;
    }
    for(i=0;current[i]!=NULL;i++)
    {
        rel_vec[current[i]->id]=i;
    }
}

void print_stations_state(state **current, FILE *results)
{
    int i;
    for(i=0;current[i]!=NULL;i++)
    {
        fprintf(results, "%d	%d	%d\n", current[i]->id, current[i]->freebk, current[i]->freebs);
    }
}

int check_day(state **station, int month, int day)
{
    int i;
    for(i=0;station[i]!=NULL;i++)
    {
        if(!(station[i]->lastCheck.tm_mon == month-1 &&  station[i]->lastCheck.tm_mday == day))
            return 0;
    }
    return 1;
}

int check_greater(time_t curr_t, state **stations)
{
    int i;
    for(i=0; stations[i]!=NULL; i++)
    {
        if( difftime( mktime(&(stations[i]->lastCheck)), curr_t) > 0)
        {
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    int i, j;
    FILE *trips_data;
    FILE *state_data;
    FILE *results;
    int rel_vec[44103];
    time_t curr_t;

    state **aux_state = malloc(N * sizeof(state*));
    state **current = malloc(N * sizeof(state*));
    trip *b_trip = (trip*)malloc(sizeof(trip));
    
    for(i=0;i<N;i++)
    {
        aux_state[i] = NULL;
        current[i] = NULL;
    }
       
    results = fopen("data_Velib.txt", "w");
    state_data = fopen("data-2015-3-1.txt", "r");
    trips_data = fopen("paris-201503.csv", "r");
    
    if (trips_data && state_data && results) {
        
        read_line_states(state_data, current);
        read_line_states(state_data, aux_state);
        
        make_rel_vec(current, rel_vec);
        while(getc(trips_data) != '\n');
        
        for(i=0; i<60000; i++)
        {
            read_line(trips_data, b_trip);
            add_to_list(mktime(&(b_trip->in_date)), b_trip->in_station);
            while(tail!=NULL && difftime( tail->arr_time, mktime(&(b_trip->out_date)) ) < 0 )
            {
                if( ( rel_vec[tail->id] >= 0 ) && difftime( tail->arr_time, mktime(&(current[rel_vec[tail->id]]->lastCheck)) ) > 0 )
                {
                    current[rel_vec[tail->id]]->freebk++;
                    current[rel_vec[tail->id]]->freebs--;
                }
                curr_t = tail->arr_time;
            
                if(tail->id==18013)
                {
                    fprintf(results, "%d	%d	%d	", tail->id, current[rel_vec[tail->id]]->freebk, current[rel_vec[tail->id]]->freebs);
                    print_date(localtime(&curr_t), results);
                    fprintf(results, "\n"); 
                }
                delete_end_list();
            }
            if( ( rel_vec[b_trip->out_station]>=0 ) && difftime(mktime(&(b_trip->out_date)), mktime(&(current[rel_vec[b_trip->out_station]]->lastCheck)) ) > 0   )
            {
                current[rel_vec[b_trip->out_station]]->freebk--;
                current[rel_vec[b_trip->out_station]]->freebs++;
            }
            curr_t = mktime(&(b_trip->out_date));
            if(b_trip->out_station==18013)
            {
                fprintf(results, "%d	%d	%d	", b_trip->out_station, current[rel_vec[b_trip->out_station]]->freebk, current[rel_vec[b_trip->out_station]]->freebs);
                print_date(localtime(&curr_t), results);
                fprintf(results, "\n");
            }
            while(!(check_greater(curr_t, aux_state)) && (read_line_states(state_data, aux_state)) )
                printf("x");
            
            for(j=0; aux_state[j]!=NULL;j++)
            {
                if(difftime(mktime(&(aux_state[j]->lastCheck)), curr_t) > 0 )
                {
                    if(j==819 && current[j]->freebk - aux_state[j]->freebk)
                        printf("%d ", current[j]->freebk - aux_state[j]->freebk);
                    current[j]->lastCheck = aux_state[j]->lastCheck;
                    current[j]->freebk = aux_state[j]->freebk;
                    current[j]->freebs = aux_state[j]->freebs;
                }
            }
        }
       
        fclose(trips_data);
        fclose(results);
        fclose(state_data);
        printf("\n* Completed *\n");
    }
    for(i=0;aux_state[i]!=NULL;i++)
    {
        free(aux_state[i]);
        aux_state[i]=NULL;
    }
    for(i=0;current[i]!=NULL;i++)
    {
        free(current[i]);
        current[i]=NULL;
    }
    free(aux_state);
    free(current);
    getchar();
    return 0;
}
