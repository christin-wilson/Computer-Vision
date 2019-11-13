  /*
	** This program implements Optical character Recognition
	**.
	*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
int main()

{
FILE		*fpt1,*fpt2;
unsigned char	*image,*template,*norm;
int *MSF;
int mean;
int *temp,max=0,min=0;
int threshold=0;
unsigned char	*final;
char		header[320];
char    header2[320];
int		ROWS,COLS,BYTES;
int   ROWS2,COLS2,BYTES2;
int		r,c,r2,c2,sum;
int   r3,c3,r4,c4;
struct timespec	tp1,tp2;

	/* read image */
if ((fpt1=fopen("parenthood.ppm","rb")) == NULL)
  {
  printf("Unable to open parenthood.ppm for reading\n");
  exit(0);
  }
if ((fpt2=fopen("parenthood_e_template.ppm","rb")) == NULL)
  {
  printf("Unable to open parenthood_e_template.ppm for reading\n");
  exit(0);
  }  
fscanf(fpt1,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);
if (strcmp(header,"P5") != 0  ||  BYTES != 255)
  {
  printf("Not a greyscale 8-bit PPM image\n");
  exit(0);
  }
fscanf(fpt2,"%s %d %d %d",header2,&COLS2,&ROWS2,&BYTES2);
if (strcmp(header2,"P5") != 0  ||  BYTES2 != 255)
  {
  printf("Not a greyscale 8-bit PPM image\n");
  exit(0);
  }  
image=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
template=(unsigned char *)calloc(ROWS2*COLS2,sizeof(unsigned char));
header[0]=fgetc(fpt1);	/* read white-space character that separates header */
header[0]=fgetc(fpt2);
fread(image,1,COLS*ROWS,fpt1);
fclose(fpt1);
fread(template,1,COLS2*ROWS2,fpt2);
fclose(fpt2);
	/* allocate memory for final version of image */
MSF=(int *)calloc(ROWS2*COLS2,sizeof(int));
temp=(int *)calloc(ROWS*COLS,sizeof(int));
final=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
norm=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));

	/* query timer */
//clock_gettime(CLOCK_REALTIME,&tp1);
//printf("%ld %ld\n",(long int)tp1.tv_sec,tp1.tv_nsec);

	/* smooth image, skipping the border points */
sum=0;
for (r=0; r<ROWS2; r++)
  {
    for (c=0; c<COLS2; c++)
    {
    sum+=template[r*COLS2+c];
    printf("%d\t",template[r*COLS2+c]);
    }
  printf("\n"); 
  } 

    printf("\n\n\n");
mean=sum/(ROWS2*COLS2);
printf("%d\n",mean);
for (r=0; r<ROWS2; r++)
  {for (c=0; c<COLS2; c++)
    {
    MSF[r*COLS2+c]=template[r*COLS2+c]-mean;
    printf("%d\t",MSF[r*COLS2+c]);
    }
  printf("\n");
  }

r3=ROWS2/2;
c3=COLS2/2;
printf("%d %d\n", r3, c3 );
for (r=r3; r<ROWS-r3; r++)
  for (c=c3; c<COLS-c3; c++)
    {
    sum=0;
    for (r2=-r3,r4=0; r2<=r3; r2++,r4++)
      for (c2=-c3,c4=0; c2<=c3; c2++,c4++)
        sum+=image[(r+r2)*COLS+(c+c2)] * MSF[r4*COLS2+c4];
    temp[r*COLS+c]=sum;
    if(temp[r*COLS+c]>max)
    {
      max=temp[r*COLS+c];
    }
    else if(temp[r*COLS+c]<min)
    {
      min=temp[r*COLS+c];
    }
  }
  printf("\nmax:%d\tmin:%d",max,min);
int max2=0;
int min2=0;
fpt1=fopen("temp1.ppm","w");
fprintf(fpt1,"P5 %d %d 255\n",COLS,ROWS);
fwrite(temp,COLS*ROWS,1,fpt1);
fclose(fpt1);

for (r=0; r<ROWS; r++)
  {for (c=0; c<COLS; c++)
    { 
      norm[r*COLS+c]= (temp[r*COLS+c]-min)*255/(max-min);    
      if(norm[r*COLS+c]>max2)
      {
        max2=norm[r*COLS+c];
      }
      else if(norm[r*COLS+c]<min2)
      {
        min2=norm[r*COLS+c];
      }
    }  
  }
  printf("\nmax:%d\tmin:%d",max2,min2); 
fpt1=fopen("temp.ppm","w");
fprintf(fpt1,"P5 %d %d 255\n",COLS,ROWS);
fwrite(norm,COLS*ROWS,1,fpt1);
fclose(fpt1);  

char alphabet;
int row,col;


FILE *fpt3;

for(threshold=0;threshold<=213;threshold++)
  { 
    for (r=0; r<ROWS; r++)
  {for (c=0; c<COLS; c++)
    { if(norm[r*COLS+c]>threshold)
      final[r*COLS+c]= 255;
      else
      final[r*COLS+c]= 0;
    }
  } 
int gt=0,ob=0;
int tp=0,fp=0,fn=0,tn=0;
int m=0;
int k=0;
fpt3 = fopen("parenthood_gt.txt" , "r");
r=fscanf(fpt3,"%c %d %d\n",&alphabet,&col,&row);
while(m != EOF)
  {
    if(alphabet=='e'){
        gt=1;
    }
    else{
      gt=0;
    }
  ob=0;

  for (r=row-r3; r<=row+r3; r++)
  for (c=col-c3; c<=col+c3; c++)
    {
    if(final[r*COLS+c]==255)
      ob=1;
      print("found");
    }

    if(gt==1&&ob==1)
    {
      tp++;
    }
    else if(gt==0&&ob==1)
    {
      fp++;
    }
    else if(gt==0&&ob==0)
    {
      tn++;
    }
    else if(gt==1&&ob==0)
    {
      fn++;
    }
    m=fscanf(fpt3,"%c %d %d\n",&alphabet,&col,&row);  
}
fclose(fpt3);
float tpr,fpr;
//  printf("%f %f %f %f\n",tp,fp,fn,tn);
tpr=tp*1.0/((tp+fn)*1.0);
fpr=fp*1.0/(fp+tn)*1.0;
  //printf("%d\n",threshold );
  //printf("Threshold: %d tp: %d fp: %d tn: %d fn: %d tpr: %f fpr: %f\n",threshold,tp,fp,tn,fn,tpr,fpr);
  printf("%f,",fpr);

}  
//} */  

//clock_gettime(CLOCK_REALTIME,&tp2);
//printf("%ld %ld\n",(long int)tp2.tv_sec,tp2.tv_nsec);

	/* report how long it took to smooth */
//printf("%ld\n",tp2.tv_nsec-tp1.tv_nsec);

	/* write out final image to see result */
fpt1=fopen("final.ppm","w");
fprintf(fpt1,"P5 %d %d 255\n",COLS,ROWS);
fwrite(final,COLS*ROWS,1,fpt1);
fclose(fpt1);
printf("%d\n",ROWS2 );
}
