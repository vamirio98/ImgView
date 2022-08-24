/**
 * paper.h
 *
 * Created by vamirio on 2022 May 01
 */
#ifndef PAPER_H
#define PAPER_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <opencv2/opencv.hpp>

#include "image_info.h"
#include "lru_cache.h"

namespace img_view {

class AntialiasImage : public QWidget {
	Q_DISABLE_COPY_MOVE(AntialiasImage)

public:
	explicit AntialiasImage(QWidget* parent = nullptr) : QWidget(parent) {};
	~AntialiasImage() = default;

	const QImage& image() const;
	void setImage(const QImage& image);

protected:
	void paintEvent(QPaintEvent* ) override;

private:
	QImage _image;
};

class Paper : public QWidget {
	Q_OBJECT

public:
	explicit Paper(QWidget* parent = nullptr);
	~Paper();

	/**
	 * @brief Check if the Paper is empty (no picture shown).
	 */
	bool empty() const;

	/**
	 * @brief Browse iamge and get its information.
	 *
	 * @return True when succeeding.
	 */
	bool browse(const QString& image);

	/**
	 * @brief Browse iamge information.
	 */
	bool browse(const ImageInfo& info);

	/**
	 * @brief Draw image.
	 *
	 * @return True when succeeding.
	 */
	bool draw();

	/**
	 * @brief Erase current image.
	 */
	void erase();

	/**
	 * @brief Zoom in image STEP, then show it
	 *
	 * @param step Zoom in step, must be non-negative
	 */
	void zoomIn(const double& step);

	/**
	 * @brief Zoom out image STEP, then show it
	 *
	 * @param step Zoom out step, must be non-negative
	 */
	void zoomOut(const double& step);

	/**
	 * @brief Scale the image to FACTOR then show it
	 *
	 * @param factor The aimed scale factor
	 */
	void scale(const double& factor);

	/**
	 * @brief Limit the image to show it fully in the display area
	 *
	 * If the image size is smaller than the display area, show it in original
	 * size; else scale it to the display area size while keeping its aspect.
	 */
	void limitToWindow();

	/**
	 * @brief Move the image a distance (DX, DY) from the current position.
	 *
	 * @param dx X-axis offset in pixel
	 * @param dy Y-axis offset in pixel
	 */
	void move(const double& dx, const double& dy);

	/**
	 * @brief Get the supported MIME types
	 *
	 * @return The list of supported MIME types
	 */
	static const QList<QByteArray>& supportedMimeTypes();

signals:
	void prevOne();
	void nextOne();

private:
	/**
	 * @brief Adjust the scroll bar position according to the image size, keep
	 *        the display area is the same part of the image
	 *
	 * @param scroll_bar Scroll bar to be changed
	 * @param factor The radio of the current size and origin size
	 */
	void adjustScrollBarPos(QScrollBar* scroll_bar, const double& factor);

	/**
	 * @brief Check if the current image is a static image
	 *
	 * @return True when it is a static image
	 */
	bool isStaticImage() const;

	/**
	 * @brief Check if the current image is a dynamic image
	 *
	 * @return True when it is a dynamic image
	 */
	bool isDynamicImage() const;

	/**
	 * @brief Draw a static image.
	 *
	 * @return True when succeeding.
	 */
	bool drawStaticImage();

	/**
	 * @brief Draw a dynamic image.
	 *
	 * @return True when succeeding.
	 */
	bool drawDynamicImage();

	/**
	 * @brief Convert cv::Mat to QImage.
	 */
	static QImage mat2Qimage(const cv::Mat& src);

protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;

private:
	QScrollArea* _scrollArea = nullptr;

	bool _show = false;  /* Whether a image shown. */
	ImageInfo _imageInfo;
	LruCache<ImageInfo, cv::Mat> _cache;
	QWidget* _container = nullptr;
	AntialiasImage* _image = nullptr;
	QLabel* _movie = nullptr;

	QPoint _mousePos;

	/* Initial scale factor of an image, it is 1.0 in general, but will be a
	 * proper value to let the full image shown in the screen when the image
	 * is too big.
	 */
	double _initScaleFactor = 1.0;
	/* Scale factor which control the image size, the actual size of an image
	 * is (originSize * _initScaleFactor * _scaleFactor).
	 */
	double _scaleFactor = 1.0;

	/* Max and min scale factors. */
	static constexpr double kMaxScaleFactor = 3.0;
	static constexpr double kMinScaleFactor = 0.5;

	/* Supported MIME types. */
	static const QList<QByteArray> kSupportedMineTypes;
	/* Supported image format. */
	static const QSet<const QByteArray> kSupportedFormats;
};

}  /* img_view */

#endif /* ifndef PAPER_H */
