#include <iostream>
#include <stdexcept>
#include <queue>

using namespace std;

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
    virtual void postaviOznakuGrane(int start, int end, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuGrane(int start, int end) const = 0;
    virtual Grana<TipOznake> dajGranu(int start, int end) = 0;
    virtual Cvor<TipOznake> dajCvor(int br) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
    virtual void BFS(Cvor<TipOznake> cvor) = 0;
    virtual void BFSGrafa() = 0;
    virtual void DFS(Cvor<TipOznake> cvor) = 0;
    virtual void DFSGrafa() = 0;

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

template <typename TipOznake>
void obrisiMatricu(Grana<TipOznake> ***mat, int maks){
    for(int i = 0; i < maks; i++){
        for(int j = 0; j < maks; j++){
            delete mat[i][j];
        }
        delete[] mat[i];
    }
    delete[] mat;
}

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake>{
    int br_cvorova;
    int maks;
    Cvor<TipOznake> **cvorovi;
    Grana<TipOznake> ***mat;
    void DaLiPostojiCvor(int a) const {
        if(a >= br_cvorova) throw domain_error("Ne postoji taj cvor");
    }
    void DaLiPostojiGrana(int start, int end) const {
        DaLiPostojiCvor(start);
        DaLiPostojiCvor(end);
        if(mat[start][end] == nullptr) throw domain_error("Ne postoji ta grana");
    }
    void DaLiJePuna(){
        if(maks <= br_cvorova){
            maks *= 2;
            Grana<TipOznake> ***pom = new Grana<TipOznake>**[maks]{};
            for(int i = 0; i < maks; i++){
                pom[i] = new Grana<TipOznake>*[maks]{};
            }
            for(int i = 0; i < br_cvorova; i++){
                for(int j = 0; j < br_cvorova; j++){
                    swap(pom[i][j],mat[i][j]);
                }
            }
            for(int i = 0; i < maks/2; i++){
                delete mat[i];
            }
            delete mat;
            mat = pom;
            Cvor<TipOznake> **pom2 = new Cvor<TipOznake>*[maks]{};
            for(int i = 0; i < maks/2; i++){
                swap(pom2[i],cvorovi[i]);
            }
            delete cvorovi;

        }
    }
  public:
    MatricaGraf(int n) : maks(100), br_cvorova(n){
        if(n>maks) maks = n;
        mat = new Grana<TipOznake>**[maks]{};
        for(int i = 0; i < maks; i++){
            mat[i] = new Grana<TipOznake>*[maks]{};
        }
        cvorovi = new Cvor<TipOznake>*[maks]{};
        for(int i = 0; i < n; i++){
            cvorovi[i] = new Cvor<TipOznake>(this,i);
        }
    };
    ~MatricaGraf(){
        obrisiMatricu(mat, maks);
        for(int i = 0; i < br_cvorova; i++){
            delete cvorovi[i];
        }
        delete[] cvorovi;
    };
    MatricaGraf<TipOznake>& operator=(MatricaGraf<TipOznake> m){
        swap(m.maks,maks);
        swap(m.br_cvorova,br_cvorova);
        swap(m.cvorovi,cvorovi);
        swap(m.mat,mat);
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
        mat[start][end] = new Grana<TipOznake>(this,start,end,tezina);
    }
    void obrisiGranu(int start, int end) override {
        DaLiPostojiGrana(start,end);
        delete mat[start][end];
        mat[start][end] = nullptr;
    }
    void postaviTezinuGrane(int start, int end, float tezina) override {
        DaLiPostojiGrana(start,end);
        mat[start][end]->postaviPravuTezinu(tezina);
    }
    float dajTezinuGrane(int start, int end) const override {
        DaLiPostojiGrana(start,end);
        return mat[start][end]->dajTezinu();
    }
    bool postojiGrana(int start, int end) const override {
        if(mat[start][end] == nullptr) return false;
        return true;
    }
    void postaviOznakuCvora(int cvor, TipOznake oznaka) override {
        DaLiPostojiCvor(cvor);
        cvorovi[cvor]->postaviPravuOznaku(oznaka);
    }
    TipOznake dajOznakuCvora(int cvor) const override {
        DaLiPostojiCvor(cvor);
        return cvorovi[cvor]->dajOznaku();
    }
    void postaviPosjecen(int a, bool t){
        cvorovi[a]->postaviPosjecen(t);
    }
    void postaviOznakuGrane(int start, int end, TipOznake oznaka) override {
        DaLiPostojiGrana(start, end);
        mat[start][end]->postaviPravuOznaku(oznaka);
    }
    TipOznake dajOznakuGrane(int start, int end) const override {
        DaLiPostojiGrana(start, end);
        return mat[start][end]->dajOznaku();
    }
    Grana<TipOznake> dajGranu(int start, int end) override {
        DaLiPostojiGrana(start, end);
        return *mat[start][end];
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
    void BFS(Cvor<TipOznake> cvor) override {
        postaviPosjecen(cvor.dajRedniBroj(),true);
        queue<int> red;
        red.push(cvor.dajRedniBroj());
        while(!red.empty()){
            int c = red.front();
            red.pop();
            cout<<c<<endl;
            for(int i = 0; i < dajBrojCvorova(); i++){
                if(postojiGrana(c,i) && !cvorovi[i]->dajPosjecen()){
                    postaviPosjecen(i,true);
                    red.push(i);
                }
            }
        }
    }
    void BFSGrafa() override {
        for(int i = 0; i < dajBrojCvorova(); i++){
            postaviPosjecen(i,false);
        }
        for(int i = 0; i < dajBrojCvorova(); i++){
            if(!cvorovi[i]->dajPosjecen()){
                BFS(*cvorovi[i]);
            }
        }
    }
    void DFS(Cvor<TipOznake> cvor) override {
        postaviPosjecen(cvor.dajRedniBroj(),true);
        cout<<cvor.dajRedniBroj()<<endl;
        for(int i = 0; i < dajBrojCvorova(); i++){
            if(postojiGrana(cvor.dajRedniBroj(),i) && !cvorovi[i]->dajPosjecen()){
                DFS(dajCvor(i));
            }
        }
    }
    void DFSGrafa() override {
        for(int i = 0; i < dajBrojCvorova(); i++){
            postaviPosjecen(i,false);
        }
        for(int i = 0; i < dajBrojCvorova(); i++){
            if(!cvorovi[i]->dajPosjecen()){
                DFS(*cvorovi[i]);
            }
        }
    }
};

template <typename TipOznake>
class GranaIterator {
    MatricaGraf<TipOznake> *mat;
    int polazniCvor;
    int dolazniCvor;
  public:
    GranaIterator(MatricaGraf<TipOznake> *m, int a, int b){
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
        throw domain_error("Greska");
    }
    GranaIterator operator++(int){
        GranaIterator pom = *this;
        return ++pom;
    }
};



int main(){

    return 0;
}
