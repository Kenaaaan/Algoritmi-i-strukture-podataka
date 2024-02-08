#include <iostream>
#include <cmath>
using namespace std;

int nzd(int x, int y){
    if(y==0)
        return x;
    return nzd(y,x%y);
}
int fib_petlja (int n) {
    if (n<=1) 
        return n;
    int pretprosli = 0, prosli = 1, rezultat;
    for (int i = 2; i<=n; i++) {
        rezultat = pretprosli+prosli;
        pretprosli = prosli;
        prosli = rezultat;
    }
return rezultat;
}
int fib2_0(int n, int a = 0, int b = 1) {
if (n==0) 
    return a;
else if(n==1)
    return b;

return fib2_0(n-1,b,a+b);
}
int main(){
    return 0;
}