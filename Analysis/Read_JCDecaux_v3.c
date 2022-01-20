
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define N 44103

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

void set_int_vec(int vec[], int n)
{
    int i;
    for(i=0;i<n;i++)
        vec[i]=0;
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

int main(void)
{
    double seconds;
    time_t aux_time;
    int c;
    trip *b_trip;
    FILE *file;
    FILE *results;

    results = fopen("data_Velib.txt", "w");
    file = fopen("paris-201503.csv", "r");
    if (file) {
        while ((c = getc(file)) != '\n');
        while(read_line(file, b_trip))
        {
            print_trip(b_trip);
            getchar();
        }
        fclose(file);
        fclose(results);
        printf("\n* Completed *\n");
    }
    getchar();
    return 0;
}
