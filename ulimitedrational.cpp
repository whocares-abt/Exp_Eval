/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "ulimitedrational.h"

// Helper function declarations
UnlimitedInt * gcd_iter(UnlimitedInt * greater, UnlimitedInt * lesser);
UnlimitedInt * get_gcd(UnlimitedInt * num_1, UnlimitedInt * num_2);
bool is_absolute_greater(UnlimitedInt * i1, UnlimitedInt * i2);
std::string remove_neg(std::string);


// Very important
// TODO: Test code here

// Constructors and destroyers
UnlimitedRational::UnlimitedRational(){
    // TODO: Check default constuctors
    p = new UnlimitedInt(0); q = new UnlimitedInt(0);
}

UnlimitedRational::UnlimitedRational(UnlimitedInt * num, UnlimitedInt * den){
    p = new UnlimitedInt(num->to_string()); q = new UnlimitedInt(den->to_string());
}

UnlimitedRational::~UnlimitedRational(){
    delete p; delete q;
}

// Getter functions
UnlimitedInt * UnlimitedRational::get_p(){return p;}
UnlimitedInt * UnlimitedRational::get_q(){return q;}
std::string UnlimitedRational::get_p_str(){return p->to_string();}
std::string UnlimitedRational::get_q_str(){return q->to_string();}
std::string UnlimitedRational::get_frac_str(){
    std::string s; s = get_p_str() + '/'; s = s + get_q_str();
    return s;
}

// Arithmetic operations
UnlimitedRational * UnlimitedRational::add(UnlimitedRational * i1, UnlimitedRational * i2){
    if (i2->get_q_str() == "0" || i1->get_q_str() == "0"){
        UnlimitedRational * res = new UnlimitedRational; return res;
    }
    UnlimitedInt * term1 = UnlimitedInt::mul(i1->get_p(), i2->get_q());
    UnlimitedInt * term2 = UnlimitedInt::mul(i1->get_q(), i2->get_p());
    UnlimitedInt * frac_num = UnlimitedInt::add(term1, term2);
    UnlimitedInt * frac_den = UnlimitedInt::mul(i1->get_q(), i2->get_q());

    UnlimitedInt * gcd = get_gcd(frac_num, frac_den);
    UnlimitedInt * num = UnlimitedInt::div(frac_num, gcd);
    UnlimitedInt * den = UnlimitedInt::div(frac_den, gcd);
    UnlimitedRational * res = new UnlimitedRational(num, den);

    delete term1; delete term2; delete frac_num; delete frac_den;
    delete gcd; delete num; delete den;
    return res;
}

UnlimitedRational * UnlimitedRational::sub(UnlimitedRational * i1, UnlimitedRational * i2){
    if (i2->get_q_str() == "0" || i1->get_q_str() == "0"){
        UnlimitedRational * res = new UnlimitedRational; return res;
    }
    UnlimitedInt * term1 = UnlimitedInt::mul(i1->get_p(), i2->get_q());
    UnlimitedInt * term2 = UnlimitedInt::mul(i1->get_q(), i2->get_p());
    UnlimitedInt * frac_num = UnlimitedInt::sub(term1, term2);
    UnlimitedInt * frac_den = UnlimitedInt::mul(i1->get_q(), i2->get_q());

    UnlimitedInt * gcd = get_gcd(frac_num, frac_den);
    UnlimitedInt * num = UnlimitedInt::div(frac_num, gcd);
    UnlimitedInt * den = UnlimitedInt::div(frac_den, gcd);
    UnlimitedRational * res = new UnlimitedRational(num, den);

    delete term1; delete term2; delete frac_num; delete frac_den;
    delete gcd; delete num; delete den;
    return res;
}

