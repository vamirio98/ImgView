/**
 * logger.h
 *
 * Created by vamirio on 2022 May 04
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QReadWriteLock>
#include <QString>
#include <QFile>

/* Max length of the message logged to file or screen. */
#define MAX_MSG_LEN  8192

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
	Screen = 0x2,
};
Q_DECLARE_FLAGS(LogPosSet, LogPos)
Q_DECLARE_OPERATORS_FOR_FLAGS(LogPosSet)

struct Log {
	int id;
	LogLevel level;
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
	void addLog(const char *const msg, const LogLevel &level = LogLevel::Info,
			const LogPosSet &pos = LogPos::None);
	QString convertLogToString(const Log &log);
	QString getLogLevelStr(const LogLevel &level);

private:
	Logger();
	~Logger();
	void printToFile(const QString &msg);
	void printToScreen(const QString &msg);

private:
	static Logger *m_instance;
	static QFile m_logFile;
	static LogLevelSet m_logFilter;
	mutable QReadWriteLock m_lock;
	int m_id = 0;
};

}  /* namespace img_view::log */

/* Helper functions. */
void logDebug(const char *format, ...);
void logDebugToFile(const char *format, ...);
void logInfo(const char *format, ...);
void logInfoToFile(const char *format, ...);
void logWarn(const char *format, ...);
void logWarnToFile(const char *format, ...);
void logError(const char *format, ...);
void logErrorToFile(const char *format, ...);
void logFatal(const char *format, ...);

}  /* namespace img_view */

#endif /* ifndef LOGGER_H */
