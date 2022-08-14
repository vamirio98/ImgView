/**
 * lru_cache.cc
 *
 * Created by vamirio on 2022 Jul 18
 */
#include "lru_cache.h"

namespace img_view
{

const cv::Mat LruCache::kNotFound = cv::Mat();

LruCache::LruCache(int mb) : _maxCapacity(mb * 1024 * 1024)
{
}

LruCache::~LruCache()
{
}

const cv::Mat& LruCache::get(const ImageInfo& key)
{
	CacheMap::iterator iter = _cacheMap.find(key);
	if (iter == _cacheMap.end())
		return kNotFound;
	update(key, _cacheMap.at(key)->second);
	return _cacheMap.at(key)->second;
}

void LruCache::put(const ImageInfo& key, const cv::Mat& value)
{
	if (_cacheMap.find(key) != _cacheMap.end()) {
		update(key, _cacheMap.at(key)->second);
		return;
	}
	while (_capacity >= _maxCapacity)
		remove();
	add(key, value);
}

void LruCache::add(const ImageInfo& key, const cv::Mat& value)
{
	_cacheList.push_back(std::make_pair(key, value));
	_cacheMap[key] = --_cacheList.end();
	_capacity += key.size();
}

void LruCache::remove()
{
	ImageInfo key = _cacheList.begin()->first;
	_capacity -= key.size();
	_cacheList.pop_front();
	_cacheMap.erase(key);
}

void LruCache::update(const ImageInfo& key, const cv::Mat& value)
{
	_cacheList.push_back(std::make_pair(key, value));
	_cacheList.erase(_cacheMap.at(key));
	_cacheMap[key] = --_cacheList.end();
}

}  /* img_view */
