/**
 * book.cc
 *
 * Created by vamirio on 2022 May 14
 */
#include "book.h"

#include <algorithm>

#include <QDir>
#include <QFileInfo>

#include "img_info.h"
#include "logger.h"

namespace img_view {

BookInfo::BookInfo(const QString &path)
{
	QDir dir(path);
	if (!dir.exists())
		return;
	QFileInfo fileinfo(path);
	m_path = dir.absolutePath();
	m_lastModified = fileinfo.lastModified().toMSecsSinceEpoch();
	QStringList filelist = dir.entryList(QDir::Files | QDir::Readable,
			QDir::Name);
	QString filepath;
	for (const QString &filename : filelist) {
		filepath = m_path + "/" + filename;
		if (getImgFormat(filepath) == unknown)
			continue;
		m_coverFilepath = filepath;
		break;
	}
}

Book::Book()
{
}

Book::Book(const QString &path)
{
	setBook(path);
}

Book::~Book()
{
}

bool Book::setBook(const QString &path)
{
	logDebug("Book.cc: starting setBook().\n");
	/* Set m_info. */
	m_info = BookInfo(path);
	if (m_info.m_path.isEmpty())
		return false;

	/* Set m_pages. */
	QDir dir(path);
	m_pages = QList<ImgInfo>();
	QStringList filelist = dir.entryList(QDir::Files | QDir::Readable,
			QDir::Name);
	QString filepath;
	for (const QString &filename : filelist) {
		filepath = dir.absolutePath() + "/" + filename;
		if (getImgFormat(filepath) == unknown)
			continue;
		m_pages.emplaceBack(filepath);
	}
	m_currPage = m_pages.begin();

	/* Set m_books. */
	m_books = QList<BookInfo>();
	QStringList dirlist = dir.entryList(QDir::Dirs | QDir::Readable,
			QDir::Name);
	for (const QString &dirname : dirlist)
		m_books.emplaceBack(dirname);

	logDebug("Book.cc: finished setBook().\n");

	return true;
}

QString Book::bookName() const
{
	qsizetype pos = m_info.m_path.lastIndexOf('/');
	if (pos == -1)
		return m_info.m_path;
	return m_info.m_path.sliced(pos + 1);
}

QString Book::absPath() const
{
	return m_info.m_path;
}

qint64 Book::lastModified() const
{
	return m_info.m_lastModified;
}

QString Book::coverFilepath() const
{
	return m_info.m_coverFilepath;
}

const ImgInfo &Book::currPageInfo() const
{
	return *m_currPage;
}

bool Book::setCurrPage(const QString &filepath)
{
	QList<ImgInfo>::iterator iter = std::find(m_pages.begin(), m_pages.end(),
			ImgInfo(filepath));
	if (iter == m_pages.end())
		return false;
	m_currPage = iter;
	return true;
}

QString Book::currPageFilepath() const
{
	return m_currPage->absPath();
}

QString Book::toPrevPage()
{
	return m_currPage == m_pages.begin()
			? m_currPage->absPath() : (--m_currPage)->absPath();
}

QString Book::toNextPage()
{
	return m_currPage == m_pages.end() - 1
			? m_currPage->absPath() : (++m_currPage)->absPath();
}

bool Book::noPage() const
{
	return m_pages.empty();
}

}  /* namespace img_view */
