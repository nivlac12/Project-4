/*
 * NOTE:

 * This is our best implementation, we sorted our output by the length of the string rather than the order of appearance in the file.
 * After discussing with some of the other teams today (Tuesday), I realized when the instructions ask the output to be sorted, they
 * were probably just referring to printing in order. I made a quick change in a separate version of this file to print the top 50
 * strings as they appeared, the output is at the end of this comment. The mergesort we used takes extra space, since it copies the
 * array Substrings, so that may be one reason for some of our more surprising results (mentioned in the review).

  0-1: <page> <title>aa
  1-2: <page> <title>aa
  2-3: <page> <title>aapg
  3-4: <page> <title>aar
  4-5: <page> <title>abb_
  5-6: :abc cinema butlins bogno
  6-7:  = abc dj\n| image =
  7-8: ndstand</title><text>{{u
  8-9: itle><text>{{other u
  9-10: <page> <title>abc
  10-11: |type:landmark_scal
  11-12: <page> <title>ab
  12-13: <page> <title>absw
  13-14: <page> <title>ab_doradus
  14-15:  book arts]], an am
  15-16: <page> <title>a
  16-17: \n\n{{disambig}}</text> </page>
  17-18: n\n* [[american co
  18-19: \n\n{{disambiguation}}</text> </page>
  19-20: </title><text>\'\'\'ac
  20-21: <page> <title>ac
  21-22: m1    = \n| body1
  22-23: lenge cup]] in 2012, downing in th
  23-24: <page> <title>a
  24-25: [[australian defence
  25-26: <page> <title>ad
  26-27: sa</title><text>
  27-28: sed a postseason o
  28-29: <page> <title>a
  29-30: \n\n{{disambig}}</text> </page>
  30-31: <page> <title>afc
  31-32: <page> <title>afc
  32-33: <page> <title>afl_trade_
  33-34: can friends service committee]]\n*[[ai
  34-35: |agla france logo]
  35-36: <page> <title>ag_
  36-37: <page> <title>a
  37-38: ence of the ieee engi
  38-39: <page> <title>aids_cl
  39-40: \'\'\'aifs\'\'\' may ref
  40-41: | coordinates         = {{coord|35|14|52|s|149|6|4|e|di
  41-42: \n\n{{disambig}}</text> </page>
  42-43: ]]\n\n{{disambig}}</text> </page>
  43-44: <page> <title>aj
  44-45:  = 2002\n| place of death    =\n}}
  45-46: ap</title><text>
  46-47: <page> <title>ak
  47-48: te=december 2008}}
  48-49: {about|the missile||alarm (disamb
  49-50: cations, and [[laser]

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_LINES 1000000
#define MAX_LINE_LENGTH 2020
#define NUM_THREADS 16

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

  for(j = 0; e!=EOF && j < MAX_LINES; j++)
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
  
  sort(0,MAX_LINES-1);
  print_results();
  

  pthread_exit(NULL);
  //#pragma omp parallel for  
  /*for(z = 0; z < 10000; z++){
    get_substrings(z);
    }*/
  return 0;
}

void init_arrays()
{
  sortedSubstring = (char **) malloc (MAX_LINES * sizeof(char *));
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
  int i;
  for(i=MAX_LINES-1; i>=MAX_LINES-50; i--)
    {
      printf("%s\n", Substrings[i]); 
    }
}

void get_substrings(void *myID)
{
  int i, j, k, l, m, first_length, second_length;
  int startPos = ((int) myID) * (MAX_LINES / NUM_THREADS);
  int endPos = startPos + (MAX_LINES / NUM_THREADS);
  for(i = startPos; i<endPos; i++)
  {
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
    if(strlen(strchr(Substrings[l1],':')) <= strlen(strchr(Substrings[l2],':')))
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
  }
  else { 
    return;
  } 
}
