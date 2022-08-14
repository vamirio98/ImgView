/**
 * logger.h
 *
 * Created by vamirio on 2022 Aug 14
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QReadWriteLock>

namespace img_view
{

enum class LogLv {
	All = -1,
	Debug = 0x1,
	Info = 0x2,
	Warn = 0x4,
	Error = 0x8,
	Fatal = 0x10,
	Off = 0
};
Q_DECLARE_FLAGS(LogLvSet, LogLv)
Q_DECLARE_OPERATORS_FOR_FLAGS(LogLvSet)

class Logger {

public:
	static void initInstance();
	static Logger* instance();

	/**
	 * @brief Set the Log file.
	 */
	void setLogFile(const QString& filename);

	/**
	 * @brief Set the log level, only logs whose level above it will be
	 *        recorded.
	 */
	void setLogLv(const LogLv& lv);

	/**
	 * @brief Set the file log level, only logs whose level above it will be
	 *        recorded in file.
	 */
	void setFileLogLv(const LogLv& lv);

	/**
	 * @brief Get the log level.
	 */
	const LogLvSet& lv() const;

	/**
	 * @brief Get the file log level.
	 */
	const LogLvSet& fileLv() const;

	/**
	 * @brief Get the lock.
	 */
	QReadWriteLock& locker() const;

	/**
	 * @brief Convert the LogLevel @level to string
	 */
	QString logLvToStr(const LogLv& lv);

	/**
	 * @brief Append the message MSG to file
	 */
	void appendToFile(const QString& msg);

private:
	Logger();
	~Logger();

private:
	static Logger* _instance;
	static QFile _logFile;          /* The log file. */
	static LogLvSet _logLv;         /* The log level. */
	static LogLvSet _fileLv;        /* The file log level. */
	mutable QReadWriteLock _locker; /* To avoid read and write races. */
}; /* Logger */

}  /* img_view */

#endif /* ifndef LOGGER_H */
