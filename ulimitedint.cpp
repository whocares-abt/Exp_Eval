/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "ulimitedint.h"

// Helper function declaration
std::string remove_neg_str(std::string);
bool is_abs_equal(UnlimitedInt * i1, UnlimitedInt * i2);
bool is_abs_greater(UnlimitedInt * i1, UnlimitedInt * i2);
bool is_abs_grtr_eq(int * i1, int * i2, int size);
void arr_sub(int * i1, int * i2, int size);
void arr_add(int * i1, int * i2, int size);

// Constructors and Destructors
UnlimitedInt::UnlimitedInt(){
    // TODO: Get values for default constructors
    sign = 0; size = 1; capacity = 1;
    unlimited_int =  new int[capacity]; unlimited_int[0] = 0;
}

UnlimitedInt::UnlimitedInt(int i){
    if (i == 0){
        sign = 0; size = 1; capacity = 1;
        unlimited_int =  new int[capacity]; unlimited_int[0] = 0;
        return;
    }
    int temp = i, digits = 0;
    while (temp != 0){
        digits++; temp = temp/10;
    }
    capacity = digits, size = digits;
    unlimited_int = new int[capacity];
    sign = (i > 0) ? 1 : -1;
    for (int j = 0; j < capacity; j++){
        unlimited_int[capacity - j - 1] = std::abs(i%10);
        i = i/10;
    }
}

UnlimitedInt::UnlimitedInt(std::string str){
    if (str[0] == '0'){
        sign = 0; size = 1; capacity = 1;
        unlimited_int =  new int[capacity]; unlimited_int[0] = 0;
        return;
    }
    if (str[0] == '-'){sign = -1;str = remove_neg_str(str);}
    else {sign = 1;}
    capacity = str.length(), size = str.length();
    unlimited_int = new int[capacity];
    for (int j = 0; j < capacity; j++){
        unlimited_int[j] = str[j] - '0';
    }
}

UnlimitedInt::UnlimitedInt(int * ulimited_int, int cap, int sgn, int sz){
    capacity = cap; size = sz; sign = sgn;
    unlimited_int = new int[cap];
    for (int i = 0; i < capacity; i++){
        unlimited_int[i] = ulimited_int[i];
    }
}

UnlimitedInt::~UnlimitedInt(){
    delete[] unlimited_int;
}

// Getter functions
int UnlimitedInt::get_sign(){return sign;}
int UnlimitedInt::get_size(){return size;}
int UnlimitedInt::get_capacity(){return capacity;}
int * UnlimitedInt::get_array(){return unlimited_int;}
// TODO: check specifications of unlimited_int and see to return
//       the entire int array or the start of the int array

// Converting to string
std::string UnlimitedInt::to_string(){
    std::string str;
    if (sign == 0){return "0";}
    if (sign == -1){str += '-';}
    for (int i = 0; i < size; i++){
        str += (char)(unlimited_int[i+capacity-size]+'0');
    }
    return str;
}

// Integer operations as defined
UnlimitedInt * UnlimitedInt::add(UnlimitedInt * i1, UnlimitedInt * i2){

    // Trivial case - one of the numbers is zero
    if (i1->sign == 0){
        UnlimitedInt * result = new UnlimitedInt(i2->to_string());
        return result;
    }
    if (i2->sign == 0){
        UnlimitedInt * result = new UnlimitedInt(i1->to_string());
        return result;
    }

    int cap = std::max(i1->get_size(), i2->get_size()) + 1, sgn, sz;
    int * arr = new int[cap], * i1_arr = new int[cap], * i2_arr = new int[cap];
    int i1_sz = i1->size, i2_sz = i2->size;

    // Creating arrays and filling them with zeroes
    for (int j = 0; j < i1_sz; j++){
        i1_arr[cap - j - 1] = i1->unlimited_int[i1->capacity - j - 1];
    }
    for (int j = 0; j < i2_sz; j++){
        i2_arr[cap - j - 1] = i2->unlimited_int[i2->capacity - j - 1];
    }

    // Adding the numbers - same sign
    if (i1->sign == i2->sign){
        sgn = i1->sign;
        int carry = 0;
        for (int i = 0; i < cap; i++){
            arr[cap - i - 1] = ((i1_arr[cap - i - 1] + i2_arr[cap - i - 1] + carry)%10);
            carry = ((i1_arr[cap - i - 1] + i2_arr[cap - i - 1] + carry)/10);
        }
    }
    // Adding numbers with equal magnituse but opposite signs
    else if (is_abs_equal(i1, i2)){
        sgn = 0; sz = 1; cap = sz;
        delete[] arr; arr = new int[1]; 
    }
    // Subtracting one number from another
    else{
        UnlimitedInt * grtr = nullptr;
        int * g_arr, * l_arr;
        if (is_abs_greater(i1, i2)){grtr = i1; g_arr = i1_arr; l_arr = i2_arr;}
        else{grtr = i2; g_arr = i2_arr; l_arr = i1_arr;}
        sgn = grtr->sign;
        int carry = 0;
        for (int i = 0; i < cap; i++){
            arr[cap - i - 1] = ((10 + g_arr[cap-i-1] + carry - l_arr[cap-i-1])%10);
            carry = ((g_arr[cap-i-1] + carry) >= l_arr[cap-i-1]) ? 0 : -1;
        }
    }

    // Size readjustment
    if (sgn != 0){
        sz = cap;
        for (int i = 0; i < cap; i++){
            if (arr[i] != 0){break;}
            sz--;
        }
        int * arr2 = new int[sz];
        for (int i = 0; i < sz; i++){
            arr2[sz-i-1] = arr[cap-i-1];
        }
        delete[] arr; arr = arr2; cap = sz;
    }

    UnlimitedInt * answer = new UnlimitedInt(arr, cap, sgn, sz);
    delete[] arr; delete[] i1_arr; delete[] i2_arr;
    return answer;
}

