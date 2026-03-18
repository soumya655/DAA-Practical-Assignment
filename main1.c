#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

long long comparisons;

/* ---------------- TIMER ---------------- */

double getTime() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1e6 + t.tv_nsec / 1e3;
}

/* ---------------- INPUT ---------------- */

void generateRandom(int arr[], int n){
    for(int i=0;i<n;i++) arr[i]=rand()%100000;
}

void generateSorted(int arr[], int n){
    for(int i=0;i<n;i++) arr[i]=i+1;
}

void generateReverse(int arr[], int n){
    for(int i=0;i<n;i++) arr[i]=n-i;
}

/* ---------------- SIMPLE SORTS ---------------- */

void insertionSort(int arr[], int n){
    for(int i=1;i<n;i++){
        int key=arr[i], j=i-1;
        while(j>=0 && arr[j]>key){
            comparisons++;
            arr[j+1]=arr[j];
            j--;
        }
        comparisons++;
        arr[j+1]=key;
    }
}

void selectionSort(int arr[], int n){
    for(int i=0;i<n-1;i++){
        int min=i;
        for(int j=i+1;j<n;j++){
            comparisons++;
            if(arr[j]<arr[min]) min=j;
        }
        int t=arr[i]; arr[i]=arr[min]; arr[min]=t;
    }
}

void bubbleSort(int arr[], int n){
    for(int i=0;i<n-1;i++){
        int swapped=0;
        for(int j=0;j<n-i-1;j++){
            comparisons++;
            if(arr[j]>arr[j+1]){
                int t=arr[j]; arr[j]=arr[j+1]; arr[j+1]=t;
                swapped=1;
            }
        }
        if(!swapped) break;
    }
}

/* ---------------- MERGE ---------------- */

void merge(int arr[], int l, int m, int r){
    int n1=m-l+1,n2=r-m;
    int *L=malloc(n1*sizeof(int));
    int *R=malloc(n2*sizeof(int));

    for(int i=0;i<n1;i++) L[i]=arr[l+i];
    for(int j=0;j<n2;j++) R[j]=arr[m+1+j];

    int i=0,j=0,k=l;

    while(i<n1 && j<n2){
        comparisons++;
        if(L[i]<=R[j]) arr[k++]=L[i++];
        else arr[k++]=R[j++];
    }

    while(i<n1) arr[k++]=L[i++];
    while(j<n2) arr[k++]=R[j++];

    free(L); free(R);
}

void mergeSort(int arr[], int l, int r){
    if(l<r){
        int m=(l+r)/2;
        mergeSort(arr,l,m);
        mergeSort(arr,m+1,r);
        merge(arr,l,m,r);
    }
}

void mergeWrapper(int arr[], int n){
    mergeSort(arr,0,n-1);
}

/* ---------------- QUICK SORT (3 VERSIONS) ---------------- */

int partition(int arr[], int low, int high, int pivotType){

    if(pivotType == 2){ // RANDOM
        int r = low + rand() % (high - low + 1);
        int t = arr[low]; arr[low] = arr[r]; arr[r] = t;
    }

    if(pivotType == 3){ // MEDIAN OF THREE
        int mid = (low + high) / 2;
        int a = arr[low], b = arr[mid], c = arr[high];
        int median;

        if((a<=b && b<=c)||(c<=b && b<=a)) median=mid;
        else if((b<=a && a<=c)||(c<=a && a<=b)) median=low;
        else median=high;

        int t=arr[low]; arr[low]=arr[median]; arr[median]=t;
    }

    int pivot = arr[low];
    int i=low+1,j=high;

    while(1){
        while(i<=high && arr[i]<=pivot){ comparisons++; i++; }
        while(j>=low+1 && arr[j]>pivot){ comparisons++; j--; }

        if(i>=j) break;

        int t=arr[i]; arr[i]=arr[j]; arr[j]=t;
    }

    int t=arr[low]; arr[low]=arr[j]; arr[j]=t;
    return j;
}

void quickSort(int arr[], int low, int high, int pivotType){
    if(low<high){
        int pi=partition(arr,low,high,pivotType);
        quickSort(arr,low,pi-1,pivotType);
        quickSort(arr,pi+1,high,pivotType);
    }
}

/* ---------------- HEAP ---------------- */

void heapify(int arr[], int n, int i){
    int largest=i,l=2*i+1,r=2*i+2;

    if(l<n){ comparisons++; if(arr[l]>arr[largest]) largest=l; }
    if(r<n){ comparisons++; if(arr[r]>arr[largest]) largest=r; }

    if(largest!=i){
        int t=arr[i]; arr[i]=arr[largest]; arr[largest]=t;
        heapify(arr,n,largest);
    }
}

