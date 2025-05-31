#pragma once
#include <vector>
#include <algorithm>

namespace SchmixEngine
{
	template<typename K, typename V>
	class FlatMap
	{
	public:
		using iterator = typename std::vector<std::pair<K, V>>::iterator;
		using const_iterator = typename std::vector<std::pair<K, V>>::const_iterator;

		FlatMap() = default;

		iterator begin();
		const_iterator begin() const;

		iterator end();
		const_iterator end() const;

		size_t size() const;

		void clear();

		iterator erase(iterator pos);
		iterator erase(const_iterator pos);

		std::pair<iterator, bool> insert(std::pair<K,V> item);

		iterator find(const K& key);
		const_iterator find(const K& key) const;

		V& operator[](const K& key);

	private:
		static bool predicate(const std::pair<K, V>& lhs, const std::pair<K, V>& rhs);

		std::vector<std::pair<K, V>> m_KeyValuePairs;
	};
}

#include <SchmixEngine/Core/Util/FlatMap.inl>