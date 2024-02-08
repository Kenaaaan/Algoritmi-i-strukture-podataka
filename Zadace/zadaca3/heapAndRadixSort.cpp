#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

//Z1

int getMax(vector<int> array) {
  int max = array[0];
  for (int i = 1; i < array.size(); i++)
    if (array[i] > max)
      max = array[i];
  return max;
}

void countSort(std::vector<int> &v, int exp)
{
    std::vector<int> output=v;
    int n=v.size();
    int i = 0, count[10] = { 0 };
  
    
    for (i = 0; i < n; i++)
        count[(v[i] / exp) % 10]++;
  

    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];
  
    for (i = n - 1; i >= 0; i--) {
        output[count[(v[i] / exp) % 10] - 1] = v[i];
        count[(v[i] / exp) % 10]--;
    }
  

    for (i = 0; i < n; i++)
        v[i] = output[i];
}
  
void radixSort(vector<int>&a)
{

    int m = getMax(a);
  
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(a,exp);

}

//Z2

/*Zadatak 2.
Implementirati funkciju
void stvoriGomilu(vector<int> &a)
koja od primljenog vektora a formira gomilu.
Također je potrebno implementirati funkcije:
void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina)
koja u primljeni vektor a dodaje umetnuti element u gomilu koja je velicine velicina i formirana je u
vektoru a.
int izbaciPrvi(vector<int> &a, int &velicina)
koja izbacuje element iz korijena gomile koja je velicine velicina i formirana je u vektoru a.
void gomilaSort(vector<int> &a)
koja će korištenjem gomile sortirati primljeni vektor a.
Napomena: potrebno je uključiti
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;*/

void popraviDolje(std::vector<int> &a, int i, int velicina) {
    while(!(i>=(velicina/2) && i<velicina)) { 
    int veci=2*i+1;
    int dd=2*i+2;
    if(dd<velicina && a[dd]>a[veci]) veci=dd;
    if(a[i]>a[veci]) return;
    int tmp=a[i];
    a[i]=a[veci];
    a[veci]=tmp;
    i=veci;
}
}

void stvoriGomilu(std::vector<int> &a) {
    int velicina=a.size();
    for(int i=(velicina/2); i>=0;i--) popraviDolje(a,i,a.size());
}

void popraviGore(std::vector<int> &a, int i) {
    while(i!=0 && a[i]>a[(i-1)/2]) {
        int tmp=a[i];
        a[i] = a[(i-1)/2];
        a[(i-1)/2]=tmp;
        i=(i-1)/2;
    }
}

void umetniUGomilu(std::vector<int> &a,int umetnuti, int&velicina) {
    if(velicina>a.size()) throw "Gomila je puna";
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a,velicina-1);
}

int izbaciPrvi(std::vector<int> &a, int &velicina) {
    if(a.size()==0) throw "Gomila je prazna";
    velicina--;
    int tmp=a[0];
    a[0]=a[velicina];
    a[velicina]=tmp;
    if(velicina!=0) popraviDolje(a,0,velicina);
    return a[velicina];
}

void gomilaSort(std::vector<int> &a) {
    stvoriGomilu(a);
    int velicina=a.size();
    for(int i=a.size()-1; i>0;i--) {
        int tmp=a[0];
        a[0]=a[i];
        a[i]=tmp;
        velicina--;
        popraviDolje(a,0,velicina);
    }
}

int main(){
vector<int> a;
    vector<int> b;
    a.push_back(545);
    a.push_back(1545);
    a.push_back(4536);
    a.push_back(124121);
    a.push_back(1);
    a.push_back(95468);
    for(int i=0;i<10000;i+=13){
        a.push_back(i);
    }
    for(int i=0;i<a.size();i++){
        b.push_back(a[i]);
    }
    radixSort(a);
    sort(b.begin(),b.end());
    bool testOk = true;
    for(int i(0);i<a.size();i++){
        if(a[i]!=b[i]) testOk = false;;
    }
    if(testOk) cout<<"OK";
    return 0;
}