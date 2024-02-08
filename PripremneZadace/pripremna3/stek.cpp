#include <iostream>
#include <cmath>

using namespace std;

template <typename T>
class  Stek{
struct Cvor{
    T element;
    Cvor* sljedeci;
};
Cvor *vrhcina=nullptr;
int br_elemenata;

public:

Stek():vrhcina(nullptr),br_elemenata(0){}

~Stek(){
    while(vrhcina!=nullptr){
        auto pomocni=vrhcina;
        vrhcina=vrhcina->sljedeci;
        delete pomocni;
    }
    br_elemenata=0;
}
void brisi(){
    while(vrhcina!=nullptr){
        auto pomocni=vrhcina;
        vrhcina=vrhcina->sljedeci;
        delete pomocni;
    }
    br_elemenata=0;
}
void stavi(const T &a){
    Cvor *novi= new Cvor;
    novi->element=a;
    if(vrhcina==nullptr)
    {
        vrhcina=novi;
        novi->sljedeci=nullptr;

    }else
    {
        novi->sljedeci=vrhcina;
        vrhcina=novi;
    }
    br_elemenata++;
}

int brojElemenata() const { return br_elemenata; }

Stek<T>(const Stek<T> &s){
    br_elemenata=0;
    if(s.br_elemenata==0)
    {
        vrhcina=nullptr;
        
    }else
    {
        auto pomocni=s.vrhcina;
        while(pomocni!=nullptr){
            stavi(pomocni->element);
            pomocni=pomocni->sljedeci;
        }
        pomocni=s.vrhcina; Cvor *tekuci=vrhcina;
        while (pomocni != nullptr) {
        tekuci->element=pomocni->element;
        pomocni=pomocni->sljedeci;
        tekuci=tekuci->sljedeci;
    } 
    br_elemenata=s.br_elemenata;
    }
}
/*
Stek<T> &operator=(const Stek<T> &s){
    auto pomocni=vrhcina;
    
    if(this!=&s){
    br_elemenata=0;
    if(s.br_elemenata==0)
    {
        vrhcina=nullptr;
    }
    else
    {
        auto pomocni=s.vrhcina;
        while(pomocni!=nullptr){
            stavi(pomocni->element);
            pomocni=pomocni->sljedeci;
        }
        pomocni=s.vrhh; Cvor *tekuci(vrhcina);
        while (pomocni != nullptr) {
        tekuci->a=pomocni->a; 
        pomocni=pomocni->sljedeci;
        tekuci=tekuci->sljedeci;
    } 
    br_elemenata=s.br_elemenata;
    }
    }
    return *this;
}
*/
Stek<T> &operator=(Stek<T> s){
    swap(vrhcina,s.vrhcina);
    swap(br_elemenata,s.br_elemenata);
    return *this;
}

T skini(){
    if(vrhcina==nullptr)
        throw logic_error("Greska stek je prazan");
    T rez=vrhcina->element;
    auto pomocni=vrhcina;
    vrhcina=vrhcina->sljedeci;
    delete pomocni;
    br_elemenata--;
    return rez;
}

T &vrh(){
    if(vrhcina==nullptr)
        throw logic_error("greska");
    return vrhcina->element;
}
};


int main(){
    Stek<int> intStek;

    intStek.stavi(10);
    intStek.stavi(20);
    intStek.stavi(30);

    std::cout << "Broj elemenata u intSteku: " << intStek.brojElemenata() << std::endl;

    std::cout << "Vrh intSteka: " << intStek.vrh() << std::endl;

    std::cout << "Uklonjen element: " << intStek.skini() << std::endl;

    std::cout << "Broj elemenata u intSteku nakon skidanja: " << intStek.brojElemenata() << std::endl;

    Stek<int> intStekKopija(intStek);

    Stek<int> intStekKopija2;
    intStekKopija2 = intStek;

    Stek<int> intStekZamjena;
    intStekZamjena.stavi(5);
    intStekZamjena.stavi(15);
    intStekZamjena.stavi(25);

    std::cout << "Prije zamjene - vrh intStekZamjena: " << intStekZamjena.vrh() << std::endl;

    intStekZamjena = intStek;
    std::cout << "Nakon zamjene - vrh intStekZamjena: " << intStekZamjena.vrh() << std::endl;
return 0;



}