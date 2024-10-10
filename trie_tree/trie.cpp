#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct NodeTrieTree {
    NodeTrieTree* alphabet[26];
    bool wordEnd; // marks the end of a word

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
        if(!current->alphabet[k-'a']) {
            NodeTrieTree* newNode = new NodeTrieTree();
            current->alphabet[k-'a'] = newNode; // creates a newNode if the current aplhabet position is empty
        }

        current = current->alphabet[k-'a'];
    }
    current->wordEnd = true;
}

void collectWords(NodeTrieTree* node, string prefix, unsigned int prefix_lenght, string& array_collectedWords) {
    if (node->wordEnd) {
        if (!array_collectedWords.empty()) array_collectedWords.append(","); 
        array_collectedWords.append(prefix); // concatenates all the found words in a string
    }
    
    for (int i = 0; i < 26; i++) {
        if (node->alphabet[i]) {
            if((prefix + char('a' + i)).length() > prefix_lenght*2) continue; // limit the number of call in the recursive function 
            collectWords(node->alphabet[i], prefix + char('a' + i), prefix_lenght, array_collectedWords);
        }
    }
}

// function responsible to find a substring present in the tree given a string(key)
string findPrefix(NodeTrieTree* root, string key) {
    NodeTrieTree* current = root;
    string prefix = "";

    for(char k:key) {
        if (!current->alphabet[k-'a']) break;

        else {
            prefix = prefix+k;
            current = current->alphabet[k-'a'];
        }
    }
    return prefix;
}

void searchWordsWithPrefix(NodeTrieTree* root, const string& prefix) {
    NodeTrieTree* current = root;
    string key;
    string array_collectedWords = "";

    //key = findPrefix(root, prefix);
    key = findPrefix(root, prefix);

    for (char ch : key) {
        int index = ch - 'a';
        current = current->alphabet[index];
    }

    collectWords(current, key, key.length(), array_collectedWords);
    
    cout << prefix << ":";
    if (!array_collectedWords.empty()) cout << array_collectedWords << endl;
    else cout << "-" << endl;
}   

int main() {
    NodeTrieTree* root = new NodeTrieTree();

    insertNode(root, "abacaxi");
    insertNode(root, "teste");
    insertNode(root, "abraço");
    insertNode(root, "loja");
    insertNode(root, "logica");
    insertNode(root, "lista");
    insertNode(root, "listzaoda");
    insertNode(root, "listede");
    insertNode(root, "listo");
    insertNode(root, "liso");
    insertNode(root, "listcasadejoao");

    insertNode(root, "facil");
    insertNode(root, "simples");
    insertNode(root, "trivial");
    insertNode(root, "banal");
    insertNode(root, "bacana");
    insertNode(root, "banano");
    insertNode(root, "banda");
    insertNode(root, "facilimo");

    cout << "\n";
    //searchWordsWithPrefix(root, "listxdo");

    searchWordsWithPrefix(root, "facin");
    searchWordsWithPrefix(root, "ban");
    searchWordsWithPrefix(root, "exemplo");
    searchWordsWithPrefix(root, "trivial");
    searchWordsWithPrefix(root, "bacaninha");

    return 0;
}