/**
 * img_view_ui.cc
 *
 * Created by vamirio on 2022 Apr 25
 * Last Modified: 2022 May 02 23:30:58
 */
#include "ui/img_view_ui.h"

namespace img_view::ui {

static int sInitWidth = 800;
static int sInitHeight = 600;

void ImgViewUi::setupUi(QMainWindow *ImgView)
{
	if (ImgView->objectName().isEmpty())
		ImgView->setObjectName("ImgView");
	ImgView->resize(sInitWidth, sInitHeight);

	setupMenubarUi(ImgView);
	setupCentralWidgetUi(ImgView);
	setupToolbarUi(ImgView);
	setupNavigationUi(ImgView);
	setupInfoUi(ImgView);

	retranslateUi(ImgView);
}

void ImgViewUi::retranslateUi(QMainWindow *ImgView)
{
	ImgView->setWindowTitle(QString::fromUtf8("ImgView"));

	retranslateFileMenuUi();
	retranslateHelpMenuUi();
}

void ImgViewUi::setupMenubarUi(QMainWindow *ImgView)
{
	m_menubar = new QMenuBar(ImgView);
	m_menubar->setObjectName(QString::fromUtf8("menubar"));
	m_menubar->setGeometry(0, 0, sInitWidth, 26);
	m_menubar->setStyleSheet("QMenuBar { background: #D8D8D8 }");
	ImgView->setMenuBar(m_menubar);

	setupFileMenuUi(ImgView);
	setupHelpMenuUi(ImgView);
}

void ImgViewUi::setupFileMenuUi(QMainWindow *ImgView)
{
	QString (*str)(QByteArrayView) = QString::fromUtf8;

	m_fileMenu = new QMenu(ImgView);
	m_fileMenu->setObjectName(str("file_menu"));
	m_menubar->addAction(m_fileMenu->menuAction());

	m_fileOpen = new QAction(ImgView);
	m_fileOpen->setObjectName(str("file_open"));
	m_fileMenu->addAction(m_fileOpen);

	m_fileClose = new QAction(ImgView);
	m_fileClose->setObjectName(str("file_close"));
	m_fileMenu->addAction(m_fileClose);

	m_fileExit = new QAction(ImgView);
	m_fileExit->setObjectName(str("file_exit"));
	m_fileMenu->addAction(m_fileExit);
}

void ImgViewUi::setupHelpMenuUi(QMainWindow *ImgView)
{
	QString (*str)(QByteArrayView) = QString::fromUtf8;

	m_helpMenu = new QMenu(ImgView);
	m_helpMenu->setObjectName(str("help_menu"));
	m_menubar->addAction(m_helpMenu->menuAction());

	m_helpAbout = new QAction(ImgView);
	m_helpAbout->setObjectName(str("help_about"));
	m_helpMenu->addAction(m_helpAbout);
}

void ImgViewUi::retranslateFileMenuUi()
{
	auto tr = [](const char *utf8) { return QObject::tr(utf8); };

	m_fileMenu->setTitle(tr("File(&F)"));

	m_fileOpen->setText(tr("Open(&O)"));
	m_fileOpen->setShortcut(QKeySequence::Open);
	m_fileClose->setText(tr("Close(&C)"));
	m_fileClose->setShortcut(QKeySequence("Ctrl+W"));
	m_fileExit->setText(tr("Exit(&X)"));
}

void ImgViewUi::retranslateHelpMenuUi()
{
	auto tr = [](const char *utf8) { return QObject::tr(utf8); };

	m_helpMenu->setTitle(tr("Help(&H)"));

	m_helpAbout->setText(tr("About(&A)"));
}

void ImgViewUi::setupCentralWidgetUi(QMainWindow *ImgView)
{
	//m_image_area = new QWidget(ImgView);
	//m_scroll_area = new QScrollArea(ImgView);
	//m_label = new QLabel(ImgView);
	//m_image = new QImage();

	////ImgView->setCentralWidget(m_scroll_area);
	//ImgView->setCentralWidget(m_image_area);
	//m_image_area->setLayout(new QGridLayout());
	//m_image_area->layout()->addWidget(m_scroll_area);

	//m_scroll_area->setBackgroundRole(QPalette::Base);
	//m_scroll_area->setVisible(false);
	//m_scroll_area->setWidget(m_label);
	//m_scroll_area->setAlignment(Qt::AlignCenter);

	//m_label->setBackgroundRole(QPalette::Dark);
	//m_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_imageArea = new ImgArea(ImgView);
	m_imageArea->init();
	ImgView->setCentralWidget(m_imageArea);
}

void ImgViewUi::setupToolbarUi(QMainWindow *ImgView)
{
	QString (*str)(QByteArrayView) = QString::fromUtf8;

	m_toolbar = new QToolBar(ImgView);
	m_toolbar->setObjectName(str("tool_bar"));
	m_toolbar->setMovable(false);
	m_toolbar->setStyleSheet("QToolBar { background: #D8D8D8 }");
	ImgView->addToolBar(Qt::ToolBarArea::TopToolBarArea, m_toolbar);
}

void ImgViewUi::setupNavigationUi(QMainWindow *ImgView)
{
	QString (*str)(QByteArrayView) = QString::fromUtf8;

	m_navigation = new QDockWidget(ImgView);
	m_navigation->setAllowedAreas(Qt::LeftDockWidgetArea);
	m_navigation->setFeatures(QDockWidget::NoDockWidgetFeatures);
	m_navigation->setStyleSheet("QDockWidget { background: #E0E0E0 }");
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

void ImgViewUi::setupInfoUi(QMainWindow *ImgView)
{
	m_info = new QDockWidget(ImgView);
	m_info->setAllowedAreas(Qt::RightDockWidgetArea);
	m_info->setFeatures(QDockWidget::NoDockWidgetFeatures);
	ImgView->addDockWidget(Qt::RightDockWidgetArea, m_info);
}


}
