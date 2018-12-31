// Copyright [2018] <Craciunoiu Cezar>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>  // inclusa pentru sortare

using std::endl;
using std::cin;
using std::vector;
using std::string;
using std::list;
using std::ofstream;

// Extra informatie a nodurilor
struct Einfo{
    bool benzinarie = false;
    bool vizitat = false;
};

// Functie de vizitare a nodurilor.
bool Vizitare(bool choice, Einfo* TaskDone) {
    if (choice){
        TaskDone->vizitat = true;
        return false;
    }
    else
    {
        if (!TaskDone->vizitat)
            return true;
        else
            return false;
    }
}
#include "./hash.h"
#include "./graph.h"
#include "./hashfunctions.h"
#include "./HTRoad.h"

int main() {
    ofstream fout("task1.out");
    int Orase, Autostrazi, i;
    string muchie;
    Node<string, Einfo> buffer;  // utilizat pentru adaugarea nodurilor in Graf.
    list<Node<string, Einfo>>* temp;
    string start;  // se salveaza startul pentru DFS.

    // Se citeste numarul de Orase si Autostrazi.
    cin >> Orase >> Autostrazi;

    // Se initializeaza 2 grafuri, unul normal si unul transpus.
    Graph<string, Einfo, unsigned int>
        Graf(&functieString, &functieNode, Orase, &Vizitare),
        GrafTranspus(&functieString, &functieNode, Orase, &Vizitare);

    // Se citesc si se adauga nodurile in Graf/graful transpus
    for (i = 0; i < Orase; ++i){
        cin >> buffer.nume;
        if (i == 0)
            start = buffer.nume;
        Graf.addNode(buffer);
        GrafTranspus.addNode(buffer);
    }
    // Se citesc si se adauga muchiile in graf/graful transpus.
    for (i = 0; i < Autostrazi; ++i){
        cin >> muchie >> buffer.nume;
        Graf.addEdge(buffer.nume, muchie);
        GrafTranspus.addEdge(muchie, buffer.nume);
    }
    // Se apeleaza functia ce rezolva primul Task.
    Task1(Orase, &Graf, fout);
    fout.close();

    fout.open("task2.out");

    // Se salveaza rezultatul celui de-al doilea task pentru afisare.
    bool Task2Result = Task2(Orase, &Graf, &GrafTranspus, start);
    if (Task2Result)
        fout << "HARTA VALIDA";
    else
        fout << "HARTA INVALIDA";
    fout.close();

    fout.open("task3.out");
    int NumarBenzinarii;
    // Se citesc si se introduc benzinariile in tabela.
    cin >> NumarBenzinarii;
    for (i = 0; i < NumarBenzinarii; ++i) {
        cin >> buffer.nume;
        temp = Graf.Ptabela()->GetValueP(functieNode(buffer, Orase*1.35));
        for (auto it = temp->begin(); it != temp->end(); ++it) {
            if ((*it).nume == buffer.nume){
                (*it).info.benzinarie = true;
                break;
            }
        }
    }

    int NrVerificari;
    bool TaskDone = false, EndlReq = false;
    string NoAccess[Orase];
    cin >> NrVerificari;

    // Se ruleaza un functia Task3 pentru fiecare oras dat, iar daca anterior
    // s-a determinat ca harta este valida se afiseaza direct mesajul, trecand
    // peste rulare. Pentru fiecare oras se afiseaza orasele la care nu are
    // acces.
    for (i = 0; i < NrVerificari; ++i){
        cin >> start;
        if (TaskDone)
            continue;
        if (Task2Result){
            fout << "Exista drum intre oricare doua Orase";
            TaskDone = true;
        }
        EndlReq = Task3(Orase, start, fout, NoAccess, &Graf, &GrafTranspus);
        if (EndlReq){
            fout << endl;
        }
    }
    fout.close();
    return 0;
}
