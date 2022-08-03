/**
 * book.cc
 *
 * Created by vamirio on 2022 May 14
 */
#include "book.h"

#include <algorithm>

#include <QDir>
#include <QFileInfo>

#include "logger.h"

namespace img_view {

Book::Book()
{
}

Book::~Book()
{
}

bool Book::open(const QString &book)
{
	logDebug("Opening book...\n");
	/* Set m_info. */
	if (!m_info.browse(book))
		return false;

	QDir dir(book);
	QStringList filelist = dir.entryList(QDir::Files | QDir::Readable,
			QDir::Name);
	QString filepath;
	for (const QString &filename : filelist) {
		filepath = dir.filePath(filename);
		ImageInfo info;
		if (!info.browse(filepath))
			continue;
		m_pageList.emplace_back(info);
	}
	m_pageNum = 0;
	logDebug("Finished opening.\n");

	return true;
}

void Book::close()
{
	m_info = BookInfo();
	m_pageList.clear();
	m_pageNum = -1;
}

bool Book::empty() const
{
	return m_pageList.empty();
}

const BookInfo &Book::info() const
{
	return m_info;
}

const QList<ImageInfo> &Book::pageList() const
{
	return m_pageList;
}

int Book::pageNum() const
{
	return m_pageNum;
}

QString Book::bookName() const
{
	return m_info.bookName();
}

QString Book::absPath() const
{
	return m_info.absPath();
}

QString Book::coverFilename() const
{
	return m_info.coverFilename();
}

QString Book::coverFilepath() const
{
	return m_info.coverFilepath();
}

qint64 Book::lastModified() const
{
	return m_info.lastModified();
}

const ImageInfo &Book::curPage() const
{
	return m_pageList.at(m_pageNum);
}

bool Book::setCurPage(const QString &pagename)
{
	auto iter = std::find_if(m_pageList.cbegin(), m_pageList.cend(),
			[&pagename](const ImageInfo &info) {
			return info.filename() == pagename; });
	if (iter == m_pageList.cend())
		return false;
	m_pageNum = iter - m_pageList.cbegin();
	return true;
}

const ImageInfo &Book::prevPage() const
{
	return m_pageList.at(m_pageNum - (m_pageNum == 0 ? 0 : 1));
}

const ImageInfo &Book::nextPage() const
{
	return m_pageList.at(m_pageNum
			+ (m_pageNum == m_pageList.size() - 1 ? 0 : 1));
}

QList<ImageInfo> Book::prevPages(int num) const
{
	QList<ImageInfo> ret;
	int i = m_pageNum - num >= 0 ? m_pageNum - num : 0;
	for (; i != m_pageNum && num > 0; ++i, --num)
		ret.push_back(m_pageList.at(i));
	return ret;
}

QList<ImageInfo> Book::nextPages(int num) const
{
	QList<ImageInfo> ret;
	for (int i = m_pageNum + 1; i != m_pageList.size() && num > 0; ++i, --num)
		ret.push_back(m_pageList.at(i));
	return ret;
}

const ImageInfo &Book::toPrevPage()
{
	return m_pageNum == 0 ? m_pageList.at(m_pageNum)
		: m_pageList.at(--m_pageNum);
}

const ImageInfo &Book::toNextPage()
{
	return m_pageNum == m_pageList.size() - 1 ? m_pageList.at(m_pageNum)
		: m_pageList.at(++m_pageNum);
}

const ImageInfo &Book::toPage(int num)
{
	m_pageNum = num < 0 ? 0 :
		(num >= m_pageList.size() ? m_pageList.size() - 1 : num);
	return m_pageList.at(m_pageNum);
}

void Book::sortPages(Sort sort)
{
	switch (sort) {
	case Sort::NameAscending:
		std::sort(m_pageList.begin(), m_pageList.end(),
				[](const ImageInfo &lhs, const ImageInfo &rhs) {
					return lhs.filename() < rhs.filename();
				});
		break;
	case Sort::NameDescending:
		std::sort(m_pageList.begin(), m_pageList.end(),
				[](const ImageInfo &lhs, const ImageInfo &rhs) {
					return lhs.filename() > rhs.filename();
				});
		break;
	case Sort::DateAscending:
		std::sort(m_pageList.begin(), m_pageList.end(),
				[](const ImageInfo &lhs, const ImageInfo &rhs) {
					return lhs.lastModified() < rhs.lastModified();
				});
		break;
	case Sort::DateDescending:
		std::sort(m_pageList.begin(), m_pageList.end(),
				[](const ImageInfo &lhs, const ImageInfo &rhs) {
					return lhs.lastModified() > rhs.lastModified();
				});
		break;
	case Sort::SizeAscending:
		std::sort(m_pageList.begin(), m_pageList.end(),
				[](const ImageInfo &lhs, const ImageInfo &rhs) {
					return lhs.size() < rhs.size();
				});
		break;
	case Sort::SizeDescending:
		std::sort(m_pageList.begin(), m_pageList.end(),
				[](const ImageInfo &lhs, const ImageInfo &rhs) {
					return lhs.size() > rhs.size();
				});
		break;
	case Sort::Shuffle:
		for (int j = m_pageList.size() - 1; j > 0; --j) {
			int i = rand() % j;
			std::swap(m_pageList[i], m_pageList[j]);
		}
		break;
	default:
		break;
	}
}

}  /* img_view */
