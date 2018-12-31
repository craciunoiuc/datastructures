// "Copyright [2018] <Craciunoiu Cezar>"
#ifndef STACK_H_
#define STACK_H_

template<typename T>
class Stack {
 private:
    T* Array;
    int MaxArray, CurrentArray;

 public:
    // Se aloca dinamic un vector de 2 elemente ce va fi
    // redimensionat in functie de nevoie.
    Stack() {
        MaxArray = 2;
        Array = new T[MaxArray];
        CurrentArray = 0;
    }

    // Se elibereaza memoria.
    ~Stack() {
        MaxArray = 2;
        CurrentArray = 0;
        delete[] Array;
    }

    // Metoda prin care se adauga elemente in stiva. Daca vectorul este plin se
    // redimensioneaza dublandui-se marimea. Se utilizeaza un buffer pentru
    // redimensionare ce este eliberat apoi. Se adauga apoi elementul in stiva.
    void push(T x) {
        int i = 0;
        if (CurrentArray == MaxArray){
            T* BufferArray = new T[MaxArray];
            while (i < MaxArray){
                BufferArray[i] = Array[i];
                i++;
            }
            delete[] Array;
            MaxArray *= 2;
            Array = new T[MaxArray];
            i = 0;
            while (i < CurrentArray){
                Array[i] = BufferArray[i];
                i++;
            }
            delete[] BufferArray;
        }
        for (i = CurrentArray-1; i >= 0; i--){
            Array[i+1] = Array[i];
        }
        CurrentArray++;
        Array[0] = x;
    }

    // Daca stiva este goala se afiseaza o eroare.
    // Se sterge elementul din stiva, iar daca apoi aceasta este mai mult
    // de jumatate goala se redimensioneaza folosind un vector de buffer.
    void pop() {
        if (CurrentArray == 0){
            return;
        }
        else
        {
            int i;
            for (i = 0; i < CurrentArray-1; i++){
                Array[i] = Array[i+1];
            }
            CurrentArray--;
            float size;
            if (CurrentArray > 0)
                size = MaxArray/CurrentArray;
            if (CurrentArray && size > 2.1){
                MaxArray /= 2;
                T* BufferArray = new T[MaxArray];
                i = 0;
                while (i < CurrentArray){
                    BufferArray[i] = Array[i];
                    i++;
                }
                delete[] Array;
                MaxArray /= 2;
                Array = new T[MaxArray];
                i = 0;
                while (i < CurrentArray){
                    Array[i] = BufferArray[i];
                    i++;
                }
                delete[] BufferArray;
            }
        }
    }

    // Se consulta varful stivei, daca aceasta este goala
    // se returneaza o eroare.
    T top() {
        if (CurrentArray > 0)
            return Array[0];
        else
        {
            T errcode;
            return errcode;
        }
    }

    // Se verifica daca stiva este goala.
    bool empty() {
        if (CurrentArray == 0)
            return true;
        else
            return false;
    }
};
#endif  // STACK_H_
