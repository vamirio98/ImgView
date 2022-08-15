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

void MainWindowUi::setupUi(QMainWindow* ImgView)
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

void MainWindowUi::retranslateUi(QMainWindow* ImgView)
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

void MainWindowUi::createActions(QMainWindow* ImgView)
{
	QString (*str)(QByteArrayView) = QString::fromUtf8;

	/* Menu. */
	_fileMenu = new QMenu(ImgView);
	_fileMenu->setObjectName(str("file_menu"));
	_viewMenu = new QMenu(ImgView);
	_viewMenu->setObjectName(str("view_menu"));
	_imageMenu = new QMenu(ImgView);
	_imageMenu->setObjectName(str("image_menu"));
	_jumpMenu = new QMenu(ImgView);
	_jumpMenu->setObjectName(str("jump_menu"));
	_pageMenu = new QMenu(ImgView);
	_pageMenu->setObjectName(str("page_menu"));
	_optionMenu = new QMenu(ImgView);
	_optionMenu->setObjectName(str("option_menu"));
	_helpMenu = new QMenu(ImgView);
	_helpMenu->setObjectName(str("help_menu"));

	/* File menu. */
	_fileOpen = new QAction(ImgView);
	_fileOpen->setObjectName(str("file_open"));
	_fileClose = new QAction(ImgView);
	_fileClose->setObjectName(str("file_close"));
	_fileRecentBooks = new QMenu(ImgView);
	_fileRecentBooks->setObjectName(str("file_recent_books"));
	_fileOpenFileLocation = new QAction(ImgView);
	_fileOpenFileLocation->setObjectName(str("file_open_file_location"));
	_fileSaveAs = new QAction(ImgView);
	_fileSaveAs->setObjectName(str("file_save_as"));
	_filePrint = new QAction(ImgView);
	_filePrint->setObjectName(str("file_print"));
	_fileExit = new QAction(ImgView);
	_fileExit->setObjectName(str("file_exit"));

	/* View menu. */
	_viewLibrary = new QAction(ImgView);
	_viewLibrary->setObjectName(str("view_library"));
	_viewPageList = new QAction(ImgView);
	_viewPageList->setObjectName(str("view_page_list"));
	_viewHistory = new QAction(ImgView);
	_viewHistory->setObjectName(str("view_history"));
	_viewInformation = new QAction(ImgView);
	_viewInformation->setObjectName(str("view_information"));
	_viewAction = new QAction(ImgView);
	_viewAction->setObjectName(str("view_action"));
	_viewSideBar = new QAction(ImgView);
	_viewSideBar->setObjectName(str("view_side_bar"));

	/* Image menu. */
	_imageSizeGroup = new QActionGroup(ImgView);
	_imageOriginSize = new QAction(ImgView);
	_imageOriginSize->setObjectName(str("image_origin_size"));
	_imageFitToWindow = new QAction(ImgView);
	_imageFitToWindow->setObjectName(str("image_fit_to_window"));
	_imageFitHeightToWindow = new QAction(ImgView);
	_imageFitHeightToWindow->setObjectName(str("image_fit_height_to_window"));
	_imageFitWidthToWindow = new QAction(ImgView);
	_imageFitWidthToWindow->setObjectName(str("image_fit_width_to_window"));
	_imageBackgroundColorGroup = new QActionGroup(ImgView);
	_imageDarkBackground = new QAction(ImgView);
	_imageDarkBackground->setObjectName(str("image_dark_background"));
	_imageLightBackground = new QAction(ImgView);
	_imageLightBackground->setObjectName(str("image_light_background"));
	_imageCustomBackground = new QAction(ImgView);
	_imageCustomBackground->setObjectName(str("image_custom_background"));

	/* Jump menu. */
	_jumpPrevPage = new QAction(ImgView);
	_jumpPrevPage->setObjectName(str("jump_prev_page"));
	_jumpNextPage = new QAction(ImgView);
	_jumpNextPage->setObjectName(str("jump_next_page"));
	_jumpFirstPage = new QAction(ImgView);
	_jumpFirstPage->setObjectName(str("jump_first_page"));
	_jumpLastPage = new QAction(ImgView);
	_jumpLastPage->setObjectName(str("jump_last_page"));
	_jumpPrevBook = new QAction(ImgView);
	_jumpPrevBook->setObjectName(str("jump_prev_book"));
	_jumpNextBook = new QAction(ImgView);
	_jumpNextBook->setObjectName(str("jump_next_book"));
	_jumpPrevLocation = new QAction(ImgView);
	_jumpPrevLocation->setObjectName(str("jump_prev_location"));
	_jumpNextLocation = new QAction(ImgView);
	_jumpNextLocation->setObjectName(str("jump_next_location"));

	/* Page menu. */
	_pagePageNumGroup = new QActionGroup(ImgView);
	_pageOnePage = new QAction(ImgView);
	_pageOnePage->setObjectName(str("page_one_page"));
	_pageTwoPage = new QAction(ImgView);
	_pageTwoPage->setObjectName(str("page_two_page"));
	_pageReadDirectionGroup = new QActionGroup(ImgView);
	_pageRightToLeft = new QAction(ImgView);
	_pageRightToLeft->setObjectName(str("page_right_to_left"));
	_pageLeftToRight = new QAction(ImgView);
	_pageLeftToRight->setObjectName(str("page_left_to_right"));
	_pageSortOptionGroup = new QActionGroup(ImgView);
	_pageNameAscending = new QAction(ImgView);
	_pageNameAscending->setObjectName(str("page_name_ascending"));
	_pageNameDescending = new QAction(ImgView);
	_pageNameDescending->setObjectName(str("page_name_descending"));
	_pageDateAscending = new QAction(ImgView);
	_pageDateAscending->setObjectName(str("page_date_ascending"));
	_pageDateDescending = new QAction(ImgView);
	_pageDateDescending->setObjectName(str("page_date_descending"));
	_pageSizeAscending = new QAction(ImgView);
	_pageSizeAscending->setObjectName(str("page_size_ascending"));
	_pageSizeDescending = new QAction(ImgView);
	_pageSizeDescending->setObjectName(str("page_size_descending"));
	_pageShuffle = new QAction(ImgView);
	_pageShuffle->setObjectName(str("page_shuffle"));

	/* Option menu. */
	_optionSettings = new QAction(ImgView);
	_optionSettings->setObjectName(str("option_settings"));
	_optionConfigureKeyboardShortcut = new QAction(ImgView);
	_optionConfigureKeyboardShortcut->setObjectName(
			str("option_configure_keyboard_shortcut"));

	/* Help menu. */
	_helpMenuHelp = new QAction(ImgView);
	_helpMenuHelp->setObjectName(str("help_menu_help"));
	_helpShortcutHelp = new QAction(ImgView);
	_helpShortcutHelp->setObjectName(str("help_shortcut_help"));
	_helpAbout = new QAction(ImgView);
	_helpAbout->setObjectName(str("help_about"));
}

