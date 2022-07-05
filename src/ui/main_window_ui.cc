/**
 * main_window_ui.cc
 *
 * Created by vamirio on 2022 Apr 25
 */
#include "ui/main_window_ui.h"

#include <QActionGroup>

namespace img_view::ui {

static int sInitWidth = 800;
static int sInitHeight = 600;

void MainWindowUi::setupUi(QMainWindow *ImgView)
{
	if (ImgView->objectName().isEmpty())
		ImgView->setObjectName("ImgView");
	ImgView->resize(sInitWidth, sInitHeight);

	createActions(ImgView);
	setupMenubarUi(ImgView);
	setupCentralWidgetUi(ImgView);
	setupToolbarUi(ImgView);
	setupNavigationUi(ImgView);
	setupInfoUi(ImgView);

	retranslateUi(ImgView);
}

void MainWindowUi::retranslateUi(QMainWindow *ImgView)
{
	ImgView->setWindowTitle(QString::fromUtf8("ImgView"));

	retranslateFileMenuUi();
	retranslateViewMenuUi();
	retranslateImageMenuUi();
	retranslateJumpMenuUi();
	retranslatePageMenuUi();
	retranslateOptionMenuUi();
	retranslateHelpMenuUi();
}

void MainWindowUi::createActions(QMainWindow *ImgView)
{
	QString (*str)(QByteArrayView) = QString::fromUtf8;

	/* Menu. */
	m_fileMenu = new QMenu(ImgView);
	m_fileMenu->setObjectName(str("file_menu"));
	m_viewMenu = new QMenu(ImgView);
	m_viewMenu->setObjectName(str("view_menu"));
	m_imageMenu = new QMenu(ImgView);
	m_imageMenu->setObjectName(str("image_menu"));
	m_jumpMenu = new QMenu(ImgView);
	m_jumpMenu->setObjectName(str("jump_menu"));
	m_pageMenu = new QMenu(ImgView);
	m_pageMenu->setObjectName(str("page_menu"));
	m_optionMenu = new QMenu(ImgView);
	m_optionMenu->setObjectName(str("option_menu"));
	m_helpMenu = new QMenu(ImgView);
	m_helpMenu->setObjectName(str("help_menu"));

	/* File menu. */
	m_fileOpen = new QAction(ImgView);
	m_fileOpen->setObjectName(str("file_open"));
	m_fileClose = new QAction(ImgView);
	m_fileClose->setObjectName(str("file_close"));
	m_fileRecentBooks = new QMenu(ImgView);
	m_fileRecentBooks->setObjectName(str("file_recent_books"));
	m_fileOpenFileLocation = new QAction(ImgView);
	m_fileOpenFileLocation->setObjectName(str("file_open_file_location"));
	m_fileSaveAs = new QAction(ImgView);
	m_fileSaveAs->setObjectName(str("file_save_as"));
	m_filePrint = new QAction(ImgView);
	m_filePrint->setObjectName(str("file_print"));
	m_fileExit = new QAction(ImgView);
	m_fileExit->setObjectName(str("file_exit"));

	/* View menu. */
	m_viewLibrary = new QAction(ImgView);
	m_viewLibrary->setObjectName(str("view_library"));
	m_viewPageList = new QAction(ImgView);
	m_viewPageList->setObjectName(str("view_page_list"));
	m_viewHistory = new QAction(ImgView);
	m_viewHistory->setObjectName(str("view_history"));
	m_viewInformation = new QAction(ImgView);
	m_viewInformation->setObjectName(str("view_information"));
	m_viewAction = new QAction(ImgView);
	m_viewAction->setObjectName(str("view_action"));
	m_viewSideBar = new QAction(ImgView);
	m_viewSideBar->setObjectName(str("view_side_bar"));

	/* Image menu. */
	m_imageSizeGroup = new QActionGroup(ImgView);
	m_imageOriginSize = new QAction(ImgView);
	m_imageOriginSize->setObjectName(str("image_origin_size"));
	m_imageFitToWindow = new QAction(ImgView);
	m_imageFitToWindow->setObjectName(str("image_fit_to_window"));
	m_imageFitHeightToWindow = new QAction(ImgView);
	m_imageFitHeightToWindow->setObjectName(str("image_fit_height_to_window"));
	m_imageFitWidthToWindow = new QAction(ImgView);
	m_imageFitWidthToWindow->setObjectName(str("image_fit_width_to_window"));
	m_imageBackgroundColorGroup = new QActionGroup(ImgView);
	m_imageDarkBackground = new QAction(ImgView);
	m_imageDarkBackground->setObjectName(str("image_dark_background"));
	m_imageLightBackground = new QAction(ImgView);
	m_imageLightBackground->setObjectName(str("image_light_background"));
	m_imageCustomBackground = new QAction(ImgView);
	m_imageCustomBackground->setObjectName(str("image_custom_background"));

	/* Jump menu. */
	m_jumpPrevPage = new QAction(ImgView);
	m_jumpPrevPage->setObjectName(str("jump_prev_page"));
	m_jumpNextPage = new QAction(ImgView);
	m_jumpNextPage->setObjectName(str("jump_next_page"));
	m_jumpFirstPage = new QAction(ImgView);
	m_jumpFirstPage->setObjectName(str("jump_first_page"));
	m_jumpLastPage = new QAction(ImgView);
	m_jumpLastPage->setObjectName(str("jump_last_page"));
	m_jumpPrevBook = new QAction(ImgView);
	m_jumpPrevBook->setObjectName(str("jump_prev_book"));
	m_jumpNextBook = new QAction(ImgView);
	m_jumpNextBook->setObjectName(str("jump_next_book"));
	m_jumpPrevLocation = new QAction(ImgView);
	m_jumpPrevLocation->setObjectName(str("jump_prev_location"));
	m_jumpNextLocation = new QAction(ImgView);
	m_jumpNextLocation->setObjectName(str("jump_next_location"));

	/* Page menu. */
	m_pagePageNumGroup = new QActionGroup(ImgView);
	m_pageOnePage = new QAction(ImgView);
	m_pageOnePage->setObjectName(str("page_one_page"));
	m_pageTwoPage = new QAction(ImgView);
	m_pageTwoPage->setObjectName(str("page_two_page"));
	m_pageReadDirectionGroup = new QActionGroup(ImgView);
	m_pageRightToLeft = new QAction(ImgView);
	m_pageRightToLeft->setObjectName(str("page_right_to_left"));
	m_pageLeftToRight = new QAction(ImgView);
	m_pageLeftToRight->setObjectName(str("page_left_to_right"));
	m_pageSortOptionGroup = new QActionGroup(ImgView);
	m_pageNameAscending = new QAction(ImgView);
	m_pageNameAscending->setObjectName(str("page_name_ascending"));
	m_pageNameDescending = new QAction(ImgView);
	m_pageNameDescending->setObjectName(str("page_name_descending"));
	m_pageDateAscending = new QAction(ImgView);
	m_pageDateAscending->setObjectName(str("page_date_ascending"));
	m_pageDateDescending = new QAction(ImgView);
	m_pageDateDescending->setObjectName(str("page_date_descending"));
	m_pageSizeAscending = new QAction(ImgView);
	m_pageSizeAscending->setObjectName(str("page_size_ascending"));
	m_pageSizeDescending = new QAction(ImgView);
	m_pageSizeDescending->setObjectName(str("page_size_descending"));
	m_pageShuffle = new QAction(ImgView);
	m_pageShuffle->setObjectName(str("page_shuffle"));

	/* Option menu. */
	m_optionSettings = new QAction(ImgView);
	m_optionSettings->setObjectName(str("option_settings"));
	m_optionConfigureKeyboardShortcut = new QAction(ImgView);
	m_optionConfigureKeyboardShortcut->setObjectName(
			str("option_configure_keyboard_shortcut"));

	/* Help menu. */
	m_helpMenuHelp = new QAction(ImgView);
	m_helpMenuHelp->setObjectName(str("help_menu_help"));
	m_helpShortcutHelp = new QAction(ImgView);
	m_helpShortcutHelp->setObjectName(str("help_shortcut_help"));
	m_helpAbout = new QAction(ImgView);
	m_helpAbout->setObjectName(str("help_about"));
}

void MainWindowUi::setupMenubarUi(QMainWindow *ImgView)
{
	m_menubar = new QMenuBar(ImgView);
	m_menubar->setObjectName(QString::fromUtf8("menubar"));
	m_menubar->setGeometry(0, 0, sInitWidth, 26);
	//m_menubar->setStyleSheet("QMenuBar { background: #D8D8D8 }");
	ImgView->setMenuBar(m_menubar);

	setupFileMenuUi(ImgView);
	setupViewMenuUi(ImgView);
	setupImageMenuUi(ImgView);
	setupJumpMenuUi(ImgView);
	setupPageMenuUi(ImgView);
	setupOptionMenuUi(ImgView);
	setupHelpMenuUi(ImgView);
}

void MainWindowUi::setupFileMenuUi(QMainWindow *ImgView)
{
	m_menubar->addAction(m_fileMenu->menuAction());

	m_fileMenu->addAction(m_fileOpen);
	m_fileMenu->addAction(m_fileClose);
	m_fileMenu->addAction(m_fileRecentBooks->menuAction());
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_fileOpenFileLocation);
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_fileSaveAs);
	m_fileMenu->addAction(m_filePrint);
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_fileExit);
}

