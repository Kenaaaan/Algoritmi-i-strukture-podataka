#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <utility>

using namespace std;

using sat = std::chrono::high_resolution_clock;

template <typename sppk, typename sppv> class Mapa {

public:
  Mapa<sppk, sppv>() {}
  virtual ~Mapa<sppk, sppv>() {}
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const sppk &kljuc) = 0;
  virtual sppv &operator[](sppk i) = 0;
  virtual const sppv operator[](sppk i) const = 0;
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

  sppv &operator[](sppk i) {

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

  const sppv operator[](sppk i) const {
    auto c = pretraga(p, i);
    if (c != nullptr)
      return c->vrijednost;
    return sppv();
  }
};

template <typename sppk, typename sppv>
class AVLStabloMapa : public Mapa<sppk, sppv> {

  struct Cvor {

    sppk kljuc;
    sppv vrijednost;
    Cvor *l_strana, *d_strana, *roditelj;
    int balans;

    Cvor(sppk i = sppk(), sppv u = sppv())
        : kljuc(i), vrijednost(u), d_strana(nullptr), l_strana(nullptr),
          roditelj(nullptr), balans(0) {}
  };

  int balanss(Cvor *t) {
    if (t == nullptr)
      return 0;
    return t->balans;
  }

  int max(int a, int b) { return (a > b) ? a : b; }

  // dodat ćemo i novi balans atribut novome čvoru

  Cvor *novi(sppk i = sppk(), sppv u = sppv()) {
    Cvor *temp = new Cvor();
    temp->kljuc = i;
    temp->vrijednost = u;
    temp->d_strana = temp->l_strana = temp->roditelj = nullptr;
    temp->balans = 0;
    return temp;
  }

  // rotiranje udesno

  void desno_rotiranje(Cvor *t) {

    Cvor *q = nullptr;
    int o = 0;
    q = t->l_strana->d_strana;
    t->balans = 0;
    Cvor *l = t->l_strana;
    Cvor *r = t->roditelj;
    l->roditelj = r;
    l->d_strana = t;

    if (!l->roditelj && !o) {
      p = l;
      o = 1;
    } else if (r->d_strana == t && !o) {
      l->roditelj->d_strana = l;
      o = 1;
    }

    if (o == 0) {
      l->roditelj->l_strana = l;
    }
    l->balans = 0;

    if (q)
      q->roditelj = t;
    t->l_strana = q;
    t->roditelj = l;
  }

  // rotiranje ulijevo

  void lijevo_rotiranje(Cvor *t) {

    Cvor *q = nullptr;
    int o = 0;
    q = t->d_strana->l_strana;
    t->balans = 0;
    Cvor *d = t->d_strana;
    Cvor *r = t->roditelj;
    d->roditelj = r;
    d->l_strana = t;

    if (!d->roditelj && !o) {
      p = d;
      o = 1;
    } else if (r->d_strana == t && !o) {
      d->roditelj->d_strana = d;
      o = 1;
    }

    if (o == 0) {
      d->roditelj->l_strana = d;
    }

    d->balans = 0;

    if (q)
      q->roditelj = t;
    t->d_strana = q;
    t->roditelj = d;
  }

  // funckija računa faktor balansa (balans lijevog cvora - balans desnog
  // cvora)

