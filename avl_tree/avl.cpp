#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

struct Dictionary {
    string* word;
    int* number_of_synonyms;
    string* synonyms;
};

class AVL_Node {
    public:
        string word;
        string synonyms;
        int balance_factor_variable;
        AVL_Node *parent;
        AVL_Node *left;
        AVL_Node *right;

    AVL_Node (): word(""), balance_factor_variable(0),left(nullptr), right(nullptr), parent(nullptr) {}

    int get_height(AVL_Node* node) { // it does not return the sub tree height, because the function only check if the right or left node is different from nullprt
        if (node == nullptr) return 0;
        int left_height = (node->left == nullptr) ? 0 : node->left->balance_factor_variable + 1;
        int right_height = (node->right == nullptr) ? 0 : node->right->balance_factor_variable + 1;
        cout << "height: "<< max(left_height, right_height) << endl;
        return max(left_height, right_height);
    }
    
    int get_node_height(AVL_Node* node) { //wrong
        if(node == nullptr) return 0;
        int height = (node->left == nullptr) ? 0: node->left->balance_factor_variable+1;
        return height;
    }

    // function to determine the sub tree height 
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
        cout << "balance_factor " << word << " balance_factor_var: " << balance_factor_variable << " left: " << left_counter << " right: "<< right_counter << endl;
    }

    /*
    void balance_factor_counter() {
        int left_height = get_node_height(left); // guess it's worong
        int right_height = get_node_height(right); // guess it's worong
        balance_factor_variable = right_height - left_height;
        cout << "balance_factor " << word << ": " << balance_factor_variable << " left: " << left_height << " right: "<< right_height << endl;
    }
    */

    void change_balance_factor() {
        balance_factor_counter();
        //cout << "balance_factor " << word << " balance_factor_var: " << balance_factor_variable << endl;
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

        cout << "left rotated" << endl;
        root->change_balance_factor();
    }

    void right_rotation(AVL_Node*& root) {
        AVL_Node* axis = root->left;
        root->left = axis->right;
        axis->right = root;
        root = axis;

        cout << "right rotaded" << endl;
        root->change_balance_factor();
    }

    AVL_Node* get_unbalanced_node(AVL_Node* node) {
        while(node != nullptr) {
            if(abs(node->balance_factor_variable) > 1)
                return node;
            else
                node = node->parent;
        }
        return nullptr;
    }

    void decide_balance_technique(AVL_Node* node) {
        if(node->balance_factor_variable > 0 && node->right->balance_factor_variable > 0) {
            left_rotation(node);
            cout << "here1" << endl;
            return;
        }

        else if(node->balance_factor_variable > 0 && node->right->balance_factor_variable < 1) {
            cout << "until here bitch" << endl;
            right_rotation(node->right);
            left_rotation(node);
            cout << "here2" << endl;
            return;
        }
        
        else if(node->balance_factor_variable < 0 && node->left->balance_factor_variable < 0) {
            right_rotation(node);
            cout << "here3" << endl;
            return;
        }
        
        else if(node->balance_factor_variable < 0 && node->left->balance_factor_variable > 1) {
            left_rotation(node->left);
            right_rotation(node);
            cout << "here4" << endl;
            return;
        }
    }

    void insert_node(string word, string synonyms) {
        if (this->word.empty()) {
            this->word = word;
            this->synonyms = synonyms;
            this->balance_factor_variable = 0;
            return;
        }

        // check if it can rotate the root

        AVL_Node* current = this;
        AVL_Node* unbalanced_node;

        while (true) {
            if (word < current->word) {
                if (current->left != nullptr) {
                    current = current->left;
                } else {
                    current->left = new AVL_Node();
                    current->left->word = word;
                    current->left->synonyms = synonyms;
                    current->left->parent = current;

                    cout << "added left " << word << ", parent: " << current->left->parent->word << endl;
                    current->change_balance_factor();
                    unbalanced_node = get_unbalanced_node(current);
                    if(unbalanced_node != nullptr) {
                        cout << "decide unbalanced node: " << unbalanced_node->word << endl;;
                        decide_balance_technique(unbalanced_node);
                        break;
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

                    cout << "added right " << word << ", parent: " << current->right->parent->word << endl;
                    unbalanced_node = get_unbalanced_node(current);
                    if(unbalanced_node != nullptr) {
                        cout << "decide unbalanced node: " << unbalanced_node->word << endl;;
                        decide_balance_technique(unbalanced_node);
                        break;
                    }  
                    break;
                }
            }
        }
    }

    void find_root(string key) {
        if(word < key && (right != nullptr)) {
            right->search_Node(key, false);
        }
        else if(word > key && (left != nullptr)) {
            left->search_Node(key, false);
        }
        else if(word == key) {
            cout << parent->word << endl;
            return;
        }
        else {
            return;
        }
    }

    // search function
    void search_Node(string key, bool first_call) {
        if(first_call == true)
            cout << "[";

        if(word < key && (right != nullptr)) {
            cout << word << " -> ";
            right->search_Node(key, false);
        }
        else if(word > key && (left != nullptr)) {
            cout << word << " -> ";
            left->search_Node(key, false);
        }
        else if(word == key) {
            cout << word << "]" << endl;
            cout << synonyms << endl;
            return;
        }
        else {
            cout << word << "]" << endl;
            cout << "-" << endl;
            return;
        }
    
    }

};

