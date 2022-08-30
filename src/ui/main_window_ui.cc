/**
 * main_window_ui.cc
 *
 * Created by vamirio on 2022 Apr 25
 */
#include "ui/main_window_ui.h"

#include <QActionGroup>

namespace img_view::ui
{

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
		setupInfoPanelUi(ImgView);

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
		_file = new QMenu(ImgView);
		_file->setObjectName(str("file_menu"));
		_view = new QMenu(ImgView);
		_view->setObjectName(str("view_menu"));
		_image = new QMenu(ImgView);
		_image->setObjectName(str("image_menu"));
		_jump = new QMenu(ImgView);
		_jump->setObjectName(str("jump_menu"));
		_page = new QMenu(ImgView);
		_page->setObjectName(str("page_menu"));
		_option = new QMenu(ImgView);
		_option->setObjectName(str("option_menu"));
		_help = new QMenu(ImgView);
		_help->setObjectName(str("help_menu"));

		/* File menu. */
		_open = new QAction(ImgView);
		_open->setObjectName(str("file_open"));
		_close = new QAction(ImgView);
		_close->setObjectName(str("file_close"));
		_recentBooks = new QMenu(ImgView);
		_recentBooks->setObjectName(str("file_recent_books"));
		_openFileLoc = new QAction(ImgView);
		_openFileLoc->setObjectName(str("file_open_file_location"));
		_saveAs = new QAction(ImgView);
		_saveAs->setObjectName(str("file_save_as"));
		_print = new QAction(ImgView);
		_print->setObjectName(str("file_print"));
		_exit = new QAction(ImgView);
		_exit->setObjectName(str("file_exit"));

		/* View menu. */
		_library = new QAction(ImgView);
		_library->setObjectName(str("view_library"));
		_pageList = new QAction(ImgView);
		_pageList->setObjectName(str("view_page_list"));
		_history = new QAction(ImgView);
		_history->setObjectName(str("view_history"));
		_info = new QAction(ImgView);
		_info->setObjectName(str("view_information"));
		_action = new QAction(ImgView);
		_action->setObjectName(str("view_action"));
		_sideBar = new QAction(ImgView);
		_sideBar->setObjectName(str("view_side_bar"));

		/* Image menu. */
		_sizeGroup = new QActionGroup(ImgView);
		_originSize = new QAction(ImgView);
		_originSize->setObjectName(str("image_origin_size"));
		_fitToWindow = new QAction(ImgView);
		_fitToWindow->setObjectName(str("image_fit_to_window"));
		_fitHeightToWindow = new QAction(ImgView);
		_fitHeightToWindow->setObjectName(str("image_fit_height_to_window"));
		_fitWidthToWindow = new QAction(ImgView);
		_fitWidthToWindow->setObjectName(str("image_fit_width_to_window"));
		_bgColorGroup = new QActionGroup(ImgView);
		_darkBg = new QAction(ImgView);
		_darkBg->setObjectName(str("image_dark_background"));
		_lightBg = new QAction(ImgView);
		_lightBg->setObjectName(str("image_light_background"));
		_customBg = new QAction(ImgView);
		_customBg->setObjectName(str("image_custom_background"));

		/* Jump menu. */
		_prevPage = new QAction(ImgView);
		_prevPage->setObjectName(str("jump_prev_page"));
		_nextPage = new QAction(ImgView);
		_nextPage->setObjectName(str("jump_next_page"));
		_firstPage = new QAction(ImgView);
		_firstPage->setObjectName(str("jump_first_page"));
		_lastPage = new QAction(ImgView);
		_lastPage->setObjectName(str("jump_last_page"));
		_prevBook = new QAction(ImgView);
		_prevBook->setObjectName(str("jump_prev_book"));
		_nextBook = new QAction(ImgView);
		_nextBook->setObjectName(str("jump_next_book"));
		_prevLoc = new QAction(ImgView);
		_prevLoc->setObjectName(str("jump_prev_location"));
		_nextLoc = new QAction(ImgView);
		_nextLoc->setObjectName(str("jump_next_location"));

