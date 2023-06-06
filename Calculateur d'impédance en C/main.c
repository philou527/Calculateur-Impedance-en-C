#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846


typedef struct{
  double f;
  double m;
  double a;
} triple;

void getImpedance(double*,double*);
double* getFrequency(int*);
triple* calculateImpedance(int, double*, double*,double*,int);
void showMenu(int*);
void printValues(triple* ,int);
void printArray(double*, int);
triple* valueRCP(double*,double*,double *, int );
triple* valueRCS(double *, double *c, double* , int );
triple* valueRLP(double*, double *, double *, int );
triple* valueRLS(double *, double *, double *, int );
triple* valueCLS(double *c, double *l,double *, int) ;
triple* valueCLP(double *c, double *l,double *, int) ;
void writeFile(triple*, int);



int main()
{
    int circuit,n;
    double z1,z2,mod,arg;
    showMenu(&circuit);
    getImpedance(&z1,&z2);
    double *freq = getFrequency(&n);
    triple* tab=calculateImpedance(circuit,&z1,&z2,freq,n);
    printValues(tab,n);

    free(tab);
    free(freq);
    return 0;
}

void showMenu(int *circuit){
  printf("Choissez votre circuit entre 1 et 6:\n");
  printf("\tchoix 1: RL serie\n");
  printf("\tchoix 2: RL parallele\n");
  printf("\tchoix 3: RC serie\n");
  printf("\tchoix 4: RC parallele\n");
  printf("\tchoix 5: CL serie\n");
  printf("\tchoix 6: CL parallele\n");
  do {
      scanf("%d",circuit);
  } while(*circuit<=0 || *circuit>6);
}

void getImpedance(double *z1,double *z2){
    printf("Entrez la valeur de Z1 et Z2\n");
    scanf("%lf%lf",z1,z2);
    return;
}

double* getFrequency(int *n){
    double fmin,fmax;
    do{
      printf("Entrez la nombre de frequences voulues ainsi que la frequence minimale et maximale :\n");
      scanf("%d%lf%lf",n,&fmin,&fmax);
    }while(*n==0 || fmin <= 0 || fmax < fmin);

    double* freq=malloc(*n*sizeof(double));
    double increment = (double)((fmax-fmin))/(*n-1);
    freq[0]=fmin;
    for(int i=1;i<*n;i++)
      freq[i]=freq[i-1]+increment;

    return freq;
}


triple* calculateImpedance(int circuit, double *z1, double *z2,double *freq,int n){
  triple *tab=malloc(3*n*sizeof(double));
  switch (circuit){
    case 1:
      printf("Impedance pour le circuit RL serie avec comme valeur de R: %lf et de L: %lf",*z1,*z2);
      tab=valueRLS(z1,z2,freq,n);
      break;
    case 2:
      printf("Impedance pour le circuit RL parallele avec comme valeur de R: %lf et de L: %lf",*z1,*z2);
      tab=valueRLP(z1,z2,freq,n);
      break;
    case 3:
      printf("Impedance pour le circuit RC serie avec comme valeur de R: %lf et de C: %lf",*z1,*z2);
      tab=valueRCS(z1,z2,freq,n);
      break;
    case 4:
      printf("Impedance pour le circuit RC parallele avec comme valeur de R: %lf et de C: %lf",*z1,*z2);
      tab=valueRCP(z1,z2,freq,n);
      break;
    case 5:
      printf("Impedance pour le circuit CL serie avec comme valeur de C: %lf et de L: %lf",*z1,*z2);
      tab=valueCLS(z1,z2,freq,n);
      break;
    case 6:
      printf("Impedance pour le circuit CL parallele avec comme valeur de C: %lf et de L: %lf",*z1,*z2);
      tab=valueCLP(z1,z2,freq,n);
      break;
    default:
      printf("Incorrect value for the circuit please start the program again");
  }
  return tab;
}

void printValues(triple *t,int n){
  printf("\nQuel affichage voulez-vous ?\n");
  printf("\tOption 1: Array\n");
  printf("\tOption 2: Plot\n");
  int choice;
  scanf("%d",&choice);


  if(choice==1){
    for(int i=0;i<n;i++)
      printf("%lf %lf %lf\n",t[i].f,t[i].m,t[i].a);
  }
  else {
    writeFile(t,n);
  }
  return;
}

