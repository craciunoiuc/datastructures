// copyright Luca Istrate, Andrei Medar

#include "./decisionTree.h"  // NOLINT(build/include)
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::string;
using std::pair;
using std::vector;
using std::unordered_map;
using std::make_shared;

// structura unui nod din decision tree
// splitIndex = dimensiunea in functie de care se imparte
// split_value = valoarea in functie de care se imparte
// is_leaf si result sunt pentru cazul in care avem un nod frunza
Node::Node() {
    is_leaf = false;
    left = nullptr;
    right = nullptr;
}

void Node::make_decision_node(const int index, const int val) {
    split_index = index;
    split_value = val;
}

void Node::make_leaf(const vector<vector<int>> &samples,
                     const bool is_single_class) {
    // TODO(you)
    // Seteaza nodul ca fiind de tip frunza (modificati is_leaf si result)
    // is_single_class = true -> toate testele au aceeasi clasa (acela e result)
    // is_single_class = false -> se alege clasa care apare cel mai des
    is_leaf = true;
    auto it = samples.begin();
    if (is_single_class) {
        result = it[0][0];
    }
    else
    {
        // Se foloseste un vector de frecventa pentru a verifica care cifra
        // apare cel mai des. In result este pusa cifra ce apare de un numar
        // maxim de ori.
        int cifre[10];
        for (int i = 0; i < 10; ++i) {
            cifre[i] = 0;
        }
        for ( ; it != samples.end(); ++it) {
            cifre[*(*it).begin()]++;
        }
        int maxi = cifre[0], ciframax = 0;
        for (int i = 1; i < 10; ++i) {
            if (cifre[i] > maxi) {
                maxi = cifre[i];
                ciframax = i;
            }
        }
        result = ciframax;
    }
}

pair<int, int> find_best_split(const vector<vector<int>> &samples,
                               const vector<int> &dimensions) {
    // TODO(you)
    // Intoarce cea mai buna dimensiune si valoare de split dintre testele
    // primite. Prin cel mai bun split (dimensiune si valoare)
    // ne referim la split-ul care maximizeaza IG
    // pair-ul intors este format din (split_index, split_value)
    int splitIndex = -1, splitValue = -1;
    double entropy, IG, IGmax = 0;
    bool BadSplit = true;
    // Pentru fiecare element din dimensions se aleg samples-urile unice,
    // si pentru fiecare element unic se face split pe indecsi, apoi se aplica
    // formula data pentru calcularea Information Gain-ului. La final se
    // salveaza split-ul cu IG maxim.
    entropy = get_entropy(samples);
    for (auto it = dimensions.begin(); it != dimensions.end(); ++it) {
        auto unique = compute_unique(samples, *it);
        for (auto it2 = unique.begin(); it2 != unique.end(); ++it2) {
            auto split = get_split_as_indexes(samples, *it, *it2);
            if (!split.first.size() || !split.second.size())
                continue;
            else
                BadSplit = false;
            IG = split.first.size()*get_entropy_by_indexes(samples, split.first)
            +split.second.size()*get_entropy_by_indexes(samples, split.second);
            IG = entropy - IG/unique.size();
            if (IG > IGmax) {
                IGmax = IG;
                /*splitIndex = *it;
                splitValue = *it2;*/
            }
        }
    }
    if (BadSplit)
        return pair<int, int>(-1, -1);
    else
        return pair<int, int>(splitIndex, splitValue);
}

