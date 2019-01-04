#include <stdio.h>
#include <stdlib.h>
typedef struct{
    float x, y, w, h;
} box;

typedef struct detection{
    box bbox;
    int classes;
    float *prob;
    float *mask;
    float objectness;
    int sort_class;
} detection;

detection *Det = (detection *)malloc(sizeof(detection)*1000);

int main(int argc, char *argv[]) 
{
	FILE  *fp;  
	char filename[100]="F:/大四/中科物栖/资料/测试数据/no_nms1.txt";
	printf("%s\n",filename);  
	int i = 0,j = 0; 
	//读文件数据 
	
	if ((fp=fopen(filename,"r"))==NULL)
	{
	 	printf("File open erroe\n ");
	 	return 1;
	}
	
	while(fscanf(fp,"%f",&Det[i].bbox.x) > 0){
		fscanf(fp,"%f",&Det[i].bbox.y);
		fscanf(fp,"%f",&Det[i].bbox.w);
		fscanf(fp,"%f",&Det[i].bbox.h);
		Det[i].prob = (float *)malloc(sizeof(float)*20);
		for(j = 0; j < 20;j ++)
		{
			fscanf(fp,"%f",&Det[i].prob[j]);
		}
		printf("%f %f %f %f ",Det[i].bbox.x,Det[i].bbox.y,Det[i].bbox.w,Det[i].bbox.h);
		for(j = 0;j < 20;j ++)
			printf("%f ",Det[i].prob[j]);
		printf("\n");
		i ++;
	}
	fclose(fp);
	return 0;
} 
