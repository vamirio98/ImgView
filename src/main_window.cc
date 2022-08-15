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
#include "debug.h"
#include "options.h"

namespace img_view {

QString MainWindow::_lastOpenPos = "";

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent),
	_ui(new ui::MainWindowUi)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::init()
{
	_ui->setupUi(this);
	_paper = _ui->_paper;
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

bool MainWindow::loadFile(const QString& filename)
{
	gDebug() << "Loading file" << filename;

	QFileInfo info(filename);
	if (!info.exists())
		return false;
	if (info.isDir()) {
		_book.open(info.canonicalFilePath());
	} else {
		_book.open(info.canonicalPath());
		_book.setCurPage(info.canonicalFilePath());
	}
	return !_book.empty() && _paper->browse(filename);
}

void MainWindow::setupSlots()
{
	connect(_ui->_fileOpen, &QAction::triggered,
			this, &MainWindow::onFileOpen);
	connect(_ui->_fileClose, &QAction::triggered,
			this, &MainWindow::onFileClose);
	connect(_ui->_fileExit, &QAction::triggered,
			this, &MainWindow::onFileExit);

	connect(_paper, &Paper::toPrevPage, this, &MainWindow::onToPrevPage);
	connect(_paper, &Paper::toNextPage, this, &MainWindow::onToNextPage);

	connect(_ui->_helpAbout, &QAction::triggered,
			this, &MainWindow::onHelpAbout);
}

void MainWindow::onFileOpen()
{
	QFileDialog dialog(this, tr("Open"));
	initImgFileDialog(&dialog, QFileDialog::AcceptOpen);
	if (dialog.exec() == QDialog::Accepted) {
		QString image = dialog.selectedFiles().constFirst();
		_lastOpenPos = dialog.directory().absolutePath();
		_book.open(_lastOpenPos);
		_book.setCurPage(image);
		if (_paper->browse(image))
			_paper->draw();
	}
}

void MainWindow::onFileClose()
{
	_paper->erase();
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

void MainWindow::onToPrevPage()
{
	_paper->erase();
	if (_paper->browse(_book.toPrevPage().absPath()))
		_paper->draw();
}

void MainWindow::onToNextPage()
{
	_paper->erase();
	if (_paper->browse(_book.toNextPage().absPath()))
		_paper->draw();
}

void MainWindow::checkFileCloseEnabled()
{
	_ui->_fileClose->setEnabled(gOpt.show());
}

void MainWindow::checkRecentBooksEnabled()
{
	_ui->_fileRecentBooks->setEnabled(gOpt.hasHistory());
}

void MainWindow::checkFileSaveAsEnabled()
{
	_ui->_fileSaveAs->setEnabled(gOpt.show());
}

void MainWindow::checkFilePrintEnabled()
{
	_ui->_filePrint->setEnabled(gOpt.show());
}

void MainWindow::checkJumpPrevPageEnabled()
{
	_ui->_jumpPrevPage->setEnabled(gOpt.show() && !gOpt.isFirstPage());
}

void MainWindow::checkJumpNextPageEnabled()
{
	_ui->_jumpNextPage->setEnabled(gOpt.show() && !gOpt.isLastPage());
}

void MainWindow::checkJumpFirstPageEnabled()
{
	_ui->_jumpFirstPage->setEnabled(gOpt.show() && !gOpt.isFirstPage());
}

void MainWindow::checkJumpLastPageEnabled()
{
	_ui->_jumpLastPage->setEnabled(gOpt.show() && !gOpt.isLastPage());
}

void MainWindow::checkJumpPrevBookEnabled()
{
	_ui->_jumpPrevBook->setEnabled(gOpt.show() && !gOpt.isFirstBook());
}

void MainWindow::checkJumpNextBookEnabled()
{
	_ui->_jumpNextBook->setEnabled(gOpt.show() && !gOpt.isLastBook());
}

void MainWindow::checkJumpPrevLocationEnabled()
{
	_ui->_jumpPrevLocation->setEnabled(!gOpt.isFirstLocation());
}

void MainWindow::checkJumpNextLocationEnabled()
{
	_ui->_jumpNextLocation->setEnabled(!gOpt.isLastLocation());
}

void MainWindow::initImgFileDialog(QFileDialog* dialog,
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

	if (!_lastOpenPos.isEmpty())
		dialog->setDirectory(_lastOpenPos);
	QStringList mime_type_filters;
	for (const QByteArray& mime_type_name : Paper::supportedMimeTypes())
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
	QShortcut* zoom_in = new QShortcut(QKeySequence::ZoomIn, this);
	connect(zoom_in, &QShortcut::activated, this, &MainWindow::onCtrlPlus);
	QShortcut* zoom_out = new QShortcut(QKeySequence::ZoomOut, this);
	connect(zoom_out, &QShortcut::activated, this, &MainWindow::onCtrlMinus);
}

void MainWindow::onCtrlPlus()
{
	_paper->zoomIn(0.01);
}

void MainWindow::onCtrlMinus()
{
	_paper->zoomOut(0.01);
}

}  /* img_view */
