#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h> 
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_permutation.h>  

#define N 11731
#define M 11731 //line
#define P 12035//node
#define U 238//user
#define OUT_TXT			"outhypergraph f.txt"


	char MakeMatrix(char **pMatrix, int row, int col) 
	{ 
    	int i, j; 
    	//pMatrix = (char **) malloc( sizeof(char *) * row); 
    	if (NULL == *pMatrix) 
    	{ 
       	 	printf("rows error!\n"); 
        	return -1; 
    	} 
     
   	 for (i=0; i <row; i++) 
    	{ 
        	pMatrix[i] = (char *) malloc( sizeof(char) * col); 
        	if (NULL == pMatrix[i]) 
        	{ 
           	 printf("cols errors!\n"); 
            	return -1; 
        	} 
    	} 
     
    	for (i=0; i <row; i++) 
        	for (j=0; j <col; j++) 
        	{ 
           	 pMatrix[i][j] =NULL; 
        	} 
         	return 0; 
	}

	char FreeMatrix(char **pMatrix, int row, int col) 
	{ 
    	int i; 
    	for (i=0; i <row; i++) 
    	{ 
        	free(pMatrix[i]); 
        	pMatrix[i] = NULL; 
    	} 
    	free(pMatrix); 
    	pMatrix = NULL; 
    	return 0; 
	} 
 
  	int MakeMatrixint(int **pMatrix, int row, int col) 
	{ 
    	int i, j; 
    	//pMatrix = (int **) malloc( sizeof(int *) * row); 
    	if (NULL == *pMatrix) 
    	{ 
        	printf("rows error!\n"); 
        	return -1; 
    	} 
     
    	for (i=0; i <row; i++) 
    	{ 
        	pMatrix[i] = (int *) malloc( sizeof(int) * col); 
        	if (NULL == pMatrix[i]) 
        	{ 
            	printf("cols errors!\n"); 
            	return -1; 
        	} 
   	 } 
     
    	for (i=0; i <row; i++) 
        for (j=0; j <col; j++) 
        { 
            pMatrix[i][j] = 0; 
        } 
        return 0; 
	} 

	char FreeMatrixint(int **pMatrix, int row, int col) 
	{ 
    		int i; 
    	for (i=0; i <row; i++) 
    	{ 
        	free(pMatrix[i]); 
        	pMatrix[i] = NULL; 
    	} 
    	free(pMatrix); 
    	pMatrix = NULL; 
   	 return 0; 
	} 


  	int MakeMatrixdouble(double **pMatrix, int row, int col) 
	{ 
    	int i, j; 
    	//pMatrix = (double **) malloc( sizeof(double *) * row); 
    	if (NULL == *pMatrix) 
    	{ 
        	printf("rows error!\n"); 
        	return -1; 
    	} 
     
    	for (i=0; i <row; i++) 
    	{ 
        pMatrix[i] = (double *) malloc( sizeof(double) * col); 
        if (NULL == pMatrix[i]) 
        	{ 
           	printf("cols errors!\n"); 
            	return -1; 
        	} 
    	} 
     
    	for (i=0; i <row; i++) 
        for (j=0; j <col; j++) 
        { 
            pMatrix[i][j] = 0.0; 
        } 
        return 0; 
	} 

	double FreeMatrixdouble(double **pMatrix, int row, int col) 
	{ 
    	int i; 
    	for (i=0; i <row; i++) 
    	{ 
       	 free(pMatrix[i]); 
       	 pMatrix[i] = NULL; 
    	} 
    	free(pMatrix); 
    	pMatrix = NULL; 
    	return 0; 
	} 



