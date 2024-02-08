#include <cmath>
#include <iostream>
#include <new>
using namespace std;
template <typename T> class Lista {
public:
  virtual ~Lista() {}                          //  //
  virtual int brojElemenata() const = 0;       //  //
  virtual T &trenutni() = 0;                   //  //
  virtual T trenutni() const = 0;              //  //
  virtual bool prethodni() = 0;                //  //
  virtual bool sljedeci() = 0;                 //  //
  virtual void pocetak() = 0;                  //  //
  virtual void kraj() = 0;                     //  //
  virtual void obrisi() = 0;                   //  //
  virtual void dodajIspred(const T &el) = 0;   //  //
  virtual void dodajIza(const T &el) = 0;      //  //
  virtual T &operator[](int i) = 0;            //  //
  virtual const T operator[](int i) const = 0; //  //
};

template <typename T> class NizLista : public Lista<T> {
protected:
  T *pok;
  int velicina, br_elemenata, trenutniE;

public:
  NizLista()
      : Lista<T>(), pok(new T[1000]), velicina(1000), br_elemenata(0),
        trenutniE(0) {}

  NizLista<T> &operator=(const NizLista<T> &a) {
    if (this == &a)
      return *this;
    br_elemenata = a.br_elemenata;
    velicina = a.velicina;
    trenutniE = a.trenutniE;
    if (a.br_elemenata == 0)
      pok = nullptr;
    else {
      delete[] pok;
      pok = (new T[a.velicina]);
      for (int i = 0; i < a.br_elemenata; i++)
        pok[i] = a.pok[i];
    }
    return *this;
  }

  NizLista<T>(const NizLista<T> &a) {
    velicina = a.velicina;
    br_elemenata = a.br_elemenata;
    trenutniE = a.trenutniE;
    if (a.br_elemenata == 0) {
      pok = nullptr;
    } else {
      pok = (new T[a.velicina]);
      for (int i = 0; i < a.br_elemenata; i++)
        pok[i] = a.pok[i];
    }
  }
  ~NizLista() { delete[] pok; }

  int brojElemenata() const { return br_elemenata; }
  T &trenutni() {
    if (br_elemenata == 0) {
      throw std::runtime_error("List is empty.");
    }
    return pok[trenutniE];
  }

  T trenutni() const {
    if (br_elemenata == 0) {
      throw std::runtime_error("List is empty.");
    }
    return pok[trenutniE];
  }
  void dodajIspred(const T &el) {
    if(br_elemenata==velicina){
    T* pok2;
    pok2 = (new T[velicina*2]);
    for(int i=0;i<br_elemenata;i++)
        pok2[i]=pok[i];
    delete[] pok;
    velicina=velicina*2;
    pok=pok2;
    }

    if (br_elemenata == 0) {
      pok[0] = el;
    } else {
      for (int i = br_elemenata; i > trenutniE; i--) {
        pok[i] = pok[i - 1];
      }
      pok[trenutniE] = el;
    }
    if (br_elemenata != 0)
      trenutniE++;
    br_elemenata++;
  }

void dodajIza(const T &el) {
    if(br_elemenata==velicina){
    T* pok2;
    pok2 = (new T[velicina*2]);
    for(int i=0;i<br_elemenata;i++)
        pok2[i]=pok[i];
    delete[] pok;
    velicina=velicina*2;
    pok=pok2;
    }
    if (br_elemenata == 0) {
      pok[0] = el;
      trenutniE = 0;
    } else {
      for (int i = br_elemenata; i  > trenutniE + 1; i--) {
        pok[i] = pok[i - 1];
      }
      pok[trenutniE + 1] = el;
    }
    br_elemenata++;
  }

  bool prethodni() {
    if (trenutniE == 0)
      return false;

    trenutniE--;
    return true;
  }
  bool sljedeci() {
    if (trenutniE == br_elemenata - 1)
      return false;

    trenutniE++;
    return true;
  }
  void pocetak() {
    if (trenutniE != 0) {
      trenutniE = 0;
    }
  }
  void kraj() { trenutniE = br_elemenata - 1; }
  void obrisi() {
    if (trenutniE == br_elemenata - 1) {
      trenutniE--;
      br_elemenata--;
    } else {
      for (int i = trenutniE; i < br_elemenata - 1; i++) {
        pok[i] = pok[i + 1];
      }
      br_elemenata--;
    }
  }
  T &operator[](int i) {
    if (i < 0 || i >= br_elemenata)
      throw std::domain_error("greska");

    return pok[i];
  }
  const T operator[](int i) const {
    if (i < 0 || i >= br_elemenata)
      throw std::domain_error("greska");

    return pok[i];
  }
};