UnlimitedInt * UnlimitedInt::sub(UnlimitedInt * i1, UnlimitedInt * i2){
    UnlimitedInt * i3, *i4;
    if (i2->sign == 1){i3 = new UnlimitedInt(i2->get_array(), i2->capacity, -1, i2->size);}
    else{i3 = new UnlimitedInt(i2->get_array(), i2->capacity, 1, i2->size);}
    i4 = UnlimitedInt::add(i1, i3);
    delete i3;
    return i4;
}

UnlimitedInt * UnlimitedInt::mul(UnlimitedInt * i1, UnlimitedInt * i2){ 

    UnlimitedInt * res = new UnlimitedInt;
    if (i1->sign == 0 || i2->sign == 0){return res;}

    int sz1 = i1->size, sz2 = i2->size; int cap = sz1 + sz2;
    int * mult_arr = new int[cap];
    int * arr_1 = new int[cap], * arr_2 = new int[cap]; 

    // Creating arrays and filling them with zeroes
    for (int j = 0; j < sz1; j++){
        arr_1[cap - j - 1] = i1->unlimited_int[sz1 - j - 1];
    }
    for (int j = 0; j < sz2; j++){
        arr_2[cap - j - 1] = i2->unlimited_int[sz2 - j - 1];
    }

    for (int k = cap - 1; k > cap - sz2 - 1; k--){
        int carry = 0;
        for (int j = k; j > -1; j--){
            int temp = mult_arr[j];
            mult_arr[j] = (mult_arr[j] + carry + arr_1[cap - k - 1 + j]*arr_2[k])%10;
            carry = (temp + carry + arr_1[cap - k - 1 + j]*arr_2[k])/10;
        }
    }
    int sz;

    // Size readjustment
    sz = cap;
    for (int i = 0; i < cap; i++){
        if (mult_arr[i] != 0){break;}
        sz--;
    }
    int * mult_arr2 = new int[sz];
    for (int i = 0; i < sz; i++){
        mult_arr2[sz-i-1] = mult_arr[cap-i-1];
    }
    delete[] mult_arr; mult_arr = mult_arr2; cap = sz;

    delete res; res = new UnlimitedInt(mult_arr, cap, 1, sz);

    delete[] arr_1; delete[] arr_2; delete[] mult_arr;
    if (i1->sign == i2->sign){res->sign = 1;}
    else {res->sign = -1;}

    return res;
}

