// "Copyright [2018] <Craciunoiu Cezar>"
#ifndef DEQUE_H_
#define DEQUE_H_

// Structura unui nod.
template <typename T>
struct Node {
    T data;
    Node<T> *next;
    Node<T> *previous;

    explicit Node(T data) {
        this->data = data;
        next = nullptr;
        previous = nullptr;
    }
};

template <typename T>
class DoublyLinkedList {
 private:
    Node<T> *head;
    Node<T> *tail;
    int NrElem;

 public:
    // Constructori.
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
        NrElem = 0;
    }

    explicit DoublyLinkedList(Node<T> *head) {
        this->head = head;
        while (head != nullptr) {
            NrElem++;
            head = head->next;
        }
    }

    // Destructor ce elibereaza memoria.
    ~DoublyLinkedList() {
        Node<T>* temp;
        while (head != nullptr){
            temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Se aloca spatiu pentru un nod nou si se trateaza pe cazuri:
    // Cazul in care nu sunt elemente, cand este doar 1 si cand sunt mai multe.
    // In fiecare caz se fac legaturile.
    void addLast(T data) {
        Node<T>* temp = new Node<T>(data);
        temp->data = data;
        temp->next = nullptr;
        if (NrElem == 0){
            temp->previous = nullptr;
            head = temp;
            tail = temp;
        }
        else
        {
            if (NrElem == 1){
                temp->previous = tail;
                head->next = temp;
            }
            else
            {
                temp->previous = tail;
                tail->next = temp;
            }
            tail = temp;
        }
        NrElem++;
    }

    // Se procedeaza ca la addLast, dar actiunile se realizeaza la inceput.
    void addFirst(T data) {
        Node<T>* temp = new Node<T>(data);
        temp->data = data;
        temp->previous = nullptr;
        if (NrElem == 0) {
            temp->next = nullptr;
            head = temp;
            tail = temp;
        }
        else
        {
            if (NrElem == 1) {
                temp->next = head;
                head->previous = temp;
            }
            else
            {
                temp->next = head;
                head->previous = temp;
            }
            head = temp;
        }
        NrElem++;
    }

    // Se salveaza informatia din nod, se rescriu legaturile iar tail devine
    // elementul de inainte.
    void removeLast() {
        Node<T>* temp;
        if (NrElem == 1) {
            head = nullptr;
            delete tail;
            tail = nullptr;
        }
        else
        {
            tail->previous->next = nullptr;
            temp = tail;
            tail = tail->previous;
            delete temp;
        }
        NrElem--;
    }

    // Se procedeaza ca la removeLast, penultimul element devenind tail.
    void removeFirst() {
        Node<T>* temp;
        if (NrElem == 1) {
            tail = nullptr;
            delete head;
            head = nullptr;
        }
        else
        {
            head->next->previous = nullptr;
            temp = head;
            head = head->next;
            delete temp;
        }
        NrElem--;
    }

    // Returneaza primul element din coada.
    T showFirst() {
        return head->data;
    }

    // Returneaza ultimul element din coada.
    T showLast() {
        return tail->data;
    }

    // Returneaza daca lista este goala.
    bool isEmpty() {
        if (NrElem)
            return false;
        else
            return true;
    }

    // Returneaza marimea listei.
    int size() {
        return NrElem;
    }
};
#endif  // DEQUE_H_
