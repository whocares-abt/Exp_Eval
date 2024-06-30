/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "evaluator.h"

// Helper function declaration
void eval_subtree(ExprTreeNode * root, SymbolTable * symtable);
void parse_subtree(ExprTreeNode * root, vector<string> &code, int ptr1, int ptr2);

// Constructors and Destroyers
Evaluator::Evaluator(){symtable = new SymbolTable();}
Evaluator::~Evaluator(){
    int size = expr_trees.size();
    for (int i = 0; i < size; i++){
        delete expr_trees[i];
    }
    delete symtable;
}

// Parsing
void Evaluator::parse(vector<string> code){
    UnlimitedRational * S = new UnlimitedRational;
    ExprTreeNode * root = new ExprTreeNode("ASSGN", S);
    root->right = new ExprTreeNode();
    root->left = new ExprTreeNode();
    root->left->id = code[0]; root->left->type = "VAR";
    delete S;
    parse_subtree(root->right, code, 2, code.size() - 1);
    expr_trees.push_back(root);
}

// Evaluation
void Evaluator::eval(){
    eval_subtree(expr_trees[expr_trees.size() - 1], symtable);
}

// Helper functions
void eval_subtree(ExprTreeNode * root, SymbolTable * symtable){
    string type = root->type;
    if (root->left){eval_subtree(root->left, symtable);}
    if (root->right){eval_subtree(root->right, symtable);}
    if (type == "VAL"){
        delete root->evaluated_value;
        root->evaluated_value = new UnlimitedRational(root->val->get_p(), root->val->get_q()); 
        return;
    }
    else if (type == "VAR"){
        delete root->evaluated_value;
        root->evaluated_value = symtable->search(root->id);
        return;
    }
    else if (type == "ADD"){
        delete root->evaluated_value;
        root->evaluated_value = UnlimitedRational::add(root->left->evaluated_value, root->right->evaluated_value);
    }
    else if (type == "SUB"){
        delete root->evaluated_value;
        root->evaluated_value = UnlimitedRational::sub(root->left->evaluated_value, root->right->evaluated_value);
    }
    else if (type == "MUL"){
        delete root->evaluated_value;
        root->evaluated_value = UnlimitedRational::mul(root->left->evaluated_value, root->right->evaluated_value);
    }
    else if (type == "DIV"){
        delete root->evaluated_value;
        root->evaluated_value = UnlimitedRational::div(root->left->evaluated_value, root->right->evaluated_value);
    }
    else{
        // Assignment, i.e., root node
        string new_var = root->left->id;
        UnlimitedRational * val = root->right->evaluated_value;
        symtable->insert(new_var, val);
    }
}

void parse_subtree(ExprTreeNode * root, vector<string> &code, int ptr1, int ptr2){
    int brac_count = 0; string oper; int oper_idx = 0;
    if (ptr1 == ptr2){
        bool pure_num = true; string token = code[ptr1];
        int size = token.size(); int ascii;
        for (int i = 0; i < size; i++){
            ascii = (int)token[i];
            if (ascii >= 58 || ascii <= 47){pure_num = false;}
        }
        if (pure_num){
            root->type = "VAL"; delete root->val;
            UnlimitedInt * num = new UnlimitedInt(token), * den = new UnlimitedInt(1);
            root->val = new UnlimitedRational(num, den);
            delete num; delete den;
        }
        else {
            root->type = "VAR"; root->id = token;
        }
        return;
    }


    for (int i = ptr1; i < ptr2 + 1; i++){
        if (code[i] == "("){brac_count++;}
        else if (code[i] == ")"){brac_count--;}
        if (brac_count == 1){
            if (code[i] == "+"){root->type = "ADD"; oper_idx = i; break;}
            else if (code[i] == "-"){root->type = "SUB"; oper_idx = i; break;}
            else if (code[i] == "*"){root->type = "MUL"; oper_idx = i; break;}
            else if (code[i] == "/"){root->type = "DIV"; oper_idx = i; break;}
        }
    }

    oper = root->type;
    if (oper == ""){
        // Double brackets case
        parse_subtree(root, code, ptr1 + 1, ptr2 + 1); return;
    }
    else {
        // Well-formed oper Well-formed
        root->left = new ExprTreeNode; root->right = new ExprTreeNode;
        parse_subtree(root->left, code, ptr1 + 1, oper_idx - 1);
        parse_subtree(root->right, code, oper_idx + 1, ptr2 - 1); return;
    }
}
