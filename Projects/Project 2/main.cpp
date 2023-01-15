//
//  main.cpp
//  Programming Project 2
//
//  Created by Preyasi Gaur on 19/04/22.
//

#include <iostream>

#include "Sequence.h"
        #include <string>
        #include <iostream>
        #include <cassert>
        using namespace std;

void test(){
    Sequence S;        //to check for constructor
            assert(S.find("Toyota") == -1);    //to check if the find function is working; return -1 if it cannot find
    assert(S.size() == 0);
            assert(S.insert("Toyota") == 0);    //to check if the insert function is working
    S.insert("Honda");
    assert(S.size() == 2);
    assert(S.find("Honda") == 0);    //To find the position of Honda
    S.insert(0, "Rolls Royce");
    S.insert(1, "Maruti");
    S.insert(2, "Toyota");
    S.insert(3, "Skoda");
    S.insert(4, "Kia");
    S.insert(4, "Mini Cooper");
    S.insert(5, "Mercedez");
    assert(S.remove("Toyota") == 2);         //to check if the remove function is working
    assert(S.size() == 7);
    S.insert(3, "Ram");
    assert(S.size() == 8);
    string x;
    assert(S.get(0, x) && x == "Rolls Royce");        //to check if the get function is working
    assert(S.set(0, "Tata"));            //to check if the set function is working
    assert(S.get(0, x) && x == "Tata");
    Sequence S1;
    S1.insert(0, "Ferraro");
    S1.insert(0, "Rotcher");
    S1.swap(S);                 //to check if the swap function is working
    assert(S.get(0, x) && x == "Rotcher");
        
}
        int main()
        {
            test();
            cout << "Passed all tests" << endl;
        }
