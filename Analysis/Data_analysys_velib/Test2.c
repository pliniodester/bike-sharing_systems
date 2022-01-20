#include <stdio.h>
#include <time.h>
#include <locale.h>
 
int main(void)
{
    time_t now;
    time(&now);

    struct tm my_time = { .tm_year=2015 - 1900, // = year 2012
                          .tm_mon=6 - 1,    // = 10th month
                          .tm_mday=24,   // = 9th day
                          .tm_hour=11,   // = 8 hours
                          .tm_min=46,   // = 10 minutes
                          .tm_sec=24,    // = 20 secs
                          .tm_isdst=-1
    };
    
    mktime(&my_time);
    printf("%s", asctime(&my_time));
    
    double seconds = difftime(now, mktime(&my_time));
    printf("%.f seconds have passed since the beginning of the month.\n", seconds);
    
    getchar();
    return 0;
}
