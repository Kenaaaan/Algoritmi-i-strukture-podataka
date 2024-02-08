#include <iostream>
#include <queue>

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

template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class Grana;

template <typename TipOznake>
class GranaIterator;

template <typename TipOznake>
class UsmjereniGraf{
  public:
    UsmjereniGraf(){};
    UsmjereniGraf(int n){};
    virtual ~UsmjereniGraf(){};
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int br) = 0;
    virtual void dodajGranu(int start, int end, float tezina = 0) = 0;
    virtual void obrisiGranu(int start, int end) = 0;
    virtual void postaviTezinuGrane(int start, int end, float tezina) = 0;
    virtual float dajTezinuGrane(int start, int end) const = 0;
    virtual bool postojiGrana(int start, int end) const = 0;
    virtual void postaviOznakuCvora(int cvor, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuCvora(int cvor) const = 0;
    virtual void postaviPosjecen(int a, bool t) = 0;
    virtual void postaviOznakuGrane(int start, int end, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuGrane(int start, int end) const = 0;
    virtual Grana<TipOznake> dajGranu(int start, int end) = 0;
    virtual Cvor<TipOznake> dajCvor(int br) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
};

template <typename TipOznake>
class Cvor{
    int broj_cvora;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *Graf;
    bool posjecen;
  public:
    Cvor(){};
    Cvor(UsmjereniGraf<TipOznake> *u, int broj){
        Graf = u;
        broj_cvora = broj; 
    };
    TipOznake dajOznaku() const {return oznaka;}
    void postaviOznaku(TipOznake o){
        oznaka = o;
        Graf->postaviOznakuCvora(broj_cvora, oznaka);
    }
    void postaviPravuOznaku(TipOznake o){oznaka  = o;}
    void postaviPosjecen(bool t){
        posjecen = t;
    }
    bool dajPosjecen(){ return posjecen;}
    int dajRedniBroj() const {return broj_cvora;}
};

template <typename TipOznake>
class Grana{
    Cvor<TipOznake> pocetak;
    Cvor<TipOznake> kraj;
    float tezinaGrane; 
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *Graf;
  public:
    Grana(UsmjereniGraf<TipOznake> *u, int start, int end, float tezina){
        pocetak = u->dajCvor(start);
        kraj = u->dajCvor(end);
        tezinaGrane = tezina;
        Graf = u;
    }
    float dajTezinu() const {return tezinaGrane;}
    void postaviTezinu(float tezina){
        tezinaGrane = tezina;
        Graf->postaviTezinuGrane(pocetak.dajRedniBroj(), kraj.dajRedniBroj(), tezina);
    }
    void postaviPravuTezinu(float tezina){tezinaGrane = tezina;}
    TipOznake dajOznaku() const {return oznaka;}
    void postaviOznaku(TipOznake o){
        oznaka = o;
        Graf->postaviOznakuGrane(pocetak.dajRedniBroj(), kraj.dajRedniBroj(), o);
    }
    void postaviPravuOznaku(TipOznake o){oznaka = o;}
    Cvor<TipOznake> dajPolazniCvor(){return pocetak;}
    Cvor<TipOznake> dajDolazniCvor(){return kraj;}

};

template <typename t1>
struct ListaCvor{
    Grana<t1> *grana;
    ListaCvor<t1> *nekst;
};

template <typename TipOznake>
void obrisiListu(ListaCvor<TipOznake> **lista, int maks){
    for(int i = 0; i < maks; i++){
        if(lista[i] != nullptr) {
            ListaCvor<TipOznake>* pom = lista[i];
            while(pom->nekst){
                ListaCvor<TipOznake>* p = pom->nekst;
                delete pom->grana;
                delete pom;
                pom = nullptr;
                pom = p;
            }
            delete pom->grana;
            delete pom;
            pom = nullptr;
            lista[i] = nullptr;
        }
    }
}

template <typename TipOznake>
class ListaGraf : public UsmjereniGraf<TipOznake>{
    int br_cvorova;
    int maks;
    Cvor<TipOznake> **cvorovi;
    ListaCvor<TipOznake> **lista;
    void DaLiPostojiCvor(int a) const {
        if(a >= br_cvorova) throw domain_error("Ne postoji taj cvor");
    }
    void DaLiPostojiGrana(int start, int end) const {
        DaLiPostojiCvor(start);
        DaLiPostojiCvor(end);
        ListaCvor<TipOznake> *pom = lista[start];
        while(pom){
            if(pom->grana->dajDolazniCvor().dajRedniBroj() == end) return;
            pom = pom->nekst;
        }
        throw domain_error("Ne postoji ta grana");

    }
    void DaLiJePuna(){
        if(maks <= br_cvorova){
            maks *= 2;
            ListaCvor<TipOznake> **pom = new ListaCvor<TipOznake>*[maks]{};
            for(int i = 0; i < maks/2; i++){
                swap(pom[i],lista[i]);
            }
            for(int i = 0; i < maks/2; i++){
                delete lista[i];
            }
            delete[] lista;
            lista = pom;
            Cvor<TipOznake> **pom2 = new Cvor<TipOznake>*[maks]{};
            for(int i = 0; i < maks/2; i++){
                swap(pom2[i],cvorovi[i]);
            }
            delete cvorovi;
            cvorovi = pom2;
        }
    }
  public:
    ListaGraf(int n) : maks(100), br_cvorova(n){
        if(n>maks) maks = n;
        lista = new ListaCvor<TipOznake>*[maks]{};
        cvorovi = new Cvor<TipOznake>*[maks]{};
        for(int i = 0; i < n; i++){
            cvorovi[i] = new Cvor<TipOznake>(this,i);
        }
    };
    ~ListaGraf(){
        obrisiListu(lista, maks);
        delete[] lista;
        for(int i = 0; i < br_cvorova; i++){
            delete cvorovi[i];
        }
        delete[] cvorovi;
    };
    ListaGraf<TipOznake>& operator=(ListaGraf<TipOznake> m){
        swap(m.maks,maks);
        swap(m.br_cvorova,br_cvorova);
        swap(m.cvorovi,cvorovi);
        swap(m.lista,lista);
        return *this;
    }
    int dajBrojCvorova() const override {return br_cvorova;}
    void postaviBrojCvorova(int br) override {
        if(br <= br_cvorova) throw domain_error("Greska");
        int p = br_cvorova;
        br_cvorova = br;
        while(br_cvorova >= maks){
            DaLiJePuna();
        }
        for(int i = p; i < br; i++){
            cvorovi[i] = new Cvor<TipOznake>(this,i);
        }
    }
    void dodajGranu(int start, int end, float tezina = 0) override {
        DaLiPostojiCvor(start);
        DaLiPostojiCvor(end);
        Grana<TipOznake> *pom = new Grana<TipOznake>(this,start,end,tezina);
        lista[start] = new ListaCvor<TipOznake>{pom,lista[start]};
    }
    void obrisiGranu(int start, int end) override {
        DaLiPostojiGrana(start,end);
        ListaCvor<TipOznake> *pom = lista[start];
        ListaCvor<TipOznake> *p = lista[start];
        while(pom){
            if(pom->grana->dajDolazniCvor().dajRedniBroj() == end){
                delete pom->grana;
                if(lista[start] == pom){
                    lista[start] = pom->nekst;
                    delete pom;
                    pom = nullptr;
                    break;
                }
                else{
                    p->nekst = pom->nekst;
                    delete pom;
                    pom = nullptr;
                    break;
                }
            }
            p = pom;
            pom = pom->nekst;
        }    
    }
    void postaviTezinuGrane(int start, int end, float tezina) override {
        DaLiPostojiGrana(start,end);
        ListaCvor<TipOznake> *pom = lista[start];
        while(pom){
            if(pom->grana->dajDolazniCvor().dajRedniBroj() == end){
                pom->grana->postaviPravuTezinu(tezina);
                break;
            }
            pom = pom->nekst;
        }
    }
    float dajTezinuGrane(int start, int end) const override {
        DaLiPostojiGrana(start,end);
        ListaCvor<TipOznake> *pom = lista[start];
        while(pom){
            if(pom->grana->dajDolazniCvor().dajRedniBroj() == end){
                return pom->grana->dajTezinu();
            }
            pom = pom->nekst;
        }
        return pom->grana->dajTezinu();
    }
    bool postojiGrana(int start, int end) const override {
        ListaCvor<TipOznake> *pom = lista[start];
        while(pom){
            if(pom->grana->dajDolazniCvor().dajRedniBroj() == end){
                return true;
            }
            pom = pom->nekst;
        }
        return false;
    }
    void postaviOznakuCvora(int cvor, TipOznake oznaka) override {
        DaLiPostojiCvor(cvor);
        cvorovi[cvor]->postaviPravuOznaku(oznaka);
    }
    TipOznake dajOznakuCvora(int cvor) const override {
        DaLiPostojiCvor(cvor);
        return cvorovi[cvor]->dajOznaku();
    }
    void postaviPosjecen(int a, bool t) override {
        cvorovi[a]->postaviPosjecen(t);
    }
    void postaviOznakuGrane(int start, int end, TipOznake oznaka) override {
        DaLiPostojiGrana(start, end);
        ListaCvor<TipOznake> *pom = lista[start];
        while(pom){
            if(pom->grana->dajDolazniCvor().dajRedniBroj() == end){
                pom->grana->postaviPravuOznaku(oznaka);
                return;
            }
            pom = pom->nekst;
        }
    }
    TipOznake dajOznakuGrane(int start, int end) const override {
        DaLiPostojiGrana(start, end);
        ListaCvor<TipOznake> *pom = lista[start];
        while(pom){
            if(pom->grana->dajDolazniCvor().dajRedniBroj() == end){
                return pom->grana->dajOznaku();
            }
            pom = pom->nekst;
        }
        return pom->grana->dajOznaku();
    }
    Grana<TipOznake> dajGranu(int start, int end) override {
        DaLiPostojiGrana(start, end);
        ListaCvor<TipOznake> *pom = lista[start];
        while(pom){
            if(pom->grana->dajDolazniCvor().dajRedniBroj() == end){
                return *pom->grana;
            }
            pom = pom->nekst;
        }
        return *pom->grana;
    }
    Cvor<TipOznake> dajCvor(int br) override {
        DaLiPostojiCvor(br);
        return *cvorovi[br];
    }
    GranaIterator<TipOznake> dajGranePocetak() override {
        for(int i = 0; i < br_cvorova; i++){
            for(int j = 0; j < br_cvorova; j++){
                if(postojiGrana(i, j)){
                    GranaIterator<TipOznake> pom(this,i,j);
                    return pom;
                }
            }
        }
        throw domain_error("Nema grana");
    }
    GranaIterator<TipOznake> dajGraneKraj() override {
        GranaIterator<TipOznake> pom(this,dajBrojCvorova(),dajBrojCvorova());
        return pom;
    }
};

template <typename TipOznake>
class GranaIterator {
    ListaGraf<TipOznake> *mat;
    int polazniCvor;
    int dolazniCvor;
  public:
    GranaIterator(ListaGraf<TipOznake> *m, int a, int b){
        mat = m;
        polazniCvor = a;
        dolazniCvor = b;
    }
    GranaIterator(GranaIterator<TipOznake> &iter) : polazniCvor(iter.polazniCvor), dolazniCvor(iter.dolazniCvor), mat(iter.mat){}
    GranaIterator(GranaIterator<TipOznake> &&iter) : polazniCvor(iter.polazniCvor), dolazniCvor(iter.dolazniCvor), mat(iter.mat){}
    GranaIterator<TipOznake> operator=(GranaIterator<TipOznake> iter){
        swap(mat,iter.mat);
        swap(polazniCvor,iter.polazniCvor);
        swap(dolazniCvor,iter.dolazniCvor);
    }
    Grana<TipOznake> operator*(){
        return mat->dajGranu(polazniCvor, dolazniCvor);
    }
    bool operator==(const GranaIterator &iter) const {
        if(mat == iter.mat && polazniCvor == iter.polazniCvor && dolazniCvor == iter.dolazniCvor) return true;
        return false;
    }
    bool operator!=(const GranaIterator &iter) const {
        if(mat == iter.mat && polazniCvor == iter.polazniCvor && dolazniCvor == iter.dolazniCvor) return false;
        return true;
    }
    GranaIterator& operator++(){
        int p = 1, j;
        for(int i = polazniCvor; i < mat->dajBrojCvorova(); i++){
            if(p){
                j = dolazniCvor + 1;
                p = 0;
            }
            else j = 0;
            for(; j < mat->dajBrojCvorova(); j++){
                if(mat->postojiGrana(i,j)){
                    polazniCvor = i;
                    dolazniCvor = j;
                    return *this;
                }
            }
        }
        polazniCvor = mat->dajBrojCvorova();
        dolazniCvor = mat->dajBrojCvorova();
        return *this;
    }
    GranaIterator operator++(int){
        GranaIterator pom = *this;
        return ++pom;
    }
};

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *g, vector<Cvor<TipOznake>> &v, Cvor<TipOznake> c){
    for(int i = 0; i < g->dajBrojCvorova(); i++){
        g->postaviPosjecen(i,false);
    }
    queue<int> red;
    red.push(c.dajRedniBroj());
    while(!red.empty()){
        int cv = red.front();
        v.push_back(g->dajCvor(cv));
        red.pop();
        for(int i = 0; i < g->dajBrojCvorova(); i++){
            if(g->postojiGrana(cv,i) && !g->dajCvor(i).dajPosjecen()){
                g->postaviPosjecen(i,true);
                red.push(i);
            }
        }
    }
}

