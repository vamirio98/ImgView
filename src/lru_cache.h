/**
 * lru_cache.h
 *
 * LRU (Least Recently Used) Cache Class
 *
 * Created by vamirio on 2022 Jul 18
 */
#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <list>
#include <unordered_map>

namespace img_view
{

template <typename Key, typename Value>
class LruCache {
private:
	typedef std::list<std::pair<Key, Value>> CacheList;
	typedef std::unordered_map<Key, typename CacheList::iterator> CacheMap;

public:
	static const Value kNotFound;

public:
	/**
	 * @param n Max number of cache items.
	 */
	LruCache(int n = 30) : _maxCapacity(n) {}
	~LruCache() {}

	/**
	 * @brief Get the value corresponding with the KEY.
	 *
	 * @param key The key.
	 *
	 * @return The value corresponding with the KEY, or
	 *         LruCache<Key, Value>::kNotFound, the default construct object of
	 *         Value class, when there is no value corresponding with the KEY.
	 */
	const Value& get(const Key& key)
	{
		if (_cacheMap.find(key) == _cacheMap.end())
			return kNotFound;
		update(key, _cacheMap.at(key)->second);
		return _cacheList.back().second;
	}

	/**
	 * @brief Put the specified KEY - VALUE pair into cache.
	 */
	void put(const Key& key, const Value& value)
	{
		if (_cacheMap.find(key) != _cacheMap.end()) {
			update(key, _cacheMap.at(key)->second);
			return;
		}
		if (_capacity >= _maxCapacity)
			remove();
		add(key, value);
	}

private:
	/* Add a new KEY - VALUE pair. */
	void add(const Key& key, const Value& value)
	{
		_cacheList.push_back(std::make_pair(key, value));
		_cacheMap[key] = --_cacheList.end();
		++_capacity;
	}

	/* Remove the the least recently used cache. */
	void remove()
	{
		Key key = _cacheList.front().first;
		_cacheList.pop_front();
		_cacheMap.erase(key);
		--_capacity;
	}

	/* Update the KEY - VALUE pair, make it the most recently used. */
	void update(const Key& key, const Value& value)
	{
		_cacheList.push_back(std::make_pair(key, value));
		_cacheList.erase(_cacheMap.at(key));
		_cacheMap[key] = --_cacheList.end();
	}

private:
	CacheList _cacheList;
	CacheMap _cacheMap;
	int _capacity = 0;
	int _maxCapacity;
};

template <typename Key, typename Value>
const Value LruCache<Key, Value>::kNotFound = Value();

};  /* img_view */

#endif  /* LRU_CACHE_H */
