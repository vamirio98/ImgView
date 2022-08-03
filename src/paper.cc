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

#include "logger.h"
#include "image_info.h"
#include "book.h"

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

const QImage &AntialiasImage::image() const
{
	return m_image;
}

void AntialiasImage::setImage(const QImage &image)
{
	D(("Set image.\n"));
	m_image = image;
	update();
}

void AntialiasImage::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.drawImage(rect(), m_image);

	D(("Finished update\n"));
}


Paper::Paper(QWidget *parent)
{
	m_scrollArea = new QScrollArea(this);
	m_container = new QWidget(m_scrollArea);
	m_image = new AntialiasImage(m_scrollArea);
	m_movie = new QLabel(m_scrollArea);

	setVisible(true);
	setLayout(new QGridLayout());
	layout()->addWidget(m_scrollArea);
	layout()->setContentsMargins(0, 0, 0, 0);

	m_scrollArea->setBackgroundRole(QPalette::Base);
	m_scrollArea->setAlignment(Qt::AlignCenter);
	m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_scrollArea->viewport()->installEventFilter(this);

	m_scrollArea->setWidget(m_container);
	QVBoxLayout *lay = new QVBoxLayout(m_container);
	lay->addWidget(m_image);
	lay->addWidget(m_movie);
}

Paper::~Paper()
{
}

bool Paper::browse(const QString &image)
{
	return m_imageInfo.browse(image)
		&& kSupportedFormats.find(imageFormatToStr(m_imageInfo.format()))
				!= kSupportedFormats.end();
}

bool Paper::draw()
{
	D(("Draw image: %s\n", m_imageInfo.absPath().toUtf8().data()));
	limitToWindow();
	return isStaticImage() ? drawStaticImage() : drawDynamicImage();
}

void Paper::erase()
{
	if (isStaticImage()) {
		m_image->setImage(QImage());
	} else {
		QMovie *movie = m_movie->movie();
		if (movie) {
			m_movie->setMovie(nullptr);
			delete movie;
		}
	}
}

/* TODO: open file which path contains Chinese character. */
bool Paper::drawStaticImage()
{
	m_movie->hide();
	m_image->show();

	cv::Mat image_mat = m_cache.get(m_imageInfo);
	if (image_mat.empty()) {
		image_mat = cv::imread(m_imageInfo.absPath().toUtf8().data());
		if (image_mat.empty())
			return false;
		m_cache.put(m_imageInfo, image_mat);
	}

	cv::Mat tmp_mat = image_mat;
	double factor = m_initScaleFactor * m_scaleFactor;
	if (factor != 1.0) {
		cv::resize(image_mat, tmp_mat, cv::Size(0, 0), factor, factor,
				factor < 1 ? cv::INTER_AREA : cv::INTER_CUBIC);
	}
	QImage image(tmp_mat.data, tmp_mat.cols, tmp_mat.rows, tmp_mat.step,
			QImage::Format_BGR888);
	m_container->resize(image.size());
	m_image->resize(image.size());
	m_image->setImage(image);

	return true;
}

bool Paper::drawDynamicImage()
{
	m_image->hide();
	m_movie->show();

	if (m_movie->movie()) {
		delete m_movie->movie();
		m_movie->setMovie(nullptr);
	}

	QMovie *movie = new QMovie(m_imageInfo.absPath());
	if (!movie->isValid())
		return false;

	m_movie->setMovie(movie);

	double factor = m_initScaleFactor * m_scaleFactor;
	m_container->resize(m_imageInfo.dimensions() * factor);
	m_movie->resize(m_imageInfo.dimensions() * factor);

	m_movie->movie()->setScaledSize(m_imageInfo.dimensions() * factor);

	m_movie->movie()->start();

	return true;
}

void Paper::zoomIn(const double &step)
{
	scale(m_scaleFactor * (1 + step));
}

void Paper::zoomOut(const double &step)
{
	scale(m_scaleFactor * (1 - step));
}

void Paper::scale(const double &factor)
{
	double prev_factor = m_scaleFactor;
	m_scaleFactor = factor > kMaxScaleFactor ? kMaxScaleFactor
		: (factor < kMinScaleFactor ? kMinScaleFactor : factor);

	isStaticImage() ? drawStaticImage() : drawDynamicImage();

	adjustScrollBarPos(m_scrollArea->horizontalScrollBar(),
			m_scaleFactor / prev_factor);
	adjustScrollBarPos(m_scrollArea->verticalScrollBar(),
			m_scaleFactor / prev_factor);
}

void Paper::limitToWindow()
{
	QSize window_size = size();
	QSize image_size = m_imageInfo.dimensions();
	m_initScaleFactor = m_scaleFactor = 1.0;

	if (image_size.width() < window_size.width()
			&& image_size.height() < window_size.height())
		return;

	double w_ratio = 1.0 * window_size.width() / image_size.width();
	double h_ratio = 1.0 * window_size.height() / image_size.height();

	/* The image may be a little larger than the window if the constant is
	 * 1.0. */
	m_initScaleFactor = 0.99 * (w_ratio < h_ratio ? w_ratio : h_ratio);
	logDebug("Window size: %d, %d", window_size.width(), window_size.height());
	logDebug("Image size: %d, %d", image_size.width(), image_size.height());
	logDebug("Initial scale factor: %f", m_initScaleFactor);
}

void Paper::adjustScrollBarPos(QScrollBar *scroll_bar, const double &factor)
{
	scroll_bar->setValue(static_cast<int>(scroll_bar->value() * factor
				+ (factor - 1) * scroll_bar->pageStep() / 2));
}

void Paper::move(const double &dx, const double &dy)
{
	QScrollBar *h = m_scrollArea->horizontalScrollBar();
	QScrollBar *v = m_scrollArea->verticalScrollBar();

	h->setValue(h->value() + dx);
	v->setValue(v->value() + dy);
}

const QList<QByteArray> &Paper::supportedMimeTypes()
{
	return kSupportedMineTypes;
}

bool Paper::isStaticImage() const
{
	return !isDynamicImage();
}

bool Paper::isDynamicImage() const
{
	return m_imageInfo.format() == ImageFormat::gif;
}


/* Events. */
/* TODO: zoom the image around the cursor. */
void Paper::wheelEvent(QWheelEvent *event)
{
	/* Most mouse type work in steps of 15 degrees, so the delta value is a
	 * multiple of 120.
	 */
	int step = event->angleDelta().y() / 120;
	if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
		step > 0 ? zoomIn(0.1) : zoomOut(0.1);
	} else {
		emit (step > 0 ? toPrevPage() : toNextPage());
	}

	event->accept();
}

void Paper::mousePressEvent(QMouseEvent *event)
{
	m_mousePos = event->pos();

	switch (event->button()) {
	case Qt::MiddleButton:
		scale(1.0);
		break;
	default:
		break;
	}

	event->accept();
}

void Paper::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();
	double dx = pos.x() - m_mousePos.x();
	double dy = pos.y() - m_mousePos.y();
	move(-dx, -dy);

	m_mousePos = pos;
}

void Paper::mouseReleaseEvent(QMouseEvent *event)
{
	event->accept();
}

bool Paper::eventFilter(QObject *obj, QEvent *event)
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

}  /* img_view */
