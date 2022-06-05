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

Logger *Logger::m_instance = nullptr;
char *Logger::m_msgBuf = nullptr;
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
	m_msgBuf = new char[MAX_MSG_LEN];

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
		delete[] m_msgBuf;
		m_instance = nullptr;
		m_msgBuf = nullptr;
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

LogLevelSet Logger::logFilter()
{
	return m_logFilter;
}

void Logger::addLog(const char *const msg, const LogLevel &level,
		const LogPosSet &pos)
{
	if (!(m_logFilter & level))
		return;

	QWriteLocker locker(&m_lock);

	const Log log = { m_id++, level, QDateTime::currentMSecsSinceEpoch(), msg };
	QString log_msg = logToStr(log);
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

QString Logger::logToStr(const Log &log)
{
	return QDateTime::fromMSecsSinceEpoch(log.timestamp)
		.toString("yyyy/MM/dd hh:mm:ss.zzz")
		+ " - "
		+ logLevelToStr(log.level)
		+ " - "
		+ log.message;
}

QString Logger::logLevelToStr(const LogLevel &level)
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

void logDebug(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(Logger::m_msgBuf, MAX_MSG_LEN, format, args);
	Logger::instance()->addLog(Logger::m_msgBuf, LogLevel::Debug,
			LogPos::Screen);
}

void logDebugToFile(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(Logger::m_msgBuf, MAX_MSG_LEN, format, args);
	Logger::instance()->addLog(Logger::m_msgBuf, LogLevel::Debug,
			LogPos::Screen | LogPos::File);
}

void logInfo(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(Logger::m_msgBuf, MAX_MSG_LEN, format, args);
	Logger::instance()->addLog(Logger::m_msgBuf, LogLevel::Info,
			LogPos::Screen);
}

void logInfoToFile(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(Logger::m_msgBuf, MAX_MSG_LEN, format, args);
	Logger::instance()->addLog(Logger::m_msgBuf, LogLevel::Info,
			LogPos::Screen | LogPos::File);
}

void logWarn(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(Logger::m_msgBuf, MAX_MSG_LEN, format, args);
	Logger::instance()->addLog(Logger::m_msgBuf, LogLevel::Warn,
			LogPos::Screen);
}

void logWarnToFile(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(Logger::m_msgBuf, MAX_MSG_LEN, format, args);
	Logger::instance()->addLog(Logger::m_msgBuf, LogLevel::Warn,
			LogPos::Screen | LogPos::File);
}

void logError(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(Logger::m_msgBuf, MAX_MSG_LEN, format, args);
	Logger::instance()->addLog(Logger::m_msgBuf, LogLevel::Error,
			LogPos::Screen);
}

void logErrorToFile(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(Logger::m_msgBuf, MAX_MSG_LEN, format, args);
	Logger::instance()->addLog(Logger::m_msgBuf, LogLevel::Error,
			LogPos::Screen | LogPos::File);
}

void logFatal(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(Logger::m_msgBuf, MAX_MSG_LEN, format, args);
	Logger::instance()->addLog(Logger::m_msgBuf, LogLevel::Fatal,
			LogPos::Screen);
}

}  /* namespace img_view */
