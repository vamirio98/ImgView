/**
 * img_view.h
 *
 * Created by vamirio on 2022 Apr 25
 */
#ifndef IMG_VIEW_H
#define IMG_VIEW_H

#include <QMainWindow>
#include <QFileDialog>

#include "display.h"
#include "book.h"

namespace img_view {

namespace ui {
	class ImgViewUi;
}  /* namespace img_view::ui */

class ImgView : public QMainWindow {

	Q_OBJECT

public:
	explicit ImgView(QWidget *parent = nullptr);
	~ImgView();
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
	ui::ImgViewUi *m_ui = nullptr;
	Display *m_display = nullptr;
	Book *m_book = nullptr;
};

}  /* namespace img_view */

#endif /* ifndef IMG_VIEW_H */