void heapSort(int arr[], int n){
    for(int i=n/2-1;i>=0;i--) heapify(arr,n,i);
    for(int i=n-1;i>0;i--){
        int t=arr[0]; arr[0]=arr[i]; arr[i]=t;
        heapify(arr,i,0);
    }
}

/* ---------------- RADIX ---------------- */

int getMax(int arr[], int n){
    int m=arr[0];
    for(int i=1;i<n;i++) if(arr[i]>m) m=arr[i];
    return m;
}

void countSort(int arr[], int n, int exp){
    int *out=malloc(n*sizeof(int));
    int count[10]={0};

    for(int i=0;i<n;i++) count[(arr[i]/exp)%10]++;
    for(int i=1;i<10;i++) count[i]+=count[i-1];

    for(int i=n-1;i>=0;i--){
        out[count[(arr[i]/exp)%10]-1]=arr[i];
        count[(arr[i]/exp)%10]--;
    }

    for(int i=0;i<n;i++) arr[i]=out[i];
    free(out);
}

void radixSort(int arr[], int n){
    int m=getMax(arr,n);
    for(int exp=1;m/exp>0;exp*=10)
        countSort(arr,n,exp);
}

/* ---------------- MAIN ---------------- */

int main(){

    srand(time(NULL));

    int sizes[]={1000,2000,3000,4000,5000,6000,7000,8000,10000};
    int num=9;
    char *types[]={"Random","Best","Worst"};

    for(int run=1;run<=5;run++){

        char fname[50];
        sprintf(fname,"results_run%d.csv",run);

        FILE *f=fopen(fname,"w");
        fprintf(f,"Algo,n,Case,Time,Comparisons\n");

        for(int i=0;i<num;i++){

            int n=sizes[i];
            int *orig=malloc(n*sizeof(int));
            int *arr=malloc(n*sizeof(int));

            for(int t=0;t<3;t++){

                if(t==0) generateRandom(orig,n);
                if(t==1) generateSorted(orig,n);
                if(t==2) generateReverse(orig,n);

                /* SIMPLE SORTS */
                memcpy(arr,orig,n*sizeof(int)); comparisons=0;
                double s=getTime(); insertionSort(arr,n); double e=getTime();
                fprintf(f,"Insertion,%d,%s,%lf,%lld\n",n,types[t],e-s,comparisons);

                memcpy(arr,orig,n*sizeof(int)); comparisons=0;
                s=getTime(); selectionSort(arr,n); e=getTime();
                fprintf(f,"Selection,%d,%s,%lf,%lld\n",n,types[t],e-s,comparisons);

                memcpy(arr,orig,n*sizeof(int)); comparisons=0;
                s=getTime(); bubbleSort(arr,n); e=getTime();
                fprintf(f,"Bubble,%d,%s,%lf,%lld\n",n,types[t],e-s,comparisons);

                /* MERGE */
                memcpy(arr,orig,n*sizeof(int)); comparisons=0;
                s=getTime(); mergeWrapper(arr,n); e=getTime();
                fprintf(f,"Merge,%d,%s,%lf,%lld\n",n,types[t],e-s,comparisons);

                /* HEAP */
                memcpy(arr,orig,n*sizeof(int)); comparisons=0;
                s=getTime(); heapSort(arr,n); e=getTime();
                fprintf(f,"Heap,%d,%s,%lf,%lld\n",n,types[t],e-s,comparisons);

                /* RADIX */
                memcpy(arr,orig,n*sizeof(int)); comparisons=0;
                s=getTime(); radixSort(arr,n); e=getTime();
                fprintf(f,"Radix,%d,%s,%lf,%lld\n",n,types[t],e-s,comparisons);

                /* QUICK (3 VERSIONS) */

                memcpy(arr,orig,n*sizeof(int)); comparisons=0;
                s=getTime(); quickSort(arr,0,n-1,1); e=getTime();
                fprintf(f,"Quick_First,%d,%s,%lf,%lld\n",n,types[t],e-s,comparisons);

                memcpy(arr,orig,n*sizeof(int)); comparisons=0;
                s=getTime(); quickSort(arr,0,n-1,2); e=getTime();
                fprintf(f,"Quick_Random,%d,%s,%lf,%lld\n",n,types[t],e-s,comparisons);

                memcpy(arr,orig,n*sizeof(int)); comparisons=0;
                s=getTime(); quickSort(arr,0,n-1,3); e=getTime();
                fprintf(f,"Quick_Median,%d,%s,%lf,%lld\n",n,types[t],e-s,comparisons);
            }

            free(orig);
            free(arr);
        }

        fclose(f);
        printf("Run %d done\n",run);
    }

    printf("All runs completed.\n");
    return 0;
}