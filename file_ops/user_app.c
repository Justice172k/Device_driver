#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define DEVICE_NODE "/dev/new_ops_device_file"


int main()
{
       int num; 
       FILE* ptr;
       char name[100];
       ptr = fopen(DEVICE_NODE, "w");
       if(ptr == NULL)
       {
       	printf("error\n");
       	exit(1);
       }
       else
       	printf("succeed\n");
       printf("Enter num: ");
       scanf("%d",&num);
       fprintf(ptr,"%d",num);
       fclose(ptr);
       return 0;
}
