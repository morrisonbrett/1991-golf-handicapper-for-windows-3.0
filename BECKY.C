#include <stdio.h>

#define TRUE  1
#define FALSE 0
#define MAXI  500
#define HIGH  1400

void main(void)
{
  char array[HIGH];
  int d, x, y, k;
  int found;

  x = 1;

  for (d=0; d<HIGH; d++)
    array[d] = FALSE;

  for (d=1; d<=MAXI; d++)
  {
    found = FALSE;
    k=1;
    while (k<HIGH && !found)
      if (!array[k])
      {
	x = k;
	found = TRUE;
      }
      else
	k++;
    y = x + d;
    array[x] = TRUE;
    array[y] = TRUE;
    fprintf(stdout, "(%d, %d) %d ", x, y, d);
    if (d % 4 == 0)
      fprintf(stdout, "\n");
  }
}