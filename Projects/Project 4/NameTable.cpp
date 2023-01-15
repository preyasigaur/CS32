//#include "NameTable.h"
//#include <string>
//#include <vector>
//#include <functional>
//using namespace std;
//
//*********NODE Struct************
//contains all the info about the nodes i.e. the value, scope, line and the pointer to the next node as we are implementing a singly linked list
//
//struct Node{
//    Node(string value, int lineNo, int scopeNo)
//    :m_val(value), m_line(lineNo), m_scope(scopeNo){            //initializing all the variables
//        next = nullptr;             //setting the pointer to start at null
//    }
//
//    string m_val;           //every node has a value
//    int m_line;             //....its line number
//    int m_scope;            //...its scoope number
//    Node* next;             // and a pointer pointing to the next node
//};
//
////***********BUCKET STRUCT*****************
////contains the buckets whose array forms the hashtable, this only contains the pointer to the node, or null if it is empty
//
//struct bucket{              //***try to get rid of this***
//    bucket(){
//        nodeptr = nullptr;          //setting the pointer to start from null
//    }
//
//    Node* nodeptr;           //every bucket has a pointer to a node
//};
//
//const int capacity = 20000;
//
//class hashTable{
//public:
//    unsigned int hashFunc(const string& value);
//    bool insert(const string& value, int lineNum, int scope, Node** &track);
//    bool search(const string& value, int lineNum, int scope);
//
//private:
//    bucket totalbucket[capacity];           //array of buckets
//};
//
//unsigned int hashTable::hashFunc(const string& value){       //this function takes in string and returns a key
//        hash<string> string_hashed;
//        unsigned int hashValue = string_hashed(value);
//        unsigned int bucketNum = hashValue % capacity;          //fits the hashed number to our buckets per the size
//
//        return (bucketNum);
//}
//
//bool hashTable::insert(const string& value, int lineNum, int scope, Node** &track){
//    int index = hashFunc(value);
//    Node* buckListPtr = totalbucket[index].nodeptr;
//    Node* newNode = new Node(value, lineNum, scope);
//
//    if(buckListPtr == nullptr){                     //if bucket is empty
//        totalbucket[index].nodeptr = newNode;
//        return true;
//    }
//
//    //if it is not null then I need to traverse the singly linked list until I find an empty spot and then make the new node
//    else{
//        Node* ptr = nullptr;
//            for (; buckListPtr!= nullptr; buckListPtr = buckListPtr->next){
//                ptr = buckListPtr;
//        }
//
//        ptr->next = newNode;
////          ptr->next->prev = buckListPtr->prev;
//        newNode->next = nullptr;
//        track = &newNode;
//        return true;
//    }
//
//    return false;
//}
//
//bool hashTable::search(const string& value, int lineNum, int scope){
//    if (lineNum < 0){
//        return false;
//    }
//
//    int index = hashFunc(value);
//    Node* buckListPtr = totalbucket[index].nodeptr;
//
//    if(buckListPtr == nullptr){
//        return (false);             //if the bucket is empty, then return false
//    }
//
//    for(Node* buckListPtr = totalbucket[index].nodeptr; buckListPtr!= nullptr; buckListPtr = buckListPtr->next){
//        if (buckListPtr->m_val == value){
//            continue;               //if the value is found in the linked list
//        }
//        else {
//            return false;
//        }
//    }
//
//    lineNum = buckListPtr->m_line;          //stores the value of the item's line in the variable lineNum
//    scope = buckListPtr->m_scope;           //stores the value of the item's line in the variable scope
//    return true;                            //if all okay, return true
//}
//
//// after making the hashTable I now need to implement the functions in the scope etc, but how?
//class NameTableImpl
//{
//  public:
//    NameTableImpl();
////    ~NameTableImpl();
//    void enterScope();
//    bool exitScope();
//    bool declare(const string& id, int lineNum);
//    int find(const string& id) const;
//
//  private:
//    hashTable* table1;
//    vector<Node**> nodeCollection;
//    int scopeNo;
//};
////
//NameTableImpl::NameTableImpl(){
//    table1 = new hashTable;
//    scopeNo = 0;
//}
////
////NameTableImpl::~NameTableImpl(){
////    size_t vSize = nodeCollection.size();
////    if(!nodeCollection.empty()){
////    for(int i = 0; i < vSize; i++){
////        if ((nodeCollection[i]) != nullptr){
////            delete (*(nodeCollection[i]));
////        }
////    }}
////    delete table1;
////}
////    size_t i = nodeCollection.size();
////
////    while (i > 0)
////
////    {
////
////        i--;
////
////        if (nodeCollection[i] != nullptr)
////
////            delete *(nodeCollection[i]);
////
////    }
////
////    delete table1;
////
////}
//
//void NameTableImpl::enterScope(){
//    nodeCollection.push_back(nullptr);         //push back a null ptr for every new scope entered, helps us keep track of the scopes in the array
//    scopeNo++;                              //increment the scope number whenever a new scope is added
//}
//
//bool NameTableImpl::exitScope(){
//    //how to exit scope?
//    size_t vSize = nodeCollection.size();
//    bool isEmpty = nodeCollection.empty();
//
//    if(isEmpty){
//        return false;}
//
//    for(int i = 0; i < vSize; i++){
//        if(nodeCollection[i] == nullptr){
//            break;
//        }
//
//        Node* delEl = *(nodeCollection[i]);
//        delete delEl;
//        delEl = nullptr;
//        nodeCollection.pop_back();
//    }
//
//    scopeNo--;
//    nodeCollection.pop_back();
//    return true;
//
//}
//
//bool NameTableImpl::declare(const string &id, int lineNum){
//    //check if it lies in the same scope, if yes return false and if not then pushback
//    int nScope = 0;
//    int nLine = 0;
//    if(lineNum < 0){
//        return false;           //if line number does not exist
//    }
//
//    bool isEmpty = id.empty();
//    if(isEmpty){
//        return false;
//    }
//
//    if(table1->search(id, nLine, nScope) == true){
//        if (nScope == scopeNo){
//            return false;
//        }
//    }
//
//    Node** ptr;
//    table1->insert(id, lineNum, scopeNo, ptr);       //inserted successfully
//    //now how do you get the pointer pointing to the node? modify the insert function
//    nodeCollection.push_back(ptr);
//
//    return true;
//}
//
//int NameTableImpl::find(const string& id) const{
//    bool isEmpty = id.empty();
//    if (!isEmpty){              //if the string is not empty
//
//        int lineG = -1, scopeG = -1;
//        bool ifFound = table1->search(id, lineG, scopeG);              //search for the specific id
//
//        if(ifFound){                                                    //if the search yields ture
//            return (lineG);                                     //...return the line number
//        }
//
//    }
//    return (-4);                    //bad input/couldn't do anything
//}
//
////*********** NameTable functions **************
//
//// For the most part, these functions simply delegate to NameTableImpl's
//// functions.
//
//NameTable::NameTable()
//{
//    m_impl = new NameTableImpl;
//}
//
//NameTable::~NameTable()
//{
//    delete m_impl;
//}
//
//void NameTable::enterScope()
//{
//    m_impl->enterScope();
//}
//
//bool NameTable::exitScope()
//{
//    return m_impl->exitScope();
//}
//
//bool NameTable::declare(const string& id, int lineNum)
//{
//    return m_impl->declare(id, lineNum);
//}
//
//int NameTable::find(const string& id) const
//{
//    return m_impl->find(id);
//}
//
//

