/**
 * img_view.cc
 *
 * Created by vamirio on 2022 Apr 25
 * Last Modified: 2022 May 10 17:38:25
 */
#include "img_view.h"

#include <QStandardPaths>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QApplication>
#include <QColorSpace>
#include <QShortcut>

#include "ui/img_view_ui.h"
#include "logger.h"

namespace img_view {

QString ImgView::m_lastOpenPos = "";

ImgView::ImgView(QWidget *parent) : QMainWindow(parent), m_ui(new ui::ImgViewUi)
{
}

ImgView::~ImgView()
{
}

void ImgView::init()
{
	m_ui->setupUi(this);
	m_display = m_ui->m_imageArea;
	setupSlots();
	setupShortCut();

	log::Logger::initInstance();
	log::Logger::setLogFilter(log::LogLevel::Debug);
}

bool ImgView::loadFile(const QString &filename)
{
	return m_display->loadImageAndShow(filename);
}

void ImgView::scaleImage(const double &factor)
{
	m_scaleFactor *= factor;
	m_scaleFactor = m_scaleFactor > 3.0 ? 3.0 : (m_scaleFactor < 0.333
			? 0.333 : m_scaleFactor);
	//m_ui->m_label->resize(m_scale_factor
	//		* m_ui->m_label->pixmap(Qt::ReturnByValue).size());
}

void ImgView::setupSlots()
{
	connect(m_ui->m_fileOpen, &QAction::triggered, this, &ImgView::onFileOpen);
	connect(m_ui->m_fileClose, &QAction::triggered, this, &ImgView::onFileClose);
	connect(m_ui->m_fileExit, &QAction::triggered, this, &ImgView::onFileExit);

	connect(m_ui->m_helpAbout, &QAction::triggered, this, &ImgView::onHelpAbout);
}

void ImgView::onFileOpen()
{
	QFileDialog dialog(this, tr("Open"));
	initImgFileDialog(&dialog, QFileDialog::AcceptOpen);
	if (dialog.exec() == QDialog::Accepted) {
		m_display->loadImageAndShow(dialog.selectedFiles().constFirst());
		m_lastOpenPos = dialog.directory().absolutePath();
	}
}

void ImgView::onFileClose()
{
	m_display->closeImage();
}

void ImgView::onFileExit()
{
	qApp->quit();
}

void ImgView::onHelpAbout()
{
	QMessageBox::about(this, tr("About ImgView"),
			tr("<p>A simple image viewer programmed in C++, based on Qt "
				"toolkit.</p>"));
}

void ImgView::initImgFileDialog(QFileDialog *dialog,
		const QFileDialog::AcceptMode accept_mode)
{
	static bool first_dialog = true;
	if (first_dialog) {
		first_dialog = false;
		const QStringList pic_locations =
			QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
		dialog->setDirectory(pic_locations.isEmpty() ? QDir::currentPath()
				: pic_locations.last());
	}

	if (!m_lastOpenPos.isEmpty())
		dialog->setDirectory(m_lastOpenPos);
	QStringList mime_type_filters;
	const QByteArrayList supported_mime_types =
		accept_mode == QFileDialog::AcceptOpen
		? QImageReader::supportedMimeTypes()
		: QImageWriter::supportedMimeTypes();
	for (const QByteArray &mime_type_name : supported_mime_types)
		mime_type_filters.append(mime_type_name);
	mime_type_filters.sort();
	dialog->setMimeTypeFilters(mime_type_filters);
	dialog->selectMimeTypeFilter("image/jpeg");
	dialog->setAcceptMode(accept_mode);
	if (accept_mode == QFileDialog::AcceptSave)
		dialog->setDefaultSuffix("jpg");
}

void ImgView::setupShortCut()
{
	QShortcut *zoom_in = new QShortcut(QKeySequence::ZoomIn, this);
	connect(zoom_in, &QShortcut::activated, this, &ImgView::onCtrlPlus);
	QShortcut *zoom_out = new QShortcut(QKeySequence::ZoomOut, this);
	connect(zoom_out, &QShortcut::activated, this, &ImgView::onCtrlMinus);
}

void ImgView::onCtrlPlus()
{
	m_display->zoomInAndShow();
}

void ImgView::onCtrlMinus()
{
	m_display->zoomOutAndShow();
}

}  /* namespace img_view */
