// "Copyright [2018] <Craciunoiu Cezar>"

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>

using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;

#include "./stack.h"
#include "./deque.h"
#include "./robot.h"

int main(){
    ifstream fin("robots.in");
    ofstream fout("robots.out");
    int N, LIN, COL;
    int i, j;
    string buffer;

    fin >> N >> LIN >> COL;

    // Se aloca dinamic o matrice.
    int** map = new int*[LIN];
    for (i = 0; i < LIN; i++)
        map[i] = new int[COL];

    // Cu ajutorul vectorului intbuff o sa se citeasca elementele, fiind
    // maxim 5 pe un rand. Se apeleaza clasa robot pentru N roboti,
    // fiecare cu coada lui. De asemenea se apeleaza clasa Stack pentru
    // 2 stive: una in care se tin comenzile executate, "Executed",
    // si una, "All", unde se tin laolalta comezile de DROP, GET si EXECUTE.
    int intbuff[5];
    Robot<int, string> robots[N];
    Stack<Cell<int, string>> Executed, All;

    // Se seteaza id-ul robotilor.
    for (i = 0; i < N; i++){
        robots[i].SetId(i);
    }

    // Se citeste matricea.
    for (i = 0; i < LIN; i++){
        for (j = 0; j < COL; j++){
            fin >> map[i][j];
        }
    }

    // While ce functioneaza cat timp mai exista lucruri de citit in fisier.
    while (fin >> buffer){
        // Variabila booleana ce se utilieaza pentru a se evita evaluarea
        // unui strcmp daca deja s-a executat o comanda anterior.
        bool PasComplet = true;

        // Se incrementeaza timpul din cozile robotilor.
        for (i = 0; i < N; i++)
            robots[i].Increment();

        // Urmeaza compararea bufferului cu diferitele comenzi
        // Daca este de tip GET sau DROP atunci se citesc 4 elemente si
        // se adauga in coada respectivului robot "intbuff[0]"
        if (strcmp(buffer.c_str(), "ADD_GET_BOX") == 0 ||
            strcmp(buffer.c_str(), "ADD_DROP_BOX") == 0){
            fin >> intbuff[0] >> intbuff[1] >> intbuff[2]
                >> intbuff[3] >> intbuff[4];
            Cell<int, string> temp;

            if (strcmp(buffer.c_str(), "ADD_GET_BOX") == 0){
                temp.CommandType = "GET";
                robots[intbuff[0]].
                Add("GET", intbuff[1], intbuff[2], intbuff[3], intbuff[4]);
            }
            else
            {
                temp.CommandType = "DROP";
                robots[intbuff[0]].
                Add("DROP", intbuff[1], intbuff[2], intbuff[3], intbuff[4]);
            }

            // Se pune temp in All, folosind campul x pentru Priority.
            temp.id = intbuff[0];
            temp.x = intbuff[4];
            All.push(temp);
            PasComplet = false;
        }
        // Se adauga comanda in All si apoi se apleaza metoda Execute.
        if (PasComplet && strcmp(buffer.c_str(), "EXECUTE") == 0){
            fin >> intbuff[0];
            Cell<int, string> temp;
            temp.CommandType = "EXECUTE";
            temp.id = intbuff[0];
            All.push(temp);

            if (strcmp(buffer.c_str(), "EXECUTE") == 0)
                robots[intbuff[0]].Execute(map, &Executed, &All, fout);
            PasComplet = false;
        }

        // Se apeleaza metoda PrintCommands in functie de id.
        if (PasComplet && strcmp(buffer.c_str(), "PRINT_COMMANDS") == 0){
            fin >> intbuff[0];
            fout << "PRINT_COMMANDS: ";
            robots[intbuff[0]].PrintCommands(fout);
            PasComplet = false;
        }

        // Se afiseaza numarul de cutii in functie de id.
        if (PasComplet && strcmp(buffer.c_str(), "HOW_MANY_BOXES") == 0){
            fin >> intbuff[0];
            fout << "HOW_MANY_BOXES: "
            << robots[intbuff[0]].HowManyBoxes() << endl;
            PasComplet = false;
        }

        // Se apeleaza metoda si daca se returneaza eroarea prin id == -1
        // atunci se afiseaza mesajul, daca nu, se afiseaza ultima comanda.
        if (PasComplet && strcmp(buffer.c_str(), "LAST_EXECUTED_COMMAND") == 0){
            Cell<int, string> temp;
            temp = robots[0].LastCommand(&Executed);
            if (temp.id == -1){
                fout << "LAST_EXECUTED_COMMAND: No command was executed"
                     << endl;
            }
            else
            {
                fout << "LAST_EXECUTED_COMMAND: " << temp.id << ": "
                     << temp.CommandType << " " << temp.x << " " << temp.y
                     << " " << temp.NrBoxes << endl;
            }
            PasComplet = false;
        }

        // Daca ultima comanda data este UNDO, se citeste in temp ultima
        // comanda din All si in temp2 ultima comanda din Executed. Se salveaza
        // datele din temp2 in temp3, se inverseaza comanda de GET/DROP in
        // DROP/GET. Se adauga comanda in coada si se scoate executia ei din
        // All si Executed. Se executa comanda inversata si se sterge aceasta
        // din Executed. Se adauga comanda initiala in coada pentru a se reveni
        // la un pas anterior: "ca si cum nu s-ar fi executat". Pe scurt, temp
        // este comanda executata, temp2 comanda ce se executa inversat, si
        // temp3 vechea comanda ce a fost executata mai demult.
        if (PasComplet && strcmp(buffer.c_str(), "UNDO") == 0){
            Cell<int, string> temp = All.top();

            if (strcmp(temp.CommandType.c_str(), "EXECUTE") == 0){
                Cell<int, string> temp2 = Executed.top();
                Cell<int, string> temp3 = temp2;
                temp3.time = 0;

                if (strcmp(temp2.CommandType.c_str(), "DROP") == 0){
                    temp2.CommandType = "GET";
                }
                else
                    temp2.CommandType = "DROP";

                robots[temp2.id].UndoExe(temp2);
                Executed.pop();
                All.pop();
                robots[temp2.id].Execute(map, &Executed, &All, fout);
                Executed.pop();
                robots[temp3.id].
                Add(temp3.CommandType, temp3.x, temp3.y, temp3.NrBoxes, 0);
            }
            else
            {
                // Daca ultima comanda data este GET/DROP se scoate din coada,
                // daca nu, atunci inseamna ca nu s-au executat comenzi.
                if (strcmp(temp.CommandType.c_str(), "GET") == 0 ||
                strcmp(temp.CommandType.c_str(), "DROP") == 0){
                    robots[temp.id].UndoAdd(temp.x);
                    All.pop();
                }
                else
                    fout << "UNDO: No History" << endl;
            }
            PasComplet = false;
        }

        // Se afiseaza timpul.
        if (PasComplet && strcmp(buffer.c_str(), "HOW_MUCH_TIME") == 0){
            if (!Executed.empty())
                fout << "HOW_MUCH_TIME: "
                     << Executed.top().time << endl;
            else
                fout << "HOW_MUCH_TIME: "
                     << "No command was executed" << endl;
        }
    }

    // Se elibereaza memoria matricei.
    for (i = 0; i < LIN; i++)
        delete[] map[i];
    delete[] map;
    return 0;
}
