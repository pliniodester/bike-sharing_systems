
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define N 1500

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
    if( (c >= '0') && (c < '10'))
        return (int)(c-'0');
    printf("ERROR_04: Treated '%c' as a number.\n", c);
    return NAN;
}

int read_line_states(FILE *file, state **stations)
{
    int i=0, c;
    while((c=getcc(file))!='\n' && c!=EOF)
    {
        if(c=='{')
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

int main(void)
{
    FILE *trips_data;
    FILE *state_data;
    FILE *results;

    state **station = malloc(N * sizeof(state*));
    trip *b_trip = (trip*)malloc(sizeof(trip));

    results = fopen("data_Velib.txt", "w");
    state_data = fopen("data-2015-3-1.txt", "r");
    trips_data = fopen("paris-201503.csv", "r");
    if (trips_data && state_data && results) {


        while(getc(trips_data) != '\n');
        while(read_line(trips_data, b_trip))
        {
            print_station(901, b_trip, results);
        }
/*        while(read_line_states(state_data, station))
        {
            print_state(0, station, s_results);
        } */
        
        fclose(trips_data);
        fclose(results);
        fclose(state_data);
        printf("\n* Completed *\n");
    }

    getchar();
    return 0;
}
