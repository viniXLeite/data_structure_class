#include <iostream>
#include <string>
#include <fstream>

using namespace std;

string realAlphabet = "abcdefghijklmnopqrstuvwxyz";

struct NodeTrieTree {
    NodeTrieTree* alphabet[26];
    NodeTrieTree* parent;
    string addedChars = "";
    bool wordEnd;

    NodeTrieTree() {
        wordEnd = false;

        for(unsigned int i = 0; i < 26; i++) {
            alphabet[i] = nullptr;
        }
    }
};

void insertNode(NodeTrieTree* root, string key) {
    NodeTrieTree* current = root;

    for(char k: key) {
        if(current->alphabet[k-'a'] == nullptr) {
            NodeTrieTree* newNoode = new NodeTrieTree();
            current->alphabet[k-'a'] = newNoode;
            current->alphabet[k-'a']->parent = current;
            current->addedChars = current->addedChars+k;
        }
        current = current->alphabet[k-'a'];
    }
    current->wordEnd = true;
}

void searchNode(NodeTrieTree* root, string key) {
    NodeTrieTree* current = root;
    cout << "\n";

    for(char k: key) {
        if(current->alphabet[k-'a'] != nullptr) {
            cout << k;
            current = current->alphabet[k-'a'];
        }
        else cout << " (not found) ";
    }     
}

string findPrefix(NodeTrieTree* root, string key) {
    NodeTrieTree* current = root;
    string prefix = "";

    for(char k:key) {
        if(current->alphabet[k-'a'] != nullptr) {
            prefix = prefix+k;
            current = current->alphabet[k-'a'];
        }
    }
    return prefix;
}

void collectWords(NodeTrieTree* node, string prefix) {
    if (node->wordEnd) {
        cout << prefix << endl;  // Aqui você pode adicionar a palavra à lista de resultados
    }
    
    for (int i = 0; i < 26; i++) {
        if (node->alphabet[i]) {
            collectWords(node->alphabet[i], prefix + char('a' + i));
        }
    }
}

// Função para buscar palavras com um determinado prefixo
void searchWordsWithPrefix(NodeTrieTree* root, const string& prefix) {
    NodeTrieTree* current = root;
    for (char ch : prefix) {
        int index = ch - 'a';
        if (!current->alphabet[index]) {
            cout << "-" << endl;
            return;
        }
        current = current->alphabet[index];
    }

    // Se chegamos ao fim do prefixo, coletar todas as palavras abaixo desse nó
    collectWords(current, prefix);
}

int main() {
    NodeTrieTree* root = new NodeTrieTree();

    insertNode(root, "abacaxi");
    insertNode(root, "teste");
    insertNode(root, "abraço");
    insertNode(root, "loja");
    insertNode(root, "logica");
    insertNode(root, "lista");
    insertNode(root, "listada");
    insertNode(root, "lisbela");
    insertNode(root, "lisboa");
    insertNode(root, "liscas");

    // so alterar os if da substr para so usar esse metodo se o addedChars filho for igual a um
    // testar outras palavras 
    cout << "\n";
    searchWordsWithPrefix(root, "lis");

    return 0;
}