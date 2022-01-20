#include <stdio.h>
#include <time.h>
#include <locale.h>
 
int main(void)
{
    struct tm my_time = { .tm_year=112, // = year 2012
                          .tm_mon=9,    // = 10th month
                          .tm_mday=9,   // = 9th day
                          .tm_hour=8,   // = 8 hours
                          .tm_min=10,   // = 10 minutes
                          .tm_sec=20,   // = 20 secs
    };

    tzset(); // load timezone information (this can be called just once)
    time_t t = mktime(my_time) - timezone;

    if (daylight == 0 && my_time->tm_isdst != 0)
        t += 3600;
    else if (daylight != 0 && timeinfo->tm_isdst == 0)
        t -= 3600;
    return t;

//TODO find POSIX or C standard way to do covert tm to time_t without in UTC instead of local time
return mktime(&timeinfo);
}