main(void)
{
	FILE *fp,*fp2,*fp3,*fp4; 
	int k,m,n,p;
	double b;
	double** Ds;
	double** De;
	double** Den;
	double** Dsn;
	int i,j,t=1;
	int *B=(int*)malloc(sizeof(int)*N); 
	double** W;
	double** C;
	int** H;
	int** HT;
	double* Cni=(double*)malloc(sizeof(double)*P*P);
	char** str; //note
	double** Dv;
	char** hyperh;//line
	char** node;//all user 
	double** X;
	char** rank;         //relationship 100 biger pehaps
	double** G;           //follow relationship
	int *z=(int*)malloc(sizeof(int)*N);
	int *V=(int*)malloc(sizeof(int)*N);
	double *q=(double*)malloc(sizeof(double)*U);//first 1/n
	double *e=(double*)malloc(sizeof(double)*U);//value
	printf("txt context:\n"); //print text
	str=(char**)malloc(P*50*sizeof(char *));
	hyperh=(char**)malloc(N*100*sizeof(char *));
	MakeMatrix(str, P, 50); 
	MakeMatrix(hyperh, N, 100); 

	fp=fopen("./node1.txt","r"); //open file
	fp2=fopen("./line.txt","r+"); //open file
		if(fp==NULL) 
	{  
		printf("file open error!\n");  
		exit(0); 
	}
	for(i=0;i<P;i++)   
	{
		fgets(str[i],50,fp);
		//fgets(*(str+i*100),100,fp); //put it in str[]
	}
	for(i=0;i<P;i++)
	{
		for(j=0;j<50;j++)//last word is enter
		{
			if(str[i][j]=='\n')str[i][j]='\0';//enter delete 
		   	if(str[i][j]=='\r')str[i][j]='\0';
		}
	}
	
	fclose(fp);
	for(j=0;j<N;j++)  
	{
		fgets(hyperh[j],100,fp2); //get txt massage
	}
	fclose(fp2);

    	H=(int**)malloc(N*P*sizeof(int *));
	MakeMatrixint(H, P, N); 
	double *a=(double*)malloc(sizeof(double)*P);
	for(i=0;i<P;i++)
		a[i]=0;
	for(m=0;m<P;m++)//note
	{
		for(n=0,b=0;n<N;n++)//line
                for(i=0;hyperh[n][i]!='\0';i++)//line
		   {
		     for(k=i,j=0;str[m][j]!='\0';)//line do not over
			    if(hyperh[n][k]==str[m][j])//one word pipei
				   k++,j++;
			    else
				   break;
			    if(str[m][j] == '\0' && j > 0)//note  get    
				{

				       	b++;
			  	   	 H[m][n];
				      	 a[m]=b; 
				}
			
		    }
	}





	Dv=(double**)malloc(P*P*sizeof(double *));
	MakeMatrixdouble(Dv, P, P); 
	for(i=0;i<P;i++)
	{
		for(j=0;j<P;j++)
		{
			Dv[i][j]=0;
		}
	}

		for(i=0;i<P;i++)
	{
		for(j=0;j<P;j++)
		{
			if(i==j)
				Dv[i][j]=a[i];
		}
	}
		printf("%f\n ",Dv[1][1]);

	W=(double**)malloc(N*N*sizeof(double *));
	MakeMatrixdouble(W, N, N); 
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			if(i==j)
			    W[i][j]=1;
	}

	HT=(int**)malloc(N*P*sizeof(double *));
	MakeMatrixint(HT, N, P); 
	for(i=0;i<P;i++)	
			for(n=0;n<N;n++)
			{
			    HT[n][i]=H[i][n];
		
			}


	De=(double**)malloc(N*N*sizeof(double *));
	MakeMatrixdouble(De, N, N); 
		for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			De[i][j]=0;
		}
	}
	for(n=0;n<N;n++)//De biaoxian
        {
		for(k=0,t=1;k<100;k++)
		if(hyperh[n][k]==' ')
			t++;
		    B[n]=t;
	}
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			if(i==j)
			    De[i][j]=B[i];
	}
	printf("De\n");



	Ds=(double**)malloc(P*P*sizeof(double *));
	MakeMatrixdouble(Ds, P, P); 
	for(i=0;i<P;i++)
	{
		for(j=0;j<P;j++)
           	Ds[i][j]=0;
	}
	for(i=0;i<P;i++)
	{
		for(j=0;j<P;j++)
			if(i==j)
			Ds[i][j]=sqrt(Dv[i][j]);
	}
	printf("%f\n",Ds[1][1]);
	FreeMatrixdouble(Dv,P,P); 

	Den=(double**)malloc(N*N*sizeof(double *));
	MakeMatrixdouble(Den, N, N); 
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			if(i==j)
			{
				if(De[i][j]==0)
				{
					Den[i][j]=1;
				}
				else
				Den[i][j]=1/De[i][j];
			}
	}

	Dsn=(double**)malloc(P*P*sizeof(double *));
	MakeMatrixdouble(Dsn, P, P); 
	for(i=0;i<P;i++)
	{
		for(j=0;j<P;j++)
			if(i==j)
			{
				if(Ds[i][j]==0)
				{
					Dsn[i][j]=1;
				}
				else
				Dsn[i][j]=1/Ds[i][j];
			}
	}
	printf("%f ",Dsn[1][1]);

	double** DsnH;
	DsnH=(double**)malloc(P*N*sizeof(double *));
	MakeMatrixdouble(DsnH, P, N); 
	for(i=0;i<P;i++)
	{
		for(j=0;j<M;j++) 
		{
			for(k=0;k<P;k++)         
			DsnH[i][j]+=Dsn[i][k]*H[k][j];
		}
	}
	printf("DsnH\n ");
	double** DsnHW;
	DsnHW=(double**)malloc(P*N*sizeof(double *));
	MakeMatrixdouble(DsnHW, P, N); 
	for(i=0;i<P;i++)
	{
		for(j=0;j<N;j++) 
		{
			for(k=0;k<N;k++)         
			DsnHW[i][j]+=DsnH[i][k]*W[k][j];
		}
	}
		printf("DsnHW\n ");

	double** DsnHWDen;
	DsnHWDen=(double**)malloc(P*N*sizeof(double *));
	MakeMatrixdouble(DsnHWDen, P, N); 
	for(i=0;i<P;i++)
	{
		for(j=0;j<N;j++) 
		{
			for(k=0;k<N;k++)         
			DsnHWDen[i][j]+=DsnHW[i][k]*Den[k][j];
		}
	}
		printf("DsnHWDn\n ");


	double** Az;
	Az=(double**)malloc(P*P*sizeof(double *));
	MakeMatrixdouble(Az, P,P); 
	for(i=0;i<P;i++)
	{
		for(j=0;j<P;j++) 
		{
			for(k=0;k<N;k++)         
			Az[i][j]+=DsnHWDen[i][k]*HT[k][j];
		}
	}
	
	printf("Az\n ");

	double** A;
	A=(double**)malloc(P*P*sizeof(double *));
	MakeMatrixdouble(A, P,P); 
	for(i=0;i<P;i++)
	{
		for(j=0;j<P;j++) 
		{
			for(k=0;k<P;k++)         
			A[i][j]+=Az[i][k]*Dsn[k][j];
		}
	}
	printf("A\n ");

	FreeMatrixdouble(Ds,P,P); 
	FreeMatrixdouble(De,N,N); 
	FreeMatrixdouble(W,N,N); 
	FreeMatrixint(H,P,N); 
	FreeMatrixint(HT,N,P); 
	FreeMatrixdouble(Dsn,P,P); 
	FreeMatrixdouble(Den,N,N); 
	FreeMatrixdouble(DsnH,P,N); 
	FreeMatrixdouble(DsnHW,P,N); 
	FreeMatrixdouble(DsnHWDen,P,N); 
	FreeMatrixdouble(Az,P,P); 

	
	printf("\n");              //qiu (I-aA)
	double x=0.3;

	X=(double**)malloc(P*P*sizeof(double *));
	MakeMatrixdouble(X, P,P); 

	C=(double**)malloc(P*P*sizeof(double *));
	MakeMatrixdouble(C, P,P); 
	for(i=0;i<P;i++)
	{
		for(j=0;j<P;j++)
			X[i][j]=x*A[i][j];
	}
	for(i=0;i<P;i++)
	{
	    for(j=0;j<P;j++)
			if(i==j)
			{
				C[i][j]=(1-X[i][j]);
			}
			else
				C[i][j]=-X[i][j];
	}

	printf("c\n ");
	
	double** Cnii;
  	for(i=0;i<P;i++)
	{		//ni juzheng
	   for(j=0;j<P;j++)
        	   Cni[i*P+j]=C[i][j];
	}
	Cnii=(double*)malloc(P*P*sizeof(double *));
	MakeMatrixdouble(Cnii,P,P);
  
   
   
	double *b_data=(double*)malloc(sizeof(double)*P); 
		for(i=0;i<P;i++)
		{
                    b_data[i]=i;
		}

    	gsl_matrix_view nj
    	= gsl_matrix_view_array(Cni, P, P);
   
    	gsl_vector_view c
    	= gsl_vector_view_array(b_data, P);
   	
    	gsl_vector *xj = gsl_vector_alloc (P);
    	gsl_matrix *mj= gsl_matrix_alloc (P,P);


   
    	int r;
   
    	gsl_permutation *pi = gsl_permutation_alloc (12035);
   
    	gsl_linalg_LU_decomp (&nj.matrix, pi, &r);
   
    	gsl_linalg_LU_invert (&nj.matrix, pi, mj);
   
    	printf ("inv Cni=\n");
     	{
        	int i;
        	for (i = 0; i < P; i++)
        	{
          		gsl_vector_view mj_i= gsl_matrix_column (mj, i);
          		for(j=0;j<P;j++)
			{    
          			Cni[i*P+j]= gsl_vector_get(&mj_i.vector, j);
			}
		}
     	}
	for(i=0;i<P;i++)
	{
		for(j=0;j<P;j++)
			Cnii[i][j]=Cni[i*P+j];
	}
      
               
    	gsl_permutation_free (pi);



	printf("\n");
                                                 
	double *y=(double*)malloc(sizeof(double)*P);  // keyword ni yo ru hypergraph no value
	char v[10]="ì";
	//printf("please input keyword:\n");
	//scanf("%s",v);
	//double *r=(double*)malloc(sizeof(double)*P);
	int h;
        //	for(i=0;i<P;i++)      //search the keyword
	//	r[i]=0;
   	 for(i=0;i<P;i++)
	{
		for(j=0;str[i][j]!='\0';j++)//node
		{
			 for(k=j,m=0;v[m]!='\0';)//line do not over
			    if(str[i][k]==v[m])//one word pipei
				   k++,m++;
			    else
				   break;
			    if(v[m]== '\0' && m > 0)//note over win    
				{
                      h=i;
				}
		}
	}
		printf("%d\n",h);
        printf("y=\n");                    // made y
    	for(i=0;i<P;i++)
		if(i==h)
		{
		    y[i]=1;
		}
		else
			y[i]=A[h][i];


	

	printf("\n");                     //fina value f
	printf("f=\n");
	double *f=(double*)malloc(sizeof(double)*P);
	for(i=0;i<P;i++)
		f[i]=0;
		for(i=0;i<P;i++)    
			for(k=0;k<P;k++)         
			f[i]+=Cnii[i][k]*y[k];
		for(i=0;i<U;i++)
		{
			printf("%f\n",f[i]);
		}
		for(i=0;i<U;i++)
		fprintf(fp3,"%f\n",f[i]);
}


