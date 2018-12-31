// "Copyright [2018] <Craciunoiu Cezar>"
#ifndef ROBOT_H_
#define ROBOT_H_

// Structura unei celule in care se tin datele.
// Se utilizeaza constructori si un operator de atribuire pentru aceasta.
template <typename T, typename C>
struct Cell{
    C CommandType;
    T NrBoxes;
    T x, y;
    T id;
    T time = 0;
    Cell(){
    }
    Cell(T id, C CommandType, T x, T y, T NrBoxes){
        this->id = id;
        this->CommandType = CommandType;
        this->x = x;
        this->y = y;
        this->NrBoxes = NrBoxes;
        this->time = 0;
    }
    void operator=(const Cell<T, C>& d){
        id = d.id;
        CommandType = d.CommandType;
        x = d.x;
        y = d.y;
        NrBoxes = d.NrBoxes;
        time = d.time;
    }
};

template <typename T, typename C>
class Robot {
 private:
    T NrBoxes;
    T id;
    DoublyLinkedList<Cell<T, C>> queue;

 public:
    Robot(){
        NrBoxes = 0;
    }

    ~Robot(){
    }

    // Se structureaza datele ce urmeaza sa fie introduse in coada si se aseaza
    // in functie de prioritate la inceputul sau sfarsitul cozii.
    void Add(C CommandType, T x, T y, T NrBoxes, T Priority){
        Cell<T, C> celula;

        // Se utilizeaza functia c_str pentru a compara cele 2 stringuri
        // fara a genera erori de conversie de la string la const char*.
        if (strcmp(CommandType.c_str(), "GET") == 0)
           celula = Cell<T, C>(id, "GET", x, y, NrBoxes);
        else
           celula = Cell<T, C>(id, "DROP", x, y, NrBoxes);
        if (Priority)
            queue.addLast(celula);
        else
            queue.addFirst(celula);
    }

    // Metoda Execute este singura care influenteaza direct matricea.
    void Execute(T** map, Stack<Cell<T, C>>* Executed,
                 Stack<Cell<T, C>>* All, ofstream& fout){
        // Daca coada este goala se afiseaza mesajul de eroare
        // si se scoate din All comanda.
        if (queue.isEmpty()){
            fout << "EXECUTE: No command to execute" << endl;
            All->pop();
            return;
        }
        Cell<T, C> celula = queue.showFirst();
        queue.removeFirst();

        // Daca comanda este de GET, atunci daca nu sunt destule cutii se aduna
        // cate cutii sunt in harta, daca sunt, se aduna numarul dorit.
        // Altfel comanda este de DROP si se realizeaza procedeul invers ca la
        // GET. La final se adauga in stiva comenzilor executate.
        if (strcmp(celula.CommandType.c_str(), "GET") == 0){
            if (celula.NrBoxes - map[celula.x][celula.y] > 0){
                this->NrBoxes += map[celula.x][celula.y];
                celula.NrBoxes = map[celula.x][celula.y];
                map[celula.x][celula.y] = 0;
            }
            else
            {
                this->NrBoxes += celula.NrBoxes;
                map[celula.x][celula.y] -= celula.NrBoxes;
            }
        }
        else
        {
            if (this->NrBoxes - celula.NrBoxes < 0){
                map[celula.x][celula.y] += this->NrBoxes;
                celula.NrBoxes = this->NrBoxes;
                this->NrBoxes = 0;
            }
            else
            {
                map[celula.x][celula.y] += celula.NrBoxes;
                this->NrBoxes -= celula.NrBoxes;
            }
        }
        Executed->push(celula);
    }

    // Se cauta ultima comanda executata in stiva, daca stiva este goala
    // se returneaza o eroare semnalata prin id-ul invalid -1.
    Cell<T, C> LastCommand(Stack<Cell<T, C>>* Executed){
        if (Executed->empty()){
            Cell<T, C> err;
            err.id = -1;
            return err;
        }
        return Executed->top();
    }

    // Pentru a se putea afisa toate elementele se afiseaza primul element
    // apoi este mutat la sfarsitul cozii. Se face acest lucru de queue.size()
    // ori pentru a se aduce la forma initiala coada.
    void PrintCommands(ofstream& fout){
        int i = queue.size();
        if (i == 0){
            fout << "No command found" << endl;
            return;
        }
        Cell<T, C> temp;
        fout << id << ": ";

        while (i != 1){
            temp = queue.showFirst();
            fout << temp.CommandType << " " << temp.x << " "
                 << temp.y << " " << temp.NrBoxes << "; ";
            queue.removeFirst();
            queue.addLast(temp);
            i--;
        }
        temp = queue.showFirst();
        fout << temp.CommandType << " " << temp.x << " "
             << temp.y << " " << temp.NrBoxes << endl;
        queue.removeFirst();
        queue.addLast(temp);
    }

    // Returneaza numarul de cutii ce le are robotul.
    T HowManyBoxes(){
        return NrBoxes;
    }

    // Functie ce atribuie id-ul fiecarui robot, utila la inceput.
    void SetId(T id){
        this->id = id;
    }

    // Se adauga la inceput comanda de execute la care s-a dat undo.
    void UndoExe(Cell <T, C> temp){
        queue.addFirst(temp);
    }

    // Daca ultima comanda adaugata a fost cu prioritate 0 se elimina primul nod
    // din coada, daca nu, ultimul.
    void UndoAdd(T priority){
        if (priority)
            queue.removeLast();
        else
            queue.removeFirst();
    }

    // Se citeste primul element si se scoate din coada, se incrementeaza
    // timpul cu 1 si apoi se pune la final realizandu-se queue.size() pasi.
    void Increment(){
        Cell<T, C> temp;
        int i = queue.size();
        while (i--){
            temp = queue.showFirst();
            queue.removeFirst();
            temp.time++;
            queue.addLast(temp);
        }
    }
};

#endif  // ROBOT_H_