void Node::train(const vector<vector<int>> &samples) {
    // TODO(you)
    // Antreneaza nodul curent si copii sai, daca e nevoie
    // 1) verifica daca toate testele primite au aceeasi clasa (raspuns)
    // Daca da, acest nod devine frunza, altfel continua algoritmul.
    // 2) Daca nu exista niciun split valid, acest nod devine frunza. Altfel,
    // ia cel mai bun split si continua recursiv
    pair<int, int> pereche;
    if (same_class(samples)) {
        make_leaf(samples, true);
        return;
    }
    else
        pereche = find_best_split(samples,
                                  random_dimensions(samples[0].size()));
    if (pereche.first == -1 && pereche.second == -1) {
        make_leaf(samples, false);
        return;
    }
    else
    {
        // Daca nodul nu este frunza inseamna ca este nod de decizie, se aloca
        // spatiu pentru copii si se porneste recursiv pe acestia.
        make_decision_node(pereche.first - 1, pereche.second);
        auto NodePair = split(samples, pereche.first - 1, pereche.second);
        if (left == nullptr) {
            left = make_shared<Node>();
            left->train(NodePair.first);
        }
        else
            left->train(samples);
        if (right == nullptr) {
            right = make_shared<Node>();
            right->train(NodePair.second);
        }
        else
            right->train(samples);
    }
}
int Node::predict(const vector<int> &image) const {
    // TODO(you)
    // Intoarce rezultatul prezis de catre decision tree
    auto it = image.begin();
    std::shared_ptr<Node> temp;
    // Pentru nodul de plecare se verifica daca este frunza si se incepe
    // parcurgerea arborelui, punandu-se conditia ceruta.
    if (!is_leaf) {
        if (it[split_index] <= split_value)
            temp = temp->left;
        else
            temp = temp->right;
    }
    else
        return result;
    // Cat timp nu s-a ajuns la o frunza se parcurge arborele si atunci cand
    // se ajunge la o frunza.
    while (!temp->is_leaf) {
        if (it[temp->split_index] <= temp->split_value) {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    temp = nullptr;
    return temp->result;
}

bool same_class(const vector<vector<int>> &samples) {
    // TODO(you)
    // Verifica daca testele primite ca argument au toate aceeasi
    // clasa(rezultat). Este folosit in train pentru a determina daca
    // mai are rost sa caute split-uri
    // Se verifica primul element din fiecare vector din samples.
    for (auto it = samples.begin(); it != samples.end(); ++it)
        if (samples[0][0] != (*it)[0])
            return false;
    return true;
}

float get_entropy(const vector<vector<int>> &samples) {
    // Intoarce entropia testelor primite
    assert(!samples.empty());
    vector<int> indexes;

    int size = samples.size();
    for (int i = 0; i < size; i++) indexes.push_back(i);

    return get_entropy_by_indexes(samples, indexes);
}

float get_entropy_by_indexes(const vector<vector<int>> &samples,
                             const vector<int> &index) {
    // TODO(you)
    // Intoarce entropia subsetului din setul de teste total(samples)
    // Cu conditia ca subsetul sa contina testele ale caror indecsi se gasesc in
    // vectorul index (Se considera doar liniile din vectorul index)
    double entropy = 0, pi;
    // Daca samples-ul nu este gol si daca pi nu este 0 se calculeaza entropia
    // prin formula data.
    for (auto it1 = index.begin(); it1 != index.end(); ++it1) {
        if (samples[*it1].size() > 0) {
            pi = static_cast<double>((*it1))/
                 static_cast<double>(samples[*it1].size());
            if (pi > 0)
                entropy += pi * log(pi);
        }
    }
    return -entropy;
}

vector<int> compute_unique(const vector<vector<int>> &samples, const int col) {
    // TODO(you)
    // Intoarce toate valorile (se elimina duplicatele)
    // care apar in setul de teste, pe coloana col
    vector<int> uniqueValues;
    for (auto it = samples.begin(); it != samples.end(); ++it)
        uniqueValues.push_back((*it)[col]);
    std::sort(uniqueValues.begin(), uniqueValues.end());
    auto it = std::unique(uniqueValues.begin(), uniqueValues.end());
    uniqueValues.resize(std::distance(uniqueValues.begin(), it));
    return uniqueValues;
}

pair<vector<vector<int>>, vector<vector<int>>> split(
    const vector<vector<int>> &samples, const int split_index,
    const int split_value) {
    // Intoarce cele 2 subseturi de teste obtinute in urma separarii
    // In functie de split_index si split_value
    vector<vector<int>> left, right;

    auto p = get_split_as_indexes(samples, split_index, split_value);
    for (const auto &i : p.first) left.push_back(samples[i]);
    for (const auto &i : p.second) right.push_back(samples[i]);

    return pair<vector<vector<int>>, vector<vector<int>>>(left, right);
}

pair<vector<int>, vector<int>> get_split_as_indexes(
    const vector<vector<int>> &samples, const int split_index,
    const int split_value) {
    // TODO(you)
    // Intoarce indecsii sample-urilor din cele 2 subseturi obtinute in urma
    // separarii in functie de split_index si split_value
    vector<int> left, right;
    int cnt = 0;

    // Pentru fiecare vector din samples se verifica primul element prin
    // conditia data si se adauga la stanga sau la dreapta.
    for (auto it = samples.begin(); it != samples.end(); ++it, cnt++) {
        auto it2 = (*it).begin();
        if (it2[split_index] <= split_value)
            left.push_back(cnt);
        else
            right.push_back(cnt);
    }
    return make_pair(left, right);
}

vector<int> random_dimensions(const int size) {
    // TODO(you)
    // Intoarce sqrt(size) dimensiuni diferite pe care sa caute splitul maxim
    // Precizare: Dimensiunile gasite sunt > 0 si < size
    vector<int> rez;
    int DimensionSize = floor(sqrt(size));
    int number;
    bool NumarUnic;
    unsigned int seed = time(NULL);

    // Cat timp mai este nevoie de dimensiuni.
    while (DimensionSize--) {
        // Cat timp nu s-a gasit un numar nou unic.
        while (true) {
            NumarUnic = false;
            number = rand_r(&seed) % size;
            while (!number) {
                number = rand_r(&seed) % size;
                seed++;
            }
            // Daca dimensiunea nu este unica se genereaza alt numar.
            for (auto it = rez.begin(); it != rez.end(); ++it) {
                if (*it == number) {
                    NumarUnic = true;
                    break;
                }
            }
            if (!NumarUnic)
                break;
        }
        rez.push_back(number);
    }
    return rez;
}
