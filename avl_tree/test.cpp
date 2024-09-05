#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

struct Dictionary {
    string* key;
    string* number_of_synonyms;
    string* synonyms;
};

// An AVL tree node 
struct Node { 
    string key;
    string synonyms;
    Node *left;
    Node *right;
    int height;

    Node(string k, string sn) { 
        key = k;
        synonyms = sn;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
}; 

// A utility function to 
// get the height of the tree 
int height(Node *N) {
    if (N == nullptr)
        return 0; 
    return N->height; 
} 

// A utility function to right 
// rotate subtree rooted with y 
Node *rightRotate(Node *y) { 
    Node *x = y->left; 
    Node *T2 = x->right; 

    // Perform rotation 
    x->right = y; 
    y->left = T2; 

    // Update heights 
    y->height = 1 + max(height(y->left), 
                    height(y->right)); 
    x->height = 1 + max(height(x->left), 
                        height(x->right)); 

    // Return new root 
    return x; 
} 

// A utility function to left rotate 
// subtree rooted with x 
Node *leftRotate(Node *x) { 
    Node *y = x->right; 
    Node *T2 = y->left; 

    // Perform rotation 
    y->left = x; 
    x->right = T2; 

    // Update heights 
    x->height = 1 + max(height(x->left), 
                        height(x->right)); 
    y->height = 1 + max(height(y->left), 
                        height(y->right)); 

    // Return new root 
    return y; 
} 

// Get balance factor of node N 
int getBalance(Node *N) { 
    if (N == nullptr) 
        return 0; 
    return height(N->left) - height(N->right); 
} 

// Recursive function to insert a key in 
// the subtree rooted with node 
Node* insert(Node* node, string key, string synonyms) { 
  
    // Perform the normal BST insertion
    if (node == nullptr) 
        return new Node(key, synonyms); 

    if (key < node->key) 
        node->left = insert(node->left, key, synonyms); 
    else if (key > node->key) 
        node->right = insert(node->right, key, synonyms); 
    else // Equal keys are not allowed in BST 
        return node; 

    // Update height of this ancestor node 
    node->height = 1 + max(height(node->left),
                           height(node->right)); 

    // Get the balance factor of this ancestor node 
    int balance = getBalance(node); 

    // If this node becomes unbalanced, 
    // then there are 4 cases 

    // Left Left Case 
    if (balance > 1 && key < node->left->key) 
        return rightRotate(node); 

    // Right Right Case 
    if (balance < -1 && key > node->right->key) 
        return leftRotate(node); 

    // Left Right Case 
    if (balance > 1 && key > node->left->key) { 
        node->left = leftRotate(node->left); 
        return rightRotate(node); 
    } 

    // Right Left Case 
    if (balance < -1 && key < node->right->key) { 
        node->right = rightRotate(node->right); 
        return leftRotate(node); 
    } 

    // Return the (unchanged) node pointer 
    return node; 
} 

// A utility function to print 
// preorder traversal of the tree 

void search_Node(ofstream& output, Node*& node, string key_search, bool first_call) {
    if(first_call == true)
        output << "[";

    if(node->key < key_search && (node->right != nullptr)) {
        output << node->key << "->";
        search_Node(output, node->right, key_search, false);
    }
    
    else if(node->key > key_search && (node->left != nullptr)) {
        output << node->key << "->";
        search_Node(output, node->left, key_search, false);
    }

    else if(node->key == key_search) {
        output << node->key << "]" << endl;
        output << node->synonyms << endl;
        return;
    }
   
    else {
        output << node->key << "->?"<< "]" << endl;
        output << "-" << endl;
        return;
    }

}

Dictionary get_dictionary(ifstream &input, unsigned int number_of_words) {
    Dictionary dict;
    unsigned int number_of_synonyms;

    dict.key = new string[number_of_words];
    dict.number_of_synonyms = new string[number_of_words];
    dict.synonyms = new string[number_of_words];

    string read_line;
    unsigned int pos;

    for(unsigned int i = 0; i < number_of_words; i++) {
        getline(input, read_line);

        pos = read_line.find(" ");
        dict.key[i] = read_line.substr(0, pos);

        read_line = read_line.substr(pos+1);
        pos = read_line.find(" ");
        
        dict.number_of_synonyms[i] = read_line.substr(0, pos);
        dict.synonyms[i] = read_line.substr(pos+1);
    }

    return dict;
}

// function that reads the words to search wrtitten in the input file
void get_words_to_search(ifstream &input, string* words_to_search, unsigned int number_of_words_search) {
    for(int i = 0; i < number_of_words_search; i++) {
        getline(input, words_to_search[i]);
    }
}

int main(int argc, char* argv[]) {
    ifstream input(argv[1]);
    ofstream output(argv[2]);

    unsigned int number_of_words;
    input >> number_of_words;
    input.ignore(); // ignore the anything past number_of_words in the line

    Dictionary dict = get_dictionary(input, number_of_words);
    Node *root = nullptr;

    for(unsigned int i = 0; i < number_of_words; i++) {
        root = insert(root, dict.key[i], dict.synonyms[i]);
    }

    cout << "Preorder traversal : \n"; 
    ///preOrder(root); 

    unsigned int number_of_words_search;
    input >> number_of_words_search;
    input.ignore();

    string words_to_search[number_of_words];
    get_words_to_search(input, words_to_search, number_of_words_search);

    //cout << endl;

    for(int i = 0; i < number_of_words_search; i++) {
        search_Node(output, root, words_to_search[i], true);
    }

    return 0;
}
