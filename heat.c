#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


/*Initialise the temperature array with the initial conditions*/
void initialisation(int intervals,float domain, float sqsize, double xy[intervals][intervals] ){
    int sqIntevalsize = (int) ((intervals * sqsize) / domain); /*size of the square in intervals*/
    int start = intervals/2 - sqIntevalsize/2; /*beginning of square interval*/

    int end = intervals/2 + sqIntevalsize/2; /*end of square interval*/

      for(int i = 0; i<intervals; i++){
          for(int j= 0; j<intervals; j++){
              if( (i>=start && i<=end ) && (j>=start && j<=end)){
                  xy[i][j] = 50;
              } else {
                  xy[i][j] = 20;
              }
          }
      }
}

/* Calculate the rate of change of temperature at each point in the domain */
void calc_rate(float spacing, float alpha,  double temp[500][500], double ra[500][500]){
    double nexti;
    double previ;
    double nextj;
    double prevj;
    double current;
    int i;
    int j;

    for(i = 0; i<500;i++){

        for(j=0; j<500;j++){

            if(i==0 || j==0 ){
                previ = 20.0;
                prevj = 20.0;
                nexti = temp[i + 1][j];
                nextj = temp[i][j + 1];
                current = 2 * temp[i][j];

            }
            else if ( i==499 || j==499){
                nexti = 20.0;
                nextj = 20.0;
                previ = temp[i - 1][j];
                prevj = temp[i][j - 1];
                current = 2 * temp[i][j];
            }
            else {
                nexti = temp[i + 1][j];
                previ = temp[i - 1][j];
                nextj = temp[i][j + 1];
                prevj = temp[i][j - 1];
                current = 2 * temp[i][j];
            }


            ra[i][j] = alpha * ((nexti - current + previ) / pow(spacing, 2) + (nextj - current + prevj) / pow(spacing, 2));


            }

        }
    }

/* Calculate temperature at each point in the domain */
void update(float t, double temp[500][500], double ra[500][500] ) {
    int i;
    int j;

    for (i = 0; i < 500; i++) {

        for (j = 0; j < 500; j++) {


            temp[i][j] = temp[i][j] + t * ra[i][j];

        }
    }
}



int main() {


    double temperatures[500][500];
    double rate_of_change[500][500];
    float timestep = 0.9 * pow(10, -6);
    initialisation(500, 0.1, 0.04, temperatures);

        for (int i = 0; i < 10000; i++) {


            calc_rate(0.0002, 0.01, temperatures, rate_of_change);

            update(timestep, temperatures, rate_of_change);

        }
        FILE *fp;
        fp = fopen("heat.txt", "w+");

        for (int i = 0; i < 500; i++) {

            for (int j = 0; j < 500; j++) {
                fprintf(fp, "%lf ", temperatures[i][j]);

            }
            printf("\n");


        }


    return 0;
}