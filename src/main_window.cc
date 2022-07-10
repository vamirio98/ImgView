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
#include "options.h"

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
	checkFileCloseEnabled();
	checkRecentBooksEnabled();
	checkFileSaveAsEnabled();
	checkFilePrintEnabled();
	checkJumpPrevPageEnabled();
	checkJumpNextPageEnabled();
	checkJumpFirstPageEnabled();
	checkJumpLastPageEnabled();
	checkJumpPrevBookEnabled();
	checkJumpNextBookEnabled();
	checkJumpPrevLocationEnabled();
	checkJumpNextLocationEnabled();
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

void MainWindow::checkFileCloseEnabled()
{
	m_ui->m_fileClose->setEnabled(gOpt.show());
}

void MainWindow::checkRecentBooksEnabled()
{
	m_ui->m_fileRecentBooks->setEnabled(gOpt.hasHistory());
}

void MainWindow::checkFileSaveAsEnabled()
{
	m_ui->m_fileSaveAs->setEnabled(gOpt.show());
}

void MainWindow::checkFilePrintEnabled()
{
	m_ui->m_filePrint->setEnabled(gOpt.show());
}

void MainWindow::checkJumpPrevPageEnabled()
{
	m_ui->m_jumpPrevPage->setEnabled(gOpt.show() && !gOpt.isFirstPage());
}

void MainWindow::checkJumpNextPageEnabled()
{
	m_ui->m_jumpNextPage->setEnabled(gOpt.show() && !gOpt.isLastPage());
}

void MainWindow::checkJumpFirstPageEnabled()
{
	m_ui->m_jumpFirstPage->setEnabled(gOpt.show() && !gOpt.isFirstPage());
}

void MainWindow::checkJumpLastPageEnabled()
{
	m_ui->m_jumpLastPage->setEnabled(gOpt.show() && !gOpt.isLastPage());
}

void MainWindow::checkJumpPrevBookEnabled()
{
	m_ui->m_jumpPrevBook->setEnabled(gOpt.show() && !gOpt.isFirstBook());
}

void MainWindow::checkJumpNextBookEnabled()
{
	m_ui->m_jumpNextBook->setEnabled(gOpt.show() && !gOpt.isLastBook());
}

void MainWindow::checkJumpPrevLocationEnabled()
{
	m_ui->m_jumpPrevLocation->setEnabled(!gOpt.isFirstLocation());
}

void MainWindow::checkJumpNextLocationEnabled()
{
	m_ui->m_jumpNextLocation->setEnabled(!gOpt.isLastLocation());
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

}  /* img_view */