void MainWindowUi::setupMenubarUi(QMainWindow* ImgView)
{
	_menubar = new QMenuBar(ImgView);
	_menubar->setObjectName(QString::fromUtf8("menubar"));
	_menubar->setGeometry(0, 0, sInitWidth, 26);
	//_menubar->setStyleSheet("QMenuBar { background: #D8D8D8 }");
	ImgView->setMenuBar(_menubar);

	setupFileMenuUi(ImgView);
	setupViewMenuUi(ImgView);
	setupImageMenuUi(ImgView);
	setupJumpMenuUi(ImgView);
	setupPageMenuUi(ImgView);
	setupOptionMenuUi(ImgView);
	setupHelpMenuUi(ImgView);
}

void MainWindowUi::setupFileMenuUi(QMainWindow* ImgView)
{
	_menubar->addAction(_fileMenu->menuAction());

	_fileMenu->addAction(_fileOpen);
	_fileMenu->addAction(_fileClose);
	_fileMenu->addAction(_fileRecentBooks->menuAction());
	_fileMenu->addSeparator();
	_fileMenu->addAction(_fileOpenFileLocation);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_fileSaveAs);
	_fileMenu->addAction(_filePrint);
	_fileMenu->addSeparator();
	_fileMenu->addAction(_fileExit);
}

