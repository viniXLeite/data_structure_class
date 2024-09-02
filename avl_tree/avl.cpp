#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

struct Dictionary {
    string* word;
    string* number_of_synonyms;
    string* synonyms;
};

class AVL_Node {
    public:
        string word;
        string* synonyms;
        int balance_factor_variable;
        AVL_Node *parent;
        AVL_Node *left;
        AVL_Node *right;

    AVL_Node (): word(""), balance_factor_variable(0),left(nullptr), right(nullptr), parent(nullptr) {}

    int get_height(AVL_Node* node) {
        if (node == nullptr) return 0;
        int left_height = (node->left == nullptr) ? 0 : node->left->balance_factor_variable + 1;
        int right_height = (node->right == nullptr) ? 0 : node->right->balance_factor_variable + 1;
        return max(left_height, right_height);
    }

    void balance_factor_counter() {
        int left_height = get_height(left);
        int right_height = get_height(right);
        balance_factor_variable = right_height - left_height;
    }

    void change_balance_factor() {
        balance_factor_counter();
        // instead of parent from current to root
        if(left != nullptr) left->balance_factor_counter();
        if(right != nullptr) right->balance_factor_counter();
        AVL_Node* node = parent;
        while(node != nullptr) {
            node->balance_factor_counter();
            node = node->parent;
        } 
    }

    void left_rotation(AVL_Node*& root) {
        AVL_Node* axis = root->right;
        root->right = axis->left;
        axis->left = root;
        root = axis;

        root->change_balance_factor();
    }

    void right_rotation(AVL_Node*& root) {
        AVL_Node* axis = root->left;
        root->left = axis->right;
        axis->right = root;
        root = axis;

        root->change_balance_factor();
    }

    AVL_Node* get_unbalanced_node(AVL_Node* node) {
        while(node != nullptr) {
            if(abs(node->balance_factor_variable) > 1)
                return node;
            else
                node = node->parent;
        }
    }

    void decide_balance_technique(AVL_Node* node) {
        if (node->balance_factor_variable > 1) {
            if (node->right->balance_factor_variable < 0) {
                right_rotation(node->right);
            }
            left_rotation(node);
        } else if (node->balance_factor_variable < -1) {
            if (node->left->balance_factor_variable > 0) {
                left_rotation(node->left);
            }
            right_rotation(node);
        }
    }

    // adjust parent node
    // creates a function that verifies if any node has balance_factor_var greater than 1
    // O recalculo do fator de balanceamento é necessário apenas para os nós no caminho de volta da inserção até a raiz e, possivelmente, alguns nós afetados por rotações.
    // while(node != nullptr) {
    // change balance factor (parent), current = current->parent}
    void insert_node(string word, string* synonyms) {
        if (this->word.empty()) {
            this->word = word;
            this->synonyms = synonyms;
            this->balance_factor_variable = 0;
            return;
        }

        AVL_Node* current = this;
        AVL_Node* unbalanced_node = nullptr;

        while (true) {
            if (word < current->word) {
                if (current->left != nullptr) {
                    current = current->left;
                } else {
                    current->left = new AVL_Node();
                    current->left->word = word;
                    current->left->synonyms = synonyms;
                    current->left->parent = current;
                    current->change_balance_factor();
                    unbalanced_node = get_unbalanced_node(current);
                    if(unbalanced_node != nullptr) {
                        decide_balance_technique(unbalanced_node);
                    }           
                    break;
                }
            }
            else {
                if (current->right != nullptr) {
                    current = current->right;
                } else {
                    current->right = new AVL_Node();
                    current->right->word = word;
                    current->right->synonyms = synonyms;
                    current->right->parent = current;
                    current->change_balance_factor();
                    unbalanced_node = get_unbalanced_node(current);
                    if(unbalanced_node != nullptr) {
                        decide_balance_technique(unbalanced_node);
                    }  
                    break;
                }
            }
        }
    }
    // search function
};

Dictionary get_dictionary(ifstream &input, unsigned int number_of_words) {
    Dictionary dict;
    unsigned int number_of_synonyms;

    dict.word = new string[number_of_words];
    dict.number_of_synonyms = new string[number_of_words];
    dict.synonyms = new string[number_of_words];

    string read_line;
    unsigned int pos;

    for(unsigned int i = 0; i < number_of_words; i++) {
        getline(input, read_line);

        pos = read_line.find(" ");
        dict.word[i] = read_line.substr(0, pos);

        read_line = read_line.substr(pos+1);
        pos = read_line.find(" ");
        
        dict.number_of_synonyms[i] = read_line.substr(0, pos);
        dict.synonyms[i] = read_line.substr(pos+1);
    }

    return dict;
}

void get_words_search(ifstream &input, string* words_to_search, unsigned int number_of_words_search) {
    for(int i = 0; i < number_of_words_search; i++) {
        getline(input, words_to_search[i]);
    }
}

int main(int argc, char* argv[]) {
    ifstream input(argv[1]);
    ofstream ouput(argv[2]);

    unsigned int number_of_words;
    input >> number_of_words;
    input.ignore();

    Dictionary dict = get_dictionary(input, number_of_words);

    for(unsigned int i = 0; i < number_of_words; i++) {
        ouput << dict.word[i] << ", Synonyms: " << dict.synonyms[i] << endl; // add to the tree
    }
    
    unsigned int number_of_words_search;
    input >> number_of_words_search;
    input.ignore();

    string words_to_search[number_of_words];
    get_words_search(input, words_to_search, number_of_words_search);

    for(int i= 0; i < number_of_words_search; i++) {
        ouput << words_to_search[i] << endl; // search function
    }

    return 0;
}

