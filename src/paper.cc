/**
 * drawing_board.cc
 *
 * Created by vamirio on 2022 May 01
 */
#include "paper.h"

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
#include <QPainter>
#include <QStyle>
#include <qboxlayout.h>
#include <qwidget.h>

#include "debug.h"
#include "image_info.h"

namespace img_view {

/* TODO: add support for webp. */
const QList<QByteArray> Paper::kSupportedMineTypes = {
	"image/bmp",
	"image/gif",
	"image/jpeg",
	"image/png",
	"image/webp"
};

const QSet<const QByteArray> Paper::kSupportedFormats = {
	"bmp",
	"gif",
	"jpeg",
	"png",
	"webp"
};

const QImage& AntialiasImage::image() const
{
	return _image;
}

void AntialiasImage::setImage(const QImage& image)
{
	_image = image;
	update();
}

void AntialiasImage::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing
	                       | QPainter::SmoothPixmapTransform, true);
	painter.drawImage(QRectF(rect()), _image);
}


Paper::Paper(QWidget* parent)
{
	_scrollArea = new QScrollArea(this);
	_container = new QWidget(_scrollArea);
	_image = new AntialiasImage(_scrollArea);
	_movie = new QLabel(_scrollArea);

	setVisible(true);
	setLayout(new QGridLayout());
	layout()->addWidget(_scrollArea);
	layout()->setContentsMargins(0, 0, 0, 0);

	_scrollArea->setBackgroundRole(QPalette::Base);
	_scrollArea->setAlignment(Qt::AlignCenter);
	_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_scrollArea->viewport()->installEventFilter(this);

	_scrollArea->setWidget(_container);
	QVBoxLayout* lay = new QVBoxLayout(_container);
	lay->addWidget(_image);
	lay->addWidget(_movie);
}

Paper::~Paper()
{
}

bool Paper::browse(const QString& image)
{
	return _imageInfo.browse(image)
		&& kSupportedFormats.find(imageFormatToStr(_imageInfo.format()))
				!= kSupportedFormats.end();
}

bool Paper::browse(const ImageInfo& info)
{
	_imageInfo = info;
	return !_imageInfo.empty();
}

bool Paper::draw()
{
	limitToWindow();
	return isStaticImage() ? drawStaticImage() : drawDynamicImage();
}

void Paper::erase()
{
	if (isStaticImage()) {
		_image->setImage(QImage());
	} else {
		QMovie* movie = _movie->movie();
		if (movie) {
			_movie->setMovie(nullptr);
			delete movie;
		}
	}
}

/* TODO: open file which path contains Chinese character. */
bool Paper::drawStaticImage()
{
	_movie->hide();
	_image->show();

	cv::Mat src = _cache.get(_imageInfo);
	if (src.empty()) {
		src = cv::imread(_imageInfo.absPath().toUtf8().data());
		if (src.empty())
			return false;
		_cache.put(_imageInfo, src);
	}

	cv::Mat tmp = src;
	double factor = _initScaleFactor *  _scaleFactor;
	if (factor != 1.0) {
		cv::resize(src, tmp, cv::Size(0, 0), factor, factor,
				factor < 1 ? cv::INTER_AREA : cv::INTER_CUBIC);
	}
	QImage dest = mat2Qimage(tmp);
	_container->resize(dest.size());
	_image->resize(dest.size());
	_image->setImage(dest);

	return true;
}

QImage Paper::mat2Qimage(const cv::Mat& src)
{
	QImage dest(static_cast<const uchar*>(src.data), src.cols, src.rows,
			src.step, QImage::Format_BGR888);
	dest.bits(); /* Enforce a deep copy, avoid reading an invalid address. */
	return dest;
}

