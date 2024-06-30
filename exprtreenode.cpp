/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "exprtreenode.h"

// Constructors and Destroyers

ExprTreeNode::ExprTreeNode(){
    type = "VAL"; val = new UnlimitedRational(); 
    evaluated_value = new UnlimitedRational();
    left = nullptr; right = nullptr;
}

ExprTreeNode::ExprTreeNode(string t, UnlimitedInt * v){
    type = t; UnlimitedInt * dummy_den = new UnlimitedInt("1");
    val = new UnlimitedRational(v, dummy_den);
    delete dummy_den;
    evaluated_value = new UnlimitedRational();
    left = nullptr; right = nullptr;
}

ExprTreeNode::ExprTreeNode(string t, UnlimitedRational * v){
    type = t; val = new UnlimitedRational(v->get_p(), v->get_q());
    evaluated_value = new UnlimitedRational();
    left = nullptr; right = nullptr;
}

ExprTreeNode::~ExprTreeNode(){
    delete this->left; delete this->right;
    delete this->val; delete this->evaluated_value;
}
