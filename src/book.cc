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

bool BookInfo::browse(const QString &book)
{
	QFileInfo info(book);
	if (!info.exists() || !info.isDir() || !info.isReadable())
		return false;
	m_lastModified = info.lastModified().toMSecsSinceEpoch();

	QDir dir(book);
	m_absPath = dir.absolutePath().toUtf8();
	m_bookname = dir.dirName().toUtf8();
	QStringList filelist = dir.entryList(QDir::Files | QDir::Readable,
			QDir::Name);
	QString filepath;
	for (const QString &filename : filelist) {
		filepath = dir.filePath(filename);
		if (getImageFormat(filepath) == ImageFormat::unknown)
			continue;
		m_coverFilepath = filepath.toUtf8();
		m_coverFilename = filename.toUtf8();
		break;
	}
	return true;
}

bool BookInfo::empty() const
{
	return m_absPath.isEmpty();
}

QString BookInfo::absPath() const
{
	return m_absPath;
}

QString BookInfo::bookName() const
{
	return m_bookname;
}

QString BookInfo::coverFilepath() const
{
	return m_coverFilepath;
}

QString BookInfo::coverFilename() const
{
	return m_coverFilename;
}

qint64 BookInfo::lastModified() const
{
	return m_lastModified;
}

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

}  /* img_view */
