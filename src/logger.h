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

#define D(s) \
{ \
	if (img_view::Logger::logFilter() | img_view::LogLevel::Debug) { \
		printf("%-12s +%-4u %-20s : ", __FILE__, __LINE__, __FUNCTION__); \
		printf s; \
		fflush(stdout); \
   	} \
}

namespace img_view {

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

friend void logDebug(const char *format, ...);
friend void logDebugToFile(const char *format, ...);
friend void logInfo(const char *format, ...);
friend void logInfoToFile(const char *format, ...);
friend void logWarn(const char *format, ...);
friend void logWarnToFile(const char *format, ...);
friend void logError(const char *format, ...);
friend void logErrorToFile(const char *format, ...);
friend void logFatal(const char *format, ...);

public:
	/**
	 * @brief Initialize the Logger instance
	 */
	static void initInstance();

	/**
	 * @brief Free the Logger instance
	 */
	static void freeInstance();

	/**
	 * @brief Get the Logger instance
	 */
	static Logger *instance();

	/**
	 * @brief Set the log filter
	 */
	static void setLogFilter(const LogLevel &level);

	/**
	 * @brief Get the log filter
	 */
	static LogLevelSet logFilter();

	/**
	 * @brief Add a @level level log @msg to position @pos
	 */
	void addLog(const QString &msg, const LogLevel &level = LogLevel::Info,
			const LogPosSet &pos = LogPos::None);

	/**
	 * @brief Add a @level level log @msg to position @pos
	 */
	void addLog(const char *const msg, const LogLevel &level = LogLevel::Info,
			const LogPosSet &pos = LogPos::None);

	/**
	 * @brief Convert the Log @log to string, format: time - level - message
	 */
	QString logToStr(const Log &log);

	/**
	 * @brief Convert the LogLevel @level to string
	 */
	QString logLevelToStr(const LogLevel &level);

private:
	Logger();
	~Logger();

	/**
	 * @brief Print the message @msg to file
	 */
	void printToFile(const QString &msg);

	/**
	 * @brief Print the message @msg to screen(stdout)
	 */
	void printToScreen(const QString &msg);

private:
	static Logger *m_instance;       /* The logger instance. */
	static char *m_msgBuf;           /* For helper function. */
	static QFile m_logFile;          /* The log file. */
	static LogLevelSet m_logFilter;  /* The log filter. */
	mutable QReadWriteLock m_lock;   /* To avoid read and write races. */
	int m_id = 0;                    /* The log ID. */
};

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

}  /* img_view */

#endif /* ifndef LOGGER_H */
