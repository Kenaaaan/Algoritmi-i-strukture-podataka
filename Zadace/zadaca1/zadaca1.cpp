#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T> class Lista {
public:
  virtual ~Lista() {}                          //
  virtual int brojElemenata() const = 0;       //
  virtual T &trenutni() = 0;                   //
  virtual T trenutni() const = 0;              //
  virtual bool prethodni() = 0;                //
  virtual bool sljedeci() = 0;                 //
  virtual void pocetak() = 0;                  //
  virtual void kraj() = 0;                     //
  virtual void obrisi() = 0;                   //
  virtual void dodajIspred(const T &el) = 0;   //
  virtual void dodajIza(const T &el) = 0;      //
  virtual T &operator[](int i) = 0;            //
  virtual const T operator[](int i) const = 0; //
};
template <typename Tip> struct Cvor {
  Cvor *prethodni = nullptr;
  Cvor *sljedeci = nullptr;
  Tip element;
};
template <typename T> class DvostrukaLista : public Lista<T> {
protected:
  Cvor<T> *prvi;
  Cvor<T> *tekuci;
  Cvor<T> *posljednji;
  int br_elemenata;

public:
  template <typename Ti> friend class Iterator;
  DvostrukaLista()
      : prvi(nullptr), tekuci(nullptr), posljednji(nullptr), br_elemenata(0) {}

  DvostrukaLista<T> &operator=(const DvostrukaLista<T> &a) {
    if (this == &a)
      return *this;
    while (prvi != nullptr) {
      tekuci = prvi;
      prvi = prvi->sljedeci;
      delete tekuci;
    }
    br_elemenata = 0;
    if (a.br_elemenata == 0) {
      prvi = nullptr;
      posljednji = nullptr;
      tekuci = nullptr;
    } else {
      Cvor<T> *temp(a.prvi);
      while (temp != nullptr) {
        dodajIza(temp->element);
        sljedeci();
        temp = temp->sljedeci;
        posljednji = tekuci;
      }
    }
    return *this;
  }
  
  /*
  
    DvostrukaLista<T> operator=(DvostrukaLista<T> a) {
      std::swap(a.br_elemenata, br_elemenata);
      std::swap(a.prvi, prvi);
      std::swap(a.tekuci, tekuci);
      std::swap(a.posljednji, posljednji);
      return *this;
    }
  */

  
  DvostrukaLista (const DvostrukaLista<T> &a) {
      
       br_elemenata = 0;
       prvi= nullptr; tekuci=nullptr; posljednji=nullptr;  

       if (a.br_elemenata == 0) { prvi = nullptr; posljednji = nullptr; tekuci = nullptr; } 
    else {

      Cvor<T> *temp(a.prvi);

      if(prvi)pocetak();

      while (temp != nullptr) {
        dodajIza(temp->element);
        sljedeci();
        temp = temp->sljedeci;

         posljednji=tekuci;    
      } 
    }    
  }

  T &operator[](int i) {
    if (i < 0 || i >= br_elemenata)
      std::__throw_range_error("Nema elemenata");
    Cvor<T> *temp(prvi);
    for (int j = 0; j < i; j++)
      temp = temp->sljedeci;
    return temp->element;
  }
  const T operator[](int i) const {
    if (i < 0 || i >= br_elemenata)
      std::__throw_range_error("Nema elemenata");
    Cvor<T> *temp(prvi);
    for (int j = 0; j < i; j++)
      temp = temp->sljedeci;
    return temp->element;
  }

  ~DvostrukaLista() {
    while (prvi != nullptr) {
      tekuci = prvi;
      prvi = prvi->sljedeci;
      delete tekuci;
    }
    br_elemenata = 0;
    prvi=posljednji=tekuci=nullptr;
  }

 void obrisi() {
    if (prvi == nullptr) std::__throw_domain_error("Nema elemenata");

    if (tekuci == prvi) {
      prvi = prvi->sljedeci;
      delete tekuci;
      tekuci = prvi; br_elemenata--;
      if ( br_elemenata == 0)
        posljednji = nullptr; 
    } 
    else {
      Cvor<T> *temp(prvi);
      while (temp->sljedeci != tekuci)
        temp = temp->sljedeci;

      temp->sljedeci = tekuci->sljedeci;

      delete tekuci;
    
      if (temp->sljedeci == nullptr)
        posljednji = tekuci = temp;
        else{
             temp->sljedeci->prethodni=temp;
             tekuci=temp->sljedeci;
        }
    
     br_elemenata--; }
  }

  int brojElemenata() const { return br_elemenata; }

  void pocetak() {
    if (br_elemenata == 0)
      throw std::domain_error("greska");
    tekuci = prvi;
  }

  void kraj() {
    if (br_elemenata == 0)
      throw std::domain_error("greska");
    tekuci = posljednji;
  }

  T &trenutni() {
    if (br_elemenata == 0)
      throw std::domain_error("greska");
    return tekuci->element;
  }

  T trenutni() const {
    if (br_elemenata == 0)
      throw std::domain_error("greska");
    return tekuci->element;
  }

  bool prethodni() {
    if (tekuci == prvi)
      return false;
    tekuci = tekuci->prethodni;
    return true;
  }

  bool sljedeci() {
    if (tekuci == posljednji)
      return false;
    tekuci = tekuci->sljedeci;
    return true;
  }

  void dodajIza(const T &el) {
    Cvor<T> *novi = new Cvor<T>;
    novi->element = el;
    if (prvi == nullptr) {
      prvi = posljednji = tekuci = novi;
      prvi->sljedeci = nullptr;
      novi->sljedeci = nullptr;
      tekuci->sljedeci = nullptr;
    } else {
      novi->sljedeci = tekuci->sljedeci;
      novi->prethodni = tekuci;
      tekuci->sljedeci = novi;

      if (tekuci == posljednji)
        posljednji = novi;
      else
        novi->sljedeci->prethodni = novi;
    }
    br_elemenata++;
  }

  void dodajIspred(const T &a) {
    Cvor<T> *novi = new Cvor<T>();
    novi->element = a;
    if (br_elemenata == 0) {
      prvi = posljednji = tekuci = novi;
    } else if (prvi == tekuci) {
      novi->sljedeci = tekuci;
      tekuci->prethodni = novi;
      prvi = novi;
    } else {
      novi->sljedeci = tekuci;
      novi->prethodni = tekuci->prethodni;
      tekuci->prethodni = novi;
      novi->prethodni->sljedeci = novi;
    }
    br_elemenata++;
  }
};

