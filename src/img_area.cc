/**
 * img_area.cc
 *
 * Created by vamirio on 2022 May 01
 * Last Modified: 2022 May 10 18:26:16
 */
#include "img_area.h"

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

#include "logger.h"

namespace img_view {

ImgArea::ImgArea(QWidget *parent)
{
}

ImgArea::~ImgArea()
{
	delete m_mousePos;
}

void ImgArea::init()
{
	m_scrollArea = new QScrollArea(this);
	m_displayArea = new QWidget(m_scrollArea);
	m_displayLayout = new QGridLayout(m_displayArea);
	m_label = new QLabel(m_displayArea);
	m_image = new QImage();
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

bool ImgArea::loadImageAndShow(const QString &filename)
{
	QImageReader reader(filename);
	reader.setAutoTransform(true);
	const QImage img = reader.read();

	if (img.isNull()) {
		QMessageBox::information(this, QApplication::applicationDisplayName(),
				tr("Can't load %1: %2")
				.arg(QDir::toNativeSeparators(filename), reader.errorString()));
		return false;
	}

	setImage(img);
	limitToWindow();
	showImage();

	return true;
}

void ImgArea::zoomInAndShow()
{
	scaleImageAndShow(m_scaleFactor + 0.05);
}

void ImgArea::zoomOutAndShow()
{
	scaleImageAndShow(m_scaleFactor - 0.05);
}

void ImgArea::scaleImageAndShow(const double &factor)
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

void ImgArea::setImage(const QImage &image)
{
	if (m_image) {
		delete m_image;
		m_image = nullptr;
	}

	m_image = new QImage(image);
	if (m_image->colorSpace().isValid())
		m_image->convertToColorSpace(QColorSpace::SRgb);
}

void ImgArea::limitToWindow()
{
	QSize window_size = size();
	QSize image_size = m_image->size();
	m_initScaleFactor = m_scaleFactor = 1.0;

	if (image_size.width() < window_size.width()
			&& image_size.height() < window_size.height())
		return;

	double w_ratio = 1.0 * window_size.width() / image_size.width();
	double h_ratio = 1.0 * window_size.height() / image_size.height();

	/* The image may be a little larger than the window if the constant is
	 * 1.0. */
	m_initScaleFactor = 0.99 * (w_ratio < h_ratio ? w_ratio : h_ratio);
}

void ImgArea::showImage()
{
	m_label->setPixmap(QPixmap::fromImage(m_image->scaled(
				m_image->size() * m_initScaleFactor * m_scaleFactor,
				Qt::KeepAspectRatio,
				Qt::SmoothTransformation)));
	m_label->adjustSize();
	m_displayArea->resize(m_label->size());
}

void ImgArea::closeImage()
{
	if (m_image) {
		delete m_image;
		m_image = nullptr;
	}
	m_label->setPixmap(QPixmap());
}

void ImgArea::adjustScrollBarPos(QScrollBar *scroll_bar, const double &factor)
{
	scroll_bar->setValue(static_cast<int>(scroll_bar->value() * factor
				+ (factor - 1) * scroll_bar->pageStep() / 2));
}

void ImgArea::loadPrevImageAndShow()
{
	//logDebug("Call goToPrevImage.");
}

void ImgArea::loadNextImageAndShow()
{
	//logDebug("Call goToNextImage.");
}

void ImgArea::moveImage(const double &dx, const double &dy)
{
	QScrollBar *h = m_scrollArea->horizontalScrollBar();
	QScrollBar *v = m_scrollArea->verticalScrollBar();

	h->setValue(h->value() + dx);
	v->setValue(v->value() + dy);
}

/* Events. */
/* TODO: zoom the image around the cursor. */
void ImgArea::wheelEvent(QWheelEvent *event)
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

void ImgArea::mousePressEvent(QMouseEvent *event)
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

void ImgArea::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();
	double dx = pos.x() - m_mousePos->x();
	double dy = pos.y() - m_mousePos->y();
	moveImage(-dx, -dy);

	*m_mousePos = pos;
}

void ImgArea::mouseReleaseEvent(QMouseEvent *event)
{
	m_mouseHold = false;

	event->accept();
}

bool ImgArea::eventFilter(QObject *obj, QEvent *event)
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
