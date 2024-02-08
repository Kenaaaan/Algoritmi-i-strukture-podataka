#include <iostream>
#include <cmath>
#include <list>
#include <stdexcept>
using namespace std;
//#define red  std::list<int>;
template <typename T>
class Red{
    struct Cvor{
        T element;
        Cvor* sljedeci=nullptr;
    };
    Cvor* pocetak;
    Cvor* kraj;
    int br_elemenata;
    public:
    Red():br_elemenata(0),pocetak(nullptr),kraj(nullptr){}
    int brojElemenata(){return br_elemenata;}
    ~Red(){
        if(br_elemenata!=0)
        {
            while(pocetak!=nullptr){
                auto pomocni=pocetak;
                pocetak=pocetak->sljedeci;
                delete pomocni;
            }
        }
        br_elemenata=0;
    }
    void stavi(const T &el){
        Cvor* novi=new Cvor;
        novi->element=el;
        if(br_elemenata==0){   
            pocetak=novi;
            kraj=novi;
        }
        else{
            kraj->sljedeci=novi;
            kraj=novi;
        }
        br_elemenata++;
    }
    void brisi(){
        kraj=nullptr;
        while(pocetak!=nullptr){
            auto pomocni=pocetak;
            pocetak=pocetak->sljedeci;
            delete pomocni;
        }
        br_elemenata=0;
        pocetak=kraj=nullptr;
    }
    /*
    Red<T> &operator=(const Red<T> &r){
        if(this!=&r)
            return *this;

        while(pocetak!=kraj){
            auto pomocni=pocetak;
            pocetak=pocetak->sljedeci;
            delete pomocni;
        }
        br_elemenata=0;
        if(r.br_elemenata==0)
            pocetak=kraj=nullptr;

        auto pomocni=r.pocetak;
        while(pomocni!=nullptr){
            stavi(pomocni->element);
            pomocni=pomocni->sljedeci;
        }
    return *this;
    }*/
    Red<T> &operator=(Red<T> s){
    swap(pocetak,s.pocetak);
    swap(kraj,s.kraj);
    swap(br_elemenata,s.br_elemenata);
    return *this;
}
    Red<T>(const Red<T> &r){
        pocetak=kraj=nullptr;
        br_elemenata=0;
        if(r.br_elemenata==0)
            pocetak=kraj=nullptr;

        auto pomocni=r.pocetak;
        while(pomocni!=nullptr){
            stavi(pomocni->element);
            pomocni=pomocni->sljedeci;
        }
    }
    T skini(){
        if(br_elemenata==0)
            throw std::domain_error("greska");

    auto pomocni=pocetak;
    T rez=pocetak->element;
    pocetak=pocetak->sljedeci;
    delete pomocni;
    br_elemenata--;
    return rez;
    }
    T &celo(){
        if(br_elemenata==0)
            throw std::domain_error("greska");

        return pocetak->element;
    }



};

int main() {
   
    // Testiranje reda sa cijelim brojevima
    Red<int> intRed;

    // Dodavanje elemenata u red
    intRed.stavi(10);
    intRed.stavi(20);
    intRed.stavi(30);

    // Ispisivanje broja elemenata u redu
    std::cout << "Broj elemenata u intRedu: " << intRed.brojElemenata() << std::endl;

    // Ispisivanje prvog elementa u redu
    std::cout << "Prvi element u intRedu: " << intRed.celo() << std::endl;

    // Skidanje elementa iz reda
    std::cout << "Skidanje elementa iz intReda: " << intRed.skini() << std::endl;

    // Ispisivanje ažuriranog broja elemenata u redu
    std::cout << "Broj elemenata u intRedu nakon skidanja: " << intRed.brojElemenata() << std::endl;

    // Testiranje kopiranja reda
    Red<int> intRedKopija(intRed);

    // Testiranje zamjene redova
    Red<int> intRedZamjena;
    intRedZamjena.stavi(5);
    intRedZamjena.stavi(15);
    intRedZamjena.stavi(25);

    std::cout << "Prije zamjene - prvi element u intRedZamjena: " << intRedZamjena.celo() << std::endl;

    intRedZamjena = intRed;
    std::cout << "Nakon zamjene - prvi element u intRedZamjena: " << intRedZamjena.celo() << std::endl;

    return 0;
}
