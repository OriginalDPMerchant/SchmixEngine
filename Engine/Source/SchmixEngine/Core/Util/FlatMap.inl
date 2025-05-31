#include <SchmixEngine/Core/Util/FlatMap.h>

namespace SchmixEngine
{
    template<typename K, typename V>
    inline typename FlatMap<K, V>::iterator FlatMap<K, V>::begin()
    {
        return m_KeyValuePairs.begin();
    }

    template<typename K, typename V>
    inline typename FlatMap<K, V>::const_iterator FlatMap<K, V>::begin() const
    {
        return m_KeyValuePairs.begin();
    }

    template<typename K, typename V>
    inline typename FlatMap<K, V>::iterator FlatMap<K, V>::end()
    {
        return m_KeyValuePairs.end();
    }

    template<typename K, typename V>
    inline typename FlatMap<K, V>::const_iterator FlatMap<K, V>::end() const
    {
        return m_KeyValuePairs.end();
    }

    template<typename K, typename V>
    inline size_t FlatMap<K, V>::size() const
    {
        return m_KeyValuePairs.size();
    }

    template<typename K, typename V>
    inline void FlatMap<K, V>::clear()
    {
        m_KeyValuePairs.clear();
    }

    template<typename K, typename V>
    inline typename FlatMap<K, V>::iterator FlatMap<K, V>::erase(iterator pos)
    {
        return m_KeyValuePairs.erase(pos);
    }

    template<typename K, typename V>
    inline typename FlatMap<K, V>::iterator FlatMap<K, V>::erase(const_iterator pos)
    {
        return m_KeyValuePairs.erase(pos);
    }

    template<typename K, typename V>
    inline std::pair<typename FlatMap<K, V>::iterator, bool> FlatMap<K, V>::insert(std::pair<K, V> Iteratorem)
    {
        auto Iterator = std::lower_bound(m_KeyValuePairs.begin(), m_KeyValuePairs.end(), Iteratorem, predicate);

        if (Iterator != m_KeyValuePairs.end() && Iterator->first == Iteratorem.first)
            return std::make_pair(Iterator, false);

        Iterator = m_KeyValuePairs.insert(Iterator, Iteratorem);
        return std::make_pair(Iterator, true);
    }

    template<typename K, typename V>
    inline typename FlatMap<K, V>::iterator FlatMap<K, V>::find(const K& key)
    {
        auto Iterator = std::lower_bound(m_KeyValuePairs.begin(), m_KeyValuePairs.end(), std::make_pair(key, V()), predicate);

        if (Iterator != m_KeyValuePairs.end() && Iterator->first == key)
            return Iterator;

        return m_KeyValuePairs.end();
    }

    template<typename K, typename V>
    inline typename FlatMap<K, V>::const_iterator FlatMap<K, V>::find(const K& key) const
    {
        auto Iterator = std::lower_bound(m_KeyValuePairs.begin(), m_KeyValuePairs.end(), std::make_pair(key, V()), predicate);

        if (Iterator != m_KeyValuePairs.end() && Iterator->first == key)
            return Iterator;

        return m_KeyValuePairs.end();
    }

    template<typename K, typename V>
    inline V& FlatMap<K, V>::operator[](const K& key)
    {
        auto Iterator = find(key);
        if (Iterator != end())
            return Iterator->second;

        Iterator = insert(std::make_pair(key, V()));
        return Iterator.first->second;
    }

    template<typename K, typename V>
    inline bool FlatMap<K, V>::predicate(const std::pair<K, V>& lhs, const std::pair<K, V>& rhs)
    {
        return lhs.first < rhs.first;
    }
}