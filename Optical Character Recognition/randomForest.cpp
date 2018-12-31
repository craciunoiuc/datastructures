// copyright Luca Istrate, Andrei Medar
#include "randomForest.h"
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include "decisionTree.h"

using std::vector;
using std::pair;
using std::string;
using std::mt19937;

vector<vector<int>> get_random_samples(const vector<vector<int>> &samples,
                                       int num_to_return) {
    // TODO(you)
    // Intoarce un vector de marime num_to_return cu elemente random,
    // diferite din samples
    vector<vector<int>> ret;
    unsigned int seed = time(NULL);
    int kMarime = samples.size(), linie;
    bool ales[kMarime];
    for (int i = 0; i < kMarime; ++i)
        ales[i] = false;

    // Se utilizeaza un vector de bool pentru a alege doar elemente unice. Daca
    // elementul generat random nu este unic se parcurge vectorul liniar pana
    // cand se gaseste unul nefolosit.
    while (num_to_return--) {
        linie = rand_r(&seed) % kMarime;
        while (true) {
            if (!ales[linie]) {
                ales[linie] = true;
                break;
            }
            else
            {
                while (ales[linie]) {
                    linie++;
                    linie %= kMarime;
                }
                ales[linie] = true;
                break;
            }
        }
        ret.push_back(samples[linie]);
    }
    return ret;
}

RandomForest::RandomForest(int num_trees, const vector<vector<int>> &samples)
    : num_trees(num_trees), images(samples) {}

void RandomForest::build() {
    // Aloca pentru fiecare Tree cate n / num_trees
    // Unde n e numarul total de teste de training
    // Apoi antreneaza fiecare tree cu testele alese
    assert(!images.empty());
    vector<vector<int>> random_samples;

    int data_size = images.size() / num_trees;

    for (int i = 0; i < num_trees; i++) {
        // cout << "Creating Tree nr: " << i << endl;
        random_samples = get_random_samples(images, data_size);

        // Construieste un Tree nou si il antreneaza
        trees.push_back(Node());
        trees[trees.size() - 1].train(random_samples);
    }
}

int RandomForest::predict(const vector<int> &image) {
    // TODO(you)
    // Va intoarce cea mai probabila prezicere pentru testul din argument
    // se va interoga fiecare Tree si se va considera raspunsul final ca
    // fiind cel majoritar
    int query[num_trees], i, result[10];
    unsigned int seed = time(NULL);
    auto it = trees.begin();

    // Precum se precizeaza si mai sus, se interogheaza fiecare tree si apoi cu
    // un vector de frecventa se verifica cifra returnata cel mai des.
    for (i = 0; i < num_trees, it != trees.end(); ++i, ++it) {
        query[i] = (*it).predict(image);
    }
    for (i = 0; i < 10; ++i) {
        result[i] = 0;
    }
    for (i = 0; i < num_trees; ++i) {
        result[query[i]]++;
    }
    int maxi = 0, cifmax = 0;
    for (i = 0; i < 10; ++i) {
        if (result[i] > maxi) {
            maxi = result[i];
            cifmax = i;
        }
    }
    return cifmax;
}
