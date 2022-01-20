
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

char getcc(FILE *file)
{
    char c;
    c=getc(file);
    while(c==0)
        c=getc(file);
    return c;
}

int print_inf(FILE *file)
{
     int i, c;
     c = getc(file);
     putchar(c);
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

int main(void)
{
    int i;
    FILE *file;
    FILE *s_results;
    state **station = malloc(N * sizeof(state*));
    for(i=0;i<N;i++)
        station[i]=NULL;

    file = fopen("data-2015-3-1.txt", "r");
    s_results = fopen("state_results.txt", "w");
    if (file && s_results);
    {
        while(read_line_states(file, station))
        {
            print_state(0, station, s_results);
        }
        fclose(file);
        fclose(s_results);
    }
    for(i=0; i<N; i++)
        free(station[i]);
    free(station);
    printf("* Completed *");
    getchar();
    return 0;
}
