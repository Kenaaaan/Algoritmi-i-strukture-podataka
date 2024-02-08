#include <iostream>
#include <cmath>
#include <list>

using namespace std;

template <typename T>
class DvostraniRed {
    struct Cvor{
        T element;
        Cvor* prethodni=nullptr;
        Cvor* sljedeci=nullptr;
    };
    Cvor* celoo;
    Cvor* vrhh;
    int br_elemenata;
public:
    
DvostraniRed():celoo(nullptr),vrhh(nullptr),br_elemenata(0){}

~DvostraniRed(){
    while(celoo!=nullptr)
    {
        auto pomocni=celoo;
        celoo=celoo->sljedeci;
        delete pomocni;
    }
    br_elemenata=0;
}

DvostraniRed(const DvostraniRed &d){
    br_elemenata=0;
    celoo=nullptr;
    vrhh=nullptr;
    if(d.br_elemenata==0)
        celoo=vrhh=nullptr;
    else {
        Cvor *temp = d.celoo;

      while (temp != nullptr) {
        staviNaVrh(temp->element);
        temp=temp->sljedeci;
      } 
    }
}

DvostraniRed &operator=(DvostraniRed d){
    swap(d.br_elemenata,br_elemenata);
    swap(d.celoo,celoo);
    swap(d.vrhh,vrhh);
    return *this;
}

void brisi(){
    while(celoo!=nullptr)
    {
        auto pomocni=celoo;
        celoo=celoo->sljedeci;
        delete pomocni;
    }
    br_elemenata=0;
}

int brojElemenata(){return br_elemenata;}

void staviNaVrh(const T &el){
    Cvor *novi = new Cvor;
    novi->element=el;
    if(br_elemenata==0)
        vrhh=celoo=novi;
    else {
        vrhh->sljedeci=novi;
        novi->prethodni=vrhh;
        vrhh=novi;
    }
    br_elemenata++;
}

T skiniSaVrha(){
    if(br_elemenata==0)
        throw domain_error("greska");
    T rez = vrhh->element;
    if(br_elemenata==1)
        celoo=nullptr;
    auto pomocni = vrhh;
    vrhh=vrhh->prethodni;
    delete pomocni;
    br_elemenata--;

    return rez;
}

void staviNaCelo(const T& el){
    Cvor *novi = new Cvor;
    novi->element=el;
    if(br_elemenata==0)
        vrhh=celoo=novi;
    else {
        novi->sljedeci=celoo;
        celoo->prethodni=novi;
        celoo=novi;
    }
    br_elemenata++;
}

T skiniSaCela(){
    if(br_elemenata==0)
        throw domain_error("greska");
    T rez = celoo->element;
    if(br_elemenata==1)
        vrhh=nullptr;
    auto pomocni = celoo;
    celoo=celoo->sljedeci;
    delete pomocni;
    br_elemenata--;

    return rez;
}

T &celo(){
    if(br_elemenata==0)
        throw domain_error("greska");
    return celoo->element;
}

T &vrh(){
    if(br_elemenata==0)
        throw domain_error("greska");
    return vrhh->element;
}

};




int main() {
    // Kreirajte objekt tipa DvostraniRed sa cijelim brojevima
    DvostraniRed<int> dr;

    // Dodajte elemente na kraj reda
    for (int i = 1; i <= 5; i++)
        dr.staviNaVrh(i);

    // Dodajte elemente na početak reda
    for (int i = 1; i <= 5; i++)
        dr.staviNaCelo(i);

    // Ispisujte broj elemenata u redu
    std::cout << "Broj elemenata u redu: " << dr.brojElemenata() << std::endl;

    // Pritisnite i ispišite prvi i zadnji element
    std::cout << "Prvi element: " << dr.celo() << std::endl;
    std::cout << "Zadnji element: " << dr.vrh() << std::endl;

    // Uklonite elemente s kraja i početka reda
    std::cout << "Uklonjeno s kraja: " << dr.skiniSaVrha() << std::endl;
    std::cout << "Uklonjeno s početka: " << dr.skiniSaCela() << std::endl;

    // Ispišite preostale elemente u redu
    std::cout << "Preostali elementi u redu: ";
    while (dr.brojElemenata() > 0) {
        std::cout << dr.skiniSaVrha() << " ";
    }
    std::cout << std::endl;

    return 0;
}