template <typename TipOznake>
void dfsTrazenje(UsmjereniGraf<TipOznake> *g, vector<Cvor<TipOznake>> &v, Cvor<TipOznake> c){
    g->postaviPosjecen(c.dajRedniBroj(),true);
    v.push_back(c);
    for(int i = 0; i < g->dajBrojCvorova(); i++){
        if(g->postojiGrana(c.dajRedniBroj(),i) && !g->dajCvor(i).dajPosjecen()){
            dfsTrazenje<TipOznake>(g,v,g->dajCvor(i));
        }
    }
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *g, vector<Cvor<TipOznake>> &v, Cvor<TipOznake> c){
    for(int i = 0; i < g->dajBrojCvorova(); i++){
        g->postaviPosjecen(i,false);
    }
    dfsTrazenje(g,v,c);
}



template <typename t1, typename t2>
struct HashCvor{
    t1 kljuc;
    t2 vrijednost;
    HashCvor<t1,t2> *nekst;
};

template <typename kljuc, typename vrijednost>
void brisanje(HashCvor<kljuc, vrijednost>** mapa, int kapacitet){
    for(int i = 0; i < kapacitet; i++){
        if(mapa[i] != nullptr) {
            HashCvor<kljuc, vrijednost>* pom = mapa[i];
            while(pom->nekst){
                HashCvor<kljuc, vrijednost>* p = pom->nekst;
                delete pom;
                pom = nullptr;
                pom = p;
            }
            delete pom;
            pom = nullptr;
            mapa[i] = nullptr;
        }
    }
}

