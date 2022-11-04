/**
 * main_window.cc
 *
 * Created by vamirio on 2022 Apr 25
 */
#include "main_window.h"

#include <QApplication>
#include <QImageWriter>
#include <QMessageBox>
#include <QProcess>
#include <QShortcut>
#include <QStandardPaths>
#include <qaction.h>
#include <qapplication.h>
#include <qmessagebox.h>

#include "debug.h"
#include "options.h"
#include "ui/main_window_ui.h"

namespace img_view
{

QString MainWindow::_lastOpenPos = "";

#if defined(_WIN32)
QString MainWindow::_fileBrowserProgram = "explorer.exe";
QString MainWindow::_fileBrowserParam = "/select,";
#else
QString MainWindow::_fileBrowserProgram = "";
QString MainWindow::_fileBrowserParam = "";
#endif

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new ui::MainWindowUi)
{
}

MainWindow::~MainWindow() {}

void MainWindow::init()
{
	_ui->setupUi(this);
	_paper = _ui->_paper;
	for (int i = 0; i != gOpt.recentBooks().capacity(); ++i) {
		QAction* action = new QAction(this);
		action->setVisible(false);
		connect(action, &QAction::triggered, this, &MainWindow::onRecentBooks);
		_recentBooksActions.push_back(action);
		_ui->_recentBooks->addAction(action);
	}

	setupSlots();
	setupShortCut();

	gOpt.load();
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
	connect(qApp, &QApplication::aboutToQuit, this, &MainWindow::onQuit);
	connect(_ui->_open, &QAction::triggered, this, &MainWindow::onOpen);
	connect(_ui->_close, &QAction::triggered, this, &MainWindow::onClose);
	connect(
	    _ui->_openFileLoc, &QAction::triggered, this, &MainWindow::onOpenFileLoc
	);
	connect(_ui->_exit, &QAction::triggered, this, &MainWindow::onExit);

	connect(&gOpt, &Options::showChanged, this, &MainWindow::toggleClose);
	connect(
	    &gOpt, &Options::hasHistoryChanged, this, &MainWindow::toggleRecentBooks
	);
	connect(&gOpt, &Options::showChanged, this, &MainWindow::toggleOpenFileLoc);
	connect(&gOpt, &Options::showChanged, this, &MainWindow::togglePrint);
	connect(&gOpt, &Options::showChanged, this, &MainWindow::toggleSaveAs);

	connect(_paper, &Paper::prevOne, this, &MainWindow::showPrevPage);
	connect(_paper, &Paper::nextOne, this, &MainWindow::showNextPage);

	connect(_ui->_about, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::onQuit() { gOpt.save(); }

void MainWindow::onOpen()
{
	QFileDialog dialog(this, tr("Open"));
	initFileDialog(&dialog, QFileDialog::AcceptOpen);
	if (dialog.exec() == QDialog::Accepted) {
		QString imagePath = dialog.selectedFiles().constFirst();
		QString bookPath = dialog.directory().absolutePath();
		if (!_book.empty() && bookPath != _lastOpenPos)
			closeBook();
		if (_book.empty() || bookPath != _lastOpenPos) {
			if (!openBook(bookPath))
				return;
			_lastOpenPos = bookPath;
		}
		if (_book.setCurPage(imagePath) && _paper->browse(_book.curPage())) {
			_paper->draw();
			gOpt.setShow(true);
			updateRecentBooks(_book.absPath(), _book.curPage().absPath());
		} else {
			QMessageBox::warning(
			    this, tr("ImgView"), tr("Cannot open file %1").arg(imagePath)
			);
		}
	}
}

void MainWindow::onClose()
{
	_paper->erase();
	closeBook();
	gOpt.setShow(false);
}

void MainWindow::onOpenFileLoc()
{
	QStringList param{_fileBrowserParam};
#if defined(_WIN32)
	param += QDir::toNativeSeparators(_book.curPage().absPath());
#endif
	if (!_fileBrowserProgram.isEmpty())
		QProcess::execute(_fileBrowserProgram, param);
}

void MainWindow::onExit() { qApp->quit(); }

void MainWindow::onAbout()
{
	QMessageBox::about(
	    this, tr("About ImgView"),
	    tr("<p>A simple image viewer programmed in C++, based on Qt "
	       "toolkit.</p>")
	);
}

void MainWindow::showPrevPage()
{
	if (_book.empty())
		return;
	_paper->erase();
	if (_paper->browse(_book.toPrevPage())) {
		_paper->draw();
		updateRecentBooks(_book.absPath(), _book.curPage().absPath());
	}
}

void MainWindow::showNextPage()
{
	if (_book.empty())
		return;
	_paper->erase();
	if (_paper->browse(_book.toNextPage())) {
		_paper->draw();
		updateRecentBooks(_book.absPath(), _book.curPage().absPath());
	}
}

void MainWindow::toggleClose() { _ui->_close->setEnabled(gOpt.show()); }

void MainWindow::toggleRecentBooks()
{
	_ui->_recentBooks->setEnabled(gOpt.hasHistory());
	if (gOpt.hasHistory())
		updateRecentBooksUi();
}

void MainWindow::toggleOpenFileLoc()
{
	_ui->_openFileLoc->setEnabled(gOpt.show());
}

void MainWindow::toggleSaveAs() { _ui->_saveAs->setEnabled(gOpt.show()); }

void MainWindow::togglePrint() { _ui->_print->setEnabled(gOpt.show()); }

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

void MainWindow::initFileDialog(
    QFileDialog* dialog, const QFileDialog::AcceptMode acceptMode
)
{
	if (_lastOpenPos.isEmpty()) {
		const QStringList picturesLoc =
		    QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
		_lastOpenPos =
		    picturesLoc.isEmpty() ? QDir::currentPath() : picturesLoc.last();
	}
	dialog->setDirectory(_lastOpenPos);

	QStringList mimeTypeFilters{"application/octet-stream"}; /* All files. */
	dialog->setMimeTypeFilters(mimeTypeFilters);
	dialog->setAcceptMode(acceptMode);
	if (acceptMode == QFileDialog::AcceptSave)
		dialog->setDefaultSuffix("jpg");
}

void MainWindow::setupShortCut()
{
	QShortcut* zoomIn = new QShortcut(QKeySequence::ZoomIn, this);
	connect(zoomIn, &QShortcut::activated, this, &MainWindow::onCtrlPlus);
	QShortcut* zoomOut = new QShortcut(QKeySequence::ZoomOut, this);
	connect(zoomOut, &QShortcut::activated, this, &MainWindow::onCtrlMinus);
}

void MainWindow::onCtrlPlus()
{
	if (!_paper->empty())
		_paper->zoomIn(0.1);
}

void MainWindow::onCtrlMinus()
{
	if (!_paper->empty())
		_paper->zoomOut(0.1);
}

bool MainWindow::openBook(const QString& bookPath)
{
	bool res = _book.open(bookPath);
	if (!res) {
		QMessageBox::warning(
		    this, tr("ImgView"), tr("Cannot open directory %1").arg(bookPath)
		);
	}
	return res;
}

void MainWindow::closeBook() { _book.close(); }

void MainWindow::onRecentBooks()
{
	if (!_book.empty())
		closeBook();
	QAction* action = qobject_cast<QAction*>(sender());
	if (action) {
		QString bookPath = action->text();
		QString imagePath = action->data().toString();

		if (!openBook(bookPath))
			return;
		_lastOpenPos = bookPath;

		if (_book.setCurPage(imagePath) && _paper->browse(_book.curPage())) {
			_paper->draw();
			gOpt.setShow(true);
			updateRecentBooks(_book.absPath(), _book.curPage().absPath());
		} else {
			QMessageBox::warning(
			    this, tr("ImgView"), tr("Cannot open file %1").arg(imagePath)
			);
		}
	}
}

void MainWindow::updateRecentBooks(const QString& book, const QString& image)
{
	QString cacheImage = gOpt.recentBooks().get(book);
	if (cacheImage.isEmpty() || cacheImage != image)
		gOpt.recentBooks().put(book, image);
	updateRecentBooksUi();

	if (!gOpt.hasHistory())
		gOpt.setHasHistory(true);
}

void MainWindow::updateRecentBooksUi()
{
	int num = 0;
	for (auto book = gOpt.recentBooks().data().crbegin();
	     book != gOpt.recentBooks().data().crend(); ++book) {
		_recentBooksActions.at(num)->setText(book->first);
		_recentBooksActions.at(num)->setData(book->second);
		_recentBooksActions.at(num)->setVisible(true);
		++num;
	}
	for (int i = num; i != _recentBooksActions.size(); ++i)
		_recentBooksActions.at(i)->setVisible(false);
}

} // namespace img_view