template <typename T> class JednostrukaLista : public Lista<T> {
  struct Cvor {
    T element;
    Cvor *prethodniE = nullptr;
  };
  int br_elemenata;
  Cvor *prvi, *zadnji, *kursor;

public:
  JednostrukaLista()
      : Lista<T>(), br_elemenata(0), prvi(nullptr), zadnji(nullptr),
        kursor(nullptr) {}
  int brojElemenata() const { return br_elemenata; }

  /////////////////////////////////////////////////////////
  JednostrukaLista<T>(const JednostrukaLista<T> &a) {
    br_elemenata = 0;
    prvi = kursor = zadnji = nullptr;
    if (a.br_elemenata == 0) {
      prvi = kursor = zadnji = nullptr;
    } else {
      Cvor *temp = a.zadnji;
      if (zadnji)
        kraj();
      while (temp != nullptr) {
        dodajIza(temp->element);
        sljedeci();
        temp = temp->prethodniE;
        zadnji = kursor;
      }
    }
  }

  JednostrukaLista<T> &operator=(const JednostrukaLista<T> &a) {
    if (this == &a)
      return *this;
    for (int i = 0; i < br_elemenata; i++) {
      kursor = zadnji;
      zadnji = zadnji->prethodniE;
      delete kursor;
    }

    br_elemenata = 0;
    if (a.br_elemenata == 0) {
      prvi = kursor = zadnji = nullptr;
    } else {
      Cvor *temp = a.zadnji;
      while (temp != nullptr) {
        dodajIza(temp->element);
        sljedeci();
        temp = temp->prethodniE;
        zadnji = kursor;
      }
    }
    return *this;
  }
  void dodajIspred(const T &el) {
    Cvor *novi = new Cvor;
    novi->element = el;

    if (br_elemenata ==1) { // ovo ako je jedan element samo odnosno kad je kursor na prvom
      kursor->prethodniE = novi;
      prvi = novi;
    } else if (br_elemenata == 0) {
      kursor = prvi = zadnji = novi;
    } else {
      novi->prethodniE = kursor->prethodniE;
      kursor->prethodniE = novi;
    }
    br_elemenata++;
  }

  void dodajIza(const T &el) {
    Cvor *novi = new Cvor;
    novi->element = el;

    if (kursor == nullptr) {
      prvi = zadnji = novi;
      kursor = novi;
    } else if (kursor == zadnji) {
      novi->prethodniE = kursor;
      zadnji = novi;
    } else {
      Cvor *pomocni = zadnji;
      while (pomocni->prethodniE != kursor)
        pomocni = pomocni->prethodniE;
      pomocni->prethodniE = novi;
      novi->prethodniE = kursor;
    }

    br_elemenata++;
  }

  T &trenutni() {
    if (br_elemenata == 0) {
      throw std::runtime_error("List is empty.");
    }
    return kursor->element;
  }

  T trenutni() const {
    if (br_elemenata == 0) {
      throw std::runtime_error("List is empty.");
    }
    return kursor->element;
  }

  ~JednostrukaLista() {
    if (br_elemenata != 0) {
      for (int i = 0; i < br_elemenata; i++) {
        kursor = zadnji;
        zadnji = zadnji->prethodniE;
        delete kursor;
      }
      br_elemenata = 0;
    }
  }
  bool prethodni() {
    if (kursor->prethodniE == nullptr)
      return false;
    kursor = kursor->prethodniE;
    return true;
  }
  bool sljedeci() {
    if (kursor == zadnji)
      return false;
    Cvor *pomocni = zadnji;
    while (pomocni->prethodniE != kursor)
      pomocni = pomocni->prethodniE;
    kursor = pomocni;
    return true;
  }
  void pocetak() { this->kursor = prvi; }
  void kraj() { this->kursor = zadnji; }
  void obrisi() {
    if (br_elemenata == 0)
      throw std::domain_error("greska");

    if (br_elemenata==1) {
      delete kursor;
      kursor = prvi = zadnji = nullptr;
    }else if (kursor == zadnji) {
      kursor = kursor->prethodniE;
      delete zadnji;
      zadnji = kursor;
    } else if (kursor == prvi) {
      Cvor *pomocni = zadnji;
      while (pomocni->prethodniE != kursor)
        pomocni = pomocni->prethodniE;
      delete kursor;
      kursor = pomocni;
      kursor->prethodniE = nullptr;
      prvi = kursor;
    } else {
      Cvor *pomocni = zadnji;
      while (pomocni->prethodniE != kursor)
        pomocni = pomocni->prethodniE;
      auto k = kursor->prethodniE;
      delete kursor;
      kursor = pomocni;
      kursor->prethodniE = k;
    }
    br_elemenata--;
  }
  T &operator[](int j) {
    Cvor *pomocni = zadnji;
    for (int i = 0; i < (br_elemenata - 1 - j); i++) {
      pomocni = pomocni->prethodniE;
    }
    return pomocni->element;
  }
  const T operator[](int j) const {
    Cvor *pomocni = zadnji;
    
    for (int i = 0; i < (br_elemenata - 1 - j); i++) {
      pomocni = pomocni->prethodniE;
    }
    return pomocni->element;
  }

};


