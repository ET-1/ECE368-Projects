#include <stdio.h>
#include <stdlib.h>

void Save_Seq1(char *Filename, int N)
{
  FILE * fptr = fopen(Filename,"wb");
  int p2 = 1;
  int gap = 1;

  int i = 0;
  int * arr;
  arr = malloc(1*sizeof(int));

  arr[0] = 1;

  while(p2 < N)
    {
      gap = p2;
      while(gap < N)
	{
	  i++;
	  arr = realloc(arr,(i*sizeof(int))+1);
	  arr[i-1] = gap;
	  gap *= 3;
	}
      p2 *= 2;
    }

  int high = 0;
  int count = 0;
  int temp;
  for(high = i-1; high > 0; high--)
    {
      for(count = 0; count < high; count++)
	{
	  if(arr[count] < arr[count+1])
	    {
	      temp = arr[count];
	      arr[count] = arr[count+1];
	      arr[count+1] = temp;
	    }
	}
    }

  int j;
  for(j=0; j<i; j++)
    {
      fprintf(fptr, "%d\n", arr[j]);
    }

  fclose(fptr);
  free(arr);
}

int main(int argc, char ** argv)
{
  Save_Seq1("a.txt", 100);
  return 0;
} 
