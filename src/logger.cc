/**
 * logger.cc
 *
 * Created by vamirio on 2022 May 04
 */
#include "logger.h"

#include <vector>

#include <QDateTime>
#include <QApplication>
#include <QTextStream>

#include <cstdio>
#include <cstdarg>

namespace img_view {

namespace log {

Logger *Logger::m_instance = nullptr;
QFile Logger::m_logFile = QFile();
LogLevelSet Logger::m_logFilter = Off;

Logger::Logger()
{
}

Logger::~Logger()
{
	freeInstance();
}

Logger *Logger::instance()
{
	return m_instance;
}

void Logger::initInstance()
{
	if (m_instance)
		return;

	m_instance = new Logger();

	QString log_filename = QApplication::applicationDirPath() + "/log.txt";
	m_logFile.setFileName(log_filename);
	/* Remove the old log. */
	if (m_logFile.exists())
		m_logFile.remove();
}

void Logger::freeInstance()
{
	if (m_instance) {
		delete m_instance;
		m_instance = nullptr;
	}
}

void Logger::setLogFilter(const LogLevel &level)
{
	std::vector<LogLevel> all_level{ Debug, Info, Warn, Error, Fatal };
	if (level == All) {
		m_logFilter = All;
	} else {
		for (const LogLevel &lv: all_level)
			m_logFilter |= lv >= level ? lv : Off;
	}
}

void Logger::addLog(const char *const msg, const LogLevel &level,
		const LogPosSet &pos)
{
	if (!(m_logFilter & level))
		return;

	QWriteLocker locker(&m_lock);

	const Log log = { m_id++, level, QDateTime::currentMSecsSinceEpoch(), msg };
	QString log_msg = convertLogToString(log);
	if (pos & LogPos::File)
		printToFile(log_msg);
	if (pos & LogPos::Screen)
		printToScreen(log_msg);

	locker.unlock();
}

void Logger::addLog(const QString &msg, const LogLevel &level,
		const LogPosSet &pos)
{
	addLog(msg.toUtf8().constData(), level, pos);
}

QString Logger::convertLogToString(const Log &log)
{
	return QDateTime::fromMSecsSinceEpoch(log.timestamp)
		.toString("yyyy/MM/dd hh:mm:ss.zzz")
		+ " - "
		+ getLogLevelStr(log.level)
		+ " - "
		+ log.message;
}

QString Logger::getLogLevelStr(const LogLevel &level)
{
	switch (level) {
	case LogLevel::Debug :
		return "DEBUG";
	case LogLevel::Info :
		return "INFO";
	case LogLevel::Warn :
		return "WARN";
	case LogLevel::Error :
		return "ERROR";
	case LogLevel::Fatal :
		return "FATAL";
	default:
		return "";
	}
	return "";
}

void Logger::printToFile(const QString &msg)
{
	m_logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
	QTextStream fs(&m_logFile);
	fs << msg << "\n";
	m_logFile.close();
}

void Logger::printToScreen(const QString &msg)
{
	printf("%s\n", msg.toUtf8().constData());
	fflush(stdout);
}

}  /* namespace log */

void logDebug(const char *format, ...)
{
	char *buf = new char[MAX_MSG_LEN];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, MAX_MSG_LEN, format, args);
	log::Logger::instance()->addLog(buf, log::LogLevel::Debug,
			log::LogPos::Screen);
	delete[] buf;
}

void logDebugToFile(const char *format, ...)
{
	char *buf = new char[MAX_MSG_LEN];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, MAX_MSG_LEN, format, args);
	log::Logger::instance()->addLog(buf, log::LogLevel::Debug,
			log::LogPos::Screen | log::LogPos::File);
	delete[] buf;
}

void logInfo(const char *format, ...)
{
	char *buf = new char[MAX_MSG_LEN];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, MAX_MSG_LEN, format, args);
	log::Logger::instance()->addLog(buf, log::LogLevel::Info,
			log::LogPos::Screen);
	delete[] buf;
}

void logInfoToFile(const char *format, ...)
{
	char *buf = new char[MAX_MSG_LEN];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, MAX_MSG_LEN, format, args);
	log::Logger::instance()->addLog(buf, log::LogLevel::Info,
			log::LogPos::Screen | log::LogPos::File);
	delete[] buf;
}

void logWarn(const char *format, ...)
{
	char *buf = new char[MAX_MSG_LEN];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, MAX_MSG_LEN, format, args);
	log::Logger::instance()->addLog(buf, log::LogLevel::Warn,
			log::LogPos::Screen);
	delete[] buf;
}

void logWarnToFile(const char *format, ...)
{
	char *buf = new char[MAX_MSG_LEN];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, MAX_MSG_LEN, format, args);
	log::Logger::instance()->addLog(buf, log::LogLevel::Warn,
			log::LogPos::Screen | log::LogPos::File);
	delete[] buf;
}

void logError(const char *format, ...)
{
	char *buf = new char[MAX_MSG_LEN];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, MAX_MSG_LEN, format, args);
	log::Logger::instance()->addLog(buf, log::LogLevel::Error,
			log::LogPos::Screen);
	delete[] buf;
}

void logErrorToFile(const char *format, ...)
{
	char *buf = new char[MAX_MSG_LEN];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, MAX_MSG_LEN, format, args);
	log::Logger::instance()->addLog(buf, log::LogLevel::Error,
			log::LogPos::Screen | log::LogPos::File);
	delete[] buf;
}

void logFatal(const char *format, ...)
{
	char *buf = new char[MAX_MSG_LEN];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, MAX_MSG_LEN, format, args);
	log::Logger::instance()->addLog(buf, log::LogLevel::Fatal,
			log::LogPos::Screen);
	delete[] buf;
}

}  /* namespace img_view */
