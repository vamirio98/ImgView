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

#include "drawing_board.h"

namespace img_view::ui {

class MainWindowUi {
public:
	/* Menu bar. */
	QMenuBar *m_menubar = nullptr;

	/* Menus. */
	QMenu *m_fileMenu = nullptr;
	QMenu *m_helpMenu = nullptr;

	/* File menu. */
	QAction *m_fileOpen = nullptr;
	QAction *m_fileClose = nullptr;
	QAction *m_fileExit = nullptr;

	/* Help menu. */
	QAction *m_helpAbout = nullptr;

	/* Tool bar. */
	QToolBar *m_toolbar = nullptr;

	/* Central widget. */
	DrawingBoard *m_board = nullptr;

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
	void setupMenubarUi(QMainWindow *ImgView);
	void setupFileMenuUi(QMainWindow *ImgView);
	void setupHelpMenuUi(QMainWindow *ImgView);
	void retranslateFileMenuUi();
	void retranslateHelpMenuUi();
	void setupCentralWidgetUi(QMainWindow *ImgView);
	void setupToolbarUi(QMainWindow *ImgView);
	void setupNavigationUi(QMainWindow *ImgView);
	void setupInfoUi(QMainWindow *ImgView);
};

}

#endif /* ifndef MAIN_WINDOW_UI_H */