#include "NameTable.h"
#include <string>
#include <vector>
using namespace std;

//*********NODE Struct************
//contains all the info about the nodes i.e. the value, scope, line and the pointer to the next node as we are implementing a singly linked list

struct Node{
    Node(string value, int lineNo, int scopeNo)     //initializing all the variables
    :m_val(value), m_line(lineNo), m_scope(scopeNo){
        next = nullptr;          //setting the pointer to start at null
    }
    
    string m_val;           //every node has a value
    int m_line;              //....its line number
    int m_scope;            //...its scoope number
    Node* next;         // and a pointer pointing to the next node
};

//***********BUCKET STRUCT*****************
//contains the buckets whose array forms the hashtable, this only contains the pointer to the node, or null if it is empty

struct bucket               //TODO: get rid of this
{
    bucket() {
        data = nullptr;         //setting the pointer to start from null
    }
    Node* data;         //every bucket has a pointer to a node
};

const int capacity = 19997;

class hashTable
{
public:
    unsigned int hashFunc(const string& value) const;
    bool insert(const string& value, int lineNum, int scope, Node** &track);
    bool search(const string& value, int lineNum, int scope);
private:
    bucket totalbucket[capacity];
};

unsigned int hashTable::hashFunc(const string& value) const{       //this function takes in string and returns a key
    hash<string> string_hashed;
    unsigned int hashValue = string_hashed(value);
    unsigned int bucketNum = hashValue % capacity;          //fits the hashed number to our buckets per the size

    return (bucketNum);
}

