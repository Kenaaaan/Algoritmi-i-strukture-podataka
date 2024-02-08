#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <stdexcept>
using namespace std; //zato sto volim using namespace std zelim da svaka fija bude is biblioteke std :D!!!

template <typename T1, typename T2>
class Mapa{
    public:
    Mapa(){}                                        //
    virtual T2 &operator[](T1 el)=0;                //
    virtual const T2 operator[](T1 el) const=0;     //
    virtual int brojElemenata()const=0;             //
    virtual void obrisi()=0;                        //
    virtual void obrisi(const T1& kljuc)=0;         //
    virtual ~Mapa(){};
};

template <typename T1,typename T2>
class NizMapa : public Mapa<T1,T2>{

    //priv atributi
    int velicina, br_elemenata;
    pair<T1,T2> **p;

    public:
    
    //konstruktori:

    NizMapa():Mapa<T1,T2>(),velicina(100000),br_elemenata(0),p(new pair<T1,T2>*[velicina]()){}

    NizMapa(const NizMapa<T1,T2> &m){
        velicina=m.velicina;
        br_elemenata=m.br_elemenata;
        if(m.br_elemenata==0)
            p=nullptr;
        else{
            p=new pair<T1,T2>*[velicina]();
        }
        for(int i=0;i<m.br_elemenata;i++)
            p[i]=new pair<T1,T2>(*m.p[i]);
    }

    //destruktor

    ~NizMapa(){
        for(int i=0;i<br_elemenata;i++)
            delete p[i];
        delete[] p;
        br_elemenata=0;
        velicina=0;
        p=nullptr;
    }

    //operatori dodjele

    NizMapa &operator=(NizMapa<T1,T2> m){
        swap(velicina,m.velicina);
        swap(br_elemenata,m.br_elemenata);
        swap(p,m.p);
        return *this;
    }

    //metode

    int brojElemenata() const{
        return br_elemenata;
    }

    void obrisi(){
        if(br_elemenata==0)
            throw std::logic_error("Nema elemenata");
        for(int i=0;i<br_elemenata;i++)
            delete p[i];
        br_elemenata=0;
    }

    void obrisi(const T1& kljuc){
        for(int i=0;i<br_elemenata;i++){
            if(p[i]->first!=kljuc)
                continue;
            delete p[i];
            for(int j=i;j<br_elemenata-1;j++)
                p[j]=p[j+1];
            p[br_elemenata]=nullptr;
            br_elemenata--;
            return;
        }
        throw std::logic_error("Nema elemenata");
    }

    T2 &operator[](T1 el){
        if(velicina==br_elemenata){
            pair<T1,T2> **pomocni;    
            pomocni=new pair<T1,T2>*[velicina*2]();
            for(int i=0;i<br_elemenata;i++)
            {
                pomocni[i]= new pair<T1,T2>(*p[i]);
                delete p[i];
            }
            delete[] p;
            velicina=velicina*2;
            p=pomocni;
        }
    for(int i=0;i<br_elemenata;i++)
        if(p[i]->first==el)return p[i]->second; 
    
    p[br_elemenata]=new pair<T1,T2>(el,T2());
    br_elemenata++;
    return p[br_elemenata-1]->second;
    }

    const T2 operator[](T1 el) const{
        for(int i=0;i<br_elemenata;i++)
        if(p[i]->first==el)return p[i]->second; 

    return T2();
    }
};

// Test funkcija za brojElemenata
void testBrojElemenata() {
    NizMapa<int, std::string> mapa;
    std::cout << "Broj elemenata: " << mapa.brojElemenata() << std::endl;

    mapa[1] = "jedan";
    mapa[2] = "dva";
    std::cout << "Broj elemenata nakon dodavanja: " << mapa.brojElemenata() << std::endl;
}

// Test funkcija za operator[]
void testOperator() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[2] = "dva";

    std::cout << "Element sa kljucem 1: " << mapa[1] << std::endl;
    std::cout << "Element sa kljucem 2: " << mapa[2] << std::endl;

    // Provera const verzije operatora[]
    const NizMapa<int, std::string> constMapa = mapa;
    std::cout << "Const element sa kljucem 1: " << constMapa[1] << std::endl;
    std::cout << "Const element sa kljucem 2: " << constMapa[2] << std::endl;
    std::cout << "Const element sa nepostojećim kljucem 3: " << constMapa[3] << std::endl;
}

// Test funkcija za obrisi
void testObrisi() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[2] = "dva";

    std::cout << "Broj elemenata pre brisanja: " << mapa.brojElemenata() << std::endl;
    mapa.obrisi(1);
    std::cout << "Broj elemenata nakon brisanja: " << mapa.brojElemenata() << std::endl;
    std::cout << "Element sa kljucem 1 nakon brisanja: " << mapa[1] << std::endl;

    // Pokušaj brisanja nepostojećeg elementa
    std::cout << "Pokusaj brisanja elementa sa kljucem 3: " << std::endl;
    try {
        mapa.obrisi(3);
    } catch (const std::logic_error& e) {
        std::cout << "Izuzetak uhvaćen: " << e.what() << std::endl;
    }
}

// Test funkcija za obrisi()
void testObrisiSve() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[2] = "dva";

    std::cout << "Broj elemenata pre brisanja: " << mapa.brojElemenata() << std::endl;
    mapa.obrisi();
    std::cout << "Broj elemenata nakon brisanja: " << mapa.brojElemenata() << std::endl;
}

int main() {
    testBrojElemenata();
    testOperator();
    testObrisi();
    testObrisiSve();

    std::cout << "Svi testovi uspesno izvrseni!" << std::endl;

    return 0;
}