void MainWindowUi::setupViewMenuUi(QMainWindow *ImgView)
{
	m_menubar->addAction(m_viewMenu->menuAction());

	m_viewMenu->addAction(m_viewLibrary);
	m_viewMenu->addAction(m_viewPageList);
	m_viewMenu->addAction(m_viewHistory);
	m_viewMenu->addAction(m_viewInformation);
	m_viewMenu->addAction(m_viewAction);
	m_viewMenu->addAction(m_viewSideBar);

	m_viewLibrary->setCheckable(true);
	m_viewPageList->setCheckable(true);
	m_viewHistory->setCheckable(true);
	m_viewInformation->setCheckable(true);
	m_viewAction->setCheckable(true);
	m_viewSideBar->setCheckable(true);
}

void MainWindowUi::setupImageMenuUi(QMainWindow *ImgView)
{
	m_menubar->addAction(m_imageMenu->menuAction());

	m_imageMenu->addAction(m_imageOriginSize);
	m_imageMenu->addAction(m_imageFitToWindow);
	m_imageMenu->addAction(m_imageFitHeightToWindow);
	m_imageMenu->addAction(m_imageFitWidthToWindow);
	m_imageMenu->addSeparator();
	m_imageMenu->addAction(m_imageDarkBackground);
	m_imageMenu->addAction(m_imageLightBackground);
	m_imageMenu->addAction(m_imageCustomBackground);

	m_imageOriginSize->setCheckable(true);
	m_imageFitToWindow->setCheckable(true);
	m_imageFitHeightToWindow->setCheckable(true);
	m_imageFitWidthToWindow->setCheckable(true);
	m_imageDarkBackground->setCheckable(true);
	m_imageLightBackground->setCheckable(true);
	m_imageCustomBackground->setCheckable(true);

	m_imageSizeGroup->addAction(m_imageOriginSize);
	m_imageSizeGroup->addAction(m_imageFitToWindow);
	m_imageSizeGroup->addAction(m_imageFitHeightToWindow);
	m_imageSizeGroup->addAction(m_imageFitWidthToWindow);
	m_imageSizeGroup->setExclusionPolicy(
			QActionGroup::ExclusionPolicy::ExclusiveOptional);

	m_imageBackgroundColorGroup->addAction(m_imageDarkBackground);
	m_imageBackgroundColorGroup->addAction(m_imageLightBackground);
	m_imageBackgroundColorGroup->addAction(m_imageCustomBackground);
	m_imageBackgroundColorGroup->setExclusionPolicy(
			QActionGroup::ExclusionPolicy::ExclusiveOptional);
}

