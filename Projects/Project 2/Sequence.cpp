#include "Sequence.h"
#include <iostream>

Sequence::Sequence()            //default constructor for sequence
        : m_size(0)
{
    head = new Node();
    head->m_next = head;
    head->m_prev = head;
}

bool Sequence::empty() const
{
    return (m_size == 0);           //return m_size = 0 for empty
}

int Sequence::size() const
{
    return m_size;                  //return the size of the sequence
}

void Sequence::insertUnchecked(int pos, const ItemType& value)      //private function to insert the value in the sequence
{
    Node* temp = head;
    for (int i = 0; i < pos; i++)
    {
        temp = temp->m_next;
    }

    Node* newest = new Node;
    newest->m_data = value;
    newest->m_next = temp->m_next;
    newest->m_prev = temp;
    newest->m_next->m_prev = newest;
    temp->m_next = newest;
    m_size++;
}

int Sequence::insert(int pos, const ItemType& value)
{
    int result = -1;        //setting result to begin with -1
    if (pos >= 0 && pos <= m_size)          //if pos is within the size, insert at pos
    {
        insertUnchecked(pos, value);
        result = pos;               //give result as pos
    }
    return result;
}

int Sequence::insert(const ItemType& value)
{
    int p = m_size;
    int index = 0;
    for (Node* temp = head->m_next; temp != head; temp = temp->m_next)
    {
        if (value <= temp->m_data)              //if value is less than the data in node, insert
        {
            p = index;
            break;
        }
        index++;
    }
    insertUnchecked(p, value);
    return p;
}

bool Sequence::erase(int pos)
{
    bool result = false;
    if (pos >= 0 && pos < m_size)       //if the position lies within the size
    {
        Node* beforePos = head;
        for (int i = 0; i < pos; i++)
        {
            beforePos = beforePos->m_next;
        }

        Node* deleteMe = beforePos->m_next;         //adjusting poointers before erasing
        deleteMe->m_next->m_prev = beforePos;
        beforePos->m_next = deleteMe->m_next;
        delete deleteMe;
        m_size--;               //decrementing size
        result = true;
    }
    return result;
}

int Sequence::remove(const ItemType& value)
{
    int count = 0;
    int pos = 0;

    Node* temp = head->m_next;
    while (temp != head)
    {
        if (temp->m_data == value)
        {
            temp = temp->m_next;
            erase(pos);
            count++;            //incrementing count whenever erasing an item that equals value
        }
        else
        {
            temp = temp->m_next;
            pos++;              // if no mathc for value, traverse the sequence
        }
    }
    return count;               //return the total number of deleted elements
}

bool Sequence::get(int pos, ItemType& value) const
{
    bool result = false;
    if (pos >= 0 && pos < m_size)
    {
        Node* temp = head->m_next;
        for (int i = 0; i < pos; i++)           //traverse the sequence one by one
        {
            temp = temp->m_next;
        }
        value = temp->m_data;           //set value equal to the data in temp
        result = true;
    }
    return result;          //return the result
}

bool Sequence::set(int pos, const ItemType& value)
{
    if (pos < 0 || pos >= m_size)
        return false;

    Node* temp = head->m_next;
    for (int i = 0; i < pos; i++)           //traversing the string
    {
        temp = temp->m_next;
    }
    temp->m_data = value;           //set the data in node equal to the value
    return true;
}

int Sequence::find(const ItemType& value) const
{
    int p = -1;
    int index = 0;

    for (Node* temp = head->m_next; temp != head; temp = temp->m_next)      //traversing the seq one by one
    {
        if (temp->m_data == value)          //if data in node equals value then make p the index and exit the loop
        {
            p = index;
            break;
        }
        index++;
    }
    return p;
}

void Sequence::swap(Sequence& other)
{
    Node* temp = head;
    head = other.head;          //swap the head by makinga temp node
    other.head = temp;
    int tempSize = m_size;          //swap the size using a temp variabl e
    m_size = other.m_size;
    other.m_size = tempSize;
}

Sequence::~Sequence()
{
    Node* temp = head->m_next;
    while (temp != head)
    {
        Node* nextNode = temp->m_next;
        delete temp;
        temp = nextNode;
    }
    delete temp;
}

Sequence::Sequence(const Sequence& other)
        : m_size(other.m_size)
{
    head = new Node;
    head->m_next = head;
    head->m_prev = head;

    insertAndFillNode(other);
}

Sequence& Sequence::operator = (const Sequence& other)
{
    if (head == other.head)
        return *this;
    Sequence temp(other);
    swap(temp);
    return *this;
}

void Sequence::insertAndFillNode(const Sequence& other)
{
    Node* previous = head;
    Node* current = nullptr;
    for (int i = 0; i < m_size; i++)
    {
        current = new Node;
        current->m_next = head;
        current->m_prev = previous;
        head->m_prev = current;
        previous->m_next = current;
        previous = current;
    }

    Node* tempThis = head->m_next;
    Node* tempOther = other.head->m_next;

    while (tempThis != head && tempOther != other.head)
    {
        tempThis->m_data = tempOther->m_data;
        tempThis = tempThis->m_next;
        tempOther = tempOther->m_next;
    }
}

void Sequence::dump() const
{
    std::cerr << "SEQUENCE PROPERTIES:" << std::endl;
    std::cerr << "Size: " << m_size << std::endl;
    std::cerr << "Elements: ";

    Node* temp = head->m_next;
    if (head->m_prev == head)
    {
        std::cerr << "empty" << std::endl;
    }
    else
    {
        for (int i = 0; i < m_size; i++)
        {
            std::cerr << temp->m_data << ", ";
            temp = temp->m_next;
        }
    }
    std::cerr << std::endl;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    if (seq1.empty() || seq2.empty())       //if either sequence is empty return -1
        return -1;

    ItemType initial2;
    seq2.get(0, initial2);

    int matchIndex = -1;

    ItemType temp1;
    for (int i = 0; i < seq1.size(); i++)           //traverse through the sequence
    {
        seq1.get(i, temp1);
        if (temp1 == initial2)      //if the value matches in the sequences store the indices
        {
            matchIndex = i;
            ItemType temp2;
            for (int k = 0; k < seq2.size(); k++)
            {
                if (k > 0)
                    i++;
                seq2.get(k, temp2);
                seq1.get(i, temp1);
                if (temp1 != temp2)
                    break;
                else if (k == seq2.size()-1)
                    return matchIndex;
            }
        }
    }
    return -1;
}


void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    Sequence newSeq1(seq1);
    Sequence newSeq2(seq2);

    int resultSize = result.size();
    for (int i = 0; i < resultSize; i++)
    {
        result.erase(0);            //delete all values in the result sequence
    }

    int larger = 0;
    if (newSeq1.size() >= newSeq2.size())           //if one of the sequences is biger than the other sequence
        larger = newSeq1.size();
    else
        larger = newSeq2.size();
    int resultIndex = 0;

    for (int i = 0; i < larger; i++)
    {
        ItemType temp;
        if (i < newSeq1.size())
        {
            newSeq1.get(i, temp);
            result.insert(resultIndex, temp);           //traverse through the sequence assigning alternate values ro the result
            resultIndex++;
        }
        if (i < newSeq2.size())
        {
            newSeq2.get(i, temp);
            result.insert(resultIndex, temp);
            resultIndex++;
        }
    }
}
