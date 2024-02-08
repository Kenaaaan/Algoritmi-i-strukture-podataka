#include <bits/types/clock_t.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream> 
#include <stdexcept>
#include <vector>
#include <math.h>
#include <string>

using namespace std;

template <typename kljuc, typename vrijednost>
class Mapa{
  public:
    Mapa(){};
    virtual ~Mapa(){};
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const kljuc &k) = 0;
    virtual vrijednost& operator[](const kljuc &k) = 0;
    virtual vrijednost operator[](const kljuc &k) const = 0;
};

template <typename kljuc, typename vrijednost>
class NizMapa : public Mapa<kljuc,vrijednost>{
    pair<kljuc, vrijednost>** mapa;
    int br_elemenata;
    int kapacitet;
    void DaLiJePun(){
        if(br_elemenata==kapacitet){
            kapacitet*=2;
            pair<kljuc, vrijednost>** pom = new pair<kljuc,vrijednost>*[kapacitet]{};
            for(int i=0; i < br_elemenata; i++){
                pom[i] = mapa[i];
                mapa[i] = nullptr;
            }
            delete[] mapa;
            mapa = pom;
        }
    }
  public:
    NizMapa() : br_elemenata(0), kapacitet(100){
        mapa = new pair<kljuc,vrijednost>*[kapacitet]{};
    }
    NizMapa(const NizMapa &m) : br_elemenata(m.br_elemenata), kapacitet(m.kapacitet){
        mapa = new pair<kljuc,vrijednost>*[kapacitet]{};
        for(int i = 0; i < br_elemenata; i++){
            mapa[i] = new pair<kljuc, vrijednost>(*m.mapa[i]);
        }
    }
    ~NizMapa(){
        for(int i=0;i<br_elemenata;i++){
            delete mapa[i];
        }
        delete[] mapa;
    }
    NizMapa& operator=(const NizMapa& m){
        if(this == &m) return *this;
        for(int i=0;i<br_elemenata;i++){
            delete mapa[i];
        }
        delete[] mapa;
        br_elemenata = m.br_elemenata;
        kapacitet = m.kapacitet;
        mapa = new pair<kljuc,vrijednost>*[kapacitet]{};
        for(int i = 0; i < br_elemenata; i++){
            mapa[i] = new pair<kljuc, vrijednost>(*m.mapa[i]);
        }
        return *this;
    }
    NizMapa& operator=(NizMapa &&m){
        swap(br_elemenata,m.br_elemenata);
        swap(kapacitet,m.kapacitet);
        swap(mapa,m.mapa);
        return *this;
    }
    int brojElemenata() const override {return br_elemenata;}
    void obrisi() override {
        for(int i=0;i<br_elemenata;i++){
            delete mapa[i];
        }
        br_elemenata = 0;
    }
    void obrisi(const kljuc &k) override {
        int i;
        for(i = 0; i < br_elemenata; i++){
            if(mapa[i]->first==k){
                delete mapa[i];
                br_elemenata--;
                for(;i<br_elemenata;i++){
                    mapa[i]=mapa[i+1];
                }
                mapa[i]=nullptr;
            }
        }
        if(i==br_elemenata) throw domain_error("Ne postoji kljuc");
    }
    vrijednost& operator[](const kljuc &k) override {
        for(int i = 0; i < br_elemenata; i++){
            if(mapa[i]->first==k){
                return mapa[i]->second;
            }
        }
        DaLiJePun();
        mapa[br_elemenata++] = new pair<kljuc, vrijednost>{k,vrijednost{}};
        return mapa[br_elemenata-1]->second;
    }
    vrijednost operator[](const kljuc &k) const override {
        for(int i = 0; i < br_elemenata; i++){
            if(mapa[i]->first==k){
                return mapa[i]->second;
            }
        }
        return vrijednost{};
    }

};