template <typename Ti> class Iterator {
  const DvostrukaLista<Ti> *lista;
  Cvor<Ti> *trenutno;

public:
  Iterator(const DvostrukaLista<Ti> &l) {
    lista = &l;
    trenutno = l.prvi;
  }
  Ti dajTrenutni() {
    if (!lista->brojElemenata())
      throw std::domain_error("Lista nema elemenata");
    return trenutno->element;
  }
  bool sljedeci() {
    if (!lista->brojElemenata())
      throw std::domain_error("Lista nema elemenata");
    if (!trenutno->sljedeci)
      return false;
    trenutno = trenutno->sljedeci;
    return true;
  }
};

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n){
if(n.brojElemenata()==0)throw std::domain_error("Nema elemenata");
    Iterator<Tip> p=dynamic_cast<const DvostrukaLista<Tip>&>(n);
    Tip max=p.dajTrenutni();
    while(p.sljedeci())
    if(p.dajTrenutni()>max)max=p.dajTrenutni(); 
    return max;
}




int main() {

    DvostrukaLista<int> lista;
    DvostrukaLista<int> lista2;

    // Dodaj nekoliko elemenata
    lista.dodajIza(5);
    lista.dodajIza(10);
    lista.dodajIspred(3);
    lista2=lista;
    cout<<"Element na indeksu 1: "<<lista2[1]<<endl;
    // Testiraj indeksiranje
    cout << "Element na indeksu 1: " << lista[1] << endl;

    // Testiraj iterator
    Iterator<int> it(lista);
    cout << "Elementi liste: ";
    while (it.sljedeci()) {
        cout << it.dajTrenutni() << " ";
    }
    cout << endl;

    // Testiraj funkciju za pronalaženje maksimuma
    try {
        int maksimum = dajMaksimum(lista);

        cout << "Maksimum u listi: " << maksimum << endl;
    } catch (const std::exception &e) {

        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}
