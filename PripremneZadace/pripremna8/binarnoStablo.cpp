#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <iomanip>
#include <chrono>

using namespace std; //zato sto volim using namespace std zelim da svaka fija bude is biblioteke std :D!!!

template <typename T1, typename T2>
class Mapa{
    public:
    Mapa(){}                                        //
    virtual T2 &operator[](const T1 &el)=0;                //
    virtual T2 operator[](const T1 &el) const=0;     //
    virtual int brojElemenata()const=0;             //
    virtual void obrisi()=0;                        //
    virtual void obrisi(const T1& kljuc)=0;         //
    virtual ~Mapa() {}                              //
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

    T2 &operator[](const T1 &el){
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

    T2 operator[](const T1 &el) const{
        for(int i=0;i<br_elemenata;i++)
        if(p[i]->first==el)return p[i]->second; 

    return T2();
    }
};

template <typename sppk, typename sppv>
class BinStabloMapa : public Mapa<sppk, sppv> {

  struct Cvor {

    sppk kljuc;
    sppv vrijednost;
    Cvor *l_strana, *d_strana, *roditelj;

    Cvor(sppk i = sppk(), sppv u = sppv())
        : kljuc(i), vrijednost(u), d_strana(nullptr), l_strana(nullptr),
          roditelj(nullptr) {}

  };      

    Cvor *novi(sppk i = sppk(), sppv u = sppv()) {
      Cvor *temp = new Cvor();
      temp->kljuc = i;
      temp->vrijednost = u;
      temp->d_strana = temp->l_strana = temp->roditelj = nullptr;
      return temp;
    }

    Cvor *pretraga(Cvor *p, sppk i) const {
      if (p == nullptr || p->kljuc == i)
        return p;
      if (p->kljuc < i)
        return pretraga(p->d_strana, i);
      else if (p->kljuc > i)
        return pretraga(p->l_strana, i);
      return p;
    }

    Cvor *dodaj(Cvor *p, sppk i, sppv u = sppv(), Cvor *rod = nullptr) {
      if (p == nullptr) {
        p = novi(i, u);
        p->roditelj = rod;
        return p;
      }
      if (i > p->kljuc)
        p->d_strana = dodaj(p->d_strana, i, u, p);
      else
        p->l_strana = dodaj(p->l_strana, i, u, p);
      return p;
    }

    Cvor *nasljednik(Cvor *t) {
      auto ovaj = t;
      while (ovaj && ovaj->l_strana != nullptr)
        ovaj = ovaj->l_strana;
      return ovaj;
    }

    void konstruktor(Cvor *a, Cvor *b) {

      if (b == nullptr)
        return;

      konstruktor(a, b->l_strana);
      konstruktor(a, b->d_strana);
      dodaj(a, b->kljuc, b->vrijednost);
    }

    Cvor *obrisiC(Cvor *t, sppk i) {

      if (t == nullptr)
        return t;

      if (i < t->kljuc)
        t->l_strana = obrisiC(t->l_strana, i);
      else if (i > t->kljuc)
        t->d_strana = obrisiC(t->d_strana, i);
      else {

        if (t->l_strana == nullptr) {
          Cvor *temp = t->d_strana;
          delete t;
          return temp;
        } else if (t->d_strana == nullptr) {
          Cvor *temp = t->l_strana;
          delete t;
          return temp;
        }

        auto q = nasljednik(t->d_strana);
        t->kljuc = q->kljuc;
        t->vrijednost=q->vrijednost;
        t->d_strana = obrisiC(t->d_strana, q->kljuc);
      }

      return t;
    }

    void obrisi_stablo(Cvor *t) {

      if (t == nullptr)
        return;

      obrisi_stablo(t->l_strana);
      obrisi_stablo(t->d_strana);
      delete t;
    }
    

  int br_ele;
  Cvor *p;

public:
  BinStabloMapa<sppk, sppv>() : br_ele(0), p(nullptr) {}

  ~BinStabloMapa<sppk, sppv>() { obrisi_stablo(p); }

  BinStabloMapa<sppk, sppv>(const BinStabloMapa<sppk, sppv> &a) {
    br_ele = a.br_ele;
    if (a.br_ele == 0)
      p = nullptr;

    else {
      p = novi(a.p->kljuc, a.p->vrijednost);
      konstruktor(p, a.p);
    }
  }

  BinStabloMapa<sppk, sppv> &operator=(const BinStabloMapa<sppk, sppv> &a) {

    if (this == &a)
      return *this;

    obrisi();
    br_ele = a.br_ele;
    if (a.br_ele == 0)
      p = nullptr;

    else {
      p->kljuc=a.p->kljuc; p->vrijednost=a.p->vrijednost;
      konstruktor(p, a.p);
    }
    return *this;
  }

  int brojElemenata() const { return br_ele; }

  void obrisi() {
    obrisi_stablo(p);
    p = novi();
    br_ele = 0;
  }

  void obrisi(const sppk &kljuc) {

    if (p->kljuc == kljuc)
      p = obrisiC(p, kljuc);
    else
      obrisiC(p, kljuc);

      br_ele--;
  }

  sppv &operator[](const sppk &i) {

    if (p == nullptr) {
      p = novi(i);
      br_ele++;
      return p->vrijednost;
    }

    auto c = pretraga(p, i);

    if (c == nullptr) {
      dodaj(p, i);
      br_ele++;
      c = pretraga(p, i);
    }

    return c->vrijednost;
  }

    sppv operator[](const sppk &i) const {
    auto c = pretraga(p, i);
    if (c != nullptr)
      return c->vrijednost;
    return sppv();
  }
};




int main() {

BinStabloMapa<int, double > t;
NizMapa<int, double >m;

auto start=std::chrono::high_resolution_clock::now();

for(int i=0;i<20000;i++)
m[std::rand()]=1./std::rand();

auto end=std::chrono::high_resolution_clock::now();

auto time=std::chrono::duration_cast<std::chrono::microseconds>(end-start);


std::cout<<"NizMapa ubacivanje: "<<time.count()<<"ms "<<std::endl;

start=std::chrono::high_resolution_clock::now();

for(int i=0;i<20000;i++)
t[std::rand()]=1./std::rand();

 end=std::chrono::high_resolution_clock::now();

time=std::chrono::duration_cast<std::chrono::microseconds>(end-start);


std::cout<<"BinStablo ubacivanje: "<<time.count()<<"ms "<<std::endl;

start=std::chrono::high_resolution_clock::now();
m[std::rand()]=std::rand();;
 end=std::chrono::high_resolution_clock::now();

  time=std::chrono::duration_cast<std::chrono::microseconds>(end-start);

 std::cout<<"NizMapa pristup: "<<time.count()<<"ms "<<std::endl;

 start=std::chrono::high_resolution_clock::now();
t[std::rand()]=std::rand();
 end=std::chrono::high_resolution_clock::now();

  time=std::chrono::duration_cast<std::chrono::microseconds>(end-start);

  std::cout<<"BinStablo pristup: "<<time.count()<<"ms "<<std::endl;

/*
NizMapa ubacivanje: 577340ms 
BinStablo ubacivanje: 16264ms 
NizMapa pristup: 64ms 
BinStablo pristup: 2ms 

Kod mene ispis izgleda ovako, mada to sve moze da varira.
Mozemo ipak vidjeti da je binStablo mnogo mnogo brze od NizMape 
po pitanju vremenske kompleksnosti, makar sto se tice dodavanja i pristupa
elementima.

*/
  return 0;
}