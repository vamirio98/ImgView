/**
 * main_window_ui.h
 *
 * Created by vamirio on 2022 Apr 25
 */
#ifndef MAIN_WINDOW_UI_H
#define MAIN_WINDOW_UI_H

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QDockWidget>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>

#include "paper.h"

namespace img_view::ui {

class MainWindowUi {
public:
	/* Menu bar. */
	QMenuBar* _menubar = nullptr;

	/* Menus. */
	QMenu* _fileMenu = nullptr;
	QMenu* _viewMenu = nullptr;
	QMenu* _imageMenu = nullptr;
	QMenu* _jumpMenu = nullptr;
	QMenu* _pageMenu = nullptr;
	QMenu* _optionMenu = nullptr;
	QMenu* _helpMenu = nullptr;

	/* File menu. */
	QAction* _fileOpen = nullptr;
	QAction* _fileClose = nullptr;
	QMenu* _fileRecentBooks = nullptr;
	QAction* _fileOpenFileLocation = nullptr;
	QAction* _fileSaveAs = nullptr;
	QAction* _filePrint = nullptr;
	QAction* _fileExit = nullptr;

	/* View menu. */
	QAction* _viewLibrary = nullptr;
	QAction* _viewPageList = nullptr;
	QAction* _viewHistory = nullptr;
	QAction* _viewInformation = nullptr;
	QAction* _viewAction = nullptr;
	QAction* _viewSideBar = nullptr;

	/* Image menu. */
	QActionGroup* _imageSizeGroup = nullptr;
	QAction* _imageOriginSize = nullptr;
	QAction* _imageFitToWindow = nullptr;
	QAction* _imageFitHeightToWindow = nullptr;
	QAction* _imageFitWidthToWindow = nullptr;
	QActionGroup* _imageBackgroundColorGroup = nullptr;
	QAction* _imageDarkBackground = nullptr;
	QAction* _imageLightBackground = nullptr;
	QAction* _imageCustomBackground = nullptr;

	/* Jump menu. */
	QAction* _jumpPrevPage = nullptr;
	QAction* _jumpNextPage = nullptr;
	QAction* _jumpFirstPage = nullptr;
	QAction* _jumpLastPage = nullptr;
	QAction* _jumpPrevBook = nullptr;
	QAction* _jumpNextBook = nullptr;
	QAction* _jumpPrevLocation = nullptr;
	QAction* _jumpNextLocation = nullptr;

	/* Page menu. */
	QActionGroup* _pagePageNumGroup = nullptr;
	QAction* _pageOnePage = nullptr;
	QAction* _pageTwoPage = nullptr;
	QActionGroup* _pageReadDirectionGroup = nullptr;
	QAction* _pageRightToLeft = nullptr;
	QAction* _pageLeftToRight = nullptr;
	QActionGroup* _pageSortOptionGroup = nullptr;
	QAction* _pageNameAscending = nullptr;
	QAction* _pageNameDescending = nullptr;
	QAction* _pageDateAscending = nullptr;
	QAction* _pageDateDescending = nullptr;
	QAction* _pageSizeAscending = nullptr;
	QAction* _pageSizeDescending = nullptr;
	QAction* _pageShuffle = nullptr;

	/* Option menu. */
	QAction* _optionSettings = nullptr;
	QAction* _optionConfigureKeyboardShortcut = nullptr;

	/* Help menu. */
	QAction* _helpMenuHelp = nullptr;
	QAction* _helpShortcutHelp = nullptr;
	QAction* _helpAbout = nullptr;

	/* Tool bar. */
	QToolBar* _toolbar = nullptr;

	/* Central widget. */
	Paper* _paper = nullptr;

	/* Left navigation area. */
	QDockWidget* _navigation = nullptr;
	QAction* _naviBook = nullptr;
	QAction* _naviBooks = nullptr;
	QAction* _naviHistory = nullptr;

	/* Right information area. */
	QDockWidget* _info = nullptr;
	QAction* _infoDetail = nullptr;

public:
	void setupUi(QMainWindow* ImgView);
	void retranslateUi(QMainWindow* ImgView);

private:
	void createActions(QMainWindow* ImgView);
	void setupMenubarUi(QMainWindow* ImgView);
	void setupFileMenuUi(QMainWindow* ImgView);
	void setupViewMenuUi(QMainWindow* ImgView);
	void setupImageMenuUi(QMainWindow* ImgView);
	void setupJumpMenuUi(QMainWindow* ImgView);
	void setupPageMenuUi(QMainWindow* ImgView);
	void setupOptionMenuUi(QMainWindow* ImgView);
	void setupHelpMenuUi(QMainWindow* ImgView);
	void retranslateFileMenuUi();
	void retranslateViewMenuUi();
	void retranslateImageMenuUi();
	void retranslateJumpMenuUi();
	void retranslatePageMenuUi();
	void retranslateOptionMenuUi();
	void retranslateHelpMenuUi();
	void setupCentralWidgetUi(QMainWindow* ImgView);
	void setupToolbarUi(QMainWindow* ImgView);
	void setupNavigationUi(QMainWindow* ImgView);
	void setupInfoUi(QMainWindow* ImgView);
};

}  /* img_view::ui */

#endif /* ifndef MAIN_WINDOW_UI_H */
