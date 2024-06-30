/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"


// Helper function declaration
void remove_subtree(SymEntry * root);
void insert_val_subtree(SymEntry * root, string &key, UnlimitedRational * v);
void remove_val_subtree(SymEntry * root, string &key);
UnlimitedRational * get_val_subtree(SymEntry * root, string &k);
SymEntry* findparentNode(SymEntry * root, string &key);
SymEntry * findNode(SymEntry * root, string &key);
void removeNode(SymEntry * root, SymEntry * parent);
SymEntry * get_predecessor(SymEntry * root);
SymEntry* get_sucessor(SymEntry * root);

// Constructors and Destroyers
SymbolTable::SymbolTable(){root = new SymEntry(); size = 1;}
SymbolTable::~SymbolTable(){remove_subtree(root);}

// Getter functions
SymEntry * SymbolTable::get_root(){return root;}
int SymbolTable::get_size(){return size;}

// Dynamic operations - Insertion and Deletion
void SymbolTable::insert(string k, UnlimitedRational * v){
    // TODO: What if the string is already in the symbol table?
    insert_val_subtree(root, k, v);
}

void SymbolTable::remove(string k){
    // TODO: What if the string is not in the symbol table?
    remove_val_subtree(root, k);
}

// Static operation - Searching
UnlimitedRational * SymbolTable::search(string k){
    // TODO: What if the string is not in the symbol table?
    return get_val_subtree(root, k);
}

// Helper functions
// Removes the entire subtree
void remove_subtree(SymEntry * root){
    if (root->left){remove_subtree(root->left);}
    if (root->right){remove_subtree(root->right);}
    delete root;
}

// Insert key, val for root recursively
void insert_val_subtree(SymEntry * root, string &key, UnlimitedRational * v){
    if (root == nullptr){
        root = new SymEntry(key, v); return;
    }
    if (root->key == key){return;}
    else if (root->key > key){
        if (root->left){insert_val_subtree(root->left, key, v);}
        else {
            SymEntry * child = new SymEntry(key, v); root->left = child; return;
        }
    }
    else {
        if (root->right){insert_val_subtree(root->right, key, v);}
        else {
            SymEntry * child = new SymEntry(key, v); root->right = child; return;
        }
    }
}

// Gets values at key - if it exists. Else, return a dummy value
UnlimitedRational * get_val_subtree(SymEntry * root, string &k){
    SymEntry * node = findNode(root, k);
    if (node){
        UnlimitedRational * new_rational = new UnlimitedRational(node->val->get_p(), node->val->get_q());
        return new_rational;
    }
    return nullptr;
}

// Removes key recursively
void remove_val_subtree(SymEntry * root, string &k){
    if (root == nullptr){return;}
    if (root->key == k && root->left == nullptr && root->right == nullptr){
        delete root; return;
    }

    SymEntry * parent_node = nullptr;
    SymEntry * node_to_delete = findNode(root, k);
    if (node_to_delete != nullptr){
        parent_node = findparentNode(root, k);
    }
    removeNode(node_to_delete, parent_node);
    return;
}

// Find the parent node of the node with key k
SymEntry* findparentNode(SymEntry * root, string &key){
    if (root->key == key){return nullptr;}
    else if (root->key > key){
        if (root->left->key == key){return root;}
        else {return findparentNode(root->left, key);}
    }
    else {
        if (root->right->key == key){return root;}
        else {return findparentNode(root->right, key);}
    }
}

// Find node with key k. Returns nullptr if not found
SymEntry * findNode(SymEntry * root, string &key){
    if (root->key == key){return root;}
    else if (root->key > key){
        if (root->left == nullptr){return nullptr;}
        return findNode(root->left, key);
    }
    else {
        if (root->right == nullptr){return nullptr;}
        return findNode(root->right, key);
    }
}

// Removes a node given the root and parent. 
// Parent required as it's child must be set to nullptr
void removeNode(SymEntry * root, SymEntry * parent){
    if (root == nullptr){return;}
    if (root->left == nullptr && root->right == nullptr){
        if (parent->left == root){parent->left = nullptr;}
        else {parent->right = nullptr;}
        delete root; return;
    }
    else if (root->left != nullptr){
        SymEntry * predecessor = get_predecessor(root);
        parent = findparentNode(root, predecessor->key);
        std::swap(predecessor->key, root->key);
        std::swap(predecessor->val, root->val);
        removeNode(predecessor, parent); return;
    }
    else {
        SymEntry * successor = get_sucessor(root);
        parent = findparentNode(root, successor->key);
        std::swap(successor->key, root->key);
        std::swap(successor->val, root->val);
        removeNode(successor, parent); return;
    }
}

// Get predecessor of a node
SymEntry * get_predecessor(SymEntry * root){
    SymEntry * curr_node = root->left;
    while (curr_node->right != nullptr){curr_node = curr_node->right;}
    return curr_node;
}

// Get successor of a node
SymEntry* get_sucessor(SymEntry * root){
    SymEntry * curr_node = root->right;
    while (curr_node->left != nullptr){curr_node = curr_node->left;}
    return curr_node;
}