template <typename kljucTip, typename vrijednostTip>
struct Cvor{
    kljucTip kljuc;
    vrijednostTip vrijednost;
    Cvor* lijevo;
    Cvor* desno;
    Cvor* roditelj;
};
template <typename kljucTip, typename vrijednostTip>
void obrisiCvor(Cvor<kljucTip,vrijednostTip> *c){
    if(!c) return;
    if(c->lijevo) obrisiCvor(c->lijevo);
    if(c->desno) obrisiCvor(c->desno);
    delete c;
}
template <typename kljucTip, typename vrijednostTip>
void kopirajCvorove(Cvor<kljucTip,vrijednostTip> *c, Cvor<kljucTip,vrijednostTip> *k){
    c->kljuc=k->kljuc;
    c->vrijednost = k->vrijednost;
    if(k->desno){
        c->desno=new Cvor<kljucTip,vrijednostTip>{};
        kopirajCvorove(c->desno,k->desno);
        c->desno->roditelj = c;
    }
    else c->desno = nullptr;
    if(k->lijevo){
        c->lijevo=new Cvor<kljucTip,vrijednostTip>{};
        kopirajCvorove(c->lijevo,k->lijevo);
        c->lijevo->roditelj = c;
    }
    else c->lijevo = nullptr;
}

template <typename kljucTip, typename vrijednostTip>
class BinStabloMapa : public Mapa<kljucTip,vrijednostTip>{
    int brojel; 
    Cvor<kljucTip,vrijednostTip>* korijen;
  public:
    BinStabloMapa():brojel(0), korijen(nullptr){};
    BinStabloMapa(const BinStabloMapa &m){
        brojel = m.brojel;
        korijen = new Cvor<kljucTip, vrijednostTip>{};
        kopirajCvorove(korijen,m.korijen);
        korijen->roditelj=nullptr;
    }
    ~BinStabloMapa() override {
        obrisiCvor(korijen);
    }
    BinStabloMapa operator=(BinStabloMapa k){
        swap(korijen,k.korijen);
        swap(brojel,k.brojel);
        return *this;
    }
    int brojElemenata() const override {return brojel;}
    void obrisi() override {
        obrisiCvor(korijen);
        brojel = 0;
        korijen = nullptr;
    }
    void obrisi(const kljucTip &k) override {
        Cvor<kljucTip, vrijednostTip> *pom = korijen;
        while(k != pom->kljuc){
            if(k>pom->kljuc) pom = pom->desno;
            if(k<pom->kljuc) pom = pom->lijevo;
        }
        if(pom->desno && pom->lijevo){
            Cvor<kljucTip, vrijednostTip>* zadnji=pom->lijevo;
            while(zadnji->desno) zadnji = zadnji->desno;
            if(zadnji==pom->lijevo){
                zadnji->desno=pom->desno;
                zadnji->roditelj=pom->roditelj;

            }
            else{
                zadnji->roditelj->desno = zadnji->lijevo;
                zadnji->desno=pom->desno;
                zadnji->roditelj=pom->roditelj;
                zadnji->lijevo = pom->lijevo;
            }
            pom->desno->roditelj=zadnji;
            if(pom->roditelj){
                pom=pom->roditelj;
                if(pom->kljuc<k){
                    delete pom->desno;
                    pom->desno = zadnji;
                }
                else{
                    delete pom->lijevo;
                    pom->lijevo = zadnji;
                }
            }
            else{
                korijen = zadnji;
                delete pom;
            }
        }
        else if(pom->desno){
            Cvor< kljucTip, vrijednostTip> *o = pom->desno;
            o->roditelj = pom->roditelj;
            if(pom->roditelj){
                pom = pom->roditelj;
                if(pom->kljuc<k){
                    delete pom->desno;
                    pom->desno = o;
                }
                else{
                    delete pom->lijevo;
                    pom->lijevo = o;
                }
            }
            else{
                korijen = o;
                delete pom;
            }
        }
        else if(pom->lijevo){
            Cvor< kljucTip, vrijednostTip> *o = pom->lijevo;
            o->roditelj = pom->roditelj;
            if(pom->roditelj){
                pom = pom->roditelj;
                if(pom->kljuc<k){
                    delete pom->desno;
                    pom->desno = o;
                }
                else{
                    delete pom->lijevo;
                    pom->lijevo = o;
                }
            }
            else{
                korijen = o;
                delete pom;
            }
        }
        else{
            if(pom->roditelj){
                pom = pom->roditelj;
                if(pom->kljuc<k){
                    delete pom->desno;
                    pom->desno = nullptr;
                }
                else{
                    delete pom->lijevo;
                    pom->lijevo = nullptr;
                }
            }
            else{
                delete pom;
                korijen = nullptr;
            }
        }
        brojel--;
    }
    vrijednostTip& operator[](const kljucTip &k) override {
        if(brojel == 0){
            korijen=new Cvor<kljucTip, vrijednostTip>{k,vrijednostTip{},nullptr,nullptr,nullptr};
            brojel++;
        }
        Cvor<kljucTip, vrijednostTip> *pom = korijen;
        while(k != pom->kljuc && (pom->kljuc<k && pom->desno || pom->kljuc>k && pom->lijevo)){
            if(k>pom->kljuc) pom = pom->desno;
            else if(k<pom->kljuc) pom = pom->lijevo;
        }
        if(k == pom->kljuc) return pom->vrijednost;
        else if(k > pom->kljuc){
            pom->desno = new Cvor<kljucTip, vrijednostTip>{k,vrijednostTip{},nullptr,nullptr,pom};
            pom = pom->desno;
        }
        else {
            pom->lijevo = new Cvor<kljucTip, vrijednostTip>{k,vrijednostTip{},nullptr,nullptr,pom};
            pom = pom->lijevo;
        }
        brojel++;
        return pom->vrijednost;

    }
    vrijednostTip operator[](const kljucTip &k) const override {
        if(brojel == 0){
            return vrijednostTip{};
        }
        Cvor<kljucTip, vrijednostTip> *pom = korijen;
        while(k != pom->kljuc && (pom->kljuc<k && pom->desno || pom->kljuc>k && pom->lijevo)){
            if(k>pom->kljuc) pom = pom->desno;
            if(k<pom->kljuc) pom = pom->lijevo;
        }
        if(k == pom->kljuc) return pom->vrijednost;
        return vrijednostTip{};
    }
};

