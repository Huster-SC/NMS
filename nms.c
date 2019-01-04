#include <stdio.h>
#include <stdlib.h>
typedef struct{
    float x, y, w, h;
} box;

typedef struct{
    int index;
    int class;
    float **probs;
} sortable_bbox;

float overlap(float x1, float w1, float x2, float w2);
float box_intersection(box a, box b)
{
    float w = overlap(a.x, a.w, b.x, b.w);
    float h = overlap(a.y, a.h, b.y, b.h);
    if(w < 0 || h < 0) return 0;
    float area = w*h;
    return area;
}

float box_union(box a, box b)
{
    float i = box_intersection(a, b);
    float u = a.w*a.h + b.w*b.h - i;
    return u;
}

float box_iou(box a, box b)
{
    return box_intersection(a, b)/box_union(a, b);
}

float overlap(float x1, float w1, float x2, float w2)
{
    float l1 = x1 - w1/2;
    float l2 = x2 - w2/2;
    float left = l1 > l2 ? l1 : l2;
    float r1 = x1 + w1/2;
    float r2 = x2 + w2/2;
    float right = r1 < r2 ? r1 : r2;
    return right - left;
}



typedef struct detection{
    box bbox;
    int classes;
    float *prob;
    float *mask;
    float objectness;
    int sort_class;
} detection;

detection *Det = (detection *)malloc(sizeof(detection)*1000);

int nms_comparator(const void *pa, const void *pb)
{
    detection a = *(detection *)pa;
    detection b = *(detection *)pb;
    float diff = 0;
    if(b.sort_class >= 0){
        diff = a.prob[b.sort_class] - b.prob[b.sort_class];
    } else {
        diff = a.objectness - b.objectness;
    }
    if(diff < 0) return 1;
    else if(diff > 0) return -1;
    return 0;
}

void do_nms_sort(detection *dets, int total, int classes, float thresh)
{
    int i, j, k;
    k = total-1;
    for(i = 0; i <= k; ++i){
        if(dets[i].objectness == 0){
            detection swap = dets[i];
            dets[i] = dets[k];
            dets[k] = swap;
            --k;
            --i;
        }
    }
    total = k+1;

    for(k = 0; k < classes; ++k){
        for(i = 0; i < total; ++i){
            dets[i].sort_class = k;
        }
        qsort(dets, total, sizeof(detection), nms_comparator);
        for(i = 0; i < total; ++i){
            if(dets[i].prob[k] == 0) continue;
            box a = dets[i].bbox;
            for(j = i+1; j < total; ++j){
                box b = dets[j].bbox;
                if (box_iou(a, b) > thresh){
                    dets[j].prob[k] = 0;
                }
            }
        }
    }
}

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
	
	do_nms_sort(Det, i, 20, 0.5);
	
	return 0;
} 
