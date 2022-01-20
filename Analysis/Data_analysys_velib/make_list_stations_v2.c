
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define pi 3.14159265358979323846
#define N 1500
#define R 0.30

typedef struct list_close_stations {
    int id;
    struct list_close_stations *next;
}lcs;

typedef struct info_station {
    int id;
    double lat;
    double lon;
    int cap;
    lcs *cs;
}info;

void add_to_list(int id, info *inf_s)
{
    lcs *aux = inf_s->cs;
    if(inf_s->cs == NULL)
    {
        inf_s->cs = (lcs*)malloc(sizeof(lcs));
        inf_s->cs->id = id;
        inf_s->cs->next = NULL;
    }
    else
    {
        while(aux->next!=NULL)
            aux = aux->next;
        aux->next = (lcs*)malloc(sizeof(lcs));
        aux = aux->next;
        aux->id = id;
        aux->next = NULL;
    }
}

int delete_end_list(info *inf_s)
{
    lcs *aux1=inf_s->cs;
    lcs *aux2=inf_s->cs;
    if(aux1==NULL)
        return 0;
    if(aux1->next == NULL)
    {
        free(aux1);
        inf_s->cs = NULL;
        return 1;
    }
    else
    {
        while(aux1->next!=NULL)
        {
            aux2 = aux1;
            aux1 = aux1->next;
        }
        free(aux1);
        aux2->next=NULL;
        return 1;      
    }
}

void free_all(info **inf_s)
{
    int i;
    lcs *aux;
    for(i=0;inf_s[i]!=NULL;i++)
    {
        while(delete_end_list(inf_s[i]));
        free(inf_s[i]);
    }
    free(inf_s);
}

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

int print_list(lcs *cs, FILE* list_cs)
{
    lcs *aux = cs;
    if(aux == NULL)
    {
        fprintf(list_cs, "0\n");
        return 0;
    }
    while(aux->next!=NULL)
    {
        fprintf(list_cs, "%d	", aux->id);
        aux = aux->next;
    }
    fprintf(list_cs, "%d	0\n", aux->id);
    return 1;
}

int main(void)
{
    int i, j;
    FILE *file;
    FILE *list_cs;
    char filename[64];
    
    file = fopen("stationAddresses_v2.txt","r");

    sprintf (filename, "list_cs_R_%.2f.txt", R);
    list_cs = fopen(filename,"w");
    
    info **inf_s = malloc(N * sizeof(info*));
    for(i=0;i<N;i++)
        inf_s[i]=NULL;
    
    while(getc(file)!='\n');
    for(i=0;i<N;i++)
    {
        inf_s[i] = (info*)malloc(sizeof(info));
        inf_s[i]->cs = NULL;
        if(fscanf(file,"%*d,%d,%Lf,%Lf,%d", &(inf_s[i]->id), &(inf_s[i]->lat), &(inf_s[i]->lon), &(inf_s[i]->cap))==EOF)
            break;
    }
    for(i=0; inf_s[i]!=NULL; i++)
    {
        for(j=i+1; inf_s[j]!=NULL; j++)
        {
            if(distance(inf_s[i]->lat, inf_s[i]->lon, inf_s[j]->lat, inf_s[j]->lon) < R)
            {
                add_to_list(inf_s[j]->id, inf_s[i]);
                add_to_list(inf_s[i]->id, inf_s[j]);
            }
        }
    }
    
    for(i=0;inf_s[i]!=NULL;i++)
    {
        fprintf(list_cs, "%d	", inf_s[i]->id);
        print_list(inf_s[i]->cs, list_cs);
    }
    
    free_all(inf_s);
    fclose(file);
    fclose(list_cs);
    printf("\n* Completed *\n");
    getchar();
    return 0;
}