void MainWindowUi::setupJumpMenuUi(QMainWindow *ImgView)
{
	m_menubar->addAction(m_jumpMenu->menuAction());

	m_jumpMenu->addAction(m_jumpPrevPage);
	m_jumpMenu->addAction(m_jumpNextPage);
	m_jumpMenu->addAction(m_jumpFirstPage);
	m_jumpMenu->addAction(m_jumpLastPage);
	m_jumpMenu->addAction(m_jumpPrevBook);
	m_jumpMenu->addAction(m_jumpNextBook);
	m_jumpMenu->addSeparator();
	m_jumpMenu->addAction(m_jumpPrevLocation);
	m_jumpMenu->addAction(m_jumpNextLocation);
}

void MainWindowUi::setupPageMenuUi(QMainWindow *ImgView)
{
	m_menubar->addAction(m_pageMenu->menuAction());

	m_pageMenu->addAction(m_pageOnePage);
	m_pageMenu->addAction(m_pageTwoPage);
	m_pageMenu->addSeparator();
	m_pageMenu->addAction(m_pageRightToLeft);
	m_pageMenu->addAction(m_pageLeftToRight);
	m_pageMenu->addSeparator();
	m_pageMenu->addAction(m_pageNameAscending);
	m_pageMenu->addAction(m_pageNameDescending);
	m_pageMenu->addAction(m_pageDateAscending);
	m_pageMenu->addAction(m_pageDateDescending);
	m_pageMenu->addAction(m_pageSizeAscending);
	m_pageMenu->addAction(m_pageSizeDescending);
	m_pageMenu->addAction(m_pageShuffle);

	m_pageOnePage->setCheckable(true);
	m_pageTwoPage->setCheckable(true);
	m_pageRightToLeft->setCheckable(true);
	m_pageLeftToRight->setCheckable(true);
	m_pageNameAscending->setCheckable(true);
	m_pageNameDescending->setCheckable(true);
	m_pageDateAscending->setCheckable(true);
	m_pageDateDescending->setCheckable(true);
	m_pageSizeAscending->setCheckable(true);
	m_pageSizeDescending->setCheckable(true);
	m_pageShuffle->setCheckable(true);

	m_pagePageNumGroup->addAction(m_pageOnePage);
	m_pagePageNumGroup->addAction(m_pageTwoPage);
	m_pagePageNumGroup->setExclusionPolicy(
			QActionGroup::ExclusionPolicy::ExclusiveOptional);
	m_pageReadDirectionGroup->addAction(m_pageRightToLeft);
	m_pageReadDirectionGroup->addAction(m_pageLeftToRight);
	m_pageReadDirectionGroup->setExclusionPolicy(
			QActionGroup::ExclusionPolicy::ExclusiveOptional);
	m_pageSortOptionGroup->addAction(m_pageNameAscending);
	m_pageSortOptionGroup->addAction(m_pageNameDescending);
	m_pageSortOptionGroup->addAction(m_pageDateAscending);
	m_pageSortOptionGroup->addAction(m_pageDateDescending);
	m_pageSortOptionGroup->addAction(m_pageSizeAscending);
	m_pageSortOptionGroup->addAction(m_pageSizeDescending);
	m_pageSortOptionGroup->addAction(m_pageShuffle);
	m_pageSortOptionGroup->setExclusionPolicy(
			QActionGroup::ExclusionPolicy::ExclusiveOptional);
}

