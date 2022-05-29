/**
 * img_info.h
 *
 * Created by vamirio on 2022 May 14
 */
#ifndef IMG_INFO_H
#define IMG_INFO_H

#include <QString>
#include <QSize>
#include <QFile>

namespace img_view {

/* TODO: add support for bmp and webp. */
enum ImgFormat {
	unknown = 0,
	bmp,
	gif,
	jpeg,
	png,
	webp
};

ImgFormat getImgFormat(const QString &filepath);
ImgFormat getImgFormat(QDataStream &data);
const char *getImgFormatStr(const ImgFormat &format);
bool isBmp(const QString &filepath);
bool isBmp(QDataStream &data);
bool isGif(const QString &filepath);
bool isGif(QDataStream &data);
bool isJpeg(const QString &filepath);
bool isJpeg(QDataStream &data);
bool isPng(const QString &filepath);
bool isPng(QDataStream &data);
bool isWebp(const QString &filepath);
bool isWebp(QDataStream &data);

class ImgInfo {

public:
	ImgInfo(const QString &filepath);
	ImgInfo();
	~ImgInfo();

	/**
	 * @brief Set the image that ImgInfo provides infomation about to @filepath
	 */
	void setImage(const QString &filepath);

	/**
	 * @brief Get the absolute file path
	 */
	QString absPath() const;

	/**
	 * @brief Get the filename
	 */
	QString filename() const;

	/**
	 * @brief Get the file extension
	 */
	QString extension() const;

	/**
	 * @brief Get the image format
	 */
	ImgFormat format() const;

	/**
	 * @brief Get the image file size in bytes
	 */
	qint64 size() const;

	/**
	 * @brief Get the image width in pixels
	 */
	qint64 width() const;

	/**
	 * @brief Get the image height in pixels
	 */
	qint64 height() const;

	/**
	 * @brief Get the dimensions of the image
	 */
	QSize dimensions() const;

	/**
	 * @brief Get the image size in pixels
	 */
	qint64 pixels() const;

	/**
	 * @brief Get the image last modified timestamp as the number of
	 *        milliseconds that have passed since 1970-01-01T00:00:00.000
	 */
	qint64 lastModified() const;

	/**
	 * @brief Get the image depth
	 */
	int depth() const;

private:
	/**
	 * @brief Initialize ImgInfo
	 */
	void initImgInfo(const QString &filepath);

private:
	char *m_path = nullptr;
	char *m_filename = nullptr;
	char *m_extension = nullptr;
	qint64 m_size = 0;
	qint64 m_lastModified = 0;
	ImgFormat m_format = unknown;
	qint64 m_width = 0;
	qint64 m_height = 0;
	int m_depth = 0;
};

}  /* namespace img_view */

#endif /* ifndef IMG_INFO_H */
