
	/*
	** This program reads bridge.ppm, a 512 x 512 PPM image.
	** It smooths it using a standard 3x3 mean filter.
	** The program also demonstrates how to time a piece of code.
	**
	** To compile, must link using -lrt  (man clock_gettime() function).
	*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
void main()

{
FILE		*fpt;
unsigned char	*image;
unsigned char	*smoothed;
int	*TEMP;
unsigned char *compare;
char		header[320];
int		ROWS,COLS,BYTES;
int		r,c,r2,c2,sum;
int count=0;
struct timespec	tp1,tp2;

	/* read image */
if ((fpt=fopen("bridge.ppm","rb")) == NULL)
  {
  printf("Unable to open bridge.ppm for reading\n");
  exit(0);
  }
fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);
if (strcmp(header,"P5") != 0  ||  BYTES != 255)
  {
  printf("Not a greyscale 8-bit PPM image\n");
  exit(0);
  }
image=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
header[0]=fgetc(fpt);	/* read white-space character that separates header */
fread(image,1,COLS*ROWS,fpt);
fclose(fpt);

	/* allocate memory for smoothed version of image */
TEMP=(int *)calloc(ROWS*COLS,sizeof(int));
smoothed=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));

	/* query timer */
clock_gettime(CLOCK_REALTIME,&tp1);
printf("%ld %ld\n",(long int)tp1.tv_sec,tp1.tv_nsec);

	/* smooth image, skipping the border points */
for (c=0; c<COLS; c++)
  {sum=0;
  for (r=3; r<ROWS-3; r++)
    {if(r==3)
      {
        for (r2=-3; r2<=3; r2++)
        sum+=image[(r+r2)*COLS+(c)];
      }
    else{
        sum+=image[(r+3)*COLS+(c)];
        sum-=image[(r-4)*COLS+(c)];

    }
    TEMP[r*COLS+c]=sum;
    }}
   
for (r=3; r<ROWS-3; r++)
  {
  sum=0;
  for (c=3; c<COLS-3; c++)
    {    
    if(c==3)
      {
        for (c2=-3; c2<=3; c2++)
        sum+=TEMP[(r)*COLS+(c+c2)];
      }
    else{
        sum+=TEMP[(r)*COLS+(c+3)];
        sum-=TEMP[(r)*COLS+(c-4)];

    }
      smoothed[r*COLS+c]=sum/49;
	}}
 
	/* query timer */

clock_gettime(CLOCK_REALTIME,&tp2);
printf("%ld %ld\n",(long int)tp2.tv_sec,tp2.tv_nsec);

	/* report how long it took to smooth */
printf("%ld\n",tp2.tv_nsec-tp1.tv_nsec);
if ((fpt=fopen("smoothed77.ppm","rb")) == NULL)
  {
  printf("Unable to open smoothed77.ppm for reading\n");
  exit(0);
  } 
fscanf(fpt,"%s %d %d %d",header,&COLS,&ROWS,&BYTES);
if (strcmp(header,"P5") != 0  ||  BYTES != 255)
  {
  printf("Not a greyscale 8-bit PPM image\n");
  exit(0);
  }
compare=(unsigned char *)calloc(ROWS*COLS,sizeof(unsigned char));
header[0]=fgetc(fpt); /* read white-space character that separates header */
fread(compare,1,COLS*ROWS,fpt);
fclose(fpt);

for (r=0; r<ROWS; r++)
  for (c=0; c<COLS; c++)
    {
    if(smoothed[(r)*COLS+(c)]!=compare[(r)*COLS+(c)])
    {
      count++;
    }
    }
printf("No of pixels that are different: %d",count);
	/* write out smoothed image to see result */
fpt=fopen("smoothedslide.ppm","w");
fprintf(fpt,"P5 %d %d 255\n",COLS,ROWS);
fwrite(smoothed,COLS*ROWS,1,fpt);
fclose(fpt);

}
