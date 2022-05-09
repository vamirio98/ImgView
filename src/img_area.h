/**
 * img_area.h
 *
 * Created by vamirio on 2022 May 01
 * Last Modified: 2022 May 09 15:43:18
 */
#ifndef IMG_AREA_H
#define IMG_AREA_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QImage>

namespace img_view {

class ImgArea : public QWidget {
public:
	explicit ImgArea(QWidget *parent = nullptr);
	~ImgArea();
	void init();
	bool loadFile(const QString &filename);
	void closeImage();
	void zoomIn();
	void zoomOut();
	void scaleImage(const double &factor);
	void goToPrevImage();
	void goToNextImage();

private:
	void setImage(const QImage &image);
	void adjustScrollBarPos(QScrollBar *scroll_bar, const double &factor);

protected:
	void wheelEvent(QWheelEvent *event) override;

private:
	QScrollArea *m_scrollArea = nullptr;
	QWidget *m_displayArea = nullptr;
	QGridLayout *m_displayLayout = nullptr;
	QLabel *m_label = nullptr;
	QImage *m_image = nullptr;
	double m_scaleFactor = 1.0;
};

}  /* namespace img_view */

#endif /* ifndef IMG_AREA_H */
