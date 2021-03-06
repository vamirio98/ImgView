/**
 * main.cc
 *
 * Created by vamirio on 2022 Apr 25
 */
#include <QApplication>
#include <QCommandLineParser>
#include <QImageReader>

#include "img_view.h"
#include "logger.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
//	QApplication::addLibraryPath(app.applicationDirPath() + "/plugins");

	img_view::Logger::initInstance();
	img_view::Logger::setLogFilter(img_view::LogLevel::Debug);

	QCommandLineParser cmd_parser;
	cmd_parser.addHelpOption();
	cmd_parser.addPositionalArgument(img_view::ImgView::tr("[file]"),
			img_view::ImgView::tr("Image file to open."));
	cmd_parser.process(QApplication::arguments());

	img_view::ImgView ImgView;
	ImgView.init();
	img_view::logInfoToFile("ImgView start up");

	ImgView.show();

	if (!cmd_parser.positionalArguments().isEmpty())
		ImgView.loadFile(cmd_parser.positionalArguments().constFirst());

	return app.exec();
}
