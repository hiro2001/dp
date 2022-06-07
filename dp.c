#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

#define MEMORY 256
#define DIMENTION 15
#define NUMOFFILE 100

typedef struct {
    char name[20];
    char onso[50];
    int  flame;
    double mcepdata[MEMORY][DIMENTION];
} mcepdata_t;

void print(int i1, int i2, int m, int n, double d){
	FILE *fp;
	fp = fopen("log.log", "a+");
	if (fp == NULL) {
		printf("error\n");
		exit(EXIT_FAILURE);
	}

  fprintf(fp, "city_temp : city%3d/city%3d_%3d.txt\n", m, m, i1 + 1);
  fprintf(fp, "city_miti : city%3d/city%3d_%3d.txt\n", n, n, i2 + 1);
	printf("Between words : %f\n", d);
  fprintf(fp, "Between words : %f\n\n", d);
	printf("...Roading now...\n");
	fclose(fp);
}

void scan(int* i, int*m){
	printf("ファイル指定\n");
	printf("city0");
	scanf("%03d", i);
	printf("city0");
	scanf("%03d", m);
}

int main(void){
	int  h0, h, i, j, k;
	FILE *fp_temp, *fp_miti;
	mcepdata_t city_temp, city_miti;
	char ch0[200]; 
	double d[MEMORY][MEMORY];
	double g[MEMORY][MEMORY];
	double distance[NUMOFFILE];
	double distance_min, a, b, c; 
	int num_matchfname = 0;
	int count = 0;
	int TEMP_NUM, MITI_NUM;
	
	scan(&TEMP_NUM, &MITI_NUM);
	fp_temp = fopen("log.log", "w");
	if (fp_temp == NULL) {
		printf("error\n");
		exit(EXIT_FAILURE);
	}
	fclose(fp_temp);
	for (h0 = 0; h0 < NUMOFFILE; h0++) {
		sprintf(ch0, "./city%03d/city%03d_%03d.txt",TEMP_NUM, TEMP_NUM, h0 + 1);
		fp_temp = fopen(ch0, "r");
		if (fp_temp == NULL) {
			printf("error\n");
			exit(EXIT_FAILURE);
		}

		fgets(city_temp.name, sizeof(city_temp.name), fp_temp);
		fgets(city_temp.onso, sizeof(city_temp.onso), fp_temp);
		fgets(ch0,sizeof(ch0), fp_temp);
		city_temp.flame = atoi(ch0);
		
		for (i = 0; i < city_temp.flame; i++) {
    	for (j = 0; j < DIMENTION; j++) {
				fscanf(fp_temp, "%lf", &city_temp.mcepdata[i][j]);
			}
		}

		for (h = 0; h < NUMOFFILE; h++) {
			sprintf(ch0, "./city%03d/city%03d_%03d.txt",MITI_NUM, MITI_NUM, h + 1);

			if ((fp_miti = fopen(ch0, "r")) == NULL) {
				printf("miti file open error!!\n");
				exit(EXIT_FAILURE);
			}
			
			fgets(city_miti.name, sizeof(city_miti.name), fp_miti);
			fgets(city_miti.onso, sizeof(city_miti.onso), fp_miti);
			fgets(ch0,sizeof(ch0), fp_miti);
			city_miti.flame = atoi(ch0);
			
			for (i = 0; i < city_miti.flame; i++) {
				for (j = 0; j < DIMENTION; j++) {
					fscanf(fp_miti, "%lf", &city_miti.mcepdata[i][j]);
				}
			}

			for (i = 0; i < city_temp.flame; i++) {
				for (j = 0; j < city_miti.flame; j++) {
        	d[i][j] = 0;
					for (k = 0; k < DIMENTION; k++) {
						d[i][j] += (city_temp.mcepdata[i][k] - city_miti.mcepdata[j][k]) * (city_temp.mcepdata[i][k] - city_miti.mcepdata[j][k]); 
					}
          sqrtl(d[i][j]);
				}
			}
	
			g[0][0] = d[0][0];
			for (i = 1; i < city_temp.flame; i++) {
				g[i][0] = g[i - 1][0] + d[i][0];
			}
			for (j = 1; j < city_miti.flame; j++) {
				g[0][j] = g[0][j - 1] + d[0][j];
			}
			
			for (i = 1; i < city_temp.flame; i++) {
				for (j = 1; j < city_miti.flame; j++) {
					a = g[i][j - 1] + d[i][j];
					b = g[i - 1][j - 1] + 2 * d[i][j];
					c = g[i - 1][j] + d[i][j];
					g[i][j] = a;
					if (b < g[i][j]) {
						g[i][j] = b;
					}
					if (c < g[i][j]) {
						g[i][j] = c;
					}
				}
			}
			distance[h] = g[city_temp.flame - 1][city_miti.flame - 1] / (city_temp.flame + city_miti.flame);
			fclose(fp_miti);
		}
      distance_min = distance[0];
      num_matchfname = 0;
		for (h = 1; h < NUMOFFILE; h++) {
			if (distance_min > distance[h] ) {
				distance_min = distance[h];
				num_matchfname = h;
			}
		}
		fclose(fp_temp);
		if (num_matchfname == h0 ) {
			count++;
		}
		if (num_matchfname != h0 ) {
    	print(h0,num_matchfname, TEMP_NUM, MITI_NUM, distance_min);
		}
	}
	printf("正答率 %d%%\n", count); 
	return 0;
}
