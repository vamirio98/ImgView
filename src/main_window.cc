/**
 * main_window.cc
 *
 * Created by vamirio on 2022 Apr 25
 */
#include "main_window.h"

#include <QStandardPaths>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QApplication>
#include <QColorSpace>
#include <QShortcut>

#include "ui/main_window_ui.h"
#include "logger.h"

namespace img_view {

QString MainWindow::m_lastOpenPos = "";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	m_ui(new ui::MainWindowUi)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::init()
{
	m_ui->setupUi(this);
	m_display = m_ui->m_board;
	m_display->setBook(&m_book);
	setupSlots();
	setupShortCut();
}

bool MainWindow::loadFile(const QString &filename)
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

void MainWindow::setupSlots()
{
	connect(m_ui->m_fileOpen, &QAction::triggered,
			this, &MainWindow::onFileOpen);
	connect(m_ui->m_fileClose, &QAction::triggered,
			this, &MainWindow::onFileClose);
	connect(m_ui->m_fileExit, &QAction::triggered,
			this, &MainWindow::onFileExit);

	connect(m_ui->m_helpAbout, &QAction::triggered,
			this, &MainWindow::onHelpAbout);
}

void MainWindow::onFileOpen()
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

void MainWindow::onFileClose()
{
	m_display->closeImage();
}

void MainWindow::onFileExit()
{
	qApp->quit();
}

void MainWindow::onHelpAbout()
{
	QMessageBox::about(this, tr("About ImgView"),
			tr("<p>A simple image viewer programmed in C++, based on Qt "
				"toolkit.</p>"));
}

void MainWindow::initImgFileDialog(QFileDialog *dialog,
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
	for (const QByteArray &mime_type_name : DrawingBoard::supportedMimeTypes())
		mime_type_filters.append(mime_type_name);
	mime_type_filters.append("application/octet-stream");  /* All files. */
	mime_type_filters.sort();
	dialog->setMimeTypeFilters(mime_type_filters);
	dialog->setAcceptMode(accept_mode);
	if (accept_mode == QFileDialog::AcceptSave)
		dialog->setDefaultSuffix("jpg");
}

void MainWindow::setupShortCut()
{
	QShortcut *zoom_in = new QShortcut(QKeySequence::ZoomIn, this);
	connect(zoom_in, &QShortcut::activated, this, &MainWindow::onCtrlPlus);
	QShortcut *zoom_out = new QShortcut(QKeySequence::ZoomOut, this);
	connect(zoom_out, &QShortcut::activated, this, &MainWindow::onCtrlMinus);
}

void MainWindow::onCtrlPlus()
{
	m_display->zoomIn(0.01);
}

void MainWindow::onCtrlMinus()
{
	m_display->zoomOut(0.01);
}

}  /* namespace img_view */