  void azurirajBalans(Cvor *t, bool b) {

    if (t == nullptr)
      return;
    else if (b)
      t->balans--;
    else
      t->balans++;

    int blns = t->balans;

    if (t->roditelj) {

      if (t->roditelj->d_strana == t)
        b = true;
      else
        b = false;
    }

    if (blns == 2) {

      if (balanss(t->l_strana) == 1)
        desno_rotiranje(t);
      else {
        lijevo_rotiranje(t->l_strana);
        desno_rotiranje(t);
      }

    } else if (blns == -2) {
      if (balanss(t->d_strana) == -1)
        lijevo_rotiranje(t);

      else {
        desno_rotiranje(t->d_strana);
        lijevo_rotiranje(t);
      }
    } else {

      if (blns == 0)
        return;
      azurirajBalans(t->roditelj, b);
    }
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

  //u dodaj metodi smo dodali azurirajBalans, kako bi svaki novi dodani bio tamo  
  //  gdje i treba


  Cvor *dodaj(Cvor *p, sppk i, sppv u = sppv(), Cvor *rod = nullptr) {

    while (p) {

      if (p->kljuc == i)
        return p;
      else if (i > p->kljuc)
        p = p->d_strana;
      else
        p = p->l_strana;
    }

    if (!p) {

      Cvor *t = AVLStabloMapa::p, *q = nullptr;
      p = novi(i, u);
      while (t) {
        q = t;
        if (t->kljuc > p->kljuc)
          t = t->l_strana;
        else
          t = t->d_strana;
      }

      if (!q)
        AVLStabloMapa::p = p;
      else if (q->kljuc > p->kljuc) {
        q->l_strana = p;
        p->roditelj = q;
      } else {
        q->d_strana = p;
        p->roditelj = q;
      }

      if (!q)
        return p;

      if (q->l_strana && q->d_strana)
        q->balans = 0;
      else {
        bool b = true;
        if (q->l_strana)
          b = false;
        azurirajBalans(q, b);
      }
    }

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

  //također kada brišemo neki element morat ćemo azurirati balans kada to i treba

  void obrisiC(Cvor *t, sppk i) {

    Cvor *r = nullptr;

    while (t != nullptr && t->kljuc != i) {

      r = t;
      if (t->kljuc < i)
        t = t->d_strana;
      else
        t = t->l_strana;
    }

    if (t == nullptr)
      return;

    Cvor *e, *q, *s ;

    if (!t->l_strana){
      e = t->d_strana;
      s = r;}
    else if (!t->d_strana){
      e = t->l_strana;
      s = r;}
    else {
      e = t->l_strana;
      q = e->d_strana;
      s = t;
      while (q) {
        s = e;
        e = q;
        q = e->d_strana;
      }
      if (t != s) {
        s->d_strana = e->l_strana;
        if (e->l_strana)
          e->l_strana->roditelj = s;
          e->l_strana = t->l_strana;
        if (t->l_strana)
          t->l_strana->roditelj = e;
      }
      e->d_strana = t->d_strana;
      if (t->d_strana)
        t->d_strana->roditelj = e;
    }

    if (!r) {
      AVLStabloMapa::p = e;
    } else {
      if (r->l_strana == t) {
        r->l_strana = e;
        r->balans--;
      } else {
        r->d_strana = e;
        r->balans++;
      }
    }

    if (e)  e->roditelj = r;

      t->d_strana = t->l_strana = t->roditelj = nullptr;
      delete t;
    
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
  AVLStabloMapa<sppk, sppv>() : br_ele(0), p(nullptr) {}

  ~AVLStabloMapa<sppk, sppv>() { if(p) obrisi_stablo(p); }

  AVLStabloMapa<sppk, sppv>(const AVLStabloMapa<sppk, sppv> &a) {
    br_ele = a.br_ele;
    if (a.br_ele == 0)
      p = nullptr;

    else {
      p = novi(a.p->kljuc, a.p->vrijednost);
      konstruktor(p, a.p);
    }
  }

  AVLStabloMapa<sppk, sppv> &operator=(const AVLStabloMapa<sppk, sppv> &a) {

    if (this == &a)
      return *this;

    obrisi();
    br_ele = a.br_ele;
    if (a.br_ele == 0)
      p = nullptr;

    else {
      if(!p)p = novi(a.p->kljuc,a.p->vrijednost);
      p->kljuc = a.p->kljuc;
      p->vrijednost = a.p->vrijednost;
      konstruktor(p, a.p);
    }
    return *this;
  }

  int brojElemenata() const { return br_ele; }

  void obrisi() {
    obrisi_stablo(p);
    p = nullptr;
    br_ele = 0;
  }

  void obrisi(const sppk &kljuc) {

    obrisiC(p, kljuc);

    br_ele--;
  }

  sppv &operator[](sppk i) {

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

  const sppv operator[](sppk i) const {
    auto c = pretraga(p, i);
    if (c != nullptr)
      return c->vrijednost;
    return sppv();
  }
};

int main() {

    BinStabloMapa<int, int> mapaBin;
    AVLStabloMapa<int, int> mapaAvl;

    for(int i = 0; i < 50000; i++) {
        mapaBin[rand()] = rand();
        mapaAvl[rand()] = rand();
    }


    mapaBin[1998] = rand();
    mapaAvl[1998] = rand();

    clock_t vrijeme = clock();

    // U AVL mapa je brza od BIN mape za umetanja elementaa sto se tice pristpupa elementima ostaje ista efikasnost

    mapaBin[rand()] = rand();
    vrijeme = clock() - vrijeme;
    std::cout << "Umetanje elementa u bin stablo vremenski iznosi: " << double(vrijeme * 1000) / CLOCKS_PER_SEC << " ms." << std::endl;

    vrijeme = clock();
    mapaAvl[rand()] = rand();
    vrijeme = clock() - vrijeme;
    std::cout << "Umetanje elementa u avl stablo vremenski iznosi: " << double(vrijeme * 1000) / CLOCKS_PER_SEC << " ms." << std::endl;

    vrijeme = clock();
    mapaBin[1998] = 23;
    vrijeme = clock() - vrijeme;
    std::cout << "Pristup elementu u bin stablu vremenski iznosi: " << double(vrijeme * 1000) / CLOCKS_PER_SEC << " ms." << std::endl;

    vrijeme = clock();
    mapaAvl[1998] = 23;
    vrijeme = clock() - vrijeme;
    std::cout << "Pristup elementu u avl stablu vremenski iznosi: " << double(vrijeme * 1000) / CLOCKS_PER_SEC << " ms." << std::endl; 


    return 0;
}