// returns the dictionary elemens that will be added to the avl tree
Dictionary get_dictionary(ifstream &input, unsigned int number_of_words) {
    Dictionary dict;
    unsigned int number_of_synonyms;

    dict.word = new string[number_of_words];
    dict.number_of_synonyms = new int[number_of_words];
    dict.synonyms = new string[number_of_words];

    string read_line;
    string synonyms_w_wspace;
    string temp_number_of_synonyms;
    unsigned int pos;
    unsigned int temp_pos;

    for(unsigned int i = 0; i < number_of_words; i++) {
        getline(input, read_line);

        pos = read_line.find(" ");
        dict.word[i] = read_line.substr(0, pos);

        read_line = read_line.substr(pos + 1);
        pos = read_line.find(" ");

        temp_number_of_synonyms = read_line.substr(0, pos);
        dict.number_of_synonyms[i] = stoi(temp_number_of_synonyms);
        cout << "--" << dict.number_of_synonyms[i] << "--" << endl;

        synonyms_w_wspace = read_line.substr(pos + 1);

        for (size_t j = 0; j < synonyms_w_wspace.size(); j++) {
            if (synonyms_w_wspace[j] == ' ') {
                synonyms_w_wspace[j] = ',';
            }
        }

        dict.synonyms[i] = synonyms_w_wspace;
        cout << dict.synonyms[i] << endl;
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
    ofstream ouput(argv[2]);

    AVL_Node root;

    unsigned int number_of_words;
    input >> number_of_words;
    input.ignore(); // ignore the anything past number_of_words in the line
    ouput << number_of_words << endl;

    Dictionary dict = get_dictionary(input, number_of_words);

    for(unsigned int i = 0; i < number_of_words; i++) {
        root.insert_node(dict.word[i], dict.synonyms[i]);
    }

    //root.find_root(root.word);
    //cout << "real root: " << real_root->parent << endl;
        
    unsigned int number_of_words_search;
    input >> number_of_words_search;
    input.ignore();

    string words_to_search[number_of_words];
    get_words_to_search(input, words_to_search, number_of_words_search);

    for(int i= 0; i < number_of_words_search; i++) {
        ouput << words_to_search[i] << endl;
        root.search_Node(words_to_search[i], true);
    }

    root.search_Node("elegante", true);

    // nao ta imprimindo certo pq demais nao e mais a raiz, mas quando chamo root.search ele parte de demais 

    return 0;
}

