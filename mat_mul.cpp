#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <string>
using namespace std;

struct arg_struct {    //struct of parameters for the threads.
    int arg_row;       //variable to store which row the thread is working on.
    int arg_col;       //variable to store which column the thread is working on.
                       //It won't be used when threads compute matrix by row.
};


int** mat1;            //input matrix 1
int** mat2;            //input matrix 2
int** mat3;            //first output matrix using element computation approach
int** mat4;            //seconf output matrix using row computation approach

int n,m,m1,m2,p;             //matrix sizes 

float time_taken[2];   //array of time taken for the 2 approches 


int** createMatrix(int rows, int cols) //function to allocate the matrices in memory
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

void Load(const char* input_file)//function that reads input matrices from a given file  
{
    FILE *pRead;        //pointer for the input file
    pRead=fopen(input_file,"r");
    
    if (pRead== NULL) 
    {
        printf("Error!");
        return ;
    }
    
    fscanf(pRead,"%d",&n);
    fscanf(pRead,"%d",&m1);
    
    mat1 = createMatrix(n,m1);
    
    int i=0,j=0;

    while(!feof(pRead) && i<n)
    {
    	
    	while( !feof(pRead) && j<m1)
    	{
    		fscanf(pRead,"%d",&mat1[i][j]);		
    		j++;
    	}
	fscanf(pRead,"\n");
	j=0;
        i++;
    }
    
    fscanf(pRead,"%d",&m2);
    fscanf(pRead,"%d",&p);
    
    i=0;
    j=0;
    
    mat2 = createMatrix(m2,p);
    
    while(!feof(pRead) && i<m2)
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
void save()//function that writes ouput matrices and time taken for each approach
{
    FILE *pWrite;   //pointer for the output file
    pWrite=fopen("output.txt","w");
    
    int i,j;

     fprintf(pWrite,"By Row\n");
    
    for (i = 0; i <n; i++) 
    {
	 for (j = 0; j < p; j++) 
	{
	      fprintf(pWrite,"%d ", mat3[i][j]);
	      
	}

	fprintf(pWrite,"\n");	
    } 
    
    fprintf(pWrite,"\nTime taken by first approach %.9f \n\n", time_taken[0]);
    
    fprintf(pWrite,"By Element\n");
    
    for (i = 0; i <n; i++) 
    {
	 for (j = 0; j < p; j++) 
	{
	      fprintf(pWrite,"%d ", mat4[i][j]);
	      
	}

	fprintf(pWrite,"\n");	
    } 
    
    fprintf(pWrite,"\nTime taken by second approach %.9f \n", time_taken[1]);
    
    fclose(pWrite);
}

void display_mat()//function that displays matrices
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
    struct arg_struct *args = (struct arg_struct *)arguments;
    
    int sum=0;
    
    for (int i=0 ; i < m ; i++)
    {
    	sum = sum + mat1[args->arg_row][i]*mat2[i][args->arg_col];	
    }
   
    mat3[args->arg_row][args->arg_col]=sum;   
    pthread_exit(NULL);
    return NULL;
}

void *row_operations(void *arguments)
{
    struct arg_struct *args = (struct arg_struct *)arguments;
    
    int sum;
    
    for (int i=0 ; i < p; i++)
    {   
         sum=0;
         
         for (int j=0 ; j < m ; j++)
         {
    	      sum = sum + mat1[args->arg_row][j]*mat2[j][i];
         }
         
         mat4[args->arg_row][i]=sum;	
    }
   
    pthread_exit(NULL);
    return NULL;
}


int main(int argc, char *argv[])
{

    if (argc != 2)
    {
         fprintf(stderr, "usage: %s <input_file>\n", argv[0]);
         return -1;
    }
    
    
    int i,j;

    const char* filename= argv[1];

    Load(filename);
    
    if(m1!=m2)
    {
         printf("Multipication of Matrix is not Possible !!\n");
         FILE *pWrite;   //pointer for the output file
         pWrite=fopen("output.txt","w");
         fprintf(pWrite,"Multipication of Matrix is not Possible !!\n");
         fclose(pWrite);
         return 0;
    }
	  
    //display_mat();
    
    m=m1;

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
    
    
    mat4 = createMatrix(n,p);
     
    pthread_t row_thread[n];
    start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i < n;i++)
    {
              arg_struct* args = (struct arg_struct*) malloc(sizeof(struct arg_struct));
              args->arg_row = i;
              args->arg_col = 0;

              if (pthread_create(&row_thread[i], NULL, &row_operations, (void *)args) != 0) 
              {
                   printf("Uh-oh!\n");
                   return -1;
              }
    }
    
   
    for(int i=0; i < n;i++)
    {    
       	pthread_join(row_thread[i], NULL);
    }
    
    end = std::chrono::high_resolution_clock::now();
    seconds = std::chrono::duration<double>(end-start).count();
    time_taken[1]= static_cast<float>(seconds);
    
    save();
    return 0;
}
