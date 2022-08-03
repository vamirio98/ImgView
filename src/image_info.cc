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

#include "logger.h"

namespace img_view {

/* kFormat and kCheckFormat hould have the same order. */
const QVector<ImageFormat> kFormat {
	ImageFormat::bmp,
	ImageFormat::gif,
	ImageFormat::jpeg,
	ImageFormat::png,
	ImageFormat::webp
};
const QVector<bool (*)(QDataStream &)> kCheckFormat {
	isBmp, isGif, isJpeg, isPng, isWebp
};

const QMap<ImageFormat, const char *> kFormatStr {
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
bool compare(QDataStream &data, const qint64 &offset,
		const qint64 &len, const char *const buf);

bool compare(QDataStream &data, const qint64 &offset,
		const qint64 &len, const char *const buf)
{
	data.device()->seek(offset);

	char *magic = new char[len];
	bool res = data.readRawData(magic, len) != -1
		&& memcmp(magic, buf, len) == 0;
	delete[] magic;

	return res;
}

ImageFormat getImageFormat(const QString &filepath)
{
	QFile img(filepath);
	img.open(QIODevice::ReadOnly);
	QDataStream data(&img);
	ImageFormat format = getImgFormat(data);
	img.close();

	return format;
}

ImageFormat getImgFormat(QDataStream &data)
{
	for (int i = 0; i != kCheckFormat.size(); ++i) {
		if (kCheckFormat[i](data))
			return kFormat[i];
	}
	return ImageFormat::unknown;
}

const char *imageFormatToStr(const ImageFormat &format)
{
	return kFormatStr[format];
}

bool isBmp(const QString &image)
{
	QFile img(image);
	if (!img.open(QIODevice::ReadOnly))
		return false;
	QDataStream data(&img);
	bool ret = compare(data, 0, 2, "\x42\x4D");
	img.close();

	return ret;
}

bool isBmp(QDataStream &data)
{
	qint64 pos = data.device()->pos();
	bool ret = compare(data, 0, 2, "\x42\x4D");
	data.device()->seek(pos);

	return ret;
}

bool isGif(const QString &image)
{
	QFile img(image);
	if (!img.open(QIODevice::ReadOnly))
		return false;
	QDataStream data(&img);
	bool ret = compare(data, 0, 4, "\x47\x49\x46\x38");
	img.close();

	return ret;
}

bool isGif(QDataStream &data)
{
	qint64 pos = data.device()->pos();
	bool ret = compare(data, 0, 4, "\x47\x49\x46\x38");
	data.device()->seek(pos);

	return ret;
}

bool isJpeg(const QString &image)
{
	QFile img(image);
	if (!img.open(QIODevice::ReadOnly))
		return false;
	QDataStream data(&img);
	bool ret = compare(data, 0, 2, "\xFF\xD8");
	img.close();

	return ret;
}

bool isJpeg(QDataStream &data)
{
	qint64 pos = data.device()->pos();
	bool ret = compare(data, 0, 2, "\xFF\xD8");
	data.device()->seek(pos);

	return ret;
}

bool isPng(const QString &image)
{
	QFile img(image);
	img.open(QIODevice::ReadOnly);
	QDataStream data(&img);
	bool ret = compare(data, 0, 8, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A");
	img.close();

	return ret;
}

bool isPng(QDataStream &data)
{
	qint64 pos = data.device()->pos();
	bool ret = compare(data, 0, 8, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A");
	data.device()->seek(pos);

	return ret;
}

bool isWebp(const QString &image)
{
	QFile img(image);
	img.open(QIODevice::ReadOnly);
	QDataStream data(&img);
	bool ret = compare(data, 0, 4, "\x52\x49\x46\x46")
		&& compare(data, 8, 4, "\x57\x45\x42\x50");
	img.close();

	return ret;
}

bool isWebp(QDataStream &data)
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


ImageInfo::ImageInfo(const ImageInfo &rhs) : m_size(rhs.m_size),
	m_lastModified(rhs.m_lastModified), m_format(rhs.m_format),
	m_width(rhs.m_width), m_height(rhs.m_height), m_depth(rhs.m_depth)
{
	m_path = new char[strlen(rhs.m_path) + 1];
	strcpy(m_path, rhs.m_path);
	m_filename = m_path + strlen(m_path) - strlen(rhs.m_filename);
	m_extension = m_path + strlen(m_path) - strlen(rhs.m_extension);
}

ImageInfo ImageInfo::operator=(const ImageInfo &rhs)
{
	if (this == &rhs)
		return *this;

	char *newdata = new char[strlen(rhs.m_path) + 1];
	if (m_path) {
		delete[] m_path;
		m_path = m_filename = m_extension = nullptr;
	}
	strcpy(m_path, rhs.m_path);
	m_filename = m_path + strlen(m_path) - strlen(rhs.m_filename);
	m_extension = m_path + strlen(m_path) - strlen(rhs.m_extension);
	m_size = rhs.m_size;
	m_lastModified = rhs.m_lastModified;
	m_format = rhs.m_format;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_depth = rhs.m_depth;

	return *this;
}

ImageInfo::ImageInfo(ImageInfo &&rhs) noexcept : m_path(rhs.m_path),
	m_filename(rhs.m_filename), m_extension(rhs.m_extension),
	m_size(rhs.m_size), m_lastModified(rhs.m_lastModified),
	m_format(rhs.m_format), m_width(rhs.m_width), m_height(rhs.m_height),
	m_depth(rhs.m_depth)
{
	rhs.m_path = rhs.m_filename = rhs.m_extension = nullptr;
}

ImageInfo ImageInfo::operator=(ImageInfo &&rhs) noexcept
{
	if (this == &rhs)
		return *this;

	m_path = rhs.m_path;
	m_filename = rhs.m_filename;
	m_extension = rhs.m_extension;
	m_size = rhs.m_size;
	m_lastModified = rhs.m_lastModified;
	m_format = rhs.m_format;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_depth = rhs.m_depth;

	rhs.m_path = rhs.m_filename = rhs.m_extension = nullptr;

	return *this;
}

ImageInfo::~ImageInfo()
{
	if (m_path)
		delete[] m_path;
}

/* TODO: check the encode. */
bool ImageInfo::browse(const QString &image)
{
	QFileInfo info(image);
	if (!info.exists() || !info.isReadable()
			|| getImageFormat(image) == ImageFormat::unknown)
		return false;

	if (m_path) {
		delete[] m_path;
		m_path = m_filename = m_extension = nullptr;
	}

	QByteArray path = info.canonicalFilePath().toUtf8();
	QByteArray filename = info.fileName().toUtf8();
	QByteArray extension = info.suffix().toUtf8();

	m_path = new char[path.size() + 1];
	strcpy(m_path, path);
	m_filename = m_path + path.size() - filename.size();
	m_extension = m_filename + filename.size() - extension.size();

	m_size = info.size();
	m_lastModified = info.lastModified().toMSecsSinceEpoch();

	m_format = getImageFormat(image);

	QImage img(image, imageFormatToStr(m_format));
	m_width = img.width();
	m_height = img.height();
	m_depth = img.depth();

	return true;
}

QString ImageInfo::absPath() const
{
	return m_path;
}

QString ImageInfo::filename() const
{
	return m_filename;
}

QString ImageInfo::extension() const
{
	return m_extension;
}

ImageFormat ImageInfo::format() const
{
	return m_format;
}

qint64 ImageInfo::size() const
{
	return m_size;
}

int ImageInfo::width() const
{
	return m_width;
}

int ImageInfo::height() const
{
	return m_height;
}

QSize ImageInfo::dimensions() const
{
	return QSize(m_width, m_height);
}

qint64 ImageInfo::pixels() const
{
	return m_width * m_height;
}

qint64 ImageInfo::lastModified() const
{
	return m_lastModified;
}

int ImageInfo::depth() const
{
	return m_depth;
}

bool operator==(const ImageInfo &lhs, const ImageInfo &rhs)
{
	return strcmp(lhs.m_path, rhs.m_path) == 0;
}

bool operator!=(const ImageInfo &lhs, const ImageInfo &rhs)
{
	return !(lhs == rhs);
}

}  /* img_view */