bool hashTable::search(const string& value, int lineNum, int scope)
{
    lineNum = -1;
    int h = hashFunc(value);
    Node* p = totalbucket[h].data;
    
    while (p!= nullptr)
    {
        if (p->m_val == value)
        {
            lineNum = p->m_line; // output: the line and scope of the found item
            scope = p->m_scope;
        }
        p = p->next;
    }
    
    if (lineNum == -1)    // either no item in bucket, or no id found
        return false;
    
    return true;
}

bool hashTable::insert(const string& value, int lineNo, int scopeNo, Node** &track)
{
    int index = hashFunc(value);
    Node* buckListPtr = totalbucket[index].data;
    Node* newNode = new Node(value, lineNo, scopeNo);
    
    if (buckListPtr == nullptr)    //if bucket is empty
    {
        buckListPtr = newNode;
        return true;
    }
    
    //if it is not null then I need to traverse the singly linked list until I find an empty spot and then make the new node
    else{
    Node* followPtr = nullptr;
        for (; buckListPtr!= nullptr; buckListPtr = buckListPtr->next)   //loop until empty spot found
    {
        followPtr = buckListPtr;
    }

    followPtr->next = newNode;
    track = &newNode;
    return true;
    }
    
    return false;
}

// This class does the real work of the implementation.

class NameTableImpl
{
public:
    NameTableImpl() {m_scope = 0; Table = new hashTable;}
    ~NameTableImpl();
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
private:
    hashTable* Table;
    vector<Node**> tableInfo;
    int m_scope;
};

NameTableImpl::~NameTableImpl()
{
    //need to free any leftover allocated Info memory and
    //free the allocated HashTable
    size_t i = tableInfo.size();
    
    while (i > 0)
    {
        i--;
        if (tableInfo[i] != nullptr)
            delete *(tableInfo[i]);
    }
    delete Table;
}

void NameTableImpl::enterScope()
{
    tableInfo.push_back(nullptr); // identify new scope
    m_scope++;
}

bool NameTableImpl::exitScope()
{
    size_t i = tableInfo.size();
    
    while (i > 0)
    {
        i--;
        if (tableInfo[i] == nullptr) // start of new scope found
            break;
        
        delete *(tableInfo[i]); // free memory and set pointer to null
        *(tableInfo[i]) = nullptr; //null allows us to recognize empty spot
        tableInfo.pop_back();
    }
    
    if (tableInfo.empty())  // unmatched }
        return false;
    
    tableInfo.pop_back();
    m_scope--;
    return true;
}

bool NameTableImpl::declare(const string& id, int lineNum)
{
    if (id.empty())
        return false;
    
    int scopeNum, line;
    if (Table->search(id, line, scopeNum) && scopeNum == m_scope)
        return false;   //found in same scope
    
    Node** p;
    Table->insert(id, lineNum, m_scope, p);
    tableInfo.push_back(p);
    return true;
}

int NameTableImpl::find(const string& id) const
{
    if (id.empty())
        return -1;
    
    int returnLine;
    int scopeNum;
    
    Table->search(id, returnLine, scopeNum);
    // if true, output returnLine; if false, output -1
    return returnLine;
}

//*********** SymbolTable functions **************

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.