void MainWindowUi::setupOptionMenuUi(QMainWindow *ImgView)
{
	m_menubar->addAction(m_optionMenu->menuAction());

	m_optionMenu->addAction(m_optionSettings);
	m_optionMenu->addAction(m_optionConfigureKeyboardShortcut);
}

void MainWindowUi::setupHelpMenuUi(QMainWindow *ImgView)
{
	m_menubar->addAction(m_helpMenu->menuAction());

	m_helpMenu->addAction(m_helpMenuHelp);
	m_helpMenu->addAction(m_helpShortcutHelp);
	m_helpMenu->addSeparator();
	m_helpMenu->addAction(m_helpAbout);
}

void MainWindowUi::retranslateFileMenuUi()
{
	auto tr = [](const char *utf8) { return QObject::tr(utf8); };

	m_fileMenu->setTitle(tr("File(&F)"));

	m_fileOpen->setText(tr("Open(&O)"));
	m_fileOpen->setShortcut(QKeySequence::Open);
	m_fileClose->setText(tr("Close(&C)"));
	m_fileClose->setShortcut(QKeySequence("Ctrl+W"));
	m_fileRecentBooks->setTitle(tr("Recent Books"));
	m_fileOpenFileLocation->setText(tr("Open File Location"));
	m_fileSaveAs->setText(tr("Save As(&S)"));
	m_filePrint->setText(tr("Print(&P)"));
	m_fileExit->setText(tr("Exit(&X)"));
}

void MainWindowUi::retranslateViewMenuUi()
{
	auto tr = [](const char *utf8) { return QObject::tr(utf8); };

	m_viewMenu->setTitle(tr("View(&V)"));

	m_viewLibrary->setText(tr("Library"));
	m_viewPageList->setText(tr("Page List"));
	m_viewHistory->setText(tr("History"));
	m_viewInformation->setText(tr("Information"));
	m_viewAction->setText(tr("Action"));
	m_viewSideBar->setText(tr("Side Bar"));
}

void MainWindowUi::retranslateImageMenuUi()
{
	auto tr = [](const char *utf8) { return QObject::tr(utf8); };

	m_imageMenu->setTitle(tr("Image(&I)"));

	m_imageOriginSize->setText(tr("Origin Size"));
	m_imageFitToWindow->setText(tr("Fit to Window"));
	m_imageFitHeightToWindow->setText(tr("Fit Height to Window"));
	m_imageFitWidthToWindow->setText(tr("Fit Width to Window"));
	m_imageDarkBackground->setText(tr("Dark Background"));
	m_imageLightBackground->setText(tr("Light Background"));
	m_imageCustomBackground->setText(tr("Custom Background"));
}