template <typename kljuc, typename vrijednost>
void brisanje(pair<kljuc, vrijednost>** mapa, int kapacitet){
    for(int i = 0; i < kapacitet; i++){
        if(mapa[i] != nullptr) {
            delete mapa[i];
            mapa[i] = nullptr;
        }
    }
}

unsigned int hashFunkcijaSaInterneta(int a, unsigned int kapacitet){
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a ^ (a >> 15);
    return a % kapacitet;
}

template <typename kljuc, typename vrijednost>
class HashMapa : public Mapa<kljuc,vrijednost>{
    pair<kljuc, vrijednost>** mapa;
    int br_elemenata;
    int kapacitet;
    pair<kljuc, vrijednost> del;
    unsigned int (*hashfunkcija)(kljuc, unsigned int);
    void DaLiJePun(){
        if(br_elemenata==kapacitet){
            kapacitet*=2;
            pair<kljuc, vrijednost>** pom = new pair<kljuc,vrijednost>*[kapacitet]{};
            for(int i=0; i < br_elemenata; i++){
                pom[i] = mapa[i];
                mapa[i] = nullptr;
            }
            delete[] mapa;
            mapa = pom;
        }
    }
  public:
    HashMapa() : br_elemenata(0), kapacitet(1000){
        mapa = new pair<kljuc,vrijednost>*[kapacitet]{};
        hashfunkcija = nullptr;
    }
    HashMapa(const HashMapa &m) : br_elemenata(m.br_elemenata), kapacitet(m.kapacitet){
        mapa = new pair<kljuc,vrijednost>*[kapacitet]{};
        for(int i = 0; i < kapacitet; i++){
            if(m.mapa[i] != nullptr)
            mapa[i] = new pair<kljuc, vrijednost>(*m.mapa[i]);
        }
        hashfunkcija = m.hashfunkcija;
    }
    ~HashMapa(){
        brisanje(mapa, kapacitet);
        delete[] mapa;
    }
    HashMapa& operator=(HashMapa m){
        swap(br_elemenata,m.br_elemenata);
        swap(kapacitet,m.kapacitet);
        swap(mapa,m.mapa);
        swap(hashfunkcija,m.hashfunkcija);
        return *this;
    }
    int brojElemenata() const override {return br_elemenata;}
    void obrisi() override {
        brisanje(mapa, kapacitet);
        br_elemenata = 0;
    }
    void obrisi(const kljuc &k) override {
        unsigned int a = hashfunkcija(k,kapacitet);
        int i = a, p = 0, c = (a - 1 + kapacitet) % kapacitet;
        while(mapa[i] != nullptr){
            if(mapa[i] == &del);
            else if(mapa[i]->first==k){
                delete mapa[i];
                br_elemenata--;
                mapa[i]=&del;
                p = 1;
                break;
            }
            if(i == c) break;
            i++;
            i = i % kapacitet;
        }
        if(!p) throw domain_error("Ne postoji kljuc");
    }
    vrijednost& operator[](const kljuc &k) override {
        if(hashfunkcija == nullptr) throw domain_error("Nije definisana Hash funkcija"); 
        unsigned int a = hashfunkcija(k,kapacitet);
        int i = a, p = -1, c = (a - 1 + kapacitet) % kapacitet;
        while(mapa[i] != nullptr){
            if(mapa[i] == &del){
                if(p == -1){
                    p = i;
                }
            }
            else if(mapa[i]->first==k){
                return mapa[i]->second;
            }
            if(i == c) break;
            i++;
            i = i % kapacitet;
        }
        if(p != -1) i = p;
        mapa[i] = new pair<kljuc, vrijednost>{k,vrijednost{}};
        br_elemenata++;
        DaLiJePun();
        return mapa[i]->second;
    }
    vrijednost operator[](const kljuc &k) const override {
        unsigned int a = hashfunkcija(k,kapacitet);
        int i = a, p = -1, c = (a - 1 + kapacitet) % kapacitet;
        while(mapa[i] != nullptr){
            if(mapa[i] == &del);
            else if(mapa[i]->first==k){
                return mapa[i]->second;
            }
            if(i == c) break;
            i++;
            i = i % kapacitet;
        }
        return vrijednost{};
    }
    void definisiHashFunkciju(unsigned int (*pok)(kljuc,unsigned int)){
        hashfunkcija = pok;
    }

};



