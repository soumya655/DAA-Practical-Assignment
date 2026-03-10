#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

long long comparisons = 0;

/* ------------------ INPUT GENERATORS ------------------ */

void generateRandom(int arr[], int n) {
    for(int i = 0; i < n; i++)
        arr[i] = rand() % 100000;
}

void generateSorted(int arr[], int n) {
    for(int i = 0; i < n; i++)
        arr[i] = i + 1;
}

void generateReverse(int arr[], int n) {
    for(int i = 0; i < n; i++)
        arr[i] = n - i;
}

/* ------------------ SIMPLE SORTS ------------------ */

void insertionSort(int arr[], int n) {
    for(int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while(j >= 0) {
            comparisons++;
            if(arr[j] > key) {
                arr[j+1] = arr[j];
                j--;
            } else break;
        }
        arr[j+1] = key;
    }
}

void selectionSort(int arr[], int n) {
    for(int i = 0; i < n-1; i++) {
        int min = i;
        for(int j = i+1; j < n; j++) {
            comparisons++;
            if(arr[j] < arr[min])
                min = j;
        }
        int temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    }
}

void bubbleSort(int arr[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            comparisons++;
            if(arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

/* ------------------ MERGE SORT ------------------ */

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for(int i = 0; i < n1; i++) L[i] = arr[l + i];
    for(int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while(i < n1 && j < n2) {
        comparisons++;
        if(L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while(i < n1) arr[k++] = L[i++];
    while(j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if(l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

/* ------------------ QUICK SORT ------------------ */

int partition(int arr[], int low, int high, int pivotType) {

    if(pivotType == 2) {
        int randIndex = low + rand() % (high - low + 1);
        int temp = arr[low];
        arr[low] = arr[randIndex];
        arr[randIndex] = temp;
    }

    if(pivotType == 3) {
        int mid = (low + high) / 2;
        int a = arr[low], b = arr[mid], c = arr[high];
        int medianIndex;

        if((a <= b && b <= c) || (c <= b && b <= a))
            medianIndex = mid;
        else if((b <= a && a <= c) || (c <= a && a <= b))
            medianIndex = low;
        else
            medianIndex = high;

        int temp = arr[low];
        arr[low] = arr[medianIndex];
        arr[medianIndex] = temp;
    }

    int pivot = arr[low];
    int i = low + 1;
    int j = high;

    while(1) {
        while(i <= high) {
            comparisons++;
            if(arr[i] > pivot) break;
            i++;
        }
        while(j >= low+1) {
            comparisons++;
            if(arr[j] <= pivot) break;
            j--;
        }
        if(i >= j) break;

        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    int temp = arr[low];
    arr[low] = arr[j];
    arr[j] = temp;

    return j;
}

void quickSort(int arr[], int low, int high, int pivotType) {
    if(low < high) {
        int pi = partition(arr, low, high, pivotType);
        quickSort(arr, low, pi-1, pivotType);
        quickSort(arr, pi+1, high, pivotType);
    }
}

/* ------------------ HEAP SORT ------------------ */

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if(left < n) {
        comparisons++;
        if(arr[left] > arr[largest])
            largest = left;
    }
    if(right < n) {
        comparisons++;
        if(arr[right] > arr[largest])
            largest = right;
    }
    if(largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for(int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for(int i = n-1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

/* ------------------ RADIX SORT ------------------ */

int getMax(int arr[], int n) {
    int max = arr[0];
    for(int i = 1; i < n; i++)
        if(arr[i] > max)
            max = arr[i];
    return max;
}

void countSort(int arr[], int n, int exp) {
    int *output = malloc(n * sizeof(int));
    int count[10] = {0};

    for(int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for(int i = 1; i < 10; i++)
        count[i] += count[i-1];

    for(int i = n-1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for(int i = 0; i < n; i++)
        arr[i] = output[i];

    free(output);
}

void radixSort(int arr[], int n) {
    int max = getMax(arr, n);
    for(int exp = 1; max/exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

/* ------------------ GENERIC RUN FUNCTION ------------------ */

void runSort(FILE *file, char *name, void (*sortFunc)(int[],int),
             int arr[], int original[], int n, char *inputType) {

    memcpy(arr, original, n*sizeof(int));
    comparisons = 0;

    clock_t start = clock();
    sortFunc(arr, n);
    clock_t end = clock();

    double micro = ((double)(end-start)/CLOCKS_PER_SEC)*1000000;

    fprintf(file,"%s,%d,%s,%lf,%lld\n",
            name,n,inputType,micro,comparisons);
}

/* ------------------ MAIN ------------------ */

int main() {

    srand(time(NULL));

    int sizes[] = {1000,2000,5000,10000,20000};
    int num_sizes = 5;
    char *inputTypes[] = {"Random","Sorted","Reverse"};

    /* -------- RUN EXPERIMENT 5 TIMES -------- */

    for(int run = 1; run <= 5; run++) {

        char filename[50];
        sprintf(filename, "results_run%d.csv", run);

        FILE *file = fopen(filename,"w");
        fprintf(file,"Algorithm,n,InputType,TimeMicroseconds,Comparisons\n");

        for(int s=0;s<num_sizes;s++){

            int n = sizes[s];
            int *original = malloc(n*sizeof(int));
            int *arr = malloc(n*sizeof(int));

            for(int t=0;t<3;t++){

                if(t==0) generateRandom(original,n);
                if(t==1) generateSorted(original,n);
                if(t==2) generateReverse(original,n);

                runSort(file,"Insertion",insertionSort,arr,original,n,inputTypes[t]);
                runSort(file,"Selection",selectionSort,arr,original,n,inputTypes[t]);
                runSort(file,"Bubble",bubbleSort,arr,original,n,inputTypes[t]);

                /* Merge */
                memcpy(arr, original, n*sizeof(int));
                comparisons = 0;
                clock_t start = clock();
                mergeSort(arr,0,n-1);
                clock_t end = clock();
                fprintf(file,"Merge,%d,%s,%lf,%lld\n",
                        n,inputTypes[t],
                        ((double)(end-start)/CLOCKS_PER_SEC)*1000000,
                        comparisons);

                /* Quick (3 pivot types) */
                for(int p=1;p<=3;p++){
                    memcpy(arr, original, n*sizeof(int));
                    comparisons = 0;
                    start = clock();
                    quickSort(arr,0,n-1,p);
                    end = clock();

                    char name[30];
                    if(p==1) strcpy(name,"Quick_First");
                    if(p==2) strcpy(name,"Quick_Random");
                    if(p==3) strcpy(name,"Quick_Median");

                    fprintf(file,"%s,%d,%s,%lf,%lld\n",
                            name,n,inputTypes[t],
                            ((double)(end-start)/CLOCKS_PER_SEC)*1000000,
                            comparisons);
                }

                runSort(file,"Heap",heapSort,arr,original,n,inputTypes[t]);
                runSort(file,"Radix",radixSort,arr,original,n,inputTypes[t]);
            }

            free(original);
            free(arr);
        }

        fclose(file);
        printf("Run %d complete → %s generated\n", run, filename);
    }

    printf("\nAll 5 runs completed successfully.\n");

    return 0;
}