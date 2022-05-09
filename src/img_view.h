/**
 * img_view.h
 *
 * Created by vamirio on 2022 Apr 25
 * Last Modified: 2022 May 05 14:56:01
 */
#ifndef IMG_VIEW_H
#define IMG_VIEW_H

#include <QMainWindow>
#include <QFileDialog>

#include "img_area.h"

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
	void scaleImage(const double &factor);

	static void initImgFileDialog(QFileDialog *dialog,
			const QFileDialog::AcceptMode accept_mode);

private:
	static QString m_lastOpenPos;
	ui::ImgViewUi *m_ui = nullptr;
	ImgArea *m_display = nullptr;

	double m_scaleFactor = 1.0;
};

}  /* namespace img_view */

#endif /* ifndef IMG_VIEW_H */
