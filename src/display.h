/**
 * img_area.h
 *
 * Created by vamirio on 2022 May 01
 */
#ifndef IMG_AREA_H
#define IMG_AREA_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>

#include "img_info.h"

namespace img_view {

class Display : public QWidget {
public:
	explicit Display(QWidget *parent = nullptr);
	~Display();

	/**
	 * @brief Initialize instance, MUST be called before any other member
	 *        functions called
	 */
	void init();

	/**
	 * @brief Load an image and show it
	 *
	 * @param filename The image file name
	 *
	 * @return True when succeeding
	 */
	bool loadAndShowImage(const QString &filename);

	/**
	 * @brief Load a static image and show it
	 *
	 * @param filename The static image file name
	 * @paran format The static image format, e.g. "jpeg"
	 *
	 * @return True when succeeding
	 */
	bool loadAndShowStaticImage(const QString &filename, const char *format);

	/**
	 * @brief Load a dynamic image and show it
	 *
	 * @param filename The dynamic image file name
	 *
	 * @return True when succeeding
	 */
	bool loadAndShowDynamicImage(const QString &filename);

	/**
	 * @brief Load an bmp file and show it
	 *
	 * @param filename The bmp file name
	 *
	 * @return True when succeeding
	 */
	bool loadAndShowBmp(const QString &filename);

	/**
	 * @brief Load an gif file and show it
	 *
	 * @param filename The gif file name
	 *
	 * @return True when succeeding
	 */
	bool loadAndShowGif(const QString &filename);

	/**
	 * @brief Load an jpeg file and show it
	 *
	 * @param filename The jpeg file name
	 *
	 * @return True when succeeding
	 */
	bool loadAndShowJpeg(const QString &filename);

	/**
	 * @brief Load an png file and show it
	 *
	 * @param filename The png file name
	 *
	 * @return True when succeeding
	 */
	bool loadAndShowPng(const QString &filename);

	/**
	 * @brief Load an webp file and show it
	 *
	 * @param filename The png file name
	 *
	 * @return True when succeeding
	 */
	bool loadAndShowWebp(const QString &filename);

	/**
	 * @brief Close current image and hide the display area
	 */
	void closeImage();

	/**
	 * @brief Zoom in image 10%, then show it
	 */
	void zoomInAndShow();

	/**
	 * @brief Zoom out image 10%, then show it
	 */
	void zoomOutAndShow();

	/**
	 * @brief Scale the image to @factor then show it
	 *
	 * @param factor The aimed scale factor
	 */
	void scaleImageAndShow(const double &factor);

	/**
	 * @brief Load the previous image of current directory then show it
	 */
	void loadPrevImageAndShow();

	/**
	 * @brief Load the next image of current directory then show it
	 */
	void loadNextImageAndShow();

	/**
	 * @brief Limit the image to show it fully in the display area
	 *
	 * If the image size is smaller than display area, show it in origin size;
	 * else show it in display area size while keeping its aspect.
	 * NOTE: this function may change m_initScaleFactor
	 */
	void limitToWindow();

	/**
	 * @brief Show image, should be called after any changes to image
	 */
	void showImage();

	/**
	 * @brief Move the image a distance (@dx, @dy) from the current position
	 *
	 * @param dx X-axis offset in pixel
	 * @param dy Y-axis offset in pixel
	 */
	void moveImage(const double &dx, const double &dy);

	/**
	 * @brief Get the supported MIME types
	 *
	 * @return The list of supported MIME types
	 */
	static const QList<QByteArray> supportedMimeTypes();

private:
	/**
	 * @brief Adjust the scroll bar position according to the image size, keep
	 *        the display area is the same part of the image
	 *
	 * @param scroll_bar Scroll bar to be changed
	 * @param factor The radio of the current size and origin size
	 */
	void adjustScrollBarPos(QScrollBar *scroll_bar, const double &factor);

	/**
	 * @brief Check if the current image is a dynamic image
	 *
	 * @return True when it is a dynamic image
	 */
	bool isDynamicImage() const;

protected:
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	bool eventFilter(QObject *obj, QEvent *event) override;

private:
	QScrollArea *m_scrollArea = nullptr;
	QWidget *m_displayArea = nullptr;
	QGridLayout *m_displayLayout = nullptr;
	QLabel *m_label = nullptr;
	QImage *m_image = nullptr;
	QPoint *m_mousePos = nullptr;
	ImgInfo *m_imageInfo = nullptr;
	bool m_mouseHold = false;

	/* Initial scale factor of an image, it is 1.0 in general, but will be a
	 * proper value to let the full image shown in the screen when the image
	 * is too big.
	 */
	double m_initScaleFactor = 1.0;
	/* Scale factor which control the image size, the actual size of an image
	 * is originSize * m_initScaleFactor * m_scaleFactor.
	 */
	double m_scaleFactor = 1.0;

	/* Max and min scale factors. */
	static constexpr double kMaxScaleFactor = 3.0;
	static constexpr double kMinScaleFactor = 0.333;

	/* Supported MIME types. */
	static const QList<QByteArray> kSupportedMineTypes;
	/* Supported image format. */
	static const QSet<const QByteArray> kSupportedFormats;
};

}  /* namespace img_view */

#endif /* ifndef IMG_AREA_H */