void MainWindowUi::retranslateJumpMenuUi()
{
	auto tr = [](const char *utf8) { return QObject::tr(utf8); };

	m_jumpMenu->setTitle(tr("Jump(&J)"));

	m_jumpPrevPage->setText(tr("Prev Page"));
	m_jumpNextPage->setText(tr("Next Page"));
	m_jumpFirstPage->setText(tr("First Page"));
	m_jumpLastPage->setText(tr("Last Page"));
	m_jumpPrevBook->setText(tr("Prev Book"));
	m_jumpNextBook->setText(tr("Next Book"));
	m_jumpPrevLocation->setText(tr("Prev Location"));
	m_jumpNextLocation->setText(tr("Next Location"));
}

void MainWindowUi::retranslatePageMenuUi()
{
	auto tr = [](const char *utf8) { return QObject::tr(utf8); };

	m_pageMenu->setTitle(tr("Page(&P)"));

	m_pageOnePage->setText(tr("One Page"));
	m_pageTwoPage->setText(tr("Two Page"));
	m_pageRightToLeft->setText(tr("Right to Left"));
	m_pageLeftToRight->setText(tr("Left to Right"));
	m_pageNameAscending->setText(tr("Name Ascending"));
	m_pageNameDescending->setText(tr("Name Descending"));
	m_pageDateAscending->setText(tr("Date Ascending"));
	m_pageDateDescending->setText(tr("Date Descending"));
	m_pageSizeAscending->setText(tr("Size Ascending"));
	m_pageSizeDescending->setText(tr("Size Descending"));
	m_pageShuffle->setText(tr("Shuffle"));
}

void MainWindowUi::retranslateOptionMenuUi()
{
	auto tr = [](const char *utf8) { return QObject::tr(utf8); };

	m_optionMenu->setTitle(tr("Option(&O)"));

	m_optionSettings->setText(tr("Settings"));
	m_optionConfigureKeyboardShortcut->setText(
			tr("Configure Keyboard Shortcut"));
}

void MainWindowUi::retranslateHelpMenuUi()
{
	auto tr = [](const char *utf8) { return QObject::tr(utf8); };

	m_helpMenu->setTitle(tr("Help(&H)"));

	m_helpAbout->setText(tr("About(&A)"));
	m_helpMenuHelp->setText(tr("Menu Help"));
	m_helpShortcutHelp->setText(tr("Shortcut Help"));
}

void MainWindowUi::setupCentralWidgetUi(QMainWindow *ImgView)
{
	m_board = new DrawingBoard(ImgView);
	m_board->init();
	ImgView->setCentralWidget(m_board);
}

void MainWindowUi::setupToolbarUi(QMainWindow *ImgView)
{
	QString (*str)(QByteArrayView) = QString::fromUtf8;

	m_toolbar = new QToolBar(ImgView);
	m_toolbar->setObjectName(str("tool_bar"));
	m_toolbar->setMovable(false);
	//m_toolbar->setStyleSheet("QToolBar { background: #D8D8D8 }");
	ImgView->addToolBar(Qt::ToolBarArea::TopToolBarArea, m_toolbar);
}

void MainWindowUi::setupNavigationUi(QMainWindow *ImgView)
{
	QString (*str)(QByteArrayView) = QString::fromUtf8;

	m_navigation = new QDockWidget(ImgView);
	m_navigation->setAllowedAreas(Qt::LeftDockWidgetArea);
	m_navigation->setFeatures(QDockWidget::NoDockWidgetFeatures);
	//m_navigation->setStyleSheet("QDockWidget { background: #E0E0E0 }");
	/* TODO: adjust the ui. */
	m_navigation->setVisible(false);
	ImgView->addDockWidget(Qt::LeftDockWidgetArea, m_navigation);

	QAction *m_navi_book = new QAction(ImgView);
	QIcon book_icon;
	book_icon.addFile(str(":/images/book.png"), QSize(),
			QIcon::Normal, QIcon::Off);
	m_navi_book->setIcon(book_icon);
	m_navigation->addAction(m_navi_book);
	QAction *m_navi_books = new QAction(ImgView);
	QAction *m_navi_history = new QAction(ImgView);
}

/* TODO: adjust the ui. */
void MainWindowUi::setupInfoUi(QMainWindow *ImgView)
{
	m_info = new QDockWidget(ImgView);
	m_info->setAllowedAreas(Qt::RightDockWidgetArea);
	m_info->setFeatures(QDockWidget::NoDockWidgetFeatures);
	/* TODO: adjust the ui. */
	m_info->setVisible(false);
	ImgView->addDockWidget(Qt::RightDockWidgetArea, m_info);
}


}  /* img_view::ui */
