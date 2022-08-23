/**
 * main_window_ui.h
 *
 * Created by vamirio on 2022 Apr 25
 */
#ifndef MAIN_WINDOW_UI_H
#define MAIN_WINDOW_UI_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QDockWidget>

#include "paper.h"

namespace img_view::ui {

class MainWindowUi {
public:
	/* Menu bar. */
	QMenuBar* _menubar = nullptr;

	/* Menus. */
	QMenu* _file = nullptr;
	QMenu* _view = nullptr;
	QMenu* _image = nullptr;
	QMenu* _jump = nullptr;
	QMenu* _page = nullptr;
	QMenu* _option = nullptr;
	QMenu* _help = nullptr;

	/* File menu. */
	QAction* _open = nullptr;
	QAction* _close = nullptr;
	QMenu* _recentBooks = nullptr;
	QAction* _openFileLoc = nullptr;
	QAction* _saveAs = nullptr;
	QAction* _print = nullptr;
	QAction* _exit = nullptr;

	/* View menu. */
	QAction* _library = nullptr;
	QAction* _pageList = nullptr;
	QAction* _history = nullptr;
	QAction* _info = nullptr;
	QAction* _action = nullptr;
	QAction* _sideBar = nullptr;

	/* Image menu. */
	QActionGroup* _sizeGroup = nullptr;
	QAction* _originSize = nullptr;
	QAction* _fitToWindow = nullptr;
	QAction* _fitHeightToWindow = nullptr;
	QAction* _fitWidthToWindow = nullptr;
	QActionGroup* _bgColorGroup = nullptr;
	QAction* _darkBg = nullptr;
	QAction* _lightBg = nullptr;
	QAction* _customBg = nullptr;

	/* Jump menu. */
	QAction* _prevPage = nullptr;
	QAction* _nextPage = nullptr;
	QAction* _firstPage = nullptr;
	QAction* _lastPage = nullptr;
	QAction* _prevBook = nullptr;
	QAction* _nextBook = nullptr;
	QAction* _prevLoc = nullptr;
	QAction* _nextLoc = nullptr;

	/* Page menu. */
	QActionGroup* _pageNumGroup = nullptr;
	QAction* _onePage = nullptr;
	QAction* _twoPage = nullptr;
	QActionGroup* _readDirectionGroup = nullptr;
	QAction* _rightToLeft = nullptr;
	QAction* _leftToRight = nullptr;
	QActionGroup* _sortGroup = nullptr;
	QAction* _nameAscending = nullptr;
	QAction* _nameDescending = nullptr;
	QAction* _dateAscending = nullptr;
	QAction* _dateDescending = nullptr;
	QAction* _sizeAscending = nullptr;
	QAction* _sizeDescending = nullptr;
	QAction* _shuffle = nullptr;

	/* Option menu. */
	QAction* _settings = nullptr;
	QAction* _configKeyboardShortcut = nullptr;

	/* Help menu. */
	QAction* _menuHelp = nullptr;
	QAction* _shortcutHelp = nullptr;
	QAction* _about = nullptr;

	/* Tool bar. */
	QToolBar* _toolbar = nullptr;

	/* Central widget. */
	Paper* _paper = nullptr;

	/* Left navigation area. */
	QDockWidget* _navigation = nullptr;
	QAction* _bookNavi = nullptr;
	QAction* _booksNavi = nullptr;
	QAction* _historyNavi = nullptr;

	/* Right information area. */
	QDockWidget* _infoPanel = nullptr;
	QAction* _detail = nullptr;

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
	void setupInfoPanelUi(QMainWindow* ImgView);
};

}  /* img_view::ui */

#endif /* ifndef MAIN_WINDOW_UI_H */
