/**
 * img_info.cc
 *
 * Created by vamirio on 2022 May 14
 */
#include "image_info.h"

#include <QFileInfo>
#include <QImage>
#include <QVector>
#include <QMap>

namespace img_view {

/* kFormat and kCheckFormat hould have the same order. */
const QVector<ImageFormat> kFormat {
	ImageFormat::bmp,
	ImageFormat::gif,
	ImageFormat::jpeg,
	ImageFormat::png,
	ImageFormat::webp
};
const QVector<bool (*)(QDataStream&)> kCheckFormat {
	isBmp, isGif, isJpeg, isPng, isWebp
};

const QMap<ImageFormat, const char* > kFormatStr {
	{ ImageFormat::unknown, "unknown" },
	{ ImageFormat::bmp, "bmp"},
	{ ImageFormat::gif, "gif" },
	{ ImageFormat::jpeg, "jpeg" },
	{ ImageFormat::png, "png" },
	{ ImageFormat::webp, "webp" }
};

/**
 * @brief Compare LEN bytes raw data between DATA that offsets OFFSET bytes
 *        from the beginning and BUF
 *
 * NOTE: this function will change the data stream status, you should save the
 * status yourself if you want to use it after calling this function.
 *
 * @return True when they are same
 */
bool compare(QDataStream& data, const qint64& offset,
		const qint64& len, const char* const buf);

bool compare(QDataStream& data, const qint64& offset,
		const qint64& len, const char* const buf)
{
	data.device()->seek(offset);

	char* magic = new char[len];
	bool res = data.readRawData(magic, len) != -1
		&& memcmp(magic, buf, len) == 0;
	delete[] magic;

	return res;
}

ImageFormat getImageFormat(const QString& filepath)
{
	QFile img(filepath);
	img.open(QIODevice::ReadOnly);
	QDataStream data(&img);
	ImageFormat format = getImgFormat(data);
	img.close();

	return format;
}

ImageFormat getImgFormat(QDataStream& data)
{
	for (int i = 0; i != kCheckFormat.size(); ++i) {
		if (kCheckFormat[i](data))
			return kFormat[i];
	}
	return ImageFormat::unknown;
}

const char* imageFormatToStr(const ImageFormat& format)
{
	return kFormatStr[format];
}

bool isBmp(const QString& image)
{
	QFile img(image);
	if (!img.open(QIODevice::ReadOnly))
		return false;
	QDataStream data(&img);
	bool ret = compare(data, 0, 2, "\x42\x4D");
	img.close();

	return ret;
}

bool isBmp(QDataStream& data)
{
	qint64 pos = data.device()->pos();
	bool ret = compare(data, 0, 2, "\x42\x4D");
	data.device()->seek(pos);

	return ret;
}

bool isGif(const QString& image)
{
	QFile img(image);
	if (!img.open(QIODevice::ReadOnly))
		return false;
	QDataStream data(&img);
	bool ret = compare(data, 0, 4, "\x47\x49\x46\x38");
	img.close();

	return ret;
}

bool isGif(QDataStream& data)
{
	qint64 pos = data.device()->pos();
	bool ret = compare(data, 0, 4, "\x47\x49\x46\x38");
	data.device()->seek(pos);

	return ret;
}

bool isJpeg(const QString& image)
{
	QFile img(image);
	if (!img.open(QIODevice::ReadOnly))
		return false;
	QDataStream data(&img);
	bool ret = compare(data, 0, 2, "\xFF\xD8");
	img.close();

	return ret;
}

bool isJpeg(QDataStream& data)
{
	qint64 pos = data.device()->pos();
	bool ret = compare(data, 0, 2, "\xFF\xD8");
	data.device()->seek(pos);

	return ret;
}

bool isPng(const QString& image)
{
	QFile img(image);
	img.open(QIODevice::ReadOnly);
	QDataStream data(&img);
	bool ret = compare(data, 0, 8, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A");
	img.close();

	return ret;
}

bool isPng(QDataStream& data)
{
	qint64 pos = data.device()->pos();
	bool ret = compare(data, 0, 8, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A");
	data.device()->seek(pos);

	return ret;
}

bool isWebp(const QString& image)
{
	QFile img(image);
	img.open(QIODevice::ReadOnly);
	QDataStream data(&img);
	bool ret = compare(data, 0, 4, "\x52\x49\x46\x46")
		&& compare(data, 8, 4, "\x57\x45\x42\x50");
	img.close();

	return ret;
}

bool isWebp(QDataStream& data)
{
	qint64 pos = data.device()->pos();
	bool ret = compare(data, 0, 4, "\x52\x49\x46\x46")
		&& compare(data, 8, 4, "\x57\x45\x42\x50");
	data.device()->seek(pos);

	return ret;
}

ImageInfo::ImageInfo()
{
}


ImageInfo::ImageInfo(const ImageInfo& rhs) : _size(rhs._size),
	_lastModified(rhs._lastModified), _format(rhs._format),
	_width(rhs._width), _height(rhs._height), _depth(rhs._depth)
{
	_path = new char[strlen(rhs._path) + 1];
	strcpy(_path, rhs._path);
	_filename = _path + strlen(_path) - strlen(rhs._filename);
	_extension = _path + strlen(_path) - strlen(rhs._extension);
}

ImageInfo ImageInfo::operator=(const ImageInfo& rhs)
{
	if (this == &rhs)
		return *this;

	char* newdata = new char[strlen(rhs._path) + 1];
	if (_path) {
		delete[] _path;
		_path = _filename = _extension = nullptr;
	}
	strcpy(_path, rhs._path);
	_filename = _path + strlen(_path) - strlen(rhs._filename);
	_extension = _path + strlen(_path) - strlen(rhs._extension);
	_size = rhs._size;
	_lastModified = rhs._lastModified;
	_format = rhs._format;
	_width = rhs._width;
	_height = rhs._height;
	_depth = rhs._depth;

	return *this;
}

ImageInfo::ImageInfo(ImageInfo&& rhs) noexcept : _path(rhs._path),
	_filename(rhs._filename), _extension(rhs._extension),
	_size(rhs._size), _lastModified(rhs._lastModified),
	_format(rhs._format), _width(rhs._width), _height(rhs._height),
	_depth(rhs._depth)
{
	rhs._path = rhs._filename = rhs._extension = nullptr;
}

ImageInfo ImageInfo::operator=(ImageInfo&& rhs) noexcept
{
	if (this == &rhs)
		return *this;

	_path = rhs._path;
	_filename = rhs._filename;
	_extension = rhs._extension;
	_size = rhs._size;
	_lastModified = rhs._lastModified;
	_format = rhs._format;
	_width = rhs._width;
	_height = rhs._height;
	_depth = rhs._depth;

	rhs._path = rhs._filename = rhs._extension = nullptr;

	return *this;
}

ImageInfo::~ImageInfo()
{
	if (_path)
		delete[] _path;
}

/* TODO: check the encode. */
bool ImageInfo::browse(const QString& image)
{
	QFileInfo info(image);
	if (!info.exists() || !info.isReadable()
			|| getImageFormat(image) == ImageFormat::unknown)
		return false;

	if (_path) {
		delete[] _path;
		_path = _filename = _extension = nullptr;
	}

	QByteArray path = info.canonicalFilePath().toUtf8();
	QByteArray filename = info.fileName().toUtf8();
	QByteArray extension = info.suffix().toUtf8();

	_path = new char[path.size() + 1];
	strcpy(_path, path);
	_filename = _path + path.size() - filename.size();
	_extension = _filename + filename.size() - extension.size();

	_size = info.size();
	_lastModified = info.lastModified().toMSecsSinceEpoch();

	_format = getImageFormat(image);

	QImage img(image, imageFormatToStr(_format));
	_width = img.width();
	_height = img.height();
	_depth = img.depth();

	return true;
}

QString ImageInfo::absPath() const
{
	return _path;
}

QString ImageInfo::filename() const
{
	return _filename;
}

QString ImageInfo::extension() const
{
	return _extension;
}

ImageFormat ImageInfo::format() const
{
	return _format;
}

qint64 ImageInfo::size() const
{
	return _size;
}

int ImageInfo::width() const
{
	return _width;
}

int ImageInfo::height() const
{
	return _height;
}

QSize ImageInfo::dimensions() const
{
	return QSize(_width, _height);
}

qint64 ImageInfo::pixels() const
{
	return _width * _height;
}

qint64 ImageInfo::lastModified() const
{
	return _lastModified;
}

int ImageInfo::depth() const
{
	return _depth;
}

bool operator==(const ImageInfo& lhs, const ImageInfo& rhs)
{
	return strcmp(lhs._path, rhs._path) == 0;
}

bool operator!=(const ImageInfo& lhs, const ImageInfo& rhs)
{
	return !(lhs == rhs);
}

}  /* img_view */
