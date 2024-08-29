#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

class AVL_Node {
    public:
        string word;
        string* synonyms;
        int balance_factor_variable;
        AVL_Node *parent;
        AVL_Node *left;
        AVL_Node *right;

    AVL_Node (): word(""), balance_factor_variable(0),left(nullptr), right(nullptr), parent(nullptr) {}

    void balance_factor_counter() {
        unsigned int left_counter = 0;
        unsigned int right_counter = 0;
        
        if(left != NULL) {
            left_counter++;
            if(left->left != NULL || left->right != NULL)
                left_counter++;    
        }

        if(right != NULL) {
            right_counter++;
            if(right->right != NULL || right->left != NULL)
                right_counter++;
        }
        // VERIFIES IF IT ZERO INITIALIZED
        balance_factor_variable = (right_counter - left_counter);
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

    void left_rotation(AVL_Node* root) {
        AVL_Node* axis = root->right;
        root->right = axis->left;
        axis->left = root;
        root = axis;

        root->change_balance_factor();
    }

    void right_rotation(AVL_Node* root) {
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
        if(node->balance_factor_variable > 0 && node->balance_factor_variable > 0) {
            left_rotation(node);
            node->change_balance_factor();
            return;
        }

        else if(node->balance_factor_variable > 0 && node->balance_factor_variable < 0) {
            right_rotation(node);
            left_rotation(node);
            node->change_balance_factor();
            return;
        }
        
        else if(node->balance_factor_variable < 0 && node->balance_factor_variable < 0) {
            right_rotation(node);
            node->change_balance_factor();
            return;
        }
        
        else {
            left_rotation(node);
            right_rotation(node);
            node->change_balance_factor();
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

int main() {
    return 0;
}

