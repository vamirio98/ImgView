/**
 * main.cc
 *
 * Created by vamirio on 2022 Apr 25
 * Last Modified: 2022 May 07 16:39:09
 */
#include <QApplication>
#include <QCommandLineParser>

#include "img_view.h"
#include "logger.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QApplication::addLibraryPath(app.applicationDirPath() + "\\plugins");

	QCommandLineParser cmd_parser;
	cmd_parser.addHelpOption();
	cmd_parser.addPositionalArgument(img_view::ImgView::tr("[file]"),
			img_view::ImgView::tr("Image file to open."));
	cmd_parser.process(QApplication::arguments());

	img_view::ImgView ImgView;
	ImgView.init();
	img_view::logInfo("ImgView start up", img_view::log::LogPos::File);

	if (!cmd_parser.positionalArguments().isEmpty()
			&& !ImgView.loadFile(cmd_parser.positionalArguments().constFirst()))
		return -1;

	ImgView.show();

	return app.exec();
}
