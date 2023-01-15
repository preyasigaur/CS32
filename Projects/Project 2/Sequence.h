#ifndef Sequence_h
#define Sequence_h

#include <string>
using ItemType = std::string;

class Sequence
{
public:
    Sequence();
    bool empty() const;
    int size() const;
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
    ~Sequence();
    Sequence(const Sequence& other);
    Sequence& operator = (const Sequence& other);
    void dump() const;

private:
    struct Node
    {
        ItemType m_data;
        Node* m_next;
        Node* m_prev;
    };

    Node* head;
    int m_size;

    void insertUnchecked(int pos, const ItemType& value);
    void insertAndFillNode(const Sequence& other);
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif /* Sequence_h */
