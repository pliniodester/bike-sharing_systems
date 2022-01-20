
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

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
            break;
    }
}

int read_line(FILE *file, int counter[], int line_inf[])
{
     int i, c;
     for(i=0; i<7; i++)
         line_inf[i] = 0;
     
     while( ((c = getc(file)) != '\n') && ((c = getc(file)) != EOF) )
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

int main(void)
{
    int counter[2]={0,0}, n_lines=0, c, line_inf[7];
    float totaltrip=0;
    FILE *file;
    file = fopen("paris-201503.csv", "r");
    if (file) {
        while ((c = getc(file)) != '\n') putchar(c);
        printf("\n");
        read_line(file, counter, line_inf);
        printf("%d %d %d %d %d %d %d\n",line_inf[0],line_inf[1],line_inf[2],line_inf[3],line_inf[4],line_inf[5],line_inf[6]);
/*
        while ((c = getc(file)) != EOF)
        {
            switch(c)
            {
                case ';':
                    counter[0]++;
                    counter[1]=0;
                    break;
                case '\n':
                    counter[0]=0;
                    n_lines++;
                    totaltrip += (trip[1]-trip[0]+86400)%86400;
//                    printf("\nTime={%f}\n", (float)(trip[1]-trip[0])/60);
                    trip[0]=0; trip[1]=0;
                    break;
                default:
                    count(c, counter, reason, trip);
                    counter[1]++;
                    break;
            }
//            putchar(c);
        }
*/
        fclose(file);
    }
    getchar();
    return 0;
}
