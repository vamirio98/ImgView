/**
 * main.cc
 *
 * Created by vamirio on 2022 Apr 25
 */
#include <QApplication>
#include <QCommandLineParser>
#include <QImageReader>

#include "main_window.h"
#include "debug.h"
#include "logger.h"

using namespace img_view;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
//	QApplication::addLibraryPath(app.applicationDirPath() + "/plugins");

	Logger::initInstance();
	Logger::instance()->setLogLv(LogLv::Debug);
	Logger::instance()->setFileLogLv(LogLv::Info);

//	QCommandLineParser cmd_parser;
//	cmd_parser.addHelpOption();
//	cmd_parser.addPositionalArgument(img_view::MainWindow::tr("[file]"),
//			img_view::MainWindow::tr("Image file to open."));
//	cmd_parser.process(QApplication::arguments());

	MainWindow ImgView;
	ImgView.init();
	ImgView.show();

	gInfo() << "ImgView started up.";

//	if (!cmd_parser.positionalArguments().isEmpty())
//		ImgView.loadFile(cmd_parser.positionalArguments().constFirst());

	return app.exec();
}