int main() {
    // Testiranje NizLista
    NizLista<int> nizLista;

    // Dodavanje elemenata
    nizLista.dodajIspred(1);
    nizLista.dodajIspred(2);
    nizLista.dodajIza(3);

    // Provjera broja elemenata
    std::cout << "Broj elemenata u NizLista: " << nizLista.brojElemenata() << std::endl;

    // Prikaz trenutnog elementa
    std::cout << "Trenutni element u NizLista: " << nizLista.trenutni() << std::endl;

    // Prikaz svih elemenata
    for (int i = 0; i < nizLista.brojElemenata(); i++) {
        std::cout << "Element[" << i << "] = " << nizLista[i] << std::endl;
    }

    // Testiranje prethodnog i sljedećeg elementa
    nizLista.prethodni();
    std::cout << "Trenutni element nakon prethodnog u NizLista: " << nizLista.trenutni() << std::endl;

    nizLista.sljedeci();
    std::cout << "Trenutni element nakon sljedećeg u NizLista: " << nizLista.trenutni() << std::endl;

    // Testiranje brisanja
    nizLista.obrisi();
    std::cout << "Broj elemenata u NizLista nakon brisanja: " << nizLista.brojElemenata() << std::endl;

    // Testiranje JednostrukaLista
    JednostrukaLista<int> jednostrukaLista;

    // Dodavanje elemenata
    jednostrukaLista.dodajIspred(4);
    jednostrukaLista.dodajIza(5);

    // Provjera broja elemenata
    std::cout << "Broj elemenata u JednostrukaLista: " << jednostrukaLista.brojElemenata() << std::endl;

    // Prikaz trenutnog elementa
    std::cout << "Trenutni element u JednostrukaLista: " << jednostrukaLista.trenutni() << std::endl;

    // Prikaz svih elemenata
    for (int i = 0; i < jednostrukaLista.brojElemenata(); i++) {
        std::cout << "Element[" << i << "] = " << jednostrukaLista[i] << std::endl;
    }

    // Testiranje prethodnog i sljedećeg elementa
    jednostrukaLista.prethodni();
    std::cout << "Trenutni element nakon prethodnog u JednostrukaLista: " << jednostrukaLista.trenutni() << std::endl;

    jednostrukaLista.sljedeci();
    std::cout << "Trenutni element nakon sljedećeg u JednostrukaLista: " << jednostrukaLista.trenutni() << std::endl;

    // Testiranje brisanja
    jednostrukaLista.obrisi();
    std::cout << "Broj elemenata u JednostrukaLista nakon brisanja: " << jednostrukaLista.brojElemenata() << std::endl;

    return 0;
}
