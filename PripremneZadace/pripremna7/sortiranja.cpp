#include <iostream>
#include <cmath>
#include <functional>
#include <algorithm>

using namespace std;

void ispisi(int array[], int size) {
  for (int i = 0; i < size; i++) {
    cout << array[i] << " ";
  }
  cout << endl;
}

template <typename Tip>
void selection_sort(Tip* niz, int vel){
    for(int i=0;i<vel-1;i++){
        int najmanji=i;
        for(int j=i+1;j<vel;j++){
            if(niz[j]<niz[najmanji])
                najmanji=j;
        }
        std::swap(niz[i],niz[najmanji]);
    }
}

template <typename spp>
void bubble_sort(spp* niz, int vel){

 for(int i=0;i<vel;i++)
 for(int j=i;j<vel;j++)
 if(niz[i]>niz[j])
 {
     auto temp=niz[i];
     niz[i]=niz[j];
     niz[j]=temp;
 }}

template <typename T>
void Merge(T*niz,const int l,const int p, const int q, const int u) {
 int i(0), j(q-l), k(l);
 T *B = new T[u-l+1];
 for(int m = 0; m <= u - l; m++)
 B[m] = niz[l+m];
 while(i <= p - l && j <= u - l) {
 if (B[i] < B[j])
 niz[k] = B[i++];
 else
 niz[k] = B[j++];
 k++;
 }
 while (i <= p-l)
 niz[k++] = B[i++];
 while (j <= u-l)
 niz[k++] = B[j++];
 delete [] B;
} 

template <typename T>
void merge_sort(T* niz, int vel){

    static int l=0;
    static int u=vel-1;

     if (u > l) {
 int p = (l + u - 1) / 2;
 int q = p + 1;
 auto z=u;
 auto y=l;
 u=p;
 merge_sort(niz, vel);
 u=z; l=q;
 merge_sort(niz, vel);
 l=y;
 Merge(niz,l, p, q, u);
 } 
 
}

template <typename T>
int Particija(T* niz,const int prvi, const int zadnji) {
 T pivot = niz[prvi];
 int p = prvi + 1;
 while (p <= zadnji) {
 if (niz[p] <= pivot)
 p++;
 else
 break;
 }
 for (int i = p+1; i <= zadnji; i++) {
 if (niz[i] < pivot) {
     auto temp=niz[p];
     niz[p]=niz[i];
     niz[i]=temp;
 p++;
 }
 }
     auto temp=niz[p-1];
     niz[p-1]=niz[prvi];
     niz[prvi]=temp;

 return p-1;
} 


template <typename T>
void quick_sort(T* niz, int vel){

     static int prvi = 0;
     static int zadnji=vel-1; 

     if (prvi < zadnji) {
 int j = Particija(niz,prvi,zadnji);
    auto q=zadnji;
    zadnji=j-1;
 quick_sort(niz,vel); 
 prvi=j+1; zadnji=q;
 quick_sort(niz,vel);
 } 
      
}

int main() {
    const int vel = 8;
    int testArray[vel] = {8, 3, 1, 7, 0, 4, 6, 2};

    // Test Bubble Sort
    cout << "Bubble Sort:\n";
    bubble_sort(testArray, vel);
    ispisi(testArray, vel);
    cout << endl;

    // Reset the array
    int resetArray[vel] = {8, 3, 1, 7, 0, 4, 6, 2};

    // Test Selection Sort
    cout << "Selection Sort:\n";
    selection_sort(resetArray, vel);
    ispisi(resetArray, vel);
    cout << endl;

    // Reset the array
    int resetArray2[vel] = {8, 3, 1, 7, 0, 4, 6, 2};

    // Test Merge Sort
    cout << "Merge Sort:\n";
    merge_sort(resetArray2, vel);
    ispisi(resetArray2, vel);
    cout << endl;

    // Reset the array
    int resetArray3[vel] = {8, 3, 1, 7, 0, 4, 6, 2};

    // Test Quick Sort
    cout << "Quick Sort:\n";
    quick_sort(resetArray3, vel);
    ispisi(resetArray3, vel);
    cout << endl;

    return 0;
}