bool Paper::drawDynamicImage()
{
	_image->hide();
	_movie->show();

	if (_movie->movie()) {
		delete _movie->movie();
		_movie->setMovie(nullptr);
	}

	QMovie* movie = new QMovie(_imageInfo.absPath());
	if (!movie->isValid())
		return false;

	_movie->setMovie(movie);

	double factor = _initScaleFactor * _scaleFactor;
	_container->resize(_imageInfo.dimensions() * factor);
	_movie->resize(_imageInfo.dimensions() * factor);

	_movie->movie()->setScaledSize(_imageInfo.dimensions() * factor);

	_movie->movie()->start();

	return true;
}

void Paper::zoomIn(const double& step)
{
	scale(_scaleFactor * (1 + step));
}

void Paper::zoomOut(const double& step)
{
	scale(_scaleFactor * (1 - step));
}

void Paper::scale(const double& factor)
{
	double prev_factor = _scaleFactor;
	_scaleFactor = factor > kMaxScaleFactor ? kMaxScaleFactor
		: (factor < kMinScaleFactor ? kMinScaleFactor : factor);

	isStaticImage() ? drawStaticImage() : drawDynamicImage();

	adjustScrollBarPos(_scrollArea->horizontalScrollBar(),
			_scaleFactor / prev_factor);
	adjustScrollBarPos(_scrollArea->verticalScrollBar(),
			_scaleFactor / prev_factor);
}

void Paper::limitToWindow()
{
	QSize window_size = size();
	QSize image_size = _imageInfo.dimensions();
	_initScaleFactor = _scaleFactor = 1.0;

	if (image_size.width() < window_size.width()
			&& image_size.height() < window_size.height())
		return;

	double w_ratio = 1.0 * window_size.width() / image_size.width();
	double h_ratio = 1.0 * window_size.height() / image_size.height();

	/* The image may be a little larger than the window if the constant is
	 * 1.0. */
	_initScaleFactor = 0.99 * (w_ratio < h_ratio ? w_ratio : h_ratio);

	gDebug() << "New W:" << _imageInfo.width() * _initScaleFactor
		<< "H:" << _imageInfo.height() * _initScaleFactor;
}

void Paper::adjustScrollBarPos(QScrollBar* scroll_bar, const double& factor)
{
	scroll_bar->setValue(static_cast<int>(scroll_bar->value() * factor
				+ (factor - 1) * scroll_bar->pageStep() / 2));
}

void Paper::move(const double& dx, const double& dy)
{
	QScrollBar* h = _scrollArea->horizontalScrollBar();
	QScrollBar* v = _scrollArea->verticalScrollBar();

	h->setValue(h->value() + dx);
	v->setValue(v->value() + dy);
}

const QList<QByteArray>& Paper::supportedMimeTypes()
{
	return kSupportedMineTypes;
}

bool Paper::isStaticImage() const
{
	return !isDynamicImage();
}

bool Paper::isDynamicImage() const
{
	return _imageInfo.format() == ImageFormat::gif;
}


/* Events. */
/* TODO: zoom the image around the cursor. */
void Paper::wheelEvent(QWheelEvent* event)
{
	/* Most mouse type work in steps of 15 degrees, so the delta value is a
	 * multiple of 120.
	 */
	int step = event->angleDelta().y() / 120;
	if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
		step > 0 ? zoomIn(0.1) : zoomOut(0.1);
	} else {
		emit (step > 0 ? prevOne() : nextOne());
	}

	event->accept();
}

void Paper::mousePressEvent(QMouseEvent* event)
{
	_mousePos = event->pos();

	switch (event->button()) {
	case Qt::MiddleButton:
		scale(1.0);
		break;
	default:
		break;
	}

	event->accept();
}

void Paper::mouseMoveEvent(QMouseEvent* event)
{
	QPoint pos = event->pos();
	double dx = pos.x() - _mousePos.x();
	double dy = pos.y() - _mousePos.y();
	move(-dx, -dy);

	_mousePos = pos;
}

void Paper::mouseReleaseEvent(QMouseEvent* event)
{
	event->accept();
}

bool Paper::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == _scrollArea->viewport()) {
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

}  /* img_view */
