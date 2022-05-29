/**
 * img_area.cc
 *
 * Created by vamirio on 2022 May 01
 */
#include "display.h"

#include <QGridLayout>
#include <QImageReader>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <QColorSpace>
#include <QScrollBar>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QKeyCombination>
#include <QMovie>

#include "logger.h"
#include "img_info.h"

namespace img_view {

/* TODO: add support for webp. */
const QList<QByteArray> Display::kSupportedMineTypes = {
	"image/bmp",
	"image/gif",
	"image/jpeg",
	"image/png"
};

const QSet<const QByteArray> Display::kSupportedFormats = {
	"bmp",
	"gif",
	"jpeg",
	"png"
};


Display::Display(QWidget *parent)
{
}

Display::~Display()
{
	delete m_mousePos;
	delete m_image;
	delete m_imageInfo;
}

void Display::init()
{
	m_scrollArea = new QScrollArea(this);
	m_displayArea = new QWidget(m_scrollArea);
	m_displayLayout = new QGridLayout(m_displayArea);
	m_label = new QLabel(m_displayArea);
	m_imageInfo = new ImgInfo();
	m_mousePos = new QPoint();

	setVisible(true);
	setLayout(new QGridLayout());
	layout()->addWidget(m_scrollArea);
	layout()->setContentsMargins(0, 0, 0, 0);

	m_scrollArea->setBackgroundRole(QPalette::Base);
	m_scrollArea->setWidget(m_displayArea);
	m_scrollArea->setAlignment(Qt::AlignCenter);
	m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_scrollArea->viewport()->installEventFilter(this);

	m_displayArea->setLayout(m_displayLayout);
	m_displayArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_displayLayout->addWidget(m_label, 0, 0, Qt::AlignCenter);
	m_displayLayout->setContentsMargins(0, 0, 0, 0);

	m_label->setBackgroundRole(QPalette::Dark);
	m_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

bool Display::isDynamicImage() const
{
	return m_imageInfo->format() == gif;
}

/* TODO: check image format and load. */
bool Display::loadAndShowImage(const QString &filename)
{
	//logDebug("Format: %s\n", getImgFormatStr(getImgFormat(filename)));
	m_imageInfo->setImage(filename);
	const QByteArray format = getImgFormatStr(m_imageInfo->format());
	if (kSupportedFormats.find(format) == kSupportedFormats.end()) {
		QMessageBox::information(this, QApplication::applicationDisplayName(),
				tr("Unsupported format: %1") .arg(format));
		return false;
	}

	bool ret = false;
	switch (m_imageInfo->format()) {
	case bmp:
	case jpeg:
	case png:
	case webp:
		ret = loadAndShowStaticImage(m_imageInfo->absPath(), format);
		break;
	case gif:
		ret = loadAndShowDynamicImage(m_imageInfo->absPath());
		break;
	default:
		ret = false;
		break;
	}

	return ret;
}

bool Display::loadAndShowStaticImage(const QString &filename,
		const char *format)
{
	QImageReader reader(filename, format);
	reader.setAutoTransform(true);
	reader.setAutoDetectImageFormat(false);
	QImage image = reader.read();

	if (image.isNull()) {
		QMessageBox::information(this, QApplication::applicationDisplayName(),
				tr("Can't load %1: %2")
				.arg(QDir::toNativeSeparators(filename), reader.errorString()));
		return false;
	}
	if (image.colorSpace().isValid())
		image.convertToColorSpace(QColorSpace(QColorSpace::SRgb));

	logDebug("Image origin size: %d, %d\n", m_imageInfo->dimensions().width(),
			m_imageInfo->dimensions().height());
	if (m_image) {
		delete m_image;
		m_image = nullptr;
	}
	m_image = new QImage(image);

	limitToWindow();
	showImage();

	return true;
}

bool Display::loadAndShowDynamicImage(const QString &filename)
{
	QMovie *movie = new QMovie(filename);
	if (!movie->isValid()) {
		QMessageBox::information(this, QApplication::applicationDisplayName(),
				tr("Can't load %1: %2")
				.arg(QDir::toNativeSeparators(filename),
					movie->lastErrorString()));
		return false;
	}

	m_label->setMovie(movie);
	limitToWindow();
	showImage();

	return true;
}

bool Display::loadAndShowBmp(const QString &filename)
{
	return loadAndShowStaticImage(filename, "bmp");
}

bool Display::loadAndShowGif(const QString &filename)
{
	return loadAndShowDynamicImage(filename);
}

bool Display::loadAndShowJpeg(const QString &filename)
{
	return loadAndShowStaticImage(filename, "jpeg");
}

bool Display::loadAndShowPng(const QString &filename)
{
	return loadAndShowStaticImage(filename, "png");
}

bool Display::loadAndShowWebp(const QString &filename)
{
	return loadAndShowStaticImage(filename, "webp");
}


void Display::zoomInAndShow()
{
	scaleImageAndShow(m_scaleFactor + 0.05);
}

void Display::zoomOutAndShow()
{
	scaleImageAndShow(m_scaleFactor - 0.05);
}

void Display::scaleImageAndShow(const double &factor)
{
	double prev_factor = m_scaleFactor;
	m_scaleFactor = factor > kMaxScaleFactor ? kMaxScaleFactor
		: (factor < kMinScaleFactor ? kMinScaleFactor : factor);

	showImage();

	adjustScrollBarPos(m_scrollArea->horizontalScrollBar(),
			m_scaleFactor / prev_factor);
	adjustScrollBarPos(m_scrollArea->verticalScrollBar(),
			m_scaleFactor / prev_factor);
}

void Display::limitToWindow()
{
	QSize window_size = size();
	QSize image_size = m_imageInfo->dimensions();
	m_initScaleFactor = m_scaleFactor = 1.0;

	if (image_size.width() < window_size.width()
			&& image_size.height() < window_size.height())
		return;

	double w_ratio = 1.0 * window_size.width() / image_size.width();
	double h_ratio = 1.0 * window_size.height() / image_size.height();

	/* The image may be a little larger than the window if the constant is
	 * 1.0. */
	m_initScaleFactor = 0.99 * (w_ratio < h_ratio ? w_ratio : h_ratio);
	logDebug("Initial scale factor: %f", m_initScaleFactor);
}

void Display::showImage()
{
	logDebug("Image origin size: %d, %d", m_imageInfo->dimensions().width(),
			m_imageInfo->dimensions().height());
	if (!isDynamicImage()) {
		m_label->setPixmap(QPixmap::fromImage(m_image->scaled(
					m_imageInfo->dimensions()
					* m_initScaleFactor * m_scaleFactor,
					Qt::KeepAspectRatio,
					Qt::SmoothTransformation)));
	} else {
		m_label->movie()->setScaledSize(
					m_imageInfo->dimensions()
					* m_initScaleFactor * m_scaleFactor);
		m_label->movie()->start();
	}
	m_label->resize(m_imageInfo->dimensions()
			* m_initScaleFactor * m_scaleFactor);
	m_displayArea->resize(m_label->size());
}

void Display::closeImage()
{
	if (m_image) {
		delete m_image;
		m_image = nullptr;
	}
	QMovie *movie = m_label->movie();
	if (movie)
		delete movie;
	m_label->setMovie(nullptr);
	m_label->setPixmap(QPixmap());
}

void Display::adjustScrollBarPos(QScrollBar *scroll_bar, const double &factor)
{
	scroll_bar->setValue(static_cast<int>(scroll_bar->value() * factor
				+ (factor - 1) * scroll_bar->pageStep() / 2));
}

void Display::loadPrevImageAndShow()
{
	//logDebug("Call goToPrevImage.");
}

void Display::loadNextImageAndShow()
{
	//logDebug("Call goToNextImage.");
}

void Display::moveImage(const double &dx, const double &dy)
{
	QScrollBar *h = m_scrollArea->horizontalScrollBar();
	QScrollBar *v = m_scrollArea->verticalScrollBar();

	h->setValue(h->value() + dx);
	v->setValue(v->value() + dy);
}

const QList<QByteArray> Display::supportedMimeTypes()
{
	return kSupportedMineTypes;
}

/* Events. */
/* TODO: zoom the image around the cursor. */
void Display::wheelEvent(QWheelEvent *event)
{
	/* Most mouse type work in steps of 15 degrees, so the delta value is a
	 * multiple of 120.
	 */
	int step = event->angleDelta().y() / 120;
	if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
		step > 0 ? zoomInAndShow() : zoomOutAndShow();
	} else {
		step > 0 ? loadPrevImageAndShow() : loadNextImageAndShow();
	}

	event->accept();
}

void Display::mousePressEvent(QMouseEvent *event)
{
	m_mouseHold = true;
	*m_mousePos = event->pos();

	switch (event->button()) {
	case Qt::MiddleButton:
		scaleImageAndShow(1.0);
		break;
	default:
		break;
	}

	event->accept();
}

void Display::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();
	double dx = pos.x() - m_mousePos->x();
	double dy = pos.y() - m_mousePos->y();
	moveImage(-dx, -dy);

	*m_mousePos = pos;
}

void Display::mouseReleaseEvent(QMouseEvent *event)
{
	m_mouseHold = false;

	event->accept();
}

bool Display::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == m_scrollArea->viewport()) {
		if (event->type() == QEvent::Wheel) {
			event->ignore();  /* Pass event to its parent object. */
			return true;
		} else {
			return false;
		}
	} else {
		/* Pass the event on to the parent class. */
		return QWidget::eventFilter(obj, event);
	}
}

}  /* namespace img_view */