UnlimitedInt * UnlimitedInt::div(UnlimitedInt * i1, UnlimitedInt * i2){

    UnlimitedInt * res = new UnlimitedInt;
    if (!(is_abs_greater(i1, i2)||is_abs_equal(i1, i2))){return res;}

    int sz1 = i1->size, sz2 = i2->size; int cap = sz1 + sz2;
    int * div_arr = new int[cap];
    int * arr_1 = new int[cap], * arr_2 = new int[cap]; 

    // Creating arrays and filling them with zeroes
    for (int j = 0; j < sz1; j++){
        arr_1[cap - j - 1] = i1->unlimited_int[sz1 - j - 1];
    }
    for (int j = 0; j < sz2; j++){
        arr_2[cap - j - 1] = i2->unlimited_int[sz2 - j - 1];
    }

    // Long division
    int ptr1 = 0, ptr2 = 0, div_arr_ptr = cap - 1;
    for (int i = 0; i < cap; i++){
        if (arr_1[i] != 0){ptr1 = i; break;}
    } 
    for (int i = 0; i < cap; i++){
        if (arr_2[i] != 0){ptr2 = i; break;}
    } 

    int * arr_1_ptr = arr_1 + ptr1;
    int * arr_2_ptr = arr_2 + ptr2;

    while (true){
        if (ptr1 == ptr2 && !(is_abs_grtr_eq(arr_1_ptr, arr_2_ptr, sz2))){break;}
        if (is_abs_grtr_eq(arr_1_ptr, arr_2_ptr, sz2)){
            arr_sub(arr_1_ptr, arr_2_ptr, sz2); div_arr[div_arr_ptr]++;
        }
        else if (is_abs_grtr_eq(arr_1_ptr, arr_2_ptr - 1, sz2 + 1)){
            arr_sub(arr_1_ptr, arr_2_ptr - 1, sz2 + 1); div_arr[div_arr_ptr - 1]++;
        }
        else{ptr1++; div_arr_ptr--; arr_1_ptr++;}
    }

    // Size readjustment
    int sz = cap;
    int * div_arr2 = new int[sz];
    for (int i = div_arr_ptr; i < cap; i++){div_arr2[cap - (i - div_arr_ptr) - 1] = div_arr[i];}
    delete[] div_arr; div_arr = div_arr2; cap = sz;

    sz = cap;
    for (int i = 0; i < cap; i++){
        if (div_arr[i] != 0){break;}
        sz--;
    }
    div_arr2 = new int[sz];
    for (int i = 0; i < sz; i++){
        div_arr2[sz-i-1] = div_arr[cap-i-1];
    }
    delete[] div_arr; div_arr = div_arr2; cap = sz;

    // Final result
    delete res; res = new UnlimitedInt(div_arr, cap, 1, sz);
    delete[] arr_1; delete[] arr_2; delete[] div_arr;

    if (i1->sign == i2->sign){res->sign = 1;}
    else {res->sign = -1;}

    return res;
}

UnlimitedInt * UnlimitedInt::mod(UnlimitedInt * i1, UnlimitedInt * i2){
    UnlimitedInt * num_1, * num_2;
    if (i1->sign == -1){num_1 = new UnlimitedInt(remove_neg_str(i1->to_string()));}
    else {num_1 = new UnlimitedInt(i1->to_string());}
    if (i2->sign == -1){num_2 = new UnlimitedInt(remove_neg_str(i2->to_string()));}
    else {num_2 = new UnlimitedInt(i2->to_string());}
 
    UnlimitedInt * quotient = UnlimitedInt::div(num_1, num_2);
    UnlimitedInt * rem = UnlimitedInt::mul(quotient, num_2);
    UnlimitedInt * mod = UnlimitedInt::sub(num_1, rem);
    delete quotient; delete rem; delete num_1; delete num_2;
    if (mod->sign != 0){mod->sign = i1->sign;}
    return mod;
}

// Helper functions

// Returns the string passed after removing the negative sign
std::string remove_neg_str(std::string str){
    std::string str2 = "";
    int size = str.length();
    for (int i = 1; i < size; i++){
        str2 += str[i];
    }
    return str2;
}

// Checks if absolute value of two unlimited integers is equal or not
bool is_abs_equal(UnlimitedInt * num1, UnlimitedInt * num2){
    if (num1->get_size() != num2->get_size()){return false;}
    int * num1_arr = num1->get_array();
    int * num2_arr = num2->get_array();
    int size = num1->get_size();
    for (int i = 0; i < size; i++){
        if (num1_arr[i] != num2_arr[i]){return false;}
    }
    return true;
}

// Checks if absolute value of first integer is greater than second
bool is_abs_greater(UnlimitedInt * num1, UnlimitedInt * num2){
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

bool is_abs_grtr_eq(int * num1_arr, int * num2_arr, int size){
    for (int i = 0; i < size; i++){
        if (num1_arr[i] > num2_arr[i]){return true;}
        else if (num1_arr[i] < num2_arr[i]){return false;}
    }
    return true;
}

void arr_sub(int * i1, int * i2, int size){
    int carry = 0, cap = size;
    for (int i = 0; i < cap; i++){
        int temp = i1[cap - i - 1];
        i1[cap - i - 1] = ((10 + i1[cap-i-1] + carry - i2[cap-i-1])%10);
        carry = ((temp + carry) >= i2[cap-i-1]) ? 0 : -1;
    }
}

void arr_add(int * i1, int * i2, int size){
    int carry = 0, cap = size;
    for (int i = 0; i < cap; i++){
        int temp = i1[cap - i - 1];
        i1[cap - i - 1] = ((i1[cap - i - 1] + i2[cap - i - 1] + carry)%10);
        carry = ((temp + i2[cap - i - 1] + carry)/10);
    }
}

