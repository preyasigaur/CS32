#include "NameTable.h"
#include <string>
#include <vector>
using namespace std;

//****NODE STRUCT*****
//contains all the info about the nodes i.e. the value, scope, line and the pointer to the next node as we are implementing a singly linked list

struct Node
{
    Node(string value, int lineNo, int scopeNo)
    :m_val(value), m_line(lineNo), m_scope(scopeNo){           //initializing all the variables
                next = nullptr; }
       
        int m_line;             //....its line number
        string m_val;           //every node has a value
        int m_scope;            //...its scoope number
    
        Node* next;             //and a pointer pointing to the next node
};

//****CONTAINER STRUCT******
//contains the buckets whose array forms the hashtable, this only contains the pointer to the node, or null if it is empty

struct container
{
    container() {
        data = nullptr;         //setting the pointer to start from null
    }
    Node* data;      //every bucket has a pointer to a node
};

//****HASHTABLE STRUCT******
//contains the array of buckets making up the hashtable

const int capacity = 19997;

struct hashTable
{
    unsigned int hashFunc(const string& value){       //this function takes in string and returns a key
            hash<string> string_hashed;
            unsigned int hashValue = string_hashed(value);
            unsigned int bucketNum = hashValue % capacity;          //fits the hashed number to our buckets per the size
    
            return (bucketNum);
    }
    
    bool collisionInsert(const string& value , int lineNo, int scopeNo, Node** &r_ptr){
        int index = hashFunc(value);                //finding the hash value for the given value
        Node* ptr = arrCont[index].data;             //finding the bucket corresponding to that hash value
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
        arrCont[index].data = newNode;               //simply set the new Node to the bucket's data
        newNode->next = nullptr;                        //set the next pointer from the new Node to point to null indicating the end of the linked list
        r_ptr = &arrCont[index].data;            //storing the address of the added
        return true; }
        
    
    bool insert(const string& value , int lineNo, int scopeNo, Node** &r_ptr){
        Node* ptr = arrCont[hashFunc(value)].data;
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
        for(trackPtr = arrCont[index].data; trackPtr != nullptr; trackPtr = trackPtr->next){
            if (findIt == trackPtr->m_val){
                lineNo = trackPtr->m_line;
                scopeNo = trackPtr->m_scope; } }
        if (scopeNo == -1){
            return (false); }
        return (true); }
    
    container arrCont[capacity];        //array of buckets
};

// after making the hashTable I now need to implement the functions in the scope etc, but how?

class NameTableImpl
{
public:
    NameTableImpl() {           //the constructor for the nametable
        NTable = new hashTable;
        scopeNo = 0;
    }
    
    ~NameTableImpl(){               //...and the destructor
        for (size_t tmp = nodeCollec.size(); tmp != 0; --tmp)        //till the size is not zero
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
//        if(!VectorEmpty){
        while(vSize!=0){            //till the size is not zero
            vSize--;            //deceremnt
            if (nodeCollec[vSize] == nullptr){ break; }         //if the beginning of scope, i.e. nullptr break out of the loop
            else if(nodeCollec[vSize] != nullptr){
                delete *(nodeCollec[vSize]);            //...else delete the entry
                *(nodeCollec[vSize]) = nullptr;         //set it to null
                nodeCollec.pop_back(); } }             //then pop
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
        if (isStringEmpty(id)){               //if the string is empty, return false
            return false; }
//        if (lineNum < 0){               //if the line num is less than 0, return false
//            return false; }
        NTable->insert(id, lineNum, scopeNo, ptr);          //if all's cool, insert in the table
        nodeCollec.push_back(ptr);                          //and push back on the vector
        return true; }                          //and return true
    
    int find(const string& id) const{
        int lineG, scopeG;
        if (isStringEmpty(id)){           //if the string is empty, return -1
            return -1; }
        if (!isStringEmpty(id)){          //..but if it is not empty, check if the id exists and return the line number
        if(NTable->exists(id, lineG, scopeG) == true){
            return lineG; }
        }
        return -1; }
    
private:
    vector<Node**> nodeCollec;
    
//    size_t vSize = nodeCollec.size();
    bool isStringEmpty(const string& id) const{
        if (id.size() == 0){            //if string is empty, return true
            return true;}
        return false;
    }
    
    bool VectorEmpty = nodeCollec.empty();
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
    
    hashTable* NTable;

};

//**** NAMETABLE FUNCTIONS *****

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