NameTable::NameTable()
{
    m_impl = new NameTableImpl;
}

NameTable::~NameTable()
{
    delete m_impl;
}

void NameTable::enterScope()
{
    m_impl->enterScope();
}

bool NameTable::exitScope()
{
    return m_impl->exitScope();
}

bool NameTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int NameTable::find(const string& id) const
{
    return m_impl->find(id);
}

#include "NameTable.h"
#include <string>
#include <vector>
using namespace std;

//****NODE STRUCT*****
//contains all the info about the nodes i.e. the value, scope, line and the pointer to the next node as we are implementing a singly linked list

struct Node
{
    Node(string value, int lineNo, int scopeNo)
         :m_val(value), m_line(lineNo), m_scope(scopeNo)           //initializing all the variables
            {
                next = nullptr;
            }
       
        int m_line;             //....its line number
        string m_val;           //every node has a value
        int m_scope;            //...its scoope number
    
        Node* next;             //and a pointer pointing to the next node
};

//****BUCKET STRUCT******
//contains the buckets whose array forms the hashtable, this only contains the pointer to the node, or null if it is empty

struct bucket
{
    bucket() {
        data = nullptr;         //setting the pointer to start from null
    }
    Node* data;      //every bucket has a pointer to a node
};

//****HASHTABLE STRUCT******
//contains the array of buckets making up the hashtable

const int capacity = 19993;

struct HashTable
{
    unsigned int hashFunc(const string& value){         //this function takes in string and returns a key
        unsigned int i = 0;
        int index = 0;
        for (index = 0; value[index]; index++)
        {i = i + value[index];}
        unsigned int hashValue =  (i % (capacity));             //dividing the hashed key by the number of buckets to get the number to fit our buckets
        return (hashValue); }
    
    bool collisionInsert(const string& value , int lineNo, int scopeNo, Node** &r_ptr){
        int index = hashFunc(value);                //finding the hash value for the given value
        Node* ptr = arrBuckets[index].data;             //finding the bucket corresponding to that hash value
        Node* newNode = new Node(value, lineNo, scopeNo);           //making the new node
        Node* followPtr = nullptr;
        do
        {   followPtr = ptr;            //travers9ing the linked list until we find the end i.e. the nullptr
            ptr = ptr->next;
        } while (ptr != nullptr);
        followPtr->next = newNode;
        newNode->next = nullptr;                //set the next pointer from the new Node to point to null indicating the end of the linked list
        r_ptr = &followPtr->next;            //storing the address of the added
        return true; }
    
    bool emptyInsert(const string& value , int lineNo, int scopeNo, Node** &r_ptr){       //if bucket is empty
        int index = hashFunc(value);
        Node* newNode = new Node(value, lineNo, scopeNo);
        arrBuckets[index].data = newNode;               //simply set the new Node to the bucket's data
        newNode->next = nullptr;                        //set the next pointer from the new Node to point to null indicating the end of the linked list
        r_ptr = &arrBuckets[index].data;            //storing the address of the added
        return true; }
        
    
    bool insert(const string& value , int lineNo, int scopeNo, Node** &r_ptr){
        Node* ptr = arrBuckets[hashFunc(value)].data;
        if(ptr != nullptr){
            collisionInsert(value , lineNo, scopeNo, r_ptr); }      //calling the two insert helper functions to insert the value in the hashtable depending on the condition
        if (ptr == nullptr){
            emptyInsert(value , lineNo, scopeNo, r_ptr); }
        return false; }     //if nothing then return false
    
    bool exists(const string& value, int& lineNo, int& scopeNo){        //checking if the string exists
        int index = hashFunc(value);
        lineNo = -1;
        scopeNo = -1;
        Node* trackPtr = nullptr;
        string findIt = value;
        for(trackPtr = arrBuckets[index].data; trackPtr != nullptr; trackPtr = trackPtr->next){
            if (findIt == trackPtr->m_val){
                lineNo = trackPtr->m_line;
                scopeNo = trackPtr->m_scope; } }
        if (scopeNo == -1){
            return (false); }
        return (true); }
    