template <typename kljuc, typename vrijednost>
class HashMapaLan : public Mapa<kljuc,vrijednost>{
    HashCvor<kljuc,vrijednost>** mapa;
    int br_elemenata;
    int kapacitet;
    unsigned int (*hashfunkcija)(kljuc, unsigned int);
    void DaLiJePun(){
        if(br_elemenata==kapacitet){
            kapacitet*=2;
            HashCvor<kljuc, vrijednost>** pom = new HashCvor<kljuc,vrijednost>*[kapacitet]{};
            for(int i=0; i < br_elemenata; i++){
                pom[i] = mapa[i];
                mapa[i] = nullptr;
            }
            delete[] mapa;
            mapa = pom;
        }
    }
  public:
    HashMapaLan() : br_elemenata(0), kapacitet(1000){
        mapa = new HashCvor<kljuc,vrijednost>*[kapacitet]{};
        hashfunkcija = nullptr;
    }
    HashMapaLan(const HashMapaLan &m) : br_elemenata(m.br_elemenata), kapacitet(m.kapacitet){
        mapa = new HashCvor<kljuc,vrijednost>*[kapacitet]{};
        for(int i = 0; i < kapacitet; i++){
            if(m.mapa[i] != nullptr){
                HashCvor<kljuc,vrijednost> *pom = m.mapa[i];
                mapa[i] = new HashCvor<kljuc, vrijednost>{pom->kljuc,pom->vrijednost,nullptr};
                HashCvor<kljuc,vrijednost> *r = mapa[i];
                while(pom->nekst){
                    pom = pom->nekst;
                    r->nekst = new HashCvor<kljuc, vrijednost>{pom->kljuc,pom->vrijednost,nullptr};
                    r = r->nekst;
                } 
            }
        }
        hashfunkcija = m.hashfunkcija;
    }
    ~HashMapaLan(){
        brisanje(mapa, kapacitet);
        delete[] mapa;
    }
    HashMapaLan& operator=(HashMapaLan m){
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
        if(mapa[a]){            
            HashCvor<kljuc,vrijednost> *pom = mapa[a];
            HashCvor<kljuc,vrijednost> *r = mapa[a];
            while(pom){
                if(pom->kljuc == k){
                    br_elemenata--;
                    if(mapa[a] == pom){
                        mapa[a] = pom->nekst;
                        delete pom;
                        pom = nullptr;
                        break;
                    }
                    else{
                        r->nekst = pom->nekst;
                        delete pom;
                        pom = nullptr;
                        break;
                    }
                }
                else if(pom->kljuc > k) break;
                r = pom;
                pom = pom->nekst;
            }
        }
    }
    vrijednost& operator[](const kljuc &k) override {
        if(hashfunkcija == nullptr) throw domain_error("Nije definisana Hash funkcija"); 
        unsigned int a = hashfunkcija(k,kapacitet);
        HashCvor<kljuc,vrijednost> *pom = mapa[a];
        HashCvor<kljuc,vrijednost> *r = mapa[a];
        if(!pom){
            br_elemenata++;
            mapa[a] = new HashCvor<kljuc,vrijednost>{k,vrijednost{},nullptr};
            return mapa[a]->vrijednost;
        }
        while(pom){
            if(pom->kljuc == k){
                return pom->vrijednost;
            }
            else if(pom->kljuc > k) break;
            r = pom;
            pom = pom->nekst;
        }
        r->nekst = new HashCvor<kljuc,vrijednost>{k,vrijednost{},r->nekst};
        br_elemenata++;
        return r->nekst->vrijednost;
    }
    vrijednost operator[](const kljuc &k) const override {
        unsigned int a = hashfunkcija(k,kapacitet);
        HashCvor<kljuc,vrijednost> *pom = mapa[a];
        HashCvor<kljuc,vrijednost> *r = mapa[a];
        while(pom){
            if(pom->kljuc == k){
                return pom->vrijednost;
            }
            else if(pom->kljuc > k) break;
            r = pom;
            pom = pom->nekst;
        }
        return vrijednost{};
    }
    void definisiHashFunkciju(unsigned int (*pok)(kljuc,unsigned int)){
        hashfunkcija = pok;
    }
};

int main(){
UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);

    std::vector<Cvor<bool> > dfs_obilazak;
    dfs(g, dfs_obilazak, g->dajCvor(0));

    for (int i = 0; i < dfs_obilazak.size(); i++)
      cout << dfs_obilazak[i].dajRedniBroj() << ",";

  delete g;

    return 0;

}