UnlimitedRational * UnlimitedRational::mul(UnlimitedRational * i1, UnlimitedRational * i2){
    if (i2->get_q_str() == "0" || i1->get_q_str() == "0"){
        UnlimitedRational * res = new UnlimitedRational; return res;
    }
    UnlimitedInt * frac_num = UnlimitedInt::mul(i1->get_p(), i2->get_p());
    UnlimitedInt * frac_den = UnlimitedInt::mul(i1->get_q(), i2->get_q());

    UnlimitedInt * gcd = get_gcd(frac_num, frac_den);
    UnlimitedInt * num = UnlimitedInt::div(frac_num, gcd);
    UnlimitedInt * den = UnlimitedInt::div(frac_den, gcd);
    UnlimitedRational * res = new UnlimitedRational(num, den);

    delete frac_num; delete frac_den; delete gcd; delete num; delete den;
    return res;
}

UnlimitedRational * UnlimitedRational::div(UnlimitedRational * i1, UnlimitedRational * i2){
    if (i2->get_q_str() == "0" || i1->get_q_str() == "0"){
        UnlimitedRational * res = new UnlimitedRational; return res;
    }
    if (i2->get_p_str() == "0"){UnlimitedRational * res = new UnlimitedRational; return res;}
    UnlimitedRational * reciprocal = new UnlimitedRational(i2->get_q(), i2->get_p());
    UnlimitedRational * res = UnlimitedRational::mul(i1, reciprocal);
    delete reciprocal;
    return res;
}

// Helper functions
// Helper function for gcd
UnlimitedInt * gcd_iter(UnlimitedInt * greater, UnlimitedInt * lesser){
    UnlimitedInt * res = UnlimitedInt::mod(greater, lesser);
    UnlimitedInt * temp = new UnlimitedInt(lesser->to_string());
    UnlimitedInt * rem = res;
    while (res->get_sign() != 0){
        res = UnlimitedInt::mod(temp, rem);
        delete temp; temp = rem; rem = res;
    }
    delete res; return temp;
}

// Returns the gcd of 2 unlimited integers
UnlimitedInt * get_gcd(UnlimitedInt * i1, UnlimitedInt * i2){
    UnlimitedInt * greater, * lesser;
    UnlimitedInt * rem, * res; bool flag = is_absolute_greater(i1, i2);

    if (i1->get_sign() == 0){
        if (i2->get_sign() == -1){greater = new UnlimitedInt(remove_neg(i2->to_string()));}
        else {greater = new UnlimitedInt(i2->to_string());}
        return greater;
    }

    if (flag){
        if (i1->get_sign() == -1){greater = new UnlimitedInt(remove_neg(i1->to_string()));}
        else {greater = new UnlimitedInt(i1->to_string());}
        if (i2->get_sign() == -1){lesser = new UnlimitedInt(remove_neg(i2->to_string()));}
        else {lesser = new UnlimitedInt(i2->to_string());}
    }
    else{
        if (i2->get_sign() == -1){greater = new UnlimitedInt(remove_neg(i2->to_string()));}
        else {greater = new UnlimitedInt(i2->to_string());}
        if (i1->get_sign() == -1){lesser = new UnlimitedInt(remove_neg(i1->to_string()));}
        else {lesser = new UnlimitedInt(i1->to_string());}
    }

    rem = UnlimitedInt::mod(greater, lesser);
    if (rem->get_sign() == 0){delete greater; delete rem; return lesser;}

    res = gcd_iter(lesser, rem);
    delete greater; delete lesser; delete rem;
    return res;
}


// Checks if absolute value of first integer is greater than second
bool is_absolute_greater(UnlimitedInt * num1, UnlimitedInt * num2){
    if (num1->get_size() > num2->get_size()){return true;}
    else if (num1->get_size() < num2->get_size()){return false;}
    int * num1_arr = num1->get_array();
    int * num2_arr = num2->get_array();
    int size = num1->get_size();
    for (int i = 0; i < size; i++){
        if (num1_arr[i] > num2_arr[i]){return true;}
        else if (num1_arr[i] < num2_arr[i]){return false;}
    }
    return false;
}

// Returns the string passed after removing the negative sign
std::string remove_neg(std::string str){
    std::string str2 = "";
    int size = str.length();
    for (int i = 1; i < size; i++){
        str2 += str[i];
    }
    return str2;
}
