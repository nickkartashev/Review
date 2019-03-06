#include <bits/stdc++.h>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap
{
private:
    std::list<std::pair<const KeyType, ValueType> > elems;
    std::vector<std::list<std::pair<const KeyType, typename std::list<std::pair<const KeyType, ValueType> >::iterator> > > place;
    int maxsize = 100000;
    int sz = 0;
    Hash hasht;
public:
    using iterator = typename std::list<std::pair<const KeyType, ValueType> >::iterator;
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType> >::const_iterator;
    HashMap(Hash hasher = Hash()) : hasht(hasher)
    {
        place.resize(maxsize);
    }
    template<class InputIter>
    HashMap(InputIter start, InputIter finish, Hash hasher = Hash()) : hasht(hasher)
    {
        place.resize(maxsize);
        for (InputIter it = start; it != finish; it++)
        {
            this->insert(*it);
        }
    }
    HashMap(std::initializer_list<std::pair<const KeyType, ValueType> > input, Hash hasher = Hash())  : hasht(hasher)
    {
        place.resize(maxsize);
        for (auto it = input.begin(); it != input.end(); it++)
        {
            this->insert(*it);
        }
    }
    HashMap& operator = (const HashMap other)
    {
        this -> clear();
        hasht = other.hash_function();
        for (auto it = other.begin(); it != other.end(); it++)
        {
            this -> insert(*it);
        }
        return *this;
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
        for (auto i = place[hasht(key) % maxsize].begin(); i != place[hasht(key) % maxsize].end(); i++)
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
        for (auto i = place[hasht(key) % maxsize].begin(); i != place[hasht(key) % maxsize].end(); i++)
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
        elems.push_back(elem);
        auto tmp = elems.end();
        tmp--;
        place[hasht(elem.first) % maxsize].push_back(std::make_pair(elem.first, tmp));
    }
    void erase(const KeyType key)
    {
        for (auto i = place[hasht(key) % maxsize].begin(); i != place[hasht(key) % maxsize].end(); i++)
        {
            if (i -> first == key)
            {
                sz--;
                elems.erase(i -> second);
                place[hasht(key) % maxsize].erase(i);
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
            place[hasht(i -> first) % maxsize].clear();
        }
        elems.clear();
        sz = 0;
    }
};
