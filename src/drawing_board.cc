/**
 * drawing_board.cc
 *
 * Created by vamirio on 2022 May 01
 */
#include "drawing_board.h"

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
#include "book.h"

namespace img_view {

/* TODO: add support for webp. */
const QList<QByteArray> DrawingBoard::kSupportedMineTypes = {
	"image/bmp",
	"image/gif",
	"image/jpeg",
	"image/png",
	"image/webp"
};

const QSet<const QByteArray> DrawingBoard::kSupportedFormats = {
	"bmp",
	"gif",
	"jpeg",
	"png",
	"webp"
};


DrawingBoard::DrawingBoard(QWidget *parent)
{
}

DrawingBoard::~DrawingBoard()
{
	/* Do not delete m_book. */
}

void DrawingBoard::init()
{
	m_scrollArea = new QScrollArea(this);
	m_displayArea = new QWidget(m_scrollArea);
	m_displayLayout = new QGridLayout(m_displayArea);
	m_label = new QLabel(m_displayArea);

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

	m_label->setBackgroundRole(QPalette::Base);
	m_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void DrawingBoard::setBook(Book *const book)
{
	m_book = book;
}

bool DrawingBoard::isStaticImage() const
{
	return !isDynamicImage();
}

bool DrawingBoard::isDynamicImage() const
{
	return m_book->currPageInfo().format() == gif;
}

bool DrawingBoard::loadCurrPage()
{
	if (m_book->noPage())
		return false;
	D(("Start loading page %s...\n",
				m_book->currPageInfo().filename().toUtf8().constData()));
	const QByteArray format = imgFormatToStr(m_book->currPageInfo().format());
	if (kSupportedFormats.find(format) == kSupportedFormats.end()) {
		QMessageBox::information(this, QApplication::applicationDisplayName(),
				tr("Can't open image %1: unsupported format.")
				.arg(m_book->currPageInfo().filename()));
		return false;
	}

	bool ret = false;
	switch (m_book->currPageInfo().format()) {
	case bmp:
	case jpeg:
	case png:
	case webp:
		ret = loadStaticImage(m_book->currPageInfo().absPath());
		break;
	case gif:
		ret = loadDynamicImage(m_book->currPageInfo().absPath());
		break;
	default:
		ret = false;
		break;
	}
	D(("Loading %s.\n", (ret ? "succeeded" : "failed")));

	return ret;
}

/* TODO: open file which path contains Chinese character. */
bool DrawingBoard::loadStaticImage(const QString &filename)
{
	cv::Mat image = cv::imread(filename.toUtf8().constData());

	if (image.empty()) {
		QMessageBox::information(this, QApplication::applicationDisplayName(),
				tr("Could not load %1.")
				.arg(QDir::toNativeSeparators(filename)));
		return false;
	}

	m_image = image;
	limitToWindow();
	showImage();

	return true;
}

bool DrawingBoard::loadDynamicImage(const QString &filename)
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

void DrawingBoard::zoomIn(const double &step)
{
	scaleImage(m_scaleFactor * (1 + step));
}

void DrawingBoard::zoomOut(const double &step)
{
	scaleImage(m_scaleFactor * (1 - step));
}

void DrawingBoard::scaleImage(const double &factor)
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

void DrawingBoard::limitToWindow()
{
	QSize window_size = size();
	QSize image_size = m_book->currPageInfo().dimensions();
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

void DrawingBoard::showImage()
{
	double factor = m_initScaleFactor * m_scaleFactor;
	if (isStaticImage()) {
		cv::Mat tmp;
		cv::resize(m_image, tmp, cv::Size(0, 0), factor, factor,
				factor <= 1 ? cv::INTER_AREA : cv::INTER_CUBIC);
		QImage image(tmp.data, tmp.cols, tmp.rows, tmp.step,
				QImage::Format_BGR888);
		m_label->setPixmap(QPixmap::fromImage(image));
	} else {
		m_label->movie()->setScaledSize(
					m_book->currPageInfo().dimensions() * factor);
		m_label->movie()->start();
	}
	m_label->resize(m_book->currPageInfo().dimensions() * factor);
	m_displayArea->resize(m_label->size());
}

void DrawingBoard::closeImage()
{
	m_image = cv::Mat();
	QMovie *movie = m_label->movie();
	if (movie)
		delete movie;
	m_label->setMovie(nullptr);
	m_label->setPixmap(QPixmap());
}

void DrawingBoard::adjustScrollBarPos(QScrollBar *scroll_bar, const double &factor)
{
	scroll_bar->setValue(static_cast<int>(scroll_bar->value() * factor
				+ (factor - 1) * scroll_bar->pageStep() / 2));
}

void DrawingBoard::loadPrevImage()
{
	if (m_book->noPage())
		return;
	m_book->toPrevPage();
	loadCurrPage();
}

void DrawingBoard::loadNextImage()
{
	if (m_book->noPage())
		return;
	m_book->toNextPage();
	loadCurrPage();
}

void DrawingBoard::moveImage(const double &dx, const double &dy)
{
	QScrollBar *h = m_scrollArea->horizontalScrollBar();
	QScrollBar *v = m_scrollArea->verticalScrollBar();

	h->setValue(h->value() + dx);
	v->setValue(v->value() + dy);
}

const QList<QByteArray> &DrawingBoard::supportedMimeTypes()
{
	return kSupportedMineTypes;
}

/* Events. */
/* TODO: zoom the image around the cursor. */
void DrawingBoard::wheelEvent(QWheelEvent *event)
{
	/* Most mouse type work in steps of 15 degrees, so the delta value is a
	 * multiple of 120.
	 */
	int step = event->angleDelta().y() / 120;
	if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
		step > 0 ? zoomIn(0.1) : zoomOut(0.1);
	} else {
		step > 0 ? loadPrevImage() : loadNextImage();
	}

	event->accept();
}

void DrawingBoard::mousePressEvent(QMouseEvent *event)
{
	m_mouseHold = true;
	m_mousePos = event->pos();

	switch (event->button()) {
	case Qt::MiddleButton:
		scaleImage(1.0);
		break;
	default:
		break;
	}

	event->accept();
}

void DrawingBoard::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();
	double dx = pos.x() - m_mousePos.x();
	double dy = pos.y() - m_mousePos.y();
	if (m_mouseHold)
		moveImage(-dx, -dy);

	m_mousePos = pos;
}

void DrawingBoard::mouseReleaseEvent(QMouseEvent *event)
{
	m_mouseHold = false;

	event->accept();
}

bool DrawingBoard::eventFilter(QObject *obj, QEvent *event)
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