    bucket arrBuckets[capacity];        //array of buckets
};

// This class does the real work of the implementation.

// after making the hashTable I now need to implement the functions in the scope etc, but how?

class NameTableImpl
{
public:
    NameTableImpl() {           //the constructor for the nametable
        NTable = new HashTable;
        scopeNo = 0;
    }
    
    ~NameTableImpl(){               //...and the destructor
        for (int tmp = nodeCollec.size(); tmp != 0; --tmp)        //till the size is not zero
        {
            if (nodeCollec[tmp] != nullptr){            //if the node is not null ptr
                delete *(nodeCollec[tmp]); } }          //delete it
        delete (NTable); }          //delete the table
    
    void enterScope(){
        nodeCollec.push_back(nullptr);          //push nullptr to indicate beginning of scope
        scopeNo++; }
    
    bool exitScope() {
        size_t vSize = nodeCollec.size();
//        if (vSize == 0){
//            return false; }
        while(vSize!=0){            //till the size is not zero
            vSize--;            //deceremnt
            if (nodeCollec[vSize] == nullptr){          //if the beginning of scope, i.e. nullptr break out of the loop
                break; }
            else{
                delete *(nodeCollec[vSize]);            //...else delete the entry
                *(nodeCollec[vSize]) = nullptr;         //set it to null
                nodeCollec.pop_back(); } }              //then pop
        if (vSize == 0){                //if size is 0, then there's unbalanced bracket
            return false; }
        nodeCollec.pop_back();          //pop from the vector
        scopeNo--;                      //...and reduce scope
        return true; }

        //
        //
        //    m_scope--;
            //    tableInfo.pop_back();
        //        return true;
        //
    
    bool declare(const string& id, int lineNum){
        Node** ptr;
        int lineG, scopeG;
        if (NTable->exists(id, lineG, scopeG)){         //checks if the id exists in the table
                if(scopeG == scopeNo){              //and in the scope. If it exists in the scope return false
                    return false; } }
        if (isEmpty(id)){               //if the string is empty, return false
            return false; }
        if (lineNum < 0){               //if the line num is less than 0, return false
            return false; }
        NTable->insert(id, lineNum, scopeNo, ptr);          //if all's cool, insert in the table
        nodeCollec.push_back(ptr);                          //and push back on the vector
        return true; }                          //and return true
    
    int find(const string& id) const{
//        int lineG, scopeG;
//        if (isEmpty(id)){           //if the string is empty, return -1
//            return -1; }
//        if (!isEmpty(id)){          //..but if it is not empty, check if the id exists and return the line number
//        if(NTable->exists(id, lineG, scopeG) == true){
//            return lineG; }
//        }
//        return -1;
        int lineG, scopeG;
               if (isEmpty(id)){
                   return -1; }
               if (!isEmpty(id)){
               if(NTable->exists(id, lineG, scopeG) == true){
                   return lineG; }
               }
               return -1;
    }
    
private:
    vector<Node**> nodeCollec;
    
//    size_t vSize = nodeCollec.size();
    bool isEmpty(const string& id) const{
        if (id.size() == 0){
            return true;}
        return false;
    }
//    bool exitWhenNull(){
//        size_t vSize = nodeCollec.size();
//        while(vSize!=0){
//            vSize--;
//            if (nodeCollec[vSize] == nullptr){
//                break; }
//            else{
//                delete *(nodeCollec[vSize]);
//                *(nodeCollec[vSize]) = nullptr;
//                nodeCollec.pop_back(); }
//            }
//        nodeCollec.pop_back();
//        scopeNo--;
//        return true;
//    }
        
    int scopeNo;
    
    HashTable* NTable;

};


//**** SymbolTable functions *****

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.

NameTable::NameTable()
{
    m_impl = new NameTableImpl;
}

NameTable::~NameTable()
{
    delete m_impl;
}

void NameTable::enterScope()
{
    m_impl->enterScope();
}

bool NameTable::exitScope()
{
    return m_impl->exitScope();
}

bool NameTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int NameTable::find(const string& id) const
{
    return m_impl->find(id);
}
