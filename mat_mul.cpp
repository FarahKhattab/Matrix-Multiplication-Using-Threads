#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>

using namespace std;

struct arg_struct {
    int arg_row;
    int arg_col;
    int arg_size;
};


FILE *pRead;
FILE *pWrite;

int** mat1;
int** mat2;
int** mat3;
int** mat4;

int n,m,p;

float time_taken[2];

int k=0;

int** createMatrix(int rows, int cols)
{
  // Create a new dynamic array of integer pointers
  int** array = (int**)malloc(rows * sizeof(int*));

  // For each row in the matrix
  for (int i = 0; i < rows; i++) 
  {
      // Allocate a new dynamic array of integers for the current row
      array[i] = (int*)malloc(cols * sizeof(int));
  }

  // Return the pointer to the matrix
  return array;
}

void Load()
{
    pRead=fopen("input.txt","r");
    
    if (pRead== NULL) 
    {
        printf("Error!");
        return ;
    }
    
    fscanf(pRead,"%d",&n);
    fscanf(pRead,"%d",&m);
    
    mat1 = createMatrix(n,m);
    
    int i=0,j=0;

   // int mat1[n][m];
    while(!feof(pRead) && i<n)
    {
    	
    	while( !feof(pRead) && j<m)
    	{
    		fscanf(pRead,"%d",&mat1[i][j]);		
    		j++;
    	}
	fscanf(pRead,"\n");
	j=0;
        i++;
    }
    
    fscanf(pRead,"%d",&m);
    fscanf(pRead,"%d",&p);
    
    i=0;
    j=0;
    
    mat2 = createMatrix(m,p);
    
    while(!feof(pRead) && i<m)
    { 
    	
    	while(!feof(pRead) && j<p)
    	{
    		fscanf(pRead,"%d",&mat2[i][j]);
    		j++;
    	}
	fscanf(pRead,"\n");
	j=0;
        i++;
    }
    
    fclose(pRead);
}
void save()
{
    pWrite=fopen("output.txt","w");
    
    int i,j;

    for (i = 0; i <n; i++) 
    {
	 for (j = 0; j < p; j++) 
	{
	      fprintf(pWrite,"%d ", mat3[i][j]);
	      
	}

	fprintf(pWrite,"\n");	
    } 
    
    fprintf(pWrite,"\nTime taken by first approach %.9f \n", time_taken[0]);
    fclose(pWrite);
}

void diplay_mat()
{
    int i,j;
    
    printf("\nMAtrix 1:\n");
	
	for (i = 0; i <n; i++) 
	{ 
		for (j = 0; j < m; j++) 
		{
			printf("%d ", mat1[i][j]);
		}
		printf("\n");	

	} 
	
	printf("\nMAtrix 2:\n");
	
	for (i = 0; i <m; i++) 
	{ 
		for (j = 0; j < p; j++) 
		{
			printf("%d ", mat2[i][j]);
		}
		printf("\n");	

	} 
}

void *dot_product(void *arguments)
{
	k++;
    struct arg_struct *args = (struct arg_struct *)arguments;
    
    int sum=0;
    
    for (int i=0 ; i < args->arg_size ; i++)
    {
       //printf("k=%d \t i=%d \tmat1[%d][%d]=%d \t mat2[%d][%d]=%d\n",k,i,args->arg_row, i,mat1[args->arg_row][i],i,args->arg_col,mat2[i][args->arg_col]);
    	sum = sum + mat1[args->arg_row][i]*mat2[i][args->arg_col];
    	
    }
   
    mat3[args->arg_row][args->arg_col]=sum;   
    //printf("k=%d \t sum=%d\n",k,sum);
    pthread_exit(NULL);
    return NULL;
}


int main()
{
    int i,j;
    
    Load();
	  
    //diplay_mat();

    mat3 = createMatrix(n,p);
    
    pthread_t element_thread[n][p];
    
    auto start = std::chrono::high_resolution_clock::now();
    for(i=0; i < n;i++)
    {
         for(j=0; j < p;j++)
         {
              arg_struct* args = (struct arg_struct*) malloc(sizeof(struct arg_struct));
              args->arg_row = i;
              args->arg_col = j;
              args->arg_size=m; 
              if (pthread_create(&element_thread[i][j], NULL, &dot_product, (void *)args) != 0) 
              {
                   printf("Uh-oh!\n");
                   return -1;
              }

            
         }
    }
   
    for(i=0; i < n;i++)
    {
         for(j=0; j < p;j++)
         {
         	pthread_join(element_thread[i][j], NULL);
         }	
    }

    auto end = std::chrono::high_resolution_clock::now();
    double seconds = std::chrono::duration<double>(end-start).count();
    time_taken[0]= static_cast<float>(seconds);
    
    save();
    return 0;
}