void MainWindowUi::setupViewMenuUi(QMainWindow* ImgView)
{
	_menubar->addAction(_viewMenu->menuAction());

	_viewMenu->addAction(_viewLibrary);
	_viewMenu->addAction(_viewPageList);
	_viewMenu->addAction(_viewHistory);
	_viewMenu->addAction(_viewInformation);
	_viewMenu->addAction(_viewAction);
	_viewMenu->addAction(_viewSideBar);

	_viewLibrary->setCheckable(true);
	_viewPageList->setCheckable(true);
	_viewHistory->setCheckable(true);
	_viewInformation->setCheckable(true);
	_viewAction->setCheckable(true);
	_viewSideBar->setCheckable(true);
}

void MainWindowUi::setupImageMenuUi(QMainWindow* ImgView)
{
	_menubar->addAction(_imageMenu->menuAction());

	_imageMenu->addAction(_imageOriginSize);
	_imageMenu->addAction(_imageFitToWindow);
	_imageMenu->addAction(_imageFitHeightToWindow);
	_imageMenu->addAction(_imageFitWidthToWindow);
	_imageMenu->addSeparator();
	_imageMenu->addAction(_imageDarkBackground);
	_imageMenu->addAction(_imageLightBackground);
	_imageMenu->addAction(_imageCustomBackground);

	_imageOriginSize->setCheckable(true);
	_imageFitToWindow->setCheckable(true);
	_imageFitHeightToWindow->setCheckable(true);
	_imageFitWidthToWindow->setCheckable(true);
	_imageDarkBackground->setCheckable(true);
	_imageLightBackground->setCheckable(true);
	_imageCustomBackground->setCheckable(true);

	_imageSizeGroup->addAction(_imageOriginSize);
	_imageSizeGroup->addAction(_imageFitToWindow);
	_imageSizeGroup->addAction(_imageFitHeightToWindow);
	_imageSizeGroup->addAction(_imageFitWidthToWindow);
	_imageSizeGroup->setExclusionPolicy(
			QActionGroup::ExclusionPolicy::ExclusiveOptional);

	_imageBackgroundColorGroup->addAction(_imageDarkBackground);
	_imageBackgroundColorGroup->addAction(_imageLightBackground);
	_imageBackgroundColorGroup->addAction(_imageCustomBackground);
	_imageBackgroundColorGroup->setExclusionPolicy(
			QActionGroup::ExclusionPolicy::ExclusiveOptional);
}

void MainWindowUi::setupJumpMenuUi(QMainWindow* ImgView)
{
	_menubar->addAction(_jumpMenu->menuAction());

	_jumpMenu->addAction(_jumpPrevPage);
	_jumpMenu->addAction(_jumpNextPage);
	_jumpMenu->addAction(_jumpFirstPage);
	_jumpMenu->addAction(_jumpLastPage);
	_jumpMenu->addAction(_jumpPrevBook);
	_jumpMenu->addAction(_jumpNextBook);
	_jumpMenu->addSeparator();
	_jumpMenu->addAction(_jumpPrevLocation);
	_jumpMenu->addAction(_jumpNextLocation);
}

