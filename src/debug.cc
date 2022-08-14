/**
 * debug.cc
 *
 * Created by vamirio on 2022 May 04
 */
#include "debug.h"

#include <vector>
#include <cstdio>
#include <cstdarg>

#include <QDateTime>

namespace img_view {

Debug::Debug(const LogLv& lv) : _lv(lv), _valid(true)
{
	if ((Logger::instance()->lv() & _lv)
			|| (Logger::instance()->fileLv() & _lv)) {
		_ts.setString(&_msg);
		_ts << QDateTime::fromMSecsSinceEpoch(
				QDateTime::currentMSecsSinceEpoch()
				).toString("MM/dd hh:mm:ss.zzz")
			<< " - " << Logger::instance()->logLvToStr((_lv)) << " - ";
	}
}

Debug::~Debug()
{
	if (_valid) {
		if (Logger::instance()->lv() & _lv)
			std::cout << _msg.toUtf8().data() << std::endl;
		if (Logger::instance()->fileLv() & _lv) {
			QWriteLocker locker(&(Logger::instance()->locker()));
			Logger::instance()->appendToFile(_msg);
			locker.unlock();
		}
	}
}

Debug::Debug(Debug&& rhs) : _lv(rhs._lv), _valid(true),
	_msg(std::move(rhs._msg))
{
	_ts.setString(&_msg);
	rhs._valid = false;
}

auto Debug::addMsg() -> Debug&&
{
	return std::move(*this);
}

void Debug::addMsg(const char* format, ...)
{
	if (!_msg.isEmpty()) {
		va_list args;
		va_start(args, format);
		char* msgBuf = new char[MAX_MSG_LEN];
		vsnprintf(msgBuf, MAX_MSG_LEN, format, args);
		_ts << msgBuf;
		delete[] msgBuf;
	}
}

}  /* img_view */
