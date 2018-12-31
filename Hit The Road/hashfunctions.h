// Copyright [2018] <Craciunoiu Cezar>
#ifndef HASHFUNCTIONS_H_
#define HASHFUNCTIONS_H_
#define GOLDENRATIO 1.61803398875
#include <string>

// Functie care primeste un string si returneaza un int.
unsigned int functieString(string& Encode, unsigned int MaxLength){
    unsigned int hash = 0;

    // Codul ASCII al fiecarui caracter este inmulit cu 31 si adunat la hash.
    for (unsigned int i = 0; i != Encode.length() ; ++i) {
        hash += 31 * Encode[i];
    }

    // Se inmulteste cu numarul de aur pentru a modifica hash-ul, se observa ca
    // hashul este modificat mai mult daca are o valoare mai mare.
    hash *= GOLDENRATIO;

    // Se aduna lungimea inmultita cu primul caracter pentru a face diferenta
    // intre mai multe siruri de lungimi diferite si acelasi hash.
    hash += Encode.length() * (int)Encode[0];

    // Se aduna si ultima cifra din acelasi motiv, pentru a asigura un hash
    // cat mai uniform.
    hash += (int)Encode[Encode.length()-1];

    // In final se truncheaza la marimea dorita.
    return hash % MaxLength;
}

// Functie care primeste un nod si returneaza un int. Se urmeaza aceeasi pasi
// ca pentru cea care lucreaza cu string.
unsigned int functieNode(Node<string, Einfo>& Encode, unsigned int MaxLength){
    unsigned int hash = 0;
    for (unsigned int i = 0; i != Encode.nume.length() ; ++i) {
        hash += 31 * Encode.nume[i];
    }
    hash *= GOLDENRATIO;
    hash += Encode.nume.length() * (int)Encode.nume[0];
    hash += (int)Encode.nume[Encode.nume.length()-1];
    return hash % MaxLength;
}

// Functie care primeste un int si returneaza un int pozitiv trunchiat.
unsigned int functieInt(int Encode, unsigned int MaxLength){
    if (Encode < 0) {
        Encode = -Encode;
    }
    return Encode % MaxLength;
}
#endif  // HASHFUNCTIONS_H_
