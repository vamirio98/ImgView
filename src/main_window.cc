/**
 * main_window.cc
 *
 * Created by vamirio on 2022 Apr 25
 */
#include "main_window.h"

#include <QStandardPaths>
#include <QImageWriter>
#include <QMessageBox>
#include <QApplication>
#include <QShortcut>
#include <QProcess>

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

	toggleClose();
	toggleRecentBooks();
	toggleOpenFileLoc();
	toggleSaveAs();
	togglePrint();
	togglePrevPage();
	toggleNextPage();
	toggleFirstPage();
	toggleLastPage();
	togglePrevBook();
	toggleNextBook();
	togglePrevLoc();
	toggleNextLoc();
}

void MainWindow::setupSlots()
{
	connect(_ui->_open, &QAction::triggered,
			this, &MainWindow::onOpen);
	connect(_ui->_close, &QAction::triggered,
			this, &MainWindow::onClose);
	connect(_ui->_openFileLoc, &QAction::triggered,
			this, &MainWindow::onOpenFileLoc);
	connect(_ui->_exit, &QAction::triggered,
			this, &MainWindow::onExit);

	connect(&gOpt, &Options::showChanged,
			this, &MainWindow::toggleClose);
	connect(&gOpt, &Options::showChanged,
			this, &MainWindow::toggleOpenFileLoc);
	connect(&gOpt, &Options::showChanged,
			this, &MainWindow::togglePrint);
	connect(&gOpt, &Options::showChanged,
			this, &MainWindow::toggleSaveAs);

	connect(_paper, &Paper::prevOne, this, &MainWindow::showPrevPage);
	connect(_paper, &Paper::nextOne, this, &MainWindow::showNextPage);

	connect(_ui->_about, &QAction::triggered,
			this, &MainWindow::onAbout);
}

void MainWindow::onOpen()
{
	QFileDialog dialog(this, tr("Open"));
	initFileDialog(&dialog, QFileDialog::AcceptOpen);
	if (dialog.exec() == QDialog::Accepted) {
		QString imagePath = dialog.selectedFiles().constFirst();
		QString bookPath = dialog.directory().absolutePath();
		if (_book.empty() || bookPath != _lastOpenPos) {
			_lastOpenPos = bookPath;
			if (!_book.open(_lastOpenPos)) {
				QMessageBox::warning(this, tr("Error"),
						tr("Can't open directory %1").arg(bookPath));
			}
		}
		if (_book.setCurPage(imagePath) && _paper->browse(_book.curPage())) {
			_paper->draw();
			gOpt.setShow(true);
		} else {
			QMessageBox::warning(this, tr("Error"),
					tr("Can't open file %1").arg(imagePath));
		}
	}
}

void MainWindow::onClose()
{
	_paper->erase();
	_book.close();
	gOpt.setShow(false);
}

void MainWindow::onOpenFileLoc()
{
#if defined(_WIN32)
	QString exploerer("explorer.exe");
	QStringList param{ "/select,",
	                   QDir::toNativeSeparators(_book.curPage().absPath()) };
	QProcess::execute(exploerer, param);
#endif
}

void MainWindow::onExit()
{
	qApp->quit();
}

void MainWindow::onAbout()
{
	QMessageBox::about(this, tr("About ImgView"),
			tr("<p>A simple image viewer programmed in C++, based on Qt "
				"toolkit.</p>"));
}

void MainWindow::showPrevPage()
{
	if (_book.empty())
		return;
	_paper->erase();
	if (_paper->browse(_book.toPrevPage()))
		_paper->draw();
}

void MainWindow::showNextPage()
{
	if (_book.empty())
		return;
	_paper->erase();
	if (_paper->browse(_book.toNextPage()))
		_paper->draw();
}

void MainWindow::toggleClose()
{
	_ui->_close->setEnabled(gOpt.show());
}

void MainWindow::toggleRecentBooks()
{
	_ui->_recentBooks->setEnabled(gOpt.hasHistory());
}

void MainWindow::toggleOpenFileLoc()
{
	_ui->_openFileLoc->setEnabled(gOpt.show());
}

void MainWindow::toggleSaveAs()
{
	_ui->_saveAs->setEnabled(gOpt.show());
}

void MainWindow::togglePrint()
{
	_ui->_print->setEnabled(gOpt.show());
}

void MainWindow::togglePrevPage()
{
	_ui->_prevPage->setEnabled(gOpt.show() && !gOpt.isFirstPage());
}

void MainWindow::toggleNextPage()
{
	_ui->_nextPage->setEnabled(gOpt.show() && !gOpt.isLastPage());
}

void MainWindow::toggleFirstPage()
{
	_ui->_firstPage->setEnabled(gOpt.show() && !gOpt.isFirstPage());
}

void MainWindow::toggleLastPage()
{
	_ui->_lastPage->setEnabled(gOpt.show() && !gOpt.isLastPage());
}

void MainWindow::togglePrevBook()
{
	_ui->_prevBook->setEnabled(gOpt.show() && !gOpt.isFirstBook());
}

void MainWindow::toggleNextBook()
{
	_ui->_nextBook->setEnabled(gOpt.show() && !gOpt.isLastBook());
}

void MainWindow::togglePrevLoc()
{
	_ui->_prevLoc->setEnabled(!gOpt.isFirstLoc());
}

void MainWindow::toggleNextLoc()
{
	_ui->_nextLoc->setEnabled(!gOpt.isLastLoc());
}

void MainWindow::initFileDialog(QFileDialog* dialog,
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