int main(){


    BinStabloMapa<int,int> binmapa;
    NizMapa<int,int> nizmapa;
    HashMapa<int,int> hashmapa;
    hashmapa.definisiHashFunkciju(hashFunkcijaSaInterneta);
    srand(time(0));
    for(int i = 0; i < 10000; i++){
        int a = rand();
        binmapa[a] = i;
        nizmapa[a] = i;
        hashmapa[a] = i;
    }
    clock_t start, end;
    int velicina = 7000;

    start = clock();
    for(int i = -velicina; i < velicina; i++){
        binmapa[rand()] = i;
    }
    end = clock();
    double t = double(end - start) / double(CLOCKS_PER_SEC);
    double prosjekb = t;


    start = clock();
    for(int i = -velicina; i < velicina; i++){
        nizmapa[rand()] = i;
    }
    end = clock();
    t = double(end - start) / double(CLOCKS_PER_SEC);
    double prosjekn = t;


    start = clock();
    for(int i = -velicina; i < velicina; i++){
        hashmapa[rand()] = i;
    }
    end = clock();
    t = double(end - start) / double(CLOCKS_PER_SEC);
    double prosjekh = t;

    cout<<fixed<<setprecision(8)<<"U binarnu mapu se element ubacuje "<<prosjekb<<" sekundi, dok u nizMapu ubacivanje traje "<<prosjekn<<" sekundi, a u hashmapu ubacivanje u traje "<<prosjekh<<" sekundi"<<endl;
    //binarna mapa ubacuje elemente otprilike 100-200 puta brze od nizmape i hash mape

    for(int i = 0; i < 30; i++){
        binmapa[pow(-2,i)] = i;
        nizmapa[pow(-2,i)] = i;
        hashmapa[pow(-2,i)] = i;
    }
    
    start = clock();
    for(int i = -velicina; i < velicina; i++){
        nizmapa[i];
    }
    end = clock();
    t = double(end - start) / double(CLOCKS_PER_SEC);
    double prosjekn2 = t;

    start = clock();
    for(int i = -velicina; i < velicina; i++){
        binmapa[i];
    }
    end = clock();
    t = double(end - start) / double(CLOCKS_PER_SEC);
    double prosjekb2 = t;

    start = clock();
    for(int i = -velicina; i < velicina; i++){
        hashmapa[i];
    }
    end = clock();
    t = double(end - start) / double(CLOCKS_PER_SEC);
    double prosjekh2 = t;

    cout<<fixed<<setprecision(8)<<"Pristupanje elementu u binarnoj mapi traje  "<<prosjekb2<<" sekundi, dok pristupanje elementu u niz mapi traje "<<prosjekn2<<" sekundi, a pristupanje elementu u hashmapi traje "<<prosjekh2<<" sekundi"<<endl;
    // i pristupanje elementu je puno brze kod binarne mape nego kod niz mape i hashmape, otprilike 100-200 puta
    
    return 0;
}
