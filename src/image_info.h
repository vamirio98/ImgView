/**
 * image_info.h
 *
 * Created by vamirio on 2022 May 14
 */
#ifndef IMAGE_INFO_H
#define IMAGE_INFO_H

#include <QString>
#include <QSize>
#include <QFile>

namespace img_view {

enum class ImageFormat {
	unknown = 0,
	bmp,
	gif,
	jpeg,
	png,
	webp
};

/**
 * @brief Get the format of the IMAGE
 *
 * @return The image format
 */
ImageFormat getImageFormat(const QString& image);

/**
 * @brief Get the format of the image data stream IMAGE
 *
 * @return The image format
 */
ImageFormat getImgFormat(QDataStream& image);

/**
 * @brief Covert the ImageFormat to string
 *
 * @return The pointer to the format string, do NOT modify or delete it, make a
 *         deep copy of it if you want to modify it
 */
const char* imageFormatToStr(const ImageFormat& format);

/**
 * @brief Check if the file @filepath is a bmp file
 */
bool isBmp(const QString& filepath);

/**
 * @brief Check if the file @filepath is a bmp file
 */
bool isBmp(QDataStream& data);

/**
 * @brief Check if the file @filepath is a gif file
 */
bool isGif(const QString& filepath);

/**
 * @brief Check if the file @filepath is a gif file
 */
bool isGif(QDataStream& data);

/**
 * @brief Check if the file @filepath is a jpeg file
 */
bool isJpeg(const QString& filepath);

/**
 * @brief Check if the file @filepath is a jpeg file
 */
bool isJpeg(QDataStream& data);

/**
 * @brief Check if the file @filepath is a png file
 */
bool isPng(const QString& filepath);

/**
 * @brief Check if the file @filepath is a png file
 */
bool isPng(QDataStream& data);

/**
 * @brief Check if the file @filepath is a webp file
 */
bool isWebp(const QString& filepath);

/**
 * @brief Check if the file @filepath is a webp file
 */
bool isWebp(QDataStream& data);

class ImageInfo {
	friend bool operator==(const ImageInfo& lhs, const ImageInfo& rhs);
	friend bool operator!=(const ImageInfo& lhs, const ImageInfo& rhs);

public:
	ImageInfo();
	ImageInfo(const ImageInfo& rhs);
	ImageInfo operator=(const ImageInfo& rhs);
	ImageInfo(ImageInfo&& rhs) noexcept;
	ImageInfo operator=(ImageInfo&& rhs) noexcept;
	~ImageInfo();

	/**
	 * @brief Browse the image and get its infomation
	 *
	 * @param iamge The filepath of the image browsed
	 *
	 * @return True when succeeded, false when the image is not exist, is not
	 *         an image file, or you don't have the permission to open it
	 */
	bool browse(const QString& image);

	/**
	 * @brief Get the absolute path of the image
	 */
	QString absPath() const;

	/**
	 * @brief Get the image filename
	 */
	QString filename() const;

	/**
	 * @brief Get the image file extension
	 */
	QString extension() const;

	/**
	 * @brief Get the image MIME format
	 */
	ImageFormat format() const;

	/**
	 * @brief Get the image file size in bytes
	 */
	qint64 size() const;

	/**
	 * @brief Get the image width in pixels
	 */
	int width() const;

	/**
	 * @brief Get the image height in pixels
	 */
	int height() const;

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

	/**
	 * @brief Check if the image information is empty.
	 */
	bool empty() const;

private:
	char* _path = nullptr;
	char* _filename = nullptr;
	char* _extension = nullptr;
	qint64 _size = 0;
	qint64 _lastModified = 0;
	ImageFormat _format = ImageFormat::unknown;
	int _width = 0;
	int _height = 0;
	int _depth = 0;
};

bool operator==(const ImageInfo& lhs, const ImageInfo& rhs);
bool operator!=(const ImageInfo& lhs, const ImageInfo& rhs);

}  /* img_view */

namespace std
{

template<>
struct hash<img_view::ImageInfo> {
	std::size_t operator()(const img_view::ImageInfo& key) const
	{
		return hash<QString>{}(key.absPath())
			^ (hash<qint64>{}(key.lastModified()) << 1);
	}
};

}  /* std */

#endif /* ifndef IMAGE_INFO_H */
