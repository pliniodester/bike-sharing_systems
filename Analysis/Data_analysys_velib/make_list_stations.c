
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define pi 3.14159265358979323846
#define N 1500
#define R 0.5

typedef struct list_close_stations {
    int id;
    lcs *next;
}lcs;

typedef struct info_station {
    int id;
    double lat;
    double lon;
    int cap;
    lcs *cs;
}info;

double deg2rad(double deg) {
  return (deg * pi / 180);
}

double rad2deg(double rad) {
  return (rad * 180 / pi);
}

double distance(double lat1, double lon1, double lat2, double lon2) {
  double theta, dist;
  theta = lon1 - lon2;
  dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
  dist = acos(dist);
  dist = rad2deg(dist);
  dist = dist * 60 * 1.1515;
  return (dist * 1.609344);
}

int main(void)
{
    int i, j;
    FILE *file;
    file = fopen("stationAddresses_v2.txt","r");
    
    info **inf_s = malloc(N * sizeof(info*));
    for(i=0;i<N;i++)
        inf_s[i]=NULL;
    
    while(getc(file)!='\n');
    for(i=0;i<N;i++)
    {
        inf_s[i] = (info*)malloc(sizeof(info));
        if(fscanf(file,"%*d,%d,%Lf,%Lf,%d", &(inf_s[i]->id), &(inf_s[i]->lat), &(inf_s[i]->lon), &(inf_s[i]->cap))==EOF)
            break;
    }
    
    for(i=0; inf_s[i]!=NULL; i++)
    {
        for(j=i+1; inf_s[j]!=NULL; j++)
        {
            if(distance(inf_s[i]->lat, inf_s[i]->lon, inf_s[j]->lat, inf_s[j]->lon) < R)
            {
                
            }
        }
    }
    
    for(i=0;inf_s[i]!=NULL;i++)
        free(inf_s[i];
    free(inf_s);
    fclose(file);
    getchar();
    return 0;
}
