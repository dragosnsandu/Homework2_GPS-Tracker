//##########################################################\\
//															\\
//		TEMA 2, SDA, 313AB									\\
//															\\
//	Author: Dragos-Nicolae Sandu, dragos.n.sandu@gmail.com 	\\
//															\\
//##########################################################\\

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define COST_MAX 10000

//Variabile globale
int X;
int Y;
int nr_locatii;
int **cost=NULL;

//Functie pentru calcularea coordonatelor punctului de intersectie obtinut pe baza coordonatelor raportate de cei trei sateliti
void calculare_coordonate(int x0,int y0,int r0,int x1,int y1,int r1,int x2,int y2,int r2)
{
	//Formula de calcul a abscisei pozitiei curente
	X=((y2-y0)*((x1*x1)+(y1*y1)-(r1*r1)-(x0*x0)-(y0*y0)+(r0*r0))+(y1-y0)*(-(x2*x2)-(y2*y2)+(r2*r2)+(x0*x0)+(y0*y0)-(r0*r0)))/(2*((y0-y1)*(x2-x0)+(x1-x0)*(y2-y0)));
	//Formula de calcul a ordonatei pozitiei curente
	Y=(((-2)*X*(x1-x2))+(x1*x1)+(y1*y1)-(r1*r1)-(x2*x2)-(y2*y2)+(r2*r2))/(2*(y1-y2));
}

void graph_print()
{
	int i, j;

	printf("      ");
	for (i = 0; i < nr_locatii; ++i)
		printf("%3d ", i);
	printf("\n");

	printf("      ");
	for (i = 0; i < nr_locatii; ++i)
		printf("----");
	printf("\n");

	for (i = 0; i < nr_locatii; ++i) {
		printf("%3d | ", i);
		for (j = 0; j < nr_locatii; ++j)
			printf("%3d ", cost[i][j] >= COST_MAX ? 0 : cost[i][j]);
		printf("\n");
	}
}

void preluare_coordonate(char *fisier_coordonatein, char *fisier_coordonateout)
{
	FILE *filein,*fileout;
	int x0,y0,r0;
	int x1,y1,r1;
	int x2,y2,r2;
	int i;
	if((filein=fopen(fisier_coordonatein,"r"))==NULL)
	{
		printf("Fisierul %s nu a putut fi deschis", fisier_coordonatein);
		exit(1);
	}
	fscanf(filein,"%d",&nr_locatii);//citirea numarului de locatii din fisier
	if((fileout=fopen(fisier_coordonateout,"w"))==NULL)
	{
		printf("Fisierul %s nu poate fi deschis", fisier_coordonateout);
		exit(1);
	}
	for(i=0;i<nr_locatii;i++)
	{
		//preluarea coordonatelor raportate de cei trei sateliti
		fscanf(filein,"%*s %*s %d %d %d", &x0, &y0, &r0);
		fscanf(filein,"%*s %d %d %d", &x1, &y1, &r1);
		fscanf(filein,"%*s %d %d %d", &x2, &y2, &r2);
		//calcularea coordonatelor pozitiei actuale
		calculare_coordonate(x0,y0,r0,x1,y1,r1,x2,y2,r2);
		fprintf(fileout,"%d %d\n",X,Y); //scrierea in fisier a coordonatelor obtinute
	}
	fclose(fileout);
fclose(filein);
}

