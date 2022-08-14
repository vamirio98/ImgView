/**
 * logger.cc
 *
 * Created by vamirio on 2022 Aug 14
 */
#include "logger.h"

#include <QApplication>
#include <QTextStream>

namespace img_view
{

Logger* Logger::_instance = nullptr;
QFile Logger::_logFile = QFile();        /* The log file. */
LogLvSet Logger::_logLv = LogLv::Off;    /* The log level. */
LogLvSet Logger::_fileLv = LogLv::Off;   /* The file log level. */

Logger::Logger()
{
}

Logger::~Logger()
{
	if (_instance)
		delete _instance;
}

void Logger::initInstance()
{
	if (_instance)
		return;

	QString logFilename = QApplication::applicationDirPath() + "/log.txt";
	_logFile.setFileName(logFilename);
	/* Remove the old log. */
	if (_logFile.exists())
		_logFile.remove();
}

Logger* Logger::instance()
{
	return _instance;
}

void Logger::setLogFile(const QString& filename)
{
	_logFile.setFileName(filename);
	/* Remove the old log. */
	if (_logFile.exists())
		_logFile.remove();
}

void Logger::setLogLv(const LogLv& lv)
{
	std::vector<LogLv> allLevels{ LogLv::Debug, LogLv::Info, LogLv::Warn,
		LogLv::Error, LogLv::Fatal };
	if (lv == LogLv::All) {
		_logLv = LogLv::All;
	} else {
		for (const LogLv& level : allLevels)
			_logLv |= level >= lv ? level : LogLv::Off;
	}
}

void Logger::setFileLogLv(const LogLv& lv)
{
	std::vector<LogLv> allLevels{ LogLv::Debug, LogLv::Info, LogLv::Warn,
		LogLv::Error, LogLv::Fatal };
	if (lv == LogLv::All) {
		_fileLv = LogLv::All;
	} else {
		for (const LogLv& level : allLevels)
			_fileLv |= level >= lv ? level : LogLv::Off;
	}
}

const LogLvSet& Logger::lv() const
{
	return _logLv;
}

const LogLvSet& Logger::fileLv() const
{
	return _fileLv;
}

QReadWriteLock& Logger::locker() const
{
	return _locker;
}

QString Logger::logLvToStr(const LogLv& lv)
{
	switch (lv) {
	case LogLv::Debug :
		return "DEBUG";
	case LogLv::Info :
		return "INFO";
	case LogLv::Warn :
		return "WARN";
	case LogLv::Error :
		return "ERROR";
	case LogLv::Fatal :
		return "FATAL";
	default:
		return "";
	}
	return "";
}

void Logger::appendToFile(const QString& msg)
{
	_logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
	QTextStream fs(&_logFile);
	fs << msg << "\n";
	_logFile.close();
}

}  /* img_view */
