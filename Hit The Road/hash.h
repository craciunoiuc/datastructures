// Copyright [2018] <Craciunoiu Cezar>
#ifndef HASH_H_
#define HASH_H_

#include <list>

template <typename T, typename C>
struct Pair {
    list<T> values;  // Lista de valori ce au acelasi hash.
    C key;  // Cheie a unei pozitii.
};

template <typename T, typename C>  // T - value, C - key
class Hashtable {
 private:
    unsigned int Nrelem, Nrmax;
    Pair<T, C> *elemente = nullptr;
    C (*Hfunction)(T&, unsigned int);
    list<T> errcode;

 public:
    // Constructor ce asigneaza numarul maxim de elemente si functie de hash
    // ce va fi folosita, cat si aloca memorie pentru perechile cheie-valoare.
    Hashtable(C (*h)(T&, unsigned int), unsigned int maxi){
        Nrmax = maxi;
        Nrelem = 0;
        Hfunction = h;
        if (elemente == nullptr && Nrmax > 0)
            elemente = new Pair<T, C>[Nrmax];
    }

    // Un constructor fara niciun parametru in caz ca se doreste
    // o initializare fara a crea elemente.
    Hashtable(){
        Nrelem = 0;
        Nrmax = 0;
    }

    // Operator de assignment in caz ca se doreste copiere unei tabele in alta.
    Hashtable& operator=(const Hashtable& other){
        Nrelem = other.Nrelem;
        if (elemente != nullptr && Nrmax > 0)
            delete[] elemente;
        Nrmax = other.Nrmax;
        Hfunction = other.Hfunction;
        elemente = new Pair<T, C>[Nrmax];
        for (unsigned int i = 0; i < other.Nrmax; ++i) {
            elemente[i].key = other.elemente[i].key;
            elemente[i].values = other.elemente[i].values;
        }
        return *this;
    }

    // Elibereaza memoria ocupata de elementele tabelei.
    ~Hashtable(){
        if (this->Nrmax > 0)
            delete[] elemente;
    }

    // Adauga un element in lista de valori de la o anumita cheie.
    void Add(T& newelem){
        C temp = Hfunction(newelem, Nrmax);
        if (elemente[temp].values.empty())
            Nrelem++;
        elemente[temp].values.push_back(newelem);
    }

    // Sterge elementul din lista de la o anumita cheie.
    void Remove(T& elem){
        C temp = Hfunction(elem, Nrmax);
        for (auto it = elemente[temp].values.begin();
            it != elemente[temp].values.end(); ++it) {
            if (*it == elem) {
                elemente[temp].values.erase(it);
                if (elemente[temp].values.empty())
                    Nrelem--;
                break;
            }
        }
    }

    // Returneaza o lista cu valorile de la o anumita cheie, daca lista este
    // goala, se returneaza o eroare.
    list<T>& GetValue(C key){
        if (!elemente[key].values.empty()) {
            return elemente[key].values;
        }
        return errcode;
    }

    // Se returneaza un pointer la o lista cu o anumita valoare din tabela.
    // Daca lista este goala se returneaza o eroare.
    list<T>* GetValueP(C key){
        if (!elemente[key].values.empty()) {
            return &elemente[key].values;
        }
        return &errcode;
    }

    // Inlocuieste o lista de la o anumita cheie cu una noua.
    void Update(list<T>& value){
        C key = Hfunction(value.front(), Nrmax);
        elemente[key].values = value;
    }

    // Returneaza adevarat sau fals daca lista de la o anumita cheie contine
    // sau nu elemente.
    bool HasKey(C& key){
        return !elemente[key].values.empty();
    }

    // Returneaza cate liste de elemente negoale contine hashtable-ul.
    int size(){
        return Nrelem;
    }

    // Returneaza cantitatea maxima de liste ce le poate contine hashtable-ul.
    int Size_Max(){
        return Nrmax;
    }

    // Returneaza factorul de ocupare al hashtable-ului.
    float factor(){
        return (float)(Nrelem) / (float)(Nrmax);
    }
};

#endif  // HASH_H_
