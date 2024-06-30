/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "entry.h"

// Constructors and Destructors
SymEntry::SymEntry(){
    key = ""; val = new UnlimitedRational();
    left = nullptr; right = nullptr;
}

SymEntry::SymEntry(std::string k, UnlimitedRational * v){
    key = k; val = new UnlimitedRational(v->get_p(), v->get_q());
    left = nullptr; right = nullptr;
}

SymEntry::~SymEntry(){delete val;}