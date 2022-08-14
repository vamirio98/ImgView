/**
 * debug.h
 *
 * Created by vamirio on 2022 May 04
 */
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#include <QObject>
#include <QReadWriteLock>
#include <QString>
#include <QFile>
#include <QTextStream>

#include "logger.h"

/* Max length of the message logged to file or screen. */
#define MAX_MSG_LEN  8192

#define gDebug img_view::Debug(img_view::LogLv::Debug).addMsg
#define gInfo img_view::Debug(img_view::LogLv::Info).addMsg
#define gWarn img_view::Debug(img_view::LogLv::Warn).addMsg
#define gError img_view::Debug(img_view::LogLv::Error).addMsg
#define gFatal img_view::Debug(img_view::LogLv::Fatal).addMsg

namespace img_view {

class Debug {
	template<typename T>
	friend Debug operator<<(Debug&& os, const T& x);

public:
	Debug(const LogLv& lv);
	~Debug();
	Debug(const Debug& rhs) = delete;
	Debug& operator=(const Debug& rhs) = delete;
	Debug(Debug&& rhs);
	Debug&& operator=(const Debug&& rhs) = delete;

	Debug&& addMsg();
	void addMsg(const char* format, ...);

private:
	LogLv _lv;
	QString _msg;
	QTextStream _ts;
	bool _valid;
}; /* Debug */

template <typename T>
class img_view::Debug operator<<(class img_view::Debug&& os, const T& x)
{
	if (!os._msg.isEmpty())
		os._ts << x << " ";
	return std::move(os);
}

}  /* img_view */

#endif /* ifndef DEBUG_H */
