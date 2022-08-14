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
#include <opencv2/opencv.hpp>

#include "image_info.h"

namespace img_view
{

class LruCache {
private:
	typedef std::list<std::pair<ImageInfo, cv::Mat>> CacheList;
	typedef std::unordered_map<ImageInfo, CacheList::iterator> CacheMap;

public:
	static const cv::Mat kNotFound;

public:
	LruCache(int mb = 100);
	~LruCache();

	/**
	 * @brief Get the value corresponding with the KEY,
	 *
	 * @param key The key.
	 *
	 * @return The value corresponding with the KEY, or LruCache::kNotFound when
	 *         there is no value corresponding with the KEY
	 */
	const cv::Mat& get(const ImageInfo& key);

	/**
	 * @brief Put the specified KEY - VALUE pair into cache.
	 */
	void put(const ImageInfo& key, const cv::Mat& value);

private:
	/* Add a new KEY - VALUE pair. */
	void add(const ImageInfo& key, const cv::Mat& value);
	/* Remove the the least recently used cache. */
	void remove();
	/* Update the KEY - VALUE pair, make it the most recently used. */
	void update(const ImageInfo& key, const cv::Mat& value);

private:
	CacheList _cacheList;
	CacheMap _cacheMap;
	qint64 _capacity = 0;
	qint64 _maxCapacity;
};

};  /* img_view */

#endif  /* LRU_CACHE_H */
