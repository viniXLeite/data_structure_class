#include <iostream>
#include <string>
#include <fstream>

using namespace std;

unsigned int insertion_order = 0;

struct Doc {
    string* name;
    string* type;
    unsigned int* size;
};

class TreeNode {
    public:
        string Name;
        string type;
        unsigned int size;
        unsigned int order;

        TreeNode* left;
        TreeNode* right;
    
    TreeNode(): Name(""), left(nullptr), right(nullptr) {}

    void insert_node(string name, string type, unsigned int size) {// add more parameters 
        if (this->Name.empty()) {
            this->Name = name;
            this->type = type;
            this->size = size;
            this->order = insertion_order;
            insertion_order++;
            return;
        }

        TreeNode* current = this;

        while (true) {
            if (name < current->Name) {
                if (current->left != nullptr) {
                    current = current->left;
                } else {
                    current->left = new TreeNode();
                    current->left->Name = name;
                    current->left->type = type;
                    current->left->size = size;
                    current->left->order = insertion_order;
                    insertion_order++;
                    break;
                }
            }
            else if(name == current->Name && current->type == "rw") {
                current->type = type;
                current->order = insertion_order;
                current->size = size;
                insertion_order++;
                break;
            }
            else {
                if (current->right != nullptr) {
                    current = current->right;
                } else {
                    current->right = new TreeNode();
                    current->right->Name = name;
                    current->right->type = type;
                    current->right->size = size;
                    current->right->order = insertion_order;
                    insertion_order++;
                    break;
                }
            }
        }
    }

    void print_in_order(ofstream &output) {
        if(left != nullptr) {
            left->print_in_order(output);
        }
        if (size > 1) output << order << " " << Name << " " << type << " " << size << " bytes" << endl;
        else output << order << " " << Name << " " << type << " " << size << " byte" << endl;
        if(right != nullptr) {
            right->print_in_order(output);
        }
    }

    void preOrder(ofstream &output) {
        if (size > 1) output << order << " " << Name << " " << type << " " << size << " bytes" << endl;
        else output << order << " " << Name << " " << type << " " << size << " byte" << endl;        
        
        if(left != nullptr) {
            left->preOrder(output);
        }
        if(right != nullptr) {
            right->preOrder(output);
        }
    }

    void postOrder(ofstream &output) {
        if(left != nullptr) {
            left->postOrder(output);
        }
        if(right != nullptr) {
            right->postOrder(output);
        }
        if (size > 1) output << order << " " << Name << " " << type << " " << size << " bytes" << endl;
        else output << order << " " << Name << " " << type << " " << size << " byte" << endl;
    }
};

Doc get_docs(ifstream &input, unsigned int number_of_docs) {
    Doc doc;
    doc.name = new string[number_of_docs];
    doc.type = new string[number_of_docs];
    doc.size = new unsigned int[number_of_docs];

    for(unsigned int i = 0; i < number_of_docs; i++) {
        input >> doc.name[i] >> doc.type[i] >> doc.size[i];
    }
    return doc;
}

int main(int argc, char* argv[]) {
    ifstream input(argv[1]);
    ofstream output(argv[2]);

    unsigned int number_of_docs;
    input >> number_of_docs;
    Doc doc = get_docs(input, number_of_docs);
    cout << "OK get_docs" << endl;

    TreeNode root;

    for(unsigned int i = 0; i < number_of_docs; i++) {
        root.insert_node(doc.name[i], doc.type[i], doc.size[i]); // root.inset_node(doc.name[i], doc.size[i], doc.type[i])
    }
    cout << "OK insert nodes" << endl;

    output << "EPD:" << endl;
    root.print_in_order(output);
    output << "PED:" << endl;
    root.preOrder(output);
    output << "EDP:" << endl;
    root.postOrder(output);

    return 0;
}