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

enum ImgFormat {
	unknown = 0,
	bmp,
	gif,
	jpeg,
	png,
	webp
};

/**
 * @brief Get the image format of the @filepath
 *
 * @return The image format
 */
ImgFormat getImgFormat(const QString &filepath);

/**
 * @brief Get the image format of the data stream @data
 *
 * @return The image format
 */
ImgFormat getImgFormat(QDataStream &data);

/**
 * @brief Covert the ImgFormat to string
 *
 * @return The pointer to the format string, do NOT modify or delete it, make a
 *         deep copy of it if you want to modify it
 */
const char *imgFormatToStr(const ImgFormat &format);

/**
 * @brief Check if the file @filepath is a bmp file
 */
bool isBmp(const QString &filepath);

/**
 * @brief Check if the file @filepath is a bmp file
 */
bool isBmp(QDataStream &data);

/**
 * @brief Check if the file @filepath is a gif file
 */
bool isGif(const QString &filepath);

/**
 * @brief Check if the file @filepath is a gif file
 */
bool isGif(QDataStream &data);

/**
 * @brief Check if the file @filepath is a jpeg file
 */
bool isJpeg(const QString &filepath);

/**
 * @brief Check if the file @filepath is a jpeg file
 */
bool isJpeg(QDataStream &data);

/**
 * @brief Check if the file @filepath is a png file
 */
bool isPng(const QString &filepath);

/**
 * @brief Check if the file @filepath is a png file
 */
bool isPng(QDataStream &data);

/**
 * @brief Check if the file @filepath is a webp file
 */
bool isWebp(const QString &filepath);

/**
 * @brief Check if the file @filepath is a webp file
 */
bool isWebp(QDataStream &data);

class ImgInfo {
	friend bool operator==(const ImgInfo &lhs, const ImgInfo &rhs);
	friend bool operator!=(const ImgInfo &lhs, const ImgInfo &rhs);

public:
	ImgInfo();
	ImgInfo(const QString &filepath);
	ImgInfo(const ImgInfo &rhs);
	ImgInfo operator=(const ImgInfo &rhs);
	ImgInfo(ImgInfo &&rhs) noexcept;
	ImgInfo operator=(ImgInfo &&rhs) noexcept;
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

bool operator==(const ImgInfo &lhs, const ImgInfo &rhs);
bool operator!=(const ImgInfo &lhs, const ImgInfo &rhs);

}  /* namespace img_view */

#endif /* ifndef IMG_INFO_H */
