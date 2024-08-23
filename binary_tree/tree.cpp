#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class TreeNode {
    public:
        string Name;
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
        if (left != nullptr) {
            left->print_in_order();
        }
        cout << Name << endl;
        if (right != nullptr) {
            right->print_in_order();
        }
    }

};

int main() {
    TreeNode root;
    root.insert_node("EU");
    root.insert_node("TU");
    root.insert_node("ELE");
    root.insert_node("NOS");
    root.insert_node("VOS");
    root.insert_node("ELES");

    return 0;
}