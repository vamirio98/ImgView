/**
 * canvas.h
 *
 * Created by vamirio on 2022 Jun 21
 */
#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace img_view
{

class Canvas : QWidget {
	Q_OBJECT

public:
	explicit Canvas(QWidget *parent = nullptr);
	~Canvas();

	/**
	 * @brief Draw a picture
	 *
	 * @param data Origin data
	 *
	 * @return True when succeeded
	 */
	bool draw(const cv::Mat &data);
};

} /* img_view */

#endif /* ifndef CANVAS_H */
