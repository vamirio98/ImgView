/**
 * book.cc
 *
 * Created by vamirio on 2022 May 14
 */
#include "book.h"

#include <algorithm>

#include <QDir>
#include <QFileInfo>

#include "debug.h"

namespace img_view
{

	Book::Book() {}

	Book::~Book() {}

	bool Book::open(const QString& book)
	{
		gDebug() << "Opening book...";
		/* Set _info. */
		if (!_info.browse(book))
			return false;

		QDir dir(book);
		QStringList filelist =
		    dir.entryList(QDir::Files | QDir::Readable, QDir::Name);
		QString filepath;
		/* TODO: uncorrect sequence. */
		for (const QString& filename : filelist) {
			filepath = dir.filePath(filename);
			ImageInfo info;
			if (!info.getBasicInfo(filepath))
				continue;
			_pageList.emplace_back(info);
		}
		_pageNum = 0;
		gDebug() << "Finished opening.";

		return true;
	}

	void Book::close()
	{
		_info = BookInfo();
		_pageList.clear();
		_pageNum = -1;
	}

	bool Book::empty() const { return _pageList.empty(); }

	const BookInfo& Book::info() const { return _info; }

	const QList<ImageInfo>& Book::pageList() const { return _pageList; }

	int Book::pageNum() const { return _pageNum; }

	QString Book::bookName() const { return _info.bookName(); }

	QString Book::absPath() const { return _info.absPath(); }

	QString Book::coverFilename() const { return _info.coverFilename(); }

	QString Book::coverFilepath() const { return _info.coverFilepath(); }

	qint64 Book::lastModified() const { return _info.lastModified(); }

	const ImageInfo& Book::curPage()
	{
		if (!_pageList.at(_pageNum).hasDetailInfo())
			_pageList[_pageNum].getDetailInfo();
		return _pageList.at(_pageNum);
	}

	bool Book::setCurPage(const QString& pagename)
	{
		auto iter = std::find_if(
		    _pageList.cbegin(), _pageList.cend(),
		    [&pagename](const ImageInfo& info) {
			    return info.absPath() == pagename;
		    }
		);
		if (iter == _pageList.cend())
			return false;
		_pageNum = iter - _pageList.cbegin();
		return true;
	}

	const ImageInfo& Book::prevPage()
	{
		int aimPageNum = _pageNum - (_pageNum == 0 ? 0 : 1);
		if (!_pageList.at(aimPageNum).hasDetailInfo())
			_pageList[aimPageNum].getDetailInfo();
		return _pageList.at(aimPageNum);
	}

	const ImageInfo& Book::nextPage()
	{
		int aimPageNum = _pageNum + (_pageNum == _pageList.size() - 1 ? 0 : 1);
		if (!_pageList.at(aimPageNum).hasDetailInfo())
			_pageList[aimPageNum].getDetailInfo();
		return _pageList.at(aimPageNum);
	}

	QList<ImageInfo> Book::prevPages(int num)
	{
		QList<ImageInfo> ret;
		int i = _pageNum - num >= 0 ? _pageNum - num : 0;
		for (; i != _pageNum && num > 0; ++i, --num) {
			if (!_pageList.at(i).hasDetailInfo())
				_pageList[i].getDetailInfo();
			ret.push_back(_pageList.at(i));
		}
		return ret;
	}

	QList<ImageInfo> Book::nextPages(int num)
	{
		QList<ImageInfo> ret;
		for (int i = _pageNum + 1; i != _pageList.size() && num > 0;
		     ++i, --num) {
			if (!_pageList.at(i).hasDetailInfo())
				_pageList[i].getDetailInfo();
			ret.push_back(_pageList.at(i));
		}
		return ret;
	}

	const ImageInfo& Book::toPrevPage()
	{
		_pageNum -= _pageNum == 0 ? 0 : 1;
		if (!_pageList.at(_pageNum).hasDetailInfo())
			_pageList[_pageNum].getDetailInfo();
		return _pageList.at(_pageNum);
	}

	const ImageInfo& Book::toNextPage()
	{
		_pageNum += _pageNum == _pageList.size() - 1 ? 0 : 1;
		if (!_pageList.at(_pageNum).hasDetailInfo())
			_pageList[_pageNum].getDetailInfo();
		return _pageList.at(_pageNum);
	}

	const ImageInfo& Book::toPage(int num)
	{
		_pageNum = num < 0
		               ? 0
		               : (num >= _pageList.size() ? _pageList.size() - 1 : num);
		if (!_pageList.at(_pageNum).hasDetailInfo())
			_pageList[_pageNum].getDetailInfo();
		return _pageList.at(_pageNum);
	}

	void Book::sortPages(Sort sort)
	{
		switch (sort) {
		case Sort::NameAscending:
			std::sort(
			    _pageList.begin(), _pageList.end(),
			    [](const ImageInfo& lhs, const ImageInfo& rhs) {
				    return lhs.filename() < rhs.filename();
			    }
			);
			break;
		case Sort::NameDescending:
			std::sort(
			    _pageList.begin(), _pageList.end(),
			    [](const ImageInfo& lhs, const ImageInfo& rhs) {
				    return lhs.filename() > rhs.filename();
			    }
			);
			break;
		case Sort::DateAscending:
			std::sort(
			    _pageList.begin(), _pageList.end(),
			    [](const ImageInfo& lhs, const ImageInfo& rhs) {
				    return lhs.lastModified() < rhs.lastModified();
			    }
			);
			break;
		case Sort::DateDescending:
			std::sort(
			    _pageList.begin(), _pageList.end(),
			    [](const ImageInfo& lhs, const ImageInfo& rhs) {
				    return lhs.lastModified() > rhs.lastModified();
			    }
			);
			break;
		case Sort::SizeAscending:
			std::sort(
			    _pageList.begin(), _pageList.end(),
			    [](const ImageInfo& lhs, const ImageInfo& rhs) {
				    return lhs.size() < rhs.size();
			    }
			);
			break;
		case Sort::SizeDescending:
			std::sort(
			    _pageList.begin(), _pageList.end(),
			    [](const ImageInfo& lhs, const ImageInfo& rhs) {
				    return lhs.size() > rhs.size();
			    }
			);
			break;
		case Sort::Shuffle:
			for (int j = _pageList.size() - 1; j > 0; --j) {
				int i = rand() % j;
				std::swap(_pageList[i], _pageList[j]);
			}
			break;
		default:
			break;
		}
	}

} // namespace img_view