void formare_matrice(char *fisier_coordonateout, char *fisier_avarii)
{
	FILE *file, *fileavarii;
	char c;
	int numar_avarii_fisier=0;
	int i,j, *x,*y;
	int loc1, loc2;
	//alocarea a doi vectori x si y. vectorul x va stoca abscisele coordonatelor obtinute anterior, citite acum din fisier, iar y ordonatele acestora. 
	x=(int *)malloc(nr_locatii*sizeof(int));
	y=(int *)malloc(nr_locatii*sizeof(int));
	if((file=fopen(fisier_coordonateout,"r"))==NULL)
	{
		printf("Fisierul %s nu a putut fi deschis", fisier_coordonateout);
		exit(1);
	}

	//alocarea matricei care va stoca costurile drumului dintre oricare doua localitati
	cost=(int**)malloc(nr_locatii*sizeof(int*));
	for(i=0;i<nr_locatii;i++)
		cost[i]=(int*)malloc(nr_locatii*(sizeof(int)));
	for(i=0;i<nr_locatii;i++)
		for(j=0;j<nr_locatii;j++)
			cost[i][j]=COST_MAX; //initializarea tuturor drumurilor cu costul maxim.

	//citirea din fisier a coordonatelor obtinute cu ajutorul functiei calculare_coordonate si introducerea lor in cei doi vectori, x si y.
	for(i=0;i<nr_locatii;i++)
		fscanf(file,"%d %d\n", &x[i], &y[i]);
	fclose(file);

	//se parcurg cei doi vectori cu ajutorul a doua for-uri, astfel incat sa fie 
	//calculata si introdusa in matrice 'cost' distanta (costul) dintre oricare doua localitati.
	for(i=0;i<nr_locatii-1;i++)
		for(j=i+1;j<nr_locatii;j++)
		{
			cost[i][j]=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
			cost[j][i]=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
		}
	//eliberez memoria folosita de cei doi vectori
	free(x);
	free(y);

	if((fileavarii=fopen(fisier_avarii,"r"))==NULL)
	{
		printf("Fisierul %s nu a putut fi deschis", fisier_avarii);
		exit(1);
	}
	
	//se afla numarul de linii din fisierul de avarii, reprezentand numarul avariilor.
	while(!feof(fileavarii))
	{
  		c = fgetc(fileavarii);
  		if(c == '\n')
  			numar_avarii_fisier++;
	}
	fseek(fileavarii,0,SEEK_SET);//muta cursorul din nou la inceputul fisierului, pentru a efectua cu succes citirea avariilor
	for(i=0;i<numar_avarii_fisier;i++)
	{
		//preluarea numai a valorii intregi, reprezentand numarul de ordine al locatiei
		fscanf(fileavarii,"Locatie%d " "Locatie%d\n", &loc1, &loc2);
		//drumurile avariate din fisier sunt marcate in matrice cu COST_MAX.
		printf("%d %d;",loc1,loc2);
		cost[loc1-1][loc2-1]=COST_MAX;
		cost[loc2-1][loc1-1]=COST_MAX;
	}	
	fclose(fileavarii);
}

void gasire_cost_minim(char *destinatie, char *output_file)
{
	FILE *output;
	int k,i,j,val_loc_dest;
	if((output=fopen(output_file,"w"))==NULL)
	{
		printf("Fisierul %s nu a putut fi deschis\n", output_file);
		exit(1);
	}

	//preluarea din intreg string-ul reprezentand Locatia de destinatie numai a valorii int (numarul de ordine al locatiei)
	sscanf(destinatie,"Locatie%d", &val_loc_dest);
	//algoritmul Floyd-Warshall pentru calcularea drumului cu costul cel mai mic dintre doua locatii.
	//se suprascrie matricea cu costurile initiale.
	for(k=0;k<nr_locatii;k++)
	{
		for(i=0;i<nr_locatii;i++)
		{
			for(j=0;j<nr_locatii;j++)
			{
				if(cost[i][j] > cost[i][k] + cost[k][j])
					cost[i][j]=cost[i][k] + cost[k][j];
			}
		}
	}
	//se scrie in fisierul rezultat costul minim dintre locatia de inceput si destinatia aleasa.
		fprintf(output,"%d",cost[0][val_loc_dest-1]);
	fclose(output);

	//eliberez memoria folosita de matrice
	for(i=0;i<nr_locatii;i++)
		free(cost[i]);
	free(cost);
}

int main(int argc,char *argv[])
{
	preluare_coordonate(argv[1],argv[4]);
	formare_matrice(argv[4], argv[2]);
	graph_print();
	gasire_cost_minim(argv[3],argv[5]);
	graph_print();
	return 0;
}