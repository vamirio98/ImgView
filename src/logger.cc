/**
 * logger.cc
 *
 * Created by vamirio on 2022 May 04
 * Last Modified: 2022 May 07 16:38:04
 */
#include "logger.h"

#include <vector>

#include <QDateTime>
#include <QApplication>
#include <QTextStream>

#include <cstdio>

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

void Logger::addLog(const QString &msg, const LogLevel &level,
		const LogPosSet &pos)
{
	if (!(m_logFilter & level))
		return;

	QWriteLocker locker(&m_lock);

	const Log log = { m_id++, level, QDateTime::currentMSecsSinceEpoch(), msg };
	QString log_msg =
		QDateTime::fromMSecsSinceEpoch(log.timestamp)
		.toString("yyyy/MM/dd hh:mm:ss.zzz")
		+ " - "
		+ log.message;
	if (pos & LogPos::File) {
		m_logFile.open(QIODevice::ReadWrite | QIODevice::Text
				| QIODevice::Append);
		QTextStream fs(&m_logFile);
		fs << log_msg << "\n";
		m_logFile.close();
	}
	if (pos & LogPos::Stdout)
		fprintf(stdout, "%s\n", log_msg.toUtf8().constData());
	if (pos & LogPos::Stderr)
		fprintf(stderr, "%s\n", log_msg.toUtf8().constData());

	locker.unlock();
}

}  /* namespace log */

void logDebug(const QString &msg, const log::LogPosSet &pos)
{
	log::Logger::instance()->addLog("DEBUG - " + msg, log::LogLevel::Debug,
			pos | log::LogPos::Stdout);
}

void logInfo(const QString &msg, const log::LogPosSet &pos)
{
	log::Logger::instance()->addLog("INFO - " + msg, log::LogLevel::Info,
			pos | log::LogPos::Stdout);
}

void logWarn(const QString &msg, const log::LogPosSet &pos)
{
	log::Logger::instance()->addLog("WARN - " + msg, log::LogLevel::Warn,
			pos | log::LogPos::Stderr);
}

void logError(const QString &msg, const log::LogPosSet &pos)
{
	log::Logger::instance()->addLog("ERROR - " + msg, log::LogLevel::Debug,
			pos | log::LogPos::Stderr);
}

void logFatal(const QString &msg, const log::LogPosSet &pos)
{
	log::Logger::instance()->addLog("FATAL - " + msg, log::LogLevel::Debug,
			pos | log::LogPos::Stderr | log::LogPos::File);
}

}  /* namespace img_view */
