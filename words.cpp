/**
 * @file words.cpp
 * @author mjh316
 * 
 * Created to solve the "five words, 25 distinct characters" problem posed by Stand-up Maths
 * Possibly also known as the "Jotto Problem" in literature
 */
#include "bits/stdc++.h"
using namespace std;

constexpr int WORDS_SIZE = 15920;

vector<string> words;
uint32_t encoding[WORDS_SIZE+1];
unordered_set<int> graph[WORDS_SIZE]; // possibly try changing to unordered_set for faster intersection


/**
 * @param s The word that we want to encode as an integer
 * @return The encoding of the passed word parameter
 */
uint32_t encode(string &s) {
    uint32_t ret = 0;
    for (char &c: s) {
        ret |= (1 << (c - 'a'));
    }
    return ret;
}

/**
 * @param filename The filename that we want to load our words from
 */
void load_words(string filename) {
    ifstream fin(filename);
    string tmp;
    while (fin >> tmp) { 
        // each word has to have 5 distinct letters, so we can
        // actually optimize this more by removing the ones that don't
        // we check using (std::popcount(encode(tmp)) == 5)
        while (tmp.size() > 0 && tmp.back() == ' ') {
            tmp.pop_back();
        }
        if (tmp.size() != 5) continue;
        if (popcount(encode(tmp)) != 5) continue;
        words.emplace_back(tmp);
    }
    fin.close();
}

auto layer(const unordered_set<int>& allowed, int cur) {
    unordered_set<int> ret;
    for (int neighbor: allowed) {
        if (graph[cur].count(neighbor)) ret.emplace(neighbor);
    }
    return ret;
}

int main() {
    load_words("words_alpha.txt");
    // assert(words.size() == WORDS_SIZE);

    for (int i = 0; i < words.size(); ++i) {
        encoding[i] = encode(words[i]);
    }

    for (int i = 0; i < words.size(); ++i) {
        for (int j = i + 1; j < words.size(); ++j) {
            if (std::popcount(encoding[i] ^ encoding[j]) != 10) {
                continue;
            }
            graph[i].emplace(j);
            graph[j].emplace(i);
        }
        //cout << "i: " << i << " graph[i].size(): " << graph[i].size() << '\n';
    }
    cout << "boo" << endl;
    int ret = 0;
    for (int i = 0; i < words.size(); ++i) {
        if (graph[i].size() == 0) continue;
        //auto allowed = graph[i];
        //cout << "i: " << i << endl;
        for (int j : graph[i]) {
            if (j < i) continue;
            auto a1 = layer(graph[i], j);
            if (a1.size() < 3) continue;
            //cout << "j: " << j << endl;
            for (int k : a1) {
               if (k < j) continue;
               auto a2 = layer(a1, k);
                if (a2.size() < 2) continue;
                //cout << "size: " << a2.size() << '\n';
                //cout << "k: " << k << endl;
                for (int l: a2) {
                   if (l < k) continue;
                   auto a3 = layer(a2, l);
                    if (a3.size() < 1) continue;
                    //cout << "ret4: " << ret << endl;
                    for (int m: a3) {
                        if (m < l) continue;
                        cout << words[i] << " " << words[j] << " " << words[k] << " " << words[l] << " " << words[m] << endl;
                        //return 0;
                        ++ret;
                        cout << "ret5: " << ret << endl;
                    }
                }
            }
        }
    }
    cout << "ret: " << ret << '\n';
    return 0;
}