void writeFile(triple *t, int n){
  FILE *fichier=fopen("donnee.txt","w");
  if(fichier==NULL){
    printf("erreur lors de la creSation du fichier");
  }
  else {
    for(int i=0;i<n;i++)
      fprintf(fichier,"%lf %lf %lf\n",t[i].f,t[i].m,t[i].a);
      
    fclose(fichier);
    printf("done");
  }
  return;
}

triple* valueRCP(double *r,double *c,double *freq, int n){
    double re,im,denominator,mod,arg;
    triple *tab=malloc(3*n*sizeof(double));
    if(tab==NULL){
      printf("Malloc failed try again");
    }
    else {
      for(int i = 0;i<n;i++){
          denominator=1+(*r)*(*r)*(*c)*(*c)*2*PI*(freq[i]);
          re=(*r)/denominator;
          im=(-(*r)*(*r)*(*c)*2*PI*(freq[i]))/denominator;
          //*mod= sqrt(pow(re,2)+pow(im,2));
          mod = 1/(sqrt( pow((1/(*r)),2) + pow((2*PI*(freq[i])*(*c)),2)));
          arg=atan(im/re);
          tab[i].f = freq[i];
          tab[i].m = mod;
          tab[i].a = arg;
      }
    }

    return tab;
}

triple* valueRCS(double *r, double *c,double *freq, int n){
    double re, im,mod,arg;
    triple *tab=malloc(3*n*sizeof(double));
    if(tab==NULL){
      printf("Malloc failed try again");
    }
    else {
      for(int i = 0;i<n;i++){
        re = *r;
        im = (-1)/((*c)*(2*PI*(freq[i])));
        mod = sqrt(pow(re,2)+pow(im,2));
        arg = atan(im/re);
        tab[i].f = freq[i];
        tab[i].m = mod;
        tab[i].a = arg;
      }
    }
    return tab;
}

triple* valueRLP(double *r, double *l,double *freq, int n){
    double re, im,mod,arg;
    triple *tab=malloc(3*n*sizeof(double));
    if(tab==NULL){
      printf("Malloc failed try again");
    }
    else {
      for(int i = 0;i<n;i++){
        double w = 2*PI*(freq[i]);
        double denom = pow((*r),2) + pow((w),2)*pow((*l),2);
        re = ((*r)*pow((*l),2)*pow(w,2))/denom;
        im = (pow((*r),2)*(*l)*(w))/denom;
        mod = sqrt(pow(re,2)+pow(im,2));
        arg = atan(im/re);
        tab[i].f = freq[i];
        tab[i].m = mod;
        tab[i].a = arg;
      }
    }
    return tab;

}

triple* valueRLS(double *r, double *l,double *freq, int n){
    double re, im,mod,arg;
    triple *tab=malloc(3*n*sizeof(double));
    if(tab==NULL){
      printf("Malloc failed try again");
    }
    else {
    for(int i = 0;i<n;i++){

        double w = 2*PI*(freq[i]);
        re = (*r);
        im = ((*l)*w);
        mod = sqrt(pow(re,2)+pow(im,2));
        arg = atan(im/re);
        tab[i].f = freq[i];
        tab[i].m = mod;
        tab[i].a = arg;
      }
    }
    return tab;
}

triple* valueCLS(double *c, double *l,double *freq, int n){
    double re, im,mod,arg;
    triple *tab=malloc(3*n*sizeof(double));
    if(tab==NULL){
      printf("Malloc failed try again");
    }
    else {
      for(int i = 0;i<n;i++){

        double w = 2*PI*(freq[i]);
        re = 0;
        im = (-1+(*l)*(*c)*pow(w,2))/((*c)*w);

        mod = sqrt(pow(re,2)+pow(im,2));
        arg = atan(im/re);
        tab[i].f = freq[i];
        tab[i].m = mod;
        tab[i].a = arg;
      }
    }
    return tab;


}
triple* valueCLP(double *c, double *l,double *freq, int n){
    double re, im,mod,arg;
    triple *tab=malloc(3*n*sizeof(double));
    if(tab==NULL){
      printf("Malloc failed try again");
    }
    else {
      for(int i = 0;i<n;i++){

        double w = 2*PI*(freq[i]);
        re = 0;
        im = ((*l)*w)/(1-(*c)*(*l)*pow(w,2));

        mod = sqrt(pow(re,2)+pow(im,2));
        arg = atan(im/re);
        tab[i].f = freq[i];
        tab[i].m = mod;
        tab[i].a = arg;
      }
    }
    return tab;
}

void printArray(double *a, int n){
  for(int i=0;i<n;i++)
    printf("%lf ",a[i]);

  return;
}
