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

NodeTrieTree* returnPrefixNode(NodeTrieTree* root, string key) {
    NodeTrieTree* current = root;

    for(char k:key) {
        if(current->alphabet[k-'a'] != nullptr) {
            current = current->alphabet[k-'a'];

        }
    }
    return current;
}

void searchWordsByPrefix(NodeTrieTree* root, string key) {
    NodeTrieTree* current = returnPrefixNode(root, key);
    string prefix = findPrefix(root, key);

    /*  estou utilizando a função returnPrefixNode para pegar o ponteiro do ultimo char do prefixo de key
        Implementei ponteiros para o pai 
        checo se o ponteiro filho da primeiro posição dos chars adicionados nao e uma palavra (current->alphabet[current->addedChars[0]-'a']->wordEnd)
        e se é diferente de nulo;
        caso nao: 
            (1) imprimo o primeiro char dessa string de chars adicionados addedChars, 
            (2) passo o current para o filho daquela posição char,
            (3) utilizo o metodo substr para dar um shift left e retirar o primeiro caractere de addedchars de current e do pai de current 
        
        caso sim:
            imprimo um char e do um shift left no addedChars do current que no caso é o ultimo termo da palavra

        
        Uso um for para fazer isso a quantidade de vezes necessaria (verificar, acho que ta errado)
        no caso de um char ter dois outros filhos
    */
    NodeTrieTree* initial = returnPrefixNode(root, key);
    while(true) {
        
        if (current->addedChars.length() != 1) {
            cout << "\nsearch: " <<prefix;
            current = returnPrefixNode(root, key);
            
            while (true) {
                if(current->alphabet[current->addedChars[0]-'a']->wordEnd == false) {
                    if (current->alphabet[current->addedChars[0]-'a']->addedChars != "") {
                        cout << current->addedChars[0]; // (1)
                        current = current->alphabet[current->addedChars[0]-'a']; // (2)
                        if(current->parent->addedChars.substr(1) != "") current->parent->addedChars = current->parent->addedChars.substr(1); // (3)
                        if(current->addedChars.substr(1) != "") current->addedChars = current->addedChars.substr(1);
                    }
                }
                else {
                    cout << current->alphabet[current->addedChars[0]-'a']->addedChars[0] << current->addedChars[0];
                    if(current->addedChars.substr(1) != "") current->addedChars = current->addedChars.substr(1);
                    break;
                }

            }
        }

        // ajeitar essa repetição ta ridiculo if (current->addedChars.length() != 1)
        else {
            cout << "\nsearch: " <<prefix;
            current = returnPrefixNode(root, key);
            while (true) {
                
                if(current->alphabet[current->addedChars[0]-'a']->wordEnd == false) {
                    if (current->alphabet[current->addedChars[0]-'a']->addedChars != "") {
                        cout << current->addedChars[0]; // (1)
                        current = current->alphabet[current->addedChars[0]-'a']; // (2)
                        if(current->parent->addedChars.substr(1) != "") current->parent->addedChars = current->parent->addedChars.substr(1); // (3)
                        if(current->addedChars.substr(1) != "") current->addedChars = current->addedChars.substr(1);
                    }
                }
                else {
                    cout << current->alphabet[current->addedChars[0]-'a']->addedChars[0] << current->addedChars[0];
                    if(current->addedChars.substr(1) != "") current->addedChars = current->addedChars.substr(1);
                    break;
                }

            }
            break;
        }

    }
    
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

    searchWordsByPrefix(root, "lombada");
    searchWordsByPrefix(root, "lis"); // da problema nesse caso meo
    // so alterar os if da substr para so usar esse metodo se o addedChars filho for igual a um

    return 0;
}