		/* Page menu. */
		_pageNumGroup = new QActionGroup(ImgView);
		_onePage = new QAction(ImgView);
		_onePage->setObjectName(str("page_one_page"));
		_twoPage = new QAction(ImgView);
		_twoPage->setObjectName(str("page_two_page"));
		_readDirectionGroup = new QActionGroup(ImgView);
		_rightToLeft = new QAction(ImgView);
		_rightToLeft->setObjectName(str("page_right_to_left"));
		_leftToRight = new QAction(ImgView);
		_leftToRight->setObjectName(str("page_left_to_right"));
		_sortGroup = new QActionGroup(ImgView);
		_nameAscending = new QAction(ImgView);
		_nameAscending->setObjectName(str("page_name_ascending"));
		_nameDescending = new QAction(ImgView);
		_nameDescending->setObjectName(str("page_name_descending"));
		_dateAscending = new QAction(ImgView);
		_dateAscending->setObjectName(str("page_date_ascending"));
		_dateDescending = new QAction(ImgView);
		_dateDescending->setObjectName(str("page_date_descending"));
		_sizeAscending = new QAction(ImgView);
		_sizeAscending->setObjectName(str("page_size_ascending"));
		_sizeDescending = new QAction(ImgView);
		_sizeDescending->setObjectName(str("page_size_descending"));
		_shuffle = new QAction(ImgView);
		_shuffle->setObjectName(str("page_shuffle"));

		/* Option menu. */
		_settings = new QAction(ImgView);
		_settings->setObjectName(str("option_settings"));
		_configKeyboardShortcut = new QAction(ImgView);
		_configKeyboardShortcut->setObjectName(
		    str("option_configure_keyboard_shortcut")
		);

		/* Help menu. */
		_menuHelp = new QAction(ImgView);
		_menuHelp->setObjectName(str("help_menu_help"));
		_shortcutHelp = new QAction(ImgView);
		_shortcutHelp->setObjectName(str("help_shortcut_help"));
		_about = new QAction(ImgView);
		_about->setObjectName(str("help_about"));
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
		_menubar->addAction(_file->menuAction());

		_file->addAction(_open);
		_file->addAction(_close);
		_file->addAction(_recentBooks->menuAction());
		_file->addSeparator();
		_file->addAction(_openFileLoc);
		_file->addSeparator();
		_file->addAction(_saveAs);
		_file->addAction(_print);
		_file->addSeparator();
		_file->addAction(_exit);
	}

	void MainWindowUi::setupViewMenuUi(QMainWindow* ImgView)
	{
		_menubar->addAction(_view->menuAction());

		_view->addAction(_library);
		_view->addAction(_pageList);
		_view->addAction(_history);
		_view->addAction(_info);
		_view->addAction(_action);
		_view->addAction(_sideBar);

		_library->setCheckable(true);
		_pageList->setCheckable(true);
		_history->setCheckable(true);
		_info->setCheckable(true);
		_action->setCheckable(true);
		_sideBar->setCheckable(true);
	}

	void MainWindowUi::setupImageMenuUi(QMainWindow* ImgView)
	{
		_menubar->addAction(_image->menuAction());

		_image->addAction(_originSize);
		_image->addAction(_fitToWindow);
		_image->addAction(_fitHeightToWindow);
		_image->addAction(_fitWidthToWindow);
		_image->addSeparator();
		_image->addAction(_darkBg);
		_image->addAction(_lightBg);
		_image->addAction(_customBg);

		_originSize->setCheckable(true);
		_fitToWindow->setCheckable(true);
		_fitHeightToWindow->setCheckable(true);
		_fitWidthToWindow->setCheckable(true);
		_darkBg->setCheckable(true);
		_lightBg->setCheckable(true);
		_customBg->setCheckable(true);

		_sizeGroup->addAction(_originSize);
		_sizeGroup->addAction(_fitToWindow);
		_sizeGroup->addAction(_fitHeightToWindow);
		_sizeGroup->addAction(_fitWidthToWindow);
		_sizeGroup->setExclusionPolicy(
		    QActionGroup::ExclusionPolicy::ExclusiveOptional
		);

		_bgColorGroup->addAction(_darkBg);
		_bgColorGroup->addAction(_lightBg);
		_bgColorGroup->addAction(_customBg);
		_bgColorGroup->setExclusionPolicy(
		    QActionGroup::ExclusionPolicy::ExclusiveOptional
		);
	}

	void MainWindowUi::setupJumpMenuUi(QMainWindow* ImgView)
	{
		_menubar->addAction(_jump->menuAction());

		_jump->addAction(_prevPage);
		_jump->addAction(_nextPage);
		_jump->addAction(_firstPage);
		_jump->addAction(_lastPage);
		_jump->addAction(_prevBook);
		_jump->addAction(_nextBook);
		_jump->addSeparator();
		_jump->addAction(_prevLoc);
		_jump->addAction(_nextLoc);
	}

	void MainWindowUi::setupPageMenuUi(QMainWindow* ImgView)
	{
		_menubar->addAction(_page->menuAction());

		_page->addAction(_onePage);
		_page->addAction(_twoPage);
		_page->addSeparator();
		_page->addAction(_rightToLeft);
		_page->addAction(_leftToRight);
		_page->addSeparator();
		_page->addAction(_nameAscending);
		_page->addAction(_nameDescending);
		_page->addAction(_dateAscending);
		_page->addAction(_dateDescending);
		_page->addAction(_sizeAscending);
		_page->addAction(_sizeDescending);
		_page->addAction(_shuffle);

		_onePage->setCheckable(true);
		_twoPage->setCheckable(true);
		_rightToLeft->setCheckable(true);
		_leftToRight->setCheckable(true);
		_sizeAscending->setCheckable(true);
		_nameDescending->setCheckable(true);
		_dateAscending->setCheckable(true);
		_dateDescending->setCheckable(true);
		_sizeAscending->setCheckable(true);
		_sizeDescending->setCheckable(true);
		_shuffle->setCheckable(true);

		_pageNumGroup->addAction(_onePage);
		_pageNumGroup->addAction(_twoPage);
		_pageNumGroup->setExclusionPolicy(
		    QActionGroup::ExclusionPolicy::ExclusiveOptional
		);
		_readDirectionGroup->addAction(_rightToLeft);
		_readDirectionGroup->addAction(_leftToRight);
		_readDirectionGroup->setExclusionPolicy(
		    QActionGroup::ExclusionPolicy::ExclusiveOptional
		);
		_sortGroup->addAction(_sizeAscending);
		_sortGroup->addAction(_nameDescending);
		_sortGroup->addAction(_dateAscending);
		_sortGroup->addAction(_dateDescending);
		_sortGroup->addAction(_sizeAscending);
		_sortGroup->addAction(_sizeDescending);
		_sortGroup->addAction(_shuffle);
		_sortGroup->setExclusionPolicy(
		    QActionGroup::ExclusionPolicy::ExclusiveOptional
		);
	}

	void MainWindowUi::setupOptionMenuUi(QMainWindow* ImgView)
	{
		_menubar->addAction(_option->menuAction());

		_option->addAction(_settings);
		_option->addAction(_configKeyboardShortcut);
	}

	void MainWindowUi::setupHelpMenuUi(QMainWindow* ImgView)
	{
		_menubar->addAction(_help->menuAction());

		_help->addAction(_menuHelp);
		_help->addAction(_shortcutHelp);
		_help->addSeparator();
		_help->addAction(_about);
	}

	void MainWindowUi::retranslateFileMenuUi()
	{
		auto tr = [](const char* utf8) { return QObject::tr(utf8); };

		_file->setTitle(tr("File(&F)"));

		_open->setText(tr("Open(&O)"));
		_open->setShortcut(QKeySequence::Open);
		_close->setText(tr("Close(&C)"));
		_close->setShortcut(QKeySequence("Ctrl+W"));
		_recentBooks->setTitle(tr("Recent Books"));
		_openFileLoc->setText(tr("Open File Location"));
		_saveAs->setText(tr("Save As(&S)"));
		_print->setText(tr("Print(&P)"));
		_exit->setText(tr("Exit(&X)"));
	}

	void MainWindowUi::retranslateViewMenuUi()
	{
		auto tr = [](const char* utf8) { return QObject::tr(utf8); };

		_view->setTitle(tr("View(&V)"));

		_library->setText(tr("Library"));
		_pageList->setText(tr("Page List"));
		_history->setText(tr("History"));
		_info->setText(tr("Information"));
		_action->setText(tr("Action"));
		_sideBar->setText(tr("Side Bar"));
	}

	void MainWindowUi::retranslateImageMenuUi()
	{
		auto tr = [](const char* utf8) { return QObject::tr(utf8); };

		_image->setTitle(tr("Image(&I)"));

		_originSize->setText(tr("Origin Size"));
		_fitToWindow->setText(tr("Fit to Window"));
		_fitHeightToWindow->setText(tr("Fit Height to Window"));
		_fitWidthToWindow->setText(tr("Fit Width to Window"));
		_darkBg->setText(tr("Dark Background"));
		_lightBg->setText(tr("Light Background"));
		_customBg->setText(tr("Custom Background"));
	}

	void MainWindowUi::retranslateJumpMenuUi()
	{
		auto tr = [](const char* utf8) { return QObject::tr(utf8); };

		_jump->setTitle(tr("Jump(&J)"));

		_prevPage->setText(tr("Prev Page"));
		_nextPage->setText(tr("Next Page"));
		_firstPage->setText(tr("First Page"));
		_lastPage->setText(tr("Last Page"));
		_prevBook->setText(tr("Prev Book"));
		_nextBook->setText(tr("Next Book"));
		_prevLoc->setText(tr("Prev Location"));
		_nextLoc->setText(tr("Next Location"));
	}

	void MainWindowUi::retranslatePageMenuUi()
	{
		auto tr = [](const char* utf8) { return QObject::tr(utf8); };

		_page->setTitle(tr("Page(&P)"));

		_onePage->setText(tr("One Page"));
		_twoPage->setText(tr("Two Page"));
		_rightToLeft->setText(tr("Right to Left"));
		_leftToRight->setText(tr("Left to Right"));
		_nameAscending->setText(tr("Name Ascending"));
		_nameDescending->setText(tr("Name Descending"));
		_dateAscending->setText(tr("Date Ascending"));
		_dateDescending->setText(tr("Date Descending"));
		_sizeAscending->setText(tr("Size Ascending"));
		_sizeDescending->setText(tr("Size Descending"));
		_shuffle->setText(tr("Shuffle"));
	}

	void MainWindowUi::retranslateOptionMenuUi()
	{
		auto tr = [](const char* utf8) { return QObject::tr(utf8); };

		_option->setTitle(tr("Option(&O)"));

		_settings->setText(tr("Settings"));
		_configKeyboardShortcut->setText(tr("Configure Keyboard Shortcut"));
	}

	void MainWindowUi::retranslateHelpMenuUi()
	{
		auto tr = [](const char* utf8) { return QObject::tr(utf8); };

		_help->setTitle(tr("Help(&H)"));

		_about->setText(tr("About(&A)"));
		_menuHelp->setText(tr("Menu Help"));
		_shortcutHelp->setText(tr("Shortcut Help"));
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
		book_icon.addFile(
		    str(":/images/book.png"), QSize(), QIcon::Normal, QIcon::Off
		);
		_naviBook->setIcon(book_icon);
		_navigation->addAction(_naviBook);
		QAction* _naviBooks = new QAction(ImgView);
		QAction* _naviHistory = new QAction(ImgView);
	}

	/* TODO: adjust the ui. */
	void MainWindowUi::setupInfoPanelUi(QMainWindow* ImgView)
	{
		_infoPanel = new QDockWidget(ImgView);
		_infoPanel->setAllowedAreas(Qt::RightDockWidgetArea);
		_infoPanel->setFeatures(QDockWidget::NoDockWidgetFeatures);
		/* TODO: adjust the ui. */
		_infoPanel->setVisible(false);
		ImgView->addDockWidget(Qt::RightDockWidgetArea, _infoPanel);
	}

} // namespace img_view::ui
