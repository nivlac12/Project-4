#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 1000000
#define MAX_LINE_LENGTH 2020
#define MAX_TEST 100

char Everything[MAX_LINES*MAX_LINE_LENGTH];
char **Substrings; // [MAX_LINES];
char **sortedSubstring[MAX_TEST];

void init_arrays();
void get_substrings(int i);
void print_results();
void merge(int,int,int);
void sort(int,int);

int main()
{
  init_arrays();
  printf("Hello World\n");
  FILE *fp;
  int e,i,j, last_length = 0;
  char c;
  fp = fopen("/homes/dan/625/wiki_dump.txt","r");
  int count = 1;
  char line[MAX_LINE_LENGTH];

  for(j = 0; e!=EOF && j<=MAX_TEST; j++)
    {
      for(i=0;i < last_length; i++)
      {
	line[i]='\0';
      }
      last_length=1;
      e=getc(fp);
      for(i=0; e != '\n' && e!= EOF; i++)
	{
	  Everything[j*MAX_LINE_LENGTH+i] = e;
	  e=getc(fp);
	  last_length++;
	}
      //strcpy(&Everything[j*MAX_LINE_LENGTH], line);
      //printf("%d\n\n", strlen(&Everything[j*MAX_LINE_LENGTH]));
      count++;
    
     }
  //printf("\n\nCount=%d\n\n",count);
  //This should be all we need to parallelize the first go  
  int z;
  //#pragma omp parallel for  
  for(z = 0; z < MAX_TEST-2; z++){
    get_substrings(z);
  }
  printf("substrings ran this far%d\n\n", z);
  z = 0;
  //fills the array sortedSubstring from Substrings
  sort(z,MAX_TEST);
  printf("Longest substring: %s\n",sortedSubstring[MAX_TEST-2]);
  printf("Checking for where longest substring is\n\n");
 //print_results();
  printf("Shortest substring: %s\n",sortedSubstring[0]);
  return 0;
}

void init_arrays()
{
  sortedSubstrings = (char **) malloc (MAX_LINES * sizeof(char *));
  Substrings = (char **) malloc (MAX_LINES * sizeof(char *));
  int i;
  for(i = 0; i<100;i++)
    {
      Everything[i*MAX_LINE_LENGTH]=0;
      Substrings[i] = malloc(MAX_LINE_LENGTH);
      sortedSubstrings[i] = malloc(MAX_LINE_LENGTH);
    }
}

void print_results()
{
  //printf("%s\n", &Everything[0]);
  //printf("\n\n\n%s\n", &Everything[MAX_LINE_LENGTH*1]);
  //printf("%s\n", Substrings[0]);
}

void get_substrings(int i)
{
  // printf("In get_subs\n");
  int first_length, second_length, j, k, l,m;
  char first_line[MAX_LINE_LENGTH], second_line[MAX_LINE_LENGTH];
  char *longest;
  
  //for(i = 0; i<2; i++)
  //{
      //printf("%s\n: ", Everything[i*MAX_LINE_LENGTH]);
      strcpy(first_line, &Everything[i*MAX_LINE_LENGTH]);
      strcpy(second_line, &Everything[(i+1)*MAX_LINE_LENGTH]);
      //printf("%s\n\n\n%s\n\n\n", first_line, second_line);
      first_length = strlen(first_line);
      second_length = strlen(second_line);
      //malloc is being dumb, we think it is because we are making it ahead of time
      longest = (char *) malloc(MAX_LINE_LENGTH * sizeof(char));
      // printf("lenfirs: %d\nlensec:%d\n",first_length, second_length);
      short int table[first_length][second_length];
      //printf("table initialized\n");
      //populate and initialize the matrix
      for(j=0; j<second_length;j++)
	{
	  table[0][j]=0;
	  //longest[j]=0;
	}
      for(k=0;k<first_length;k++)
	{
	  table[k][0]=0;
	}
      //Check to see through the two different lines to check for 
      //if they are equal then we need to add 1 to the slot 
      //to keep track of how long the common substring is
      for(j=1;j<first_length;j++)
	{
	  for(k=1;k<second_length;k++)
	    {
	      if(first_line[j-1] == second_line[k-1])
		{
		  table[j][k]=table[j-1][k-1]+1;
		}
		else
		  {
		    table[j][k]=0;
		  }
	    }
	}
      // printf("Halfway through\n\n");
      //now going through to retreive the longest substring
      //iterate through the matrix to find the largest substring
      //and move it to longest to set into the substrings array
	  for(j=1;j<first_length;j++)
	    {
	      for(k=1;k<second_length;k++)
		{
		  if(table[j][k] > strlen(longest))
		    {
		      strncpy(longest, &first_line[j-table[j][k]], table[j][k]);
		    }
		}
	   }
	  // printf("%s\n\n\n",longest);
	  Substrings[i]=longest;
	  // free(longest);
	  //}
}

void merging(int low, int mid, int high) {
  int l1, l2, i;
  //iterate through the low to mid and mid to high arrays to find the 
  //comparisons of the two arrays and place the smaller inside of the
  //array sortedSubstrings to be printed later
  for(l1 = low, l2 = mid + 1, i = low; l1 <= mid && l2 <= high; i++) {
    if(Substrings[l1] <= Substrings[l2])
      sortedSubstring[i] = Substrings[l1++];
    else
      sortedSubstring[i] = Substrings[l2++];
  }

  while(l1 <= mid) 
    sortedSubstring[i++] = Substrings[l1++];

  while(l2 <= high) 
    sortedSubstring[i++] = Substrings[l2++];

  for(i = low; i <= high; i++)
    sortedSubstring[i] = Substrings[i];
}

void sort(int low, int high) {
  int mid;
  //recursively call sort to get down to two of the values then call merge
  if(low < high) {
    mid = (low + high) / 2;
    sort(low, mid);
    sort(mid+1, high);
    merging(low, mid, high);
  } else { 
    return;
  } 
}
