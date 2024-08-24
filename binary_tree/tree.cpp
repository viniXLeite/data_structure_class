#include <iostream>
#include <string>
#include <fstream>

using namespace std;

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

    void insert_node(string name) {
        if (this->Name.empty()) {
            this->Name = name;
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
                    break;
                }
            //else if(name == current->Name) do not change the pointers only the size, name and type
            } else {
                if (current->right != nullptr) {
                    current = current->right;
                } else {
                    current->right = new TreeNode();
                    current->right->Name = name;
                    break;
                }
            }
        }
    }

    void print_in_order() {
        if(left != nullptr) {
            left->print_in_order();
        }
        cout << Name << endl;
        if(right != nullptr) {
            right->print_in_order();
        }
    }

    void preOrder() {
        cout << Name << endl;
        if(left != nullptr) {
            left->preOrder();
        }
        if(right != nullptr) {
            right->preOrder();
        }
    }

    void postOrder() {
        if(left != nullptr) {
            left->postOrder();
        }
        if(right != nullptr) {
            right->postOrder();
        }
        cout << Name << endl;
    }

};

Doc get_docs(ifstream &input, unsigned int number_of_docs) {
    Doc doc;
    doc.name = new string[number_of_docs];
    doc.type = new string[number_of_docs];
    doc.size = new unsigned int[number_of_docs];

    for(int i = 0; i < number_of_docs; i++) {
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

    TreeNode root;

    for(int i = 0; i < number_of_docs; i++) {
        root.insert_node(doc.name[i]); // remember to also insert the root.type and the root.size in the tree.insert function
    }

    cout << "EPD:" << endl;
    root.print_in_order();
    cout << "PED:" << endl;
    root.preOrder();
    cout << "EDP:" << endl;
    root.postOrder();

    return 0;
}