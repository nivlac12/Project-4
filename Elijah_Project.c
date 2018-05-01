#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_LINES 160
#define MAX_LINE_LENGTH 2020
#define MAX_TEST 1000000
#define NUM_THREADS 1

char Everything[MAX_LINES*MAX_LINE_LENGTH];
char **Substrings; // [MAX_LINES];
char **sortedSubstring; //[MAX_TEST];
short int table[MAX_LINE_LENGTH][MAX_LINE_LENGTH];

int count = 0;

void init_arrays();
void get_substrings(void *);
void print_results();
void merge(int,int,int);
void sort(int,int);

int main()
{
  init_arrays();
  FILE *fp;
  int e,i,j, last_length = 0;
  char c;
  fp = fopen("/homes/dan/625/wiki_dump.txt","r");
  int count = 1;
  char line[MAX_LINE_LENGTH];

  for(j = 0; e!=EOF && j<MAX_LINES; j++)
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
      count++;
    
  }

  int rc;
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  void *status;


  /* Initialize and set thread detached attribute */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (i = 0; i < NUM_THREADS; i++ ) {
      rc = pthread_create(&threads[i], &attr, get_substrings, (void *)i);
        if (rc) {
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
        }
  }


  pthread_attr_destroy(&attr);
  for(i=0; i<NUM_THREADS; i++) {
       rc = pthread_join(threads[i], &status);
       if (rc) {
       printf("ERROR; return code from pthread_join() is %d\n", rc);
       exit(-1);
       }
  }

  printf("%s\n", Substrings[0]);

  pthread_exit(NULL);

  int z;
  //#pragma omp parallel for  
  /*for(z = 0; z < 10000; z++){
    get_substrings(z);
  }
  
  printf("substrings ran this far: %d\n\n", z);
  z = 0;
  //fills the array sortedSubstring from Substrings
  sort(z,MAX_TEST-1);
  printf("Longest substring: %s %d\n",Substrings[MAX_TEST-2],MAX_TEST-2);
  printf("Checking for where longest substring is\n\n");
 //print_results();
  printf("Shortest substring: %s\n",Substrings[0]);*/
  return 0;
}

void init_arrays()
{
  sortedSubstring = (char **) malloc (MAX_TEST * sizeof(char *));
  Substrings =  (char **) malloc (MAX_LINES * sizeof(char *));
  int i;
  for(i = 0; i<MAX_LINES;i++)
    {
      Everything[i*MAX_LINE_LENGTH]=0;
      Substrings[i] = malloc(MAX_LINE_LENGTH*sizeof(char));
      sortedSubstring[i] = malloc(MAX_LINE_LENGTH*sizeof(char));
    }
}

void print_results()
{
  //printf("%s\n", &Everything[0]);
  //printf("\n\n\n%s\n", &Everything[MAX_LINE_LENGTH*1]);
  //printf("%s\n", Substrings[0]);
}

void get_substrings(void *myID)
{
  int i, j, k, l, m, first_length, second_length;
  int startPos = ((int) myID) * (MAX_LINES / NUM_THREADS);
  int endPos = startPos + (MAX_LINES / NUM_THREADS);
  for(i = startPos; i<endPos; i++)
  {
    printf("Here\n");
    // printf("In get_subs\n");
    char first_line[MAX_LINE_LENGTH], second_line[MAX_LINE_LENGTH];
    char *longest;
    
    strcpy(first_line, &Everything[i*MAX_LINE_LENGTH]);
    strcpy(second_line, &Everything[(i+1)*MAX_LINE_LENGTH]);
    first_length = strlen(first_line);
    second_length = strlen(second_line);
    //malloc is being dumb, we think it is because we are making it ahead of time
    longest = (char *) malloc(MAX_LINE_LENGTH * sizeof(char));
    // printf("lenfirs: %d\nlensec:%d\n",first_length, second_length);
    table[first_length][second_length];

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
  	  sprintf(Substrings[i], "%d-%d: %s", i, (i+1), longest);
  	  // free(longest);
  	}
    pthread_exit(NULL);
}

void merging(int low, int mid, int high) {
  int l1, l2, i;
  //iterate through the low to mid and mid to high arrays to find the 
  //comparisons of the two arrays and place the smaller inside of the
  //array sortedSubstrings to be printed later
  for(l1 = low, l2 = mid + 1, i = low; l1 <= mid && l2 <= high; i++) {
    //printf("length of Substring[l1]: %d\n\nLine at location: %s\n\n",strlen(Substrings[l1]),Substrings[l1]);
    //printf("length of Substring[l2]: %d\n\nLine at location: %s\n\n",strlen(Substrings[l2]),Substrings[l2]);
    if(strlen(Substrings[l1]) <= strlen(Substrings[l2]))
      sortedSubstring[i] = Substrings[l1++];
    else
      sortedSubstring[i] = Substrings[l2++];
  }
 
  while(l1 <= mid) 
    sortedSubstring[i++] = Substrings[l1++];

  while(l2 <= high) 
    sortedSubstring[i++] = Substrings[l2++];
  
  for(i = low; i <= high; i++)
    Substrings[i] = sortedSubstring[i];
 //sortedSubstring[i] = Substrings[i];
}

void sort(int low, int high) {
  int mid;
  //recursively call sort to get down to two of the values then call merge
  if(low < high) {
    mid = (low + high) / 2;
    // printf("before sort call in sort\n");
    sort(low, mid);
    sort(mid+1, high);
    merging(low, mid, high);
    printf("end of sort: %d\n",++count);    
  }
  else { 
    return;
  } 
}
