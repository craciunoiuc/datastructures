// Copyright [2018] <Craciunoiu Cezar>
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <list>

template <typename T, typename C>
struct Node {
    T nume;  // numele/id-ul nodului.
    C info;  // informatie extra din nod.
    vector<T> muchii;  // noduri invecinate.
};

template <typename T, typename M, typename C>
// T - nume/id, M - info, C - functie hash
class Graph {
 private:
    unsigned int size;  // marimea maxima a grafului
    C (*Hfunction)(T&, unsigned int);  // functie de hash
    bool (*Vizitat)(bool, M*);  // functie de vizitare a nodurilor in DFS/BFS
    Hashtable<Node<T, M>, unsigned int> tabela;  // Hashtable pentru stocare
    vector<T> errcode;

 public:
    // Constructor ce primeste o functie de hash pentru informatie
    // functie de hash pentru Node, functie de vizitare si o marime maxima
    Graph(C (*h)(T&, unsigned int), C (*h2) (Node<T, M>&, unsigned int),
                 unsigned int size, bool (*V)(bool, M*)){
        this->size = size;
        Vizitat = V;
        Hfunction = h;

        // Se inmulteste cu 1.35 pentru a asigura ca infomatia este
        // raspandita cat de cat uniform in toata tabela.
        Hashtable<Node<T, M>, unsigned int> tabela2(h2, size*1.35);
        tabela = tabela2;
    }

    // Destructor care o data cu el va apela si destructorul Hastable-ului.
    ~Graph() {}

    // Construieste nodul si se adauga in tabela.
    void addNode(T& nume, M& info){
        Node<T, M> temp;
        temp.nume = nume;
        temp.info = info;
        tabela.Add(temp);
    }

    // Adauga direct nodul in tabela.
    void addNode(Node<T, M>& node){
        tabela.Add(node);
    }

    // Adauga o muchie/vecin in vectorul de muchii/vecini.
    void addEdge(T& nod, T& muchie) {
        getMuchii(nod)->push_back(muchie);
    }

    // Sterge o muchie din vectorul de vecini
    void removeEdge(T& nod, T& muchie) {
        std::vector<T>* temp = getMuchii(nod);
        for (auto it = temp->begin(); it != temp->end(); ++it)
            if (!muchie.compare(*it)){
                temp->erase();
                break;
            }
    }

    // Daca intre 2 noduri exista legatura se returneaza adevarat, fals altfel.
    bool HasEdge(T& nodsrc, T& noddst) {
        std::vector<T>* temp = getMuchii(nodsrc);
        for (auto it = temp->begin(); it != temp->end(); ++it)
            if ((*it).nume == noddst)
                return true;
        return false;
    }

    // Returneaza marimea grafului.
    unsigned int getsize() {
        return size;
    }

    // Returneaza adresa unui vector de vecini.
    std::vector<T>* getMuchii(T& nod) {
        list<Node<T, M>>* temp = tabela.GetValueP(Hfunction(nod, size*1.35));
        for (auto it = temp->begin(); it != temp->end(); ++it)
            if ((*it).nume == nod){
                return &(*it).muchii;
            }
        return &errcode;
    }

    // Returneaza un pointer la Hashtable-ul grafului.
    Hashtable<Node<T, M>, unsigned int>* Ptabela(){
        return &tabela;
    }

    // DFS recursiv pentru parcurgerea grafului. Informatia de vizitare a
    // unui nod se pastreaza in info, care in general va fi o structura.
    // Daca se doreste bifarea unui nod ca vizitat, se apeleaza cu argumentul
    // true, daca se doreste doar verificarea ca a fost vizitat, cu false.
    void DFS(C& orase, T* nod) {
        list<Node<T, M>>* temp;
        temp = tabela.GetValueP(Hfunction(*nod, static_cast<int>(orase*1.35)));
        auto it1 = temp->begin();
        for ( ; it1 != temp->end(); ++it1)
            if (!nod->compare((*it1).nume)){
                Vizitat(true, &(*it1).info);
                break;
            }
        for (auto it = (*it1).muchii.begin(); it != (*it1).muchii.end(); ++it){
            temp = tabela.GetValueP(Hfunction((*it),
                    static_cast<int>(orase*1.35)));
            for (auto it2 = temp->begin(); it2 != temp->end(); ++it2){
                if (!(*it).compare((*it2).nume) &&
                    Vizitat(false, &(*it2).info)){
                    DFS(orase, &(*it));
                    break;
                }
            }
        }
    }
};

#endif  // GRAPH_H_
