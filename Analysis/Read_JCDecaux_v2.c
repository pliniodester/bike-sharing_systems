
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define N 44103

/*
switch ( a ) {
case b:
  // Code
  break;
case c:
  // Code
  break;
default:
  // Code
  break;
} */

void set_int_vec(int vec[], int n)
{
    int i;
    for(i=0;i<n;i++)
        vec[i]=0;
}

int calctime(int n, int d)
{
    switch(d)
    {
        case 6:
             return 10*n*86400;
             break;
        case 7:
             return n*86400;
             break;
        case 8:
             return 10*n*3600;
             break;
        case 9:
             return n*3600;
             break;
        case 10:
             return 10*n*60;
             break;
        case 11:
             return n*60;
             break;
        case 12:
             return 10*n;
             break;
        case 13:
             return n;
             break;
        default:
             printf("\nError in calctime\n");
             return NAN;
             break;
    }
}

void count(int c, int counter[], int line_inf[])
{
    switch(counter[0])
    {
        case 0:
            line_inf[0]=(int)(c-'0');
            break;
        case 1:
            if(counter[1]>5)
                line_inf[1]+=calctime((int)(c-'0'),counter[1]);
            break;
        case 4:
            if(counter[1]>5)
                line_inf[4]+=calctime((int)(c-'0'),counter[1]);
            break;
        default:
            line_inf[counter[0]] = line_inf[counter[0]]*10+(int)(c-'0');
            break;
    }
}

int read_line(FILE *file, int counter[], int line_inf[])
{
     int i, c;
     for(i=0; i<7; i++)
         line_inf[i] = 0;
     
     while((c = getc(file)) != '\n' && c != EOF)
     {
            switch(c)
            {
                case ';':
                    counter[0]++;
                    counter[1]=0;
                    break;
                default:
                    count(c, counter, line_inf);
                    counter[1]++;
                    break;
            }
     }
     counter[0]=0;
     counter[1]=0;
     if(c == '\n')
         return 1;
     return 0;
}

void print_line(int line_inf[])
{
    int i;
    for(i=0 ;i<7 ;i++)
        printf("%d; ",line_inf[i]);
    printf("\n");
}

void print_dline(int line[])
{
    printf("%d - (%d,%d), (%d,%d),", line[0], line[2], line[5], line[3], line[6]);
    printf("%dd,%dh%02d-%dd,%dh%02d, %.2f min\n", line[1]/86400, line[1]%86400/3600,line[1]%3600/60 , line[4]/86400, line[4]%86400/3600,line[4]%3600/60 ,(float)((line[4]+86400-line[1]%86400)%86400)/60);
}

int main(void)
{
    int counter[2]={0,0}, c, line_inf[7], stations[N];
    int i, j, aux=0;
    int max=0;
    float totaltrip=0, avg=0;
    FILE *file;
    FILE *results;
    set_int_vec(stations, N);
    
    results = fopen("data_Velib.txt", "w");
    file = fopen("paris-201503.csv", "r");
    if (file) {
        while ((c = getc(file)) != '\n');
        while(read_line(file, counter, line_inf))
        {
            if(line_inf[0]==1 && line_inf[2]==901)
            {
                aux++;
                fprintf(results, "%d\n", aux);
            }
        if(line_inf[0]==1 && line_inf[5]==901)
            {
                aux--;
                fprintf(results, "%d\n", aux);
            }
        }
        fclose(file);
        fclose(results);
        printf("\n* Completed *\n");
    }
    getchar();
    return 0;
}
