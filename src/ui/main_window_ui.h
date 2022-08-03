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
	QMenuBar *m_menubar = nullptr;

	/* Menus. */
	QMenu *m_fileMenu = nullptr;
	QMenu *m_viewMenu = nullptr;
	QMenu *m_imageMenu = nullptr;
	QMenu *m_jumpMenu = nullptr;
	QMenu *m_pageMenu = nullptr;
	QMenu *m_optionMenu = nullptr;
	QMenu *m_helpMenu = nullptr;

	/* File menu. */
	QAction *m_fileOpen = nullptr;
	QAction *m_fileClose = nullptr;
	QMenu *m_fileRecentBooks = nullptr;
	QAction *m_fileOpenFileLocation = nullptr;
	QAction *m_fileSaveAs = nullptr;
	QAction *m_filePrint = nullptr;
	QAction *m_fileExit = nullptr;

	/* View menu. */
	QAction *m_viewLibrary = nullptr;
	QAction *m_viewPageList = nullptr;
	QAction *m_viewHistory = nullptr;
	QAction *m_viewInformation = nullptr;
	QAction *m_viewAction = nullptr;
	QAction *m_viewSideBar = nullptr;

	/* Image menu. */
	QActionGroup *m_imageSizeGroup = nullptr;
	QAction *m_imageOriginSize = nullptr;
	QAction *m_imageFitToWindow = nullptr;
	QAction *m_imageFitHeightToWindow = nullptr;
	QAction *m_imageFitWidthToWindow = nullptr;
	QActionGroup *m_imageBackgroundColorGroup = nullptr;
	QAction *m_imageDarkBackground = nullptr;
	QAction *m_imageLightBackground = nullptr;
	QAction *m_imageCustomBackground = nullptr;

	/* Jump menu. */
	QAction *m_jumpPrevPage = nullptr;
	QAction *m_jumpNextPage = nullptr;
	QAction *m_jumpFirstPage = nullptr;
	QAction *m_jumpLastPage = nullptr;
	QAction *m_jumpPrevBook = nullptr;
	QAction *m_jumpNextBook = nullptr;
	QAction *m_jumpPrevLocation = nullptr;
	QAction *m_jumpNextLocation = nullptr;

	/* Page menu. */
	QActionGroup *m_pagePageNumGroup = nullptr;
	QAction *m_pageOnePage = nullptr;
	QAction *m_pageTwoPage = nullptr;
	QActionGroup *m_pageReadDirectionGroup = nullptr;
	QAction *m_pageRightToLeft = nullptr;
	QAction *m_pageLeftToRight = nullptr;
	QActionGroup *m_pageSortOptionGroup = nullptr;
	QAction *m_pageNameAscending = nullptr;
	QAction *m_pageNameDescending = nullptr;
	QAction *m_pageDateAscending = nullptr;
	QAction *m_pageDateDescending = nullptr;
	QAction *m_pageSizeAscending = nullptr;
	QAction *m_pageSizeDescending = nullptr;
	QAction *m_pageShuffle = nullptr;

	/* Option menu. */
	QAction *m_optionSettings = nullptr;
	QAction *m_optionConfigureKeyboardShortcut = nullptr;

	/* Help menu. */
	QAction *m_helpMenuHelp = nullptr;
	QAction *m_helpShortcutHelp = nullptr;
	QAction *m_helpAbout = nullptr;

	/* Tool bar. */
	QToolBar *m_toolbar = nullptr;

	/* Central widget. */
	Paper *m_paper = nullptr;

	/* Left navigation area. */
	QDockWidget *m_navigation = nullptr;
	QAction *m_naviBook = nullptr;
	QAction *m_naviBooks = nullptr;
	QAction *m_naviHistory = nullptr;

	/* Right information area. */
	QDockWidget *m_info = nullptr;
	QAction *m_infoDetail = nullptr;

public:
	void setupUi(QMainWindow *ImgView);
	void retranslateUi(QMainWindow *ImgView);

private:
	void createActions(QMainWindow *ImgView);
	void setupMenubarUi(QMainWindow *ImgView);
	void setupFileMenuUi(QMainWindow *ImgView);
	void setupViewMenuUi(QMainWindow *ImgView);
	void setupImageMenuUi(QMainWindow *ImgView);
	void setupJumpMenuUi(QMainWindow *ImgView);
	void setupPageMenuUi(QMainWindow *ImgView);
	void setupOptionMenuUi(QMainWindow *ImgView);
	void setupHelpMenuUi(QMainWindow *ImgView);
	void retranslateFileMenuUi();
	void retranslateViewMenuUi();
	void retranslateImageMenuUi();
	void retranslateJumpMenuUi();
	void retranslatePageMenuUi();
	void retranslateOptionMenuUi();
	void retranslateHelpMenuUi();
	void setupCentralWidgetUi(QMainWindow *ImgView);
	void setupToolbarUi(QMainWindow *ImgView);
	void setupNavigationUi(QMainWindow *ImgView);
	void setupInfoUi(QMainWindow *ImgView);
};

}  /* img_view::ui */

#endif /* ifndef MAIN_WINDOW_UI_H */
