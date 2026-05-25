#include <iostream>

int* getIntPtr() {
    int number = 5;
    int* ptr = &number;
    return ptr;
}

int main(){
    //int val[] = {1, 2, 3};

    // int* val = new int[3];
    // std::cout << "val[3] = " << val[3] << std::endl;

    // int* val = new int[4];
    // int* same_mem = val;

    // std::cout << val << " " << same_mem << std::endl;
    // delete [] val;
    //delete [] same_mem;

    int* ptr = getIntPtr();
    std::cout << ptr << " " << *ptr << std::endl;

    return 0;
}