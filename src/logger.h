/**
 * logger.h
 *
 * Created by vamirio on 2022 May 04
 * Last Modified: 2022 May 05 12:47:58
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QReadWriteLock>
#include <QString>
#include <QFile>

namespace img_view {

namespace log {

enum LogLevel {
	All = -1,
	Debug = 0x1,
	Info = 0x2,
	Warn = 0x4,
	Error = 0x8,
	Fatal = 0x10,
	Off = 0
};
Q_DECLARE_FLAGS(LogLevelSet, LogLevel)
Q_DECLARE_OPERATORS_FOR_FLAGS(LogLevelSet)

enum LogPos {
	None = 0,
	File = 0x1,
	Stdout = 0x2,
	Stderr = 0x4
};
Q_DECLARE_FLAGS(LogPosSet, LogPos)
Q_DECLARE_OPERATORS_FOR_FLAGS(LogPosSet)

struct Log {
	int id;
	LogLevel type;
	qint64 timestamp;
	QString message;
};

const int kMaxLogNum = 3000;

class Logger : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY_MOVE(Logger)

public:
	static void initInstance();
	static void freeInstance();
	static Logger *instance();
	static void setLogFilter(const LogLevel &level);
	void addLog(const QString &msg, const LogLevel &level = LogLevel::Info,
			const LogPosSet &pos = LogPos::None);

private:
	Logger();
	~Logger();

private:
	static Logger *m_instance;
	static QFile m_logFile;
	static LogLevelSet m_logFilter;
	mutable QReadWriteLock m_lock;
	int m_id = 0;
};

}  /* namespace img_view::log */

/* Helper functions. */
void logDebug(const QString &msg,
		const log::LogPosSet &pos = log::LogPos::None);
void logInfo(const QString &msg,
		const log::LogPosSet &pos = log::LogPos::None);
void logWarn(const QString &msg,
		const log::LogPosSet &pos = log::LogPos::None);
void logError(const QString &msg,
		const log::LogPosSet &pos = log::LogPos::None);
void logFatal(const QString &msg,
		const log::LogPosSet &pos = log::LogPos::None);

}  /* namespace img_view */

#endif /* ifndef LOGGER_H */
