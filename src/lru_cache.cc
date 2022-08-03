/**
 * lru_cache.cc
 *
 * Created by vamirio on 2022 Jul 18
 */
#include "lru_cache.h"

namespace img_view
{

const cv::Mat LruCache::kNotFound = cv::Mat();

LruCache::LruCache(int mb) : m_maxCapacity(mb * 1024 * 1024)
{
}

LruCache::~LruCache()
{
}

const cv::Mat &LruCache::get(const ImageInfo &key)
{
	CacheMap::iterator iter = m_cacheMap.find(key);
	if (iter == m_cacheMap.end())
		return kNotFound;
	update(key, m_cacheMap.at(key)->second);
	return m_cacheMap.at(key)->second;
}

void LruCache::put(const ImageInfo &key, const cv::Mat &value)
{
	if (m_cacheMap.find(key) != m_cacheMap.end()) {
		update(key, m_cacheMap.at(key)->second);
		return;
	}
	while (m_capacity >= m_maxCapacity)
		remove();
	add(key, value);
}

void LruCache::add(const ImageInfo &key, const cv::Mat &value)
{
	m_cacheList.push_back(std::make_pair(key, value));
	m_cacheMap[key] = --m_cacheList.end();
	m_capacity += key.size();
}

void LruCache::remove()
{
	ImageInfo key = m_cacheList.begin()->first;
	m_capacity -= key.size();
	m_cacheList.pop_front();
	m_cacheMap.erase(key);
}

void LruCache::update(const ImageInfo &key, const cv::Mat &value)
{
	m_cacheList.push_back(std::make_pair(key, value));
	m_cacheList.erase(m_cacheMap.at(key));
	m_cacheMap[key] = --m_cacheList.end();
}

}  /* img_view */
