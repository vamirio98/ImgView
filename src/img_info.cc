/**
 * img_info.cc
 *
 * Created by vamirio on 2022 May 14
 */
#include "img_info.h"

#include <QFileInfo>
#include <QImage>
#include <QVector>
#include <QMap>

#include "logger.h"

namespace img_view {

/* kFormat and kCheckFormat hould have the same order. */
const QVector<ImgFormat> kFormat {
	bmp, gif, jpeg, png, webp
};
const QVector<bool (*)(QDataStream &)> kCheckFormat {
	isBmp, isGif, isJpeg, isPng, isWebp
};

const QMap<ImgFormat, const char *> kFormatStr {
	{ unknown, "unknown" },
	{ bmp, "bmp"},
	{ gif, "gif" },
	{ jpeg, "jpeg" },
	{ png, "png" },
	{ webp, "webp" }
};

/**
 * @brief Compare @len bytes raw data between @data that offsets @offset bytes
 *        from the beginning and @buf
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

ImgFormat getImgFormat(const QString &filepath)
{
	QFile img(filepath);
	img.open(QIODevice::ReadOnly);
	QDataStream data(&img);
	ImgFormat format = getImgFormat(data);
	img.close();

	return format;
}

ImgFormat getImgFormat(QDataStream &data)
{
	for (int i = 0; i != kCheckFormat.size(); ++i) {
		if (kCheckFormat[i](data))
			return kFormat[i];
	}
	return unknown;
}

const char *imgFormatToStr(const ImgFormat &format)
{
	return kFormatStr[format];
}

bool isBmp(const QString &filepath)
{
	QFile img(filepath);
	img.open(QIODevice::ReadOnly);
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

bool isGif(const QString &filepath)
{
	QFile img(filepath);
	img.open(QIODevice::ReadOnly);
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

bool isJpeg(const QString &filepath)
{
	QFile img(filepath);
	img.open(QIODevice::ReadOnly);
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

bool isPng(const QString &filepath)
{
	QFile img(filepath);
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

bool isWebp(const QString &filepath)
{
	QFile img(filepath);
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

ImgInfo::ImgInfo()
{
}

ImgInfo::ImgInfo(const QString &filepath)
{
	initImgInfo(filepath);
}

/* TODO: check the encode. */
void ImgInfo::initImgInfo(const QString &filepath)
{
	QFileInfo info(filepath);

	QByteArray path = info.canonicalFilePath().toUtf8();
	QByteArray filename = info.fileName().toUtf8();
	QByteArray extension = info.suffix().toUtf8();

	m_path = new char[path.size() + 1];
	strcpy(m_path, path);
	m_filename = m_path + path.size() - filename.size();
	m_extension = m_filename + filename.size() - extension.size();

	m_size = info.size();
	m_lastModified = info.lastModified().toMSecsSinceEpoch();

	m_format = getImgFormat(filepath);

	QImage image(filepath, imgFormatToStr(m_format));
	m_width = image.width();
	m_height = image.height();
	m_depth = image.depth();
}

ImgInfo::ImgInfo(const ImgInfo &rhs) : m_size(rhs.m_size),
	m_lastModified(rhs.m_lastModified), m_format(rhs.m_format),
	m_width(rhs.m_width), m_height(rhs.m_height), m_depth(rhs.m_depth)
{
	m_path = new char[strlen(rhs.m_path) + 1];
	strcpy(m_path, rhs.m_path);
	m_filename = m_path + strlen(m_path) - strlen(rhs.m_filename);
	m_extension = m_path + strlen(m_path) - strlen(rhs.m_extension);
}

ImgInfo ImgInfo::operator=(const ImgInfo &rhs)
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

ImgInfo::ImgInfo(ImgInfo &&rhs) noexcept : m_path(rhs.m_path),
	m_filename(rhs.m_filename), m_extension(rhs.m_extension),
	m_size(rhs.m_size), m_lastModified(rhs.m_lastModified),
	m_format(rhs.m_format), m_width(rhs.m_width), m_height(rhs.m_height),
	m_depth(rhs.m_depth)
{
	rhs.m_path = rhs.m_filename = rhs.m_extension = nullptr;
}

ImgInfo ImgInfo::operator=(ImgInfo &&rhs) noexcept
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

ImgInfo::~ImgInfo()
{
	if (m_path)
		delete[] m_path;
}

void ImgInfo::setImage(const QString &filepath)
{
	if (m_path) {
		delete[] m_path;
		m_path = m_filename = m_extension = nullptr;
	}
	initImgInfo(filepath);
}

QString ImgInfo::absPath() const
{
	return m_path;
}

QString ImgInfo::filename() const
{
	return m_filename;
}

QString ImgInfo::extension() const
{
	return m_extension;
}

ImgFormat ImgInfo::format() const
{
	return m_format;
}

qint64 ImgInfo::size() const
{
	return m_size;
}

qint64 ImgInfo::width() const
{
	return m_width;
}

qint64 ImgInfo::height() const
{
	return m_height;
}

QSize ImgInfo::dimensions() const
{
	return QSize(m_width, m_height);
}

qint64 ImgInfo::pixels() const
{
	return m_width * m_height;
}

qint64 ImgInfo::lastModified() const
{
	return m_lastModified;
}

int ImgInfo::depth() const
{
	return m_depth;
}

bool operator==(const ImgInfo &lhs, const ImgInfo &rhs)
{
	return strcmp(lhs.m_path, rhs.m_path) == 0;
}

bool operator!=(const ImgInfo &lhs, const ImgInfo &rhs)
{
	return !(lhs == rhs);
}

}  /* namespace img_view */
