#pragma once
#include<vector>
#include<iostream>
#include<list>
#include<algorithm>
#include<stdexcept>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap
{
private:
    std::list<std::pair<const KeyType, ValueType> > elems;
    std::vector<std::list<std::pair<const KeyType, typename std::list<std::pair<const KeyType, ValueType> >::iterator> > > place;
    int capacity = 100000;
    size_t sz = 0;
    Hash hasht;
public:
    using iterator = typename std::list<std::pair<const KeyType, ValueType> >::iterator;
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType> >::const_iterator;
    HashMap(Hash hasher = Hash()) : hasht(hasher)
    {
        place.resize(capacity);
    }
    template<class InputIter>
    HashMap(InputIter start, InputIter finish, Hash hasher = Hash(), cap=100000) : hasht(hasher)
    {
        capacity = cap;
        place.resize(capacity);
        for (InputIter it = start; it != finish; it++)
        {
            this->insert(*it);
        }
    }
    HashMap(std::initializer_list<std::pair<const KeyType, ValueType> > input, Hash hasher = Hash(), cap=100000) : hasht(hasher)
    {
        *this = Hashmap(input.begin(), input.end(), hasher, cap=cap);
    }
    Hash hash_function() const
    {
        return hasht;
    }
    iterator begin()
    {
        return elems.begin();
    }
    iterator end()
    {
        return elems.end();
    }
    const_iterator begin() const
    {
        return elems.begin();
    }
    const_iterator end() const
    {
        return elems.end();
    }
    iterator find(const KeyType key)
    {
        for (auto i = place[hasht(key) % capacity].begin(); i != place[hasht(key) % capacity].end(); i++)
        {
            if (i -> first == key)
            {
                return i -> second;
            }
        }
        return elems.end();
    }
    const_iterator find(const KeyType key) const
    {
        for (auto i = place[hasht(key) % capacity].begin(); i != place[hasht(key) % capacity].end(); i++)
        {
            if (i -> first == key)
            {
                return i -> second;
            }
        }
        return elems.end();
    }
    void insert(std::pair<const KeyType, ValueType> elem)
    {
        if (this->find(elem.first) != elems.end())
        {
            return;
        }
        sz++;
        auto tmp = elems.insert(elems.end(), elem);
        place[hasht(elem.first) % capacity].push_back(std::make_pair(elem.first, tmp));
        if (sz * 2 > capacity)
        {
            capacity *= 2;
            rebuild();
        }
    }
    void rebuild()
    {
        std::list<std::pair<const KeyType, ValueType> > tmp = elems;
        this -> clear();
        this* = Hashmap(elems, cap=capacity);
    }
    void erase(const KeyType key)
    {
        for (auto i = place[hasht(key) % capacity].begin(); i != place[hasht(key) % capacity].end(); i++)
        {
            if (i -> first == key)
            {
                sz--;
                elems.erase(i -> second);
                place[hasht(key) % capacity].erase(i);
                if (sz * 8 < capacity)
                {
                    capacity = sz * 2;
                    rebuild();
                }
                return;
            }
        }
    }
    size_t size() const
    {
        return sz;
    }
    bool empty() const
    {
        return !sz;
    }
    ValueType & operator[](const KeyType key) {
        iterator fin = this->find(key);
        if (fin != elems.end())
        {
            return fin -> second;
        }
        this->insert(std::make_pair(key, ValueType()));
        return elems.rbegin() -> second;
    }
    const ValueType & at(const KeyType key) const {
        const_iterator fin = this->find(key);
        if (fin != elems.end())
        {
            return fin -> second;
        }
        throw std::out_of_range("No such element");
    }
    void clear()
    {
        for (auto i = elems.begin(); i != elems.end(); i++)
        {
            place[hasht(i -> first) % capacity].clear();
        }
        elems.clear();
        sz = 0;
    }
};
