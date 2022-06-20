/**
 * img_view.cc
 *
 * Created by vamirio on 2022 Apr 25
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
	m_display = m_ui->m_display;
	m_display->setBook(&m_book);
	setupSlots();
	setupShortCut();
}

bool ImgView::loadFile(const QString &filename)
{
	D(("Loading file %s.\n", filename.toUtf8().constData()));
	QFileInfo info(filename);
	if (!info.exists())
		return false;
	if (info.isDir()) {
		m_book.setBook(info.canonicalFilePath());
	} else {
		m_book.setBook(info.canonicalPath());
		m_book.setCurrPage(info.canonicalFilePath());
	}
	return !m_book.noPage() && m_display->loadCurrPage();
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
		m_lastOpenPos = dialog.directory().absolutePath();
		m_book.setBook(m_lastOpenPos);
		D(("Book: %ld\n", (long)&m_book));
		m_book.setCurrPage(dialog.selectedFiles().constFirst());
		D(("Starting loadCurrPage().\n"));
		m_display->loadCurrPage();
		D(("Finished loadCurrPage().\n"));
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
	for (const QByteArray &mime_type_name : Display::supportedMimeTypes())
		mime_type_filters.append(mime_type_name);
	mime_type_filters.append("application/octet-stream");  /* All files. */
	mime_type_filters.sort();
	dialog->setMimeTypeFilters(mime_type_filters);
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
	m_display->zoomIn(0.01);
}

void ImgView::onCtrlMinus()
{
	m_display->zoomOut(0.01);
}

}  /* namespace img_view */