void MainWindowUi::setupPageMenuUi(QMainWindow* ImgView)
{
	_menubar->addAction(_pageMenu->menuAction());

	_pageMenu->addAction(_pageOnePage);
	_pageMenu->addAction(_pageTwoPage);
	_pageMenu->addSeparator();
	_pageMenu->addAction(_pageRightToLeft);
	_pageMenu->addAction(_pageLeftToRight);
	_pageMenu->addSeparator();
	_pageMenu->addAction(_pageNameAscending);
	_pageMenu->addAction(_pageNameDescending);
	_pageMenu->addAction(_pageDateAscending);
	_pageMenu->addAction(_pageDateDescending);
	_pageMenu->addAction(_pageSizeAscending);
	_pageMenu->addAction(_pageSizeDescending);
	_pageMenu->addAction(_pageShuffle);

	_pageOnePage->setCheckable(true);
	_pageTwoPage->setCheckable(true);
	_pageRightToLeft->setCheckable(true);
	_pageLeftToRight->setCheckable(true);
	_pageNameAscending->setCheckable(true);
	_pageNameDescending->setCheckable(true);
	_pageDateAscending->setCheckable(true);
	_pageDateDescending->setCheckable(true);
	_pageSizeAscending->setCheckable(true);
	_pageSizeDescending->setCheckable(true);
	_pageShuffle->setCheckable(true);

	_pagePageNumGroup->addAction(_pageOnePage);
	_pagePageNumGroup->addAction(_pageTwoPage);
	_pagePageNumGroup->setExclusionPolicy(
			QActionGroup::ExclusionPolicy::ExclusiveOptional);
	_pageReadDirectionGroup->addAction(_pageRightToLeft);
	_pageReadDirectionGroup->addAction(_pageLeftToRight);
	_pageReadDirectionGroup->setExclusionPolicy(
			QActionGroup::ExclusionPolicy::ExclusiveOptional);
	_pageSortOptionGroup->addAction(_pageNameAscending);
	_pageSortOptionGroup->addAction(_pageNameDescending);
	_pageSortOptionGroup->addAction(_pageDateAscending);
	_pageSortOptionGroup->addAction(_pageDateDescending);
	_pageSortOptionGroup->addAction(_pageSizeAscending);
	_pageSortOptionGroup->addAction(_pageSizeDescending);
	_pageSortOptionGroup->addAction(_pageShuffle);
	_pageSortOptionGroup->setExclusionPolicy(
			QActionGroup::ExclusionPolicy::ExclusiveOptional);
}

void MainWindowUi::setupOptionMenuUi(QMainWindow* ImgView)
{
	_menubar->addAction(_optionMenu->menuAction());

	_optionMenu->addAction(_optionSettings);
	_optionMenu->addAction(_optionConfigureKeyboardShortcut);
}

void MainWindowUi::setupHelpMenuUi(QMainWindow* ImgView)
{
	_menubar->addAction(_helpMenu->menuAction());

	_helpMenu->addAction(_helpMenuHelp);
	_helpMenu->addAction(_helpShortcutHelp);
	_helpMenu->addSeparator();
	_helpMenu->addAction(_helpAbout);
}

void MainWindowUi::retranslateFileMenuUi()
{
	auto tr = [](const char* utf8) { return QObject::tr(utf8); };

	_fileMenu->setTitle(tr("File(&F)"));

	_fileOpen->setText(tr("Open(&O)"));
	_fileOpen->setShortcut(QKeySequence::Open);
	_fileClose->setText(tr("Close(&C)"));
	_fileClose->setShortcut(QKeySequence("Ctrl+W"));
	_fileRecentBooks->setTitle(tr("Recent Books"));
	_fileOpenFileLocation->setText(tr("Open File Location"));
	_fileSaveAs->setText(tr("Save As(&S)"));
	_filePrint->setText(tr("Print(&P)"));
	_fileExit->setText(tr("Exit(&X)"));
}

void MainWindowUi::retranslateViewMenuUi()
{
	auto tr = [](const char* utf8) { return QObject::tr(utf8); };

	_viewMenu->setTitle(tr("View(&V)"));

	_viewLibrary->setText(tr("Library"));
	_viewPageList->setText(tr("Page List"));
	_viewHistory->setText(tr("History"));
	_viewInformation->setText(tr("Information"));
	_viewAction->setText(tr("Action"));
	_viewSideBar->setText(tr("Side Bar"));
}

void MainWindowUi::retranslateImageMenuUi()
{
	auto tr = [](const char* utf8) { return QObject::tr(utf8); };

	_imageMenu->setTitle(tr("Image(&I)"));

	_imageOriginSize->setText(tr("Origin Size"));
	_imageFitToWindow->setText(tr("Fit to Window"));
	_imageFitHeightToWindow->setText(tr("Fit Height to Window"));
	_imageFitWidthToWindow->setText(tr("Fit Width to Window"));
	_imageDarkBackground->setText(tr("Dark Background"));
	_imageLightBackground->setText(tr("Light Background"));
	_imageCustomBackground->setText(tr("Custom Background"));
}

