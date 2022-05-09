/**
 * img_area.cc
 *
 * Created by vamirio on 2022 May 01
 * Last Modified: 2022 May 09 15:55:39
 */
#include "img_area.h"

#include <QGridLayout>
#include <QImageReader>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <QColorSpace>
#include <QScrollBar>
#include <QKeyEvent>
#include <QKeyCombination>

#include "logger.h"

namespace img_view {

ImgArea::ImgArea(QWidget *parent)
{
}

ImgArea::~ImgArea()
{
}

void ImgArea::init()
{
	m_scrollArea = new QScrollArea(this);
	m_displayArea = new QWidget(m_scrollArea);
	m_displayLayout = new QGridLayout(m_displayArea);
	m_label = new QLabel(m_displayArea);
	m_image = new QImage();

	setVisible(false);
	setLayout(new QGridLayout());
	layout()->addWidget(m_scrollArea);
	layout()->setContentsMargins(0, 0, 0, 0);

	m_scrollArea->setBackgroundRole(QPalette::Base);
	m_scrollArea->setWidget(m_displayArea);
	m_scrollArea->setAlignment(Qt::AlignCenter);

	m_displayArea->setLayout(m_displayLayout);
	m_displayArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_displayLayout->addWidget(m_label, 0, 0, Qt::AlignCenter);
	m_displayLayout->setContentsMargins(0, 0, 0, 0);

	m_label->setBackgroundRole(QPalette::Dark);
	m_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

bool ImgArea::loadFile(const QString &filename)
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

	return true;
}

/* TODO: limit scale range. */
void ImgArea::zoomIn()
{
	scaleImage(1.1);
}

void ImgArea::zoomOut()
{
	scaleImage(0.9);
}

void ImgArea::scaleImage(const double &factor)
{
	m_scaleFactor *= factor;
	m_label->setPixmap(QPixmap::fromImage(m_image->scaled(
				m_image->size() * m_scaleFactor,
				Qt::KeepAspectRatio,
				Qt::SmoothTransformation)));
	m_label->adjustSize();
	m_displayArea->resize(m_label->size());

	adjustScrollBarPos(m_scrollArea->horizontalScrollBar(), factor);
	adjustScrollBarPos(m_scrollArea->verticalScrollBar(), factor);
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
	m_label->setPixmap(QPixmap::fromImage(*m_image));
	m_scaleFactor = 1.0;

	m_label->adjustSize();
	m_displayArea->resize(m_label->size());

	setVisible(true);
}

void ImgArea::closeImage()
{
	if (m_image) {
		delete m_image;
		m_image = nullptr;
	}
	m_label->setPixmap(QPixmap());

	setVisible(false);
}

void ImgArea::adjustScrollBarPos(QScrollBar *scroll_bar, const double &factor)
{
	scroll_bar->setValue(static_cast<int>(scroll_bar->value() * factor
				+ (factor - 1) * scroll_bar->pageStep() / 2));
}

void ImgArea::goToPrevImage()
{
	logDebug("Call goToPrevImage.");
}

void ImgArea::goToNextImage()
{
	logDebug("Call goToNextImage.");
}

/* Events. */
void ImgArea::wheelEvent(QWheelEvent *event)
{
	/* Most mouse type work in steps of 15 degrees, so the delta value is a
	 * multiple of 120.
	 */
	int step = event->angleDelta().y() / 120;
	if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
		step > 0 ? zoomIn() : zoomOut();
	} else {
		step > 0 ? goToPrevImage() : goToNextImage();
	}

	event->accept();
}

}  /* namespace img_view */
