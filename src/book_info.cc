/**
 * book_info.cc
 *
 * Created by vamirio on 2022 Jul 18
 */
#include "book_info.h"

#include <QDir>
#include <QFileInfo>

#include "image_info.h"

namespace img_view
{

bool BookInfo::browse(const QString& book)
{
	QFileInfo info(book);
	if (!info.exists() || !info.isDir() || !info.isReadable())
		return false;
	_lastModified = info.lastModified().toMSecsSinceEpoch();

	QDir dir(book);
	_absPath = dir.absolutePath().toUtf8();
	_bookname = dir.dirName().toUtf8();
	QStringList filelist =
	    dir.entryList(QDir::Files | QDir::Readable, QDir::Name);
	QString filepath;
	for (const QString& filename : filelist) {
		filepath = dir.filePath(filename);
		if (getImageFormat(filepath) == ImageFormat::unknown)
			continue;
		_coverFilepath = filepath.toUtf8();
		_coverFilename = filename.toUtf8();
		break;
	}
	return true;
}

bool BookInfo::empty() const { return _absPath.isEmpty(); }

QString BookInfo::absPath() const { return _absPath; }

QString BookInfo::bookName() const { return _bookname; }

QString BookInfo::coverFilepath() const { return _coverFilepath; }

QString BookInfo::coverFilename() const { return _coverFilename; }

qint64 BookInfo::lastModified() const { return _lastModified; }

} // namespace img_view
