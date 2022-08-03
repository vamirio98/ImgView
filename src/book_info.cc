/**
 * book_info.cc
 *
 * Created by vamirio on 2022 Jul 18
 */
#include "book_info.h"

#include <QFileInfo>
#include <QDir>

#include "image_info.h"

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

}  /* img_view */
