/**
 * main_window.h
 *
 * Created by vamirio on 2022 Apr 25
 */
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QFileDialog>
#include <QMainWindow>

#include "book.h"
#include "lru_cache.h"
#include "paper.h"

namespace img_view
{

	namespace ui
	{
		class MainWindowUi;
	} // namespace ui

	class MainWindow : public QMainWindow
	{

		Q_OBJECT

	  public:
		explicit MainWindow(QWidget* parent = nullptr);
		~MainWindow();
		void init();

	  private slots:
		void onOpen();
		void onClose();
		void onRecentBooks();
		void onOpenFileLoc();
		void onExit();
		void onAbout();
		void onCtrlPlus();
		void onCtrlMinus();
		void showPrevPage();
		void showNextPage();

		/* Toogle actions' activation. */
		void toggleClose();
		void toggleRecentBooks();
		void toggleOpenFileLoc();
		void toggleSaveAs();
		void togglePrint();
		void togglePrevPage();
		void toggleNextPage();
		void toggleFirstPage();
		void toggleLastPage();
		void togglePrevBook();
		void toggleNextBook();
		void togglePrevLoc();
		void toggleNextLoc();

	  private:
		void setupSlots();
		void setupShortCut();

		bool openBook(const QString& bookPath);
		void closeBook();
		/* Update book and page together. */
		void updateRecentOpenList(const QString& book, const QString& image);

		static void initFileDialog(
		    QFileDialog* dialog, const QFileDialog::AcceptMode acceptMode
		);

	  private:
		static QString _lastOpenPos;
		static QString _fileBrowserProgram;
		static QString _fileBrowserParam;
		LruCache<QString, QString> _recentOpenBooks;
		QList<QAction*> _recentBooksActions;

		ui::MainWindowUi* _ui = nullptr;
		Paper* _paper = nullptr;
		Book _book;
	};

} // namespace img_view

#endif /* ifndef MAIN_WINDOW_H */
