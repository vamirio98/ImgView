/**
 * main_window.h
 *
 * Created by vamirio on 2022 Apr 25
 */
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "drawing_board.h"
#include "book.h"

namespace img_view {

namespace ui {
	class MainWindowUi;
}  /* namespace img_view::ui */

class MainWindow : public QMainWindow {

	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void init();
	bool loadFile(const QString &filename);

private slots:
	void onFileOpen();
	void onFileClose();
	void onFileExit();
	void onHelpAbout();
	void onCtrlPlus();
	void onCtrlMinus();

private:
	void setupSlots();
	void setupShortCut();

	static void initImgFileDialog(QFileDialog *dialog,
			const QFileDialog::AcceptMode accept_mode);

private:
	static QString m_lastOpenPos;
	ui::MainWindowUi *m_ui = nullptr;
	DrawingBoard *m_display = nullptr;
	Book m_book;
};

}  /* img_view */

#endif /* ifndef MAIN_WINDOW_H */