void MainWindowUi::retranslateJumpMenuUi()
{
	auto tr = [](const char* utf8) { return QObject::tr(utf8); };

	_jumpMenu->setTitle(tr("Jump(&J)"));

	_jumpPrevPage->setText(tr("Prev Page"));
	_jumpNextPage->setText(tr("Next Page"));
	_jumpFirstPage->setText(tr("First Page"));
	_jumpLastPage->setText(tr("Last Page"));
	_jumpPrevBook->setText(tr("Prev Book"));
	_jumpNextBook->setText(tr("Next Book"));
	_jumpPrevLocation->setText(tr("Prev Location"));
	_jumpNextLocation->setText(tr("Next Location"));
}

void MainWindowUi::retranslatePageMenuUi()
{
	auto tr = [](const char* utf8) { return QObject::tr(utf8); };

	_pageMenu->setTitle(tr("Page(&P)"));

	_pageOnePage->setText(tr("One Page"));
	_pageTwoPage->setText(tr("Two Page"));
	_pageRightToLeft->setText(tr("Right to Left"));
	_pageLeftToRight->setText(tr("Left to Right"));
	_pageNameAscending->setText(tr("Name Ascending"));
	_pageNameDescending->setText(tr("Name Descending"));
	_pageDateAscending->setText(tr("Date Ascending"));
	_pageDateDescending->setText(tr("Date Descending"));
	_pageSizeAscending->setText(tr("Size Ascending"));
	_pageSizeDescending->setText(tr("Size Descending"));
	_pageShuffle->setText(tr("Shuffle"));
}

void MainWindowUi::retranslateOptionMenuUi()
{
	auto tr = [](const char* utf8) { return QObject::tr(utf8); };

	_optionMenu->setTitle(tr("Option(&O)"));

	_optionSettings->setText(tr("Settings"));
	_optionConfigureKeyboardShortcut->setText(
			tr("Configure Keyboard Shortcut"));
}

void MainWindowUi::retranslateHelpMenuUi()
{
	auto tr = [](const char* utf8) { return QObject::tr(utf8); };

	_helpMenu->setTitle(tr("Help(&H)"));

	_helpAbout->setText(tr("About(&A)"));
	_helpMenuHelp->setText(tr("Menu Help"));
	_helpShortcutHelp->setText(tr("Shortcut Help"));
}

void MainWindowUi::setupCentralWidgetUi(QMainWindow* ImgView)
{
	_paper = new Paper(ImgView);
	ImgView->setCentralWidget(_paper);
}

void MainWindowUi::setupToolbarUi(QMainWindow* ImgView)
{
	QString (*str)(QByteArrayView) = QString::fromUtf8;

	_toolbar = new QToolBar(ImgView);
	_toolbar->setObjectName(str("tool_bar"));
	_toolbar->setMovable(false);
	//_toolbar->setStyleSheet("QToolBar { background: #D8D8D8 }");
	ImgView->addToolBar(Qt::ToolBarArea::TopToolBarArea, _toolbar);
}

void MainWindowUi::setupNavigationUi(QMainWindow* ImgView)
{
	QString (*str)(QByteArrayView) = QString::fromUtf8;

	_navigation = new QDockWidget(ImgView);
	_navigation->setAllowedAreas(Qt::LeftDockWidgetArea);
	_navigation->setFeatures(QDockWidget::NoDockWidgetFeatures);
	//_navigation->setStyleSheet("QDockWidget { background: #E0E0E0 }");
	/* TODO: adjust the ui. */
	_navigation->setVisible(false);
	ImgView->addDockWidget(Qt::LeftDockWidgetArea, _navigation);

	QAction* _naviBook = new QAction(ImgView);
	QIcon book_icon;
	book_icon.addFile(str(":/images/book.png"), QSize(),
			QIcon::Normal, QIcon::Off);
	_naviBook->setIcon(book_icon);
	_navigation->addAction(_naviBook);
	QAction* _naviBooks = new QAction(ImgView);
	QAction* _naviHistory = new QAction(ImgView);
}

/* TODO: adjust the ui. */
void MainWindowUi::setupInfoUi(QMainWindow* ImgView)
{
	_info = new QDockWidget(ImgView);
	_info->setAllowedAreas(Qt::RightDockWidgetArea);
	_info->setFeatures(QDockWidget::NoDockWidgetFeatures);
	/* TODO: adjust the ui. */
	_info->setVisible(false);
	ImgView->addDockWidget(Qt::RightDockWidgetArea, _info);
}


}  /* img_view::ui */
