// Copyright [2018] <Craciunoiu Cezar>
#ifndef HTROAD_H_
#define HTROAD_H_

#include <list>
#include <string>
#include <algorithm>

// Functie ce rezolva primul task
void Task1(unsigned int Orase, Graph<string, Einfo, unsigned int> *Graf,
           ofstream& fout){
    unsigned int ValoareNodMaxim = 0;
    string NumeNodMaxim;
    list<Node<string, Einfo>>* temp;
    Hashtable<Node<string, Einfo>, unsigned int>* tabela = Graf->Ptabela();

    // Se trece prin fiecare element al hashtable ului si se verifica
    // daca acesta are numar mai mare de muchii se salveaza.
    for (unsigned int i = 0; i < static_cast<int>(Orase*1.35); ++i){
        // Se utilizeaza un pointer la tabela pentru a nu lucra cu multe
        // apeluri la functia GetValueP.
        temp = tabela->GetValueP(i);
        if (temp->empty())
            continue;
        // Se cauta nodul cu lista de vecini de marime maxima.
        for (auto it1 = temp->begin(); it1 != temp->end(); ++it1){
            if ((*it1).muchii.size() > ValoareNodMaxim){
                ValoareNodMaxim = (*it1).muchii.size();
                NumeNodMaxim = (*it1).nume;
            }
            else
            {
                if ((*it1).muchii.size() == ValoareNodMaxim){
                    if (NumeNodMaxim.compare((*it1).nume) > 0)
                        NumeNodMaxim = (*it1).nume;
                }
            }
        }
    }
    fout << NumeNodMaxim << " " << ValoareNodMaxim << endl;
}

// Functia ce returneaza true daca graful este tare conex
bool Task2(unsigned int Orase, Graph<string, Einfo, unsigned int> *Graf,
     Graph<string, Einfo, unsigned int> *GrafTranspus, string& start) {
    list<Node<string, Einfo>>* temp;
    // Se porneste un DFS de la un nod anume de start
    Graf->DFS(Orase, &start);

    // Daca au ramas noduri nevizitate se returneaza false
    for (int i = 0; i < static_cast<int>(Orase*1.35); i++){
        temp = Graf->Ptabela()->GetValueP(i);
        if (temp->empty())
            continue;
        for (auto it1 = temp->begin(); it1 != temp->end(); ++it1){
            if (!(*it1).info.vizitat){
                return false;
            }
        }
    }
    // Se porneste un DFS din acelasi nod pe graful transpus
    GrafTranspus->DFS(Orase, &start);

    // Daca nu au fost vizitate toate muchiile se returneaza false
    for (int i = 0; i < static_cast<int>(Orase*1.35); i++){
        temp = GrafTranspus->Ptabela()->GetValueP(i);
        if (temp->empty())
            continue;
        for (auto it1 = temp->begin(); it1 != temp->end(); ++it1){
            if (!(*it1).info.vizitat){
                return false;
            }
        }
    }
    return true;
}
// Functie returneaza false daca nu exista Orase la care nu se poate ajunge.
bool Task3(unsigned int Orase, string& start, ofstream& fout,
           string* NoAccess, Graph<string, Einfo, unsigned int> *Graf,
           Graph<string, Einfo, unsigned int> *GrafTranspus){
    list<Node<string, Einfo>>* temp, *tempT;
    int cnt = 0;
    bool inaccesibile = false;

    // Se marcheaza toate nodurile ca nevizitate in graf.
    for (int i = 0; i < static_cast<int>(Orase*1.35); i++){
        temp = Graf->Ptabela()->GetValueP(i);
        if (temp->empty())
            continue;
        for (auto it1 = temp->begin(); it1 != temp->end(); ++it1){
            (*it1).info.vizitat = false;
        }
    }

    // Se marcheaza toate nodurile ca nevizitate in graful transpus.
    for (int i = 0; i < static_cast<int>(Orase*1.35); ++i){
        temp = GrafTranspus->Ptabela()->GetValueP(i);
        if (temp->empty())
            continue;
        for (auto it1 = temp->begin(); it1 != temp->end(); ++it1){
            (*it1).info.vizitat = false;
        }
    }

    // Se ruleaza un DFS pe fiecare tip de graf.
    Graf->DFS(Orase, &start);
    GrafTranspus->DFS(Orase, &start);

    // Se trece prin fiecare nodul fiecarui graf in acelasi timp si daca
    // pe primul sau pe al doilea graf exista un nod nevizitat se trece
    // intr-un array de string-uri.
    for (int i = 0; i < static_cast<int>(Orase*1.35); ++i){
        temp = Graf->Ptabela()->GetValueP(i);
        tempT = GrafTranspus->Ptabela()->GetValueP(i);
        if (temp->empty())
            continue;
        for (auto it1 = temp->begin(), it2 = tempT->begin();
             it1 != temp->end(); ++it1, ++it2){
            if (!(*it1).info.vizitat || !(*it2).info.vizitat){
                if (!inaccesibile){
                    fout << "Orase inaccesibile pentru " << start << ": ";
                    inaccesibile = true;
                }
                NoAccess[cnt] = (*it1).nume;
                cnt++;
            }
        }
    }

    // Se sorteaza array-ul de string-uri.
    std::sort(NoAccess, NoAccess+cnt);

    // Se afiseaza array-ul de string-uri.
    if (inaccesibile){
        for (int i = 0; i < cnt; ++i) {
            fout << NoAccess[i] << " ";
        }
    }
    return inaccesibile;
}
#endif  // HTROAD_H_
