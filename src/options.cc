/**
 * options.cc
 *
 * Created by vamirio on 2022 Jun 27
 */
#include "options.h"

#include <cstring>

#include <QApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <qjsondocument.h>
#include <qjsonobject.h>

#include "debug.h"
#include "lru_cache.h"

namespace img_view
{

Options gOpt;

Options::Options() : _recentBooks(10)
{
	_imageBgColor = new char[7];
	strcpy(_imageBgColor, "000000"); /* White. */
}

Options::~Options() { delete[] _imageBgColor; }

bool Options::load()
{
	QFile configFile(QApplication::applicationDirPath() + "/ImgView.ini");
	if (!configFile.open(QIODevice::ReadOnly)) {
		gWarn() << "cannot open config file ImgView.ini.";
		return false;
	}

	QByteArray data = configFile.readAll();
	QJsonObject json(QJsonDocument::fromJson(data).object());
	if (json.contains("recentBooks") && json["recentBooks"].isArray()) {
		QJsonArray books = json["recentBooks"].toArray();
		for (int i = books.size() - 1; i >= 0; --i) {
			QString bookAndImage = books[i].toString();
			int delimiterPos = bookAndImage.indexOf('|');
			_recentBooks.put(
			    bookAndImage.first(delimiterPos),
			    bookAndImage.last(bookAndImage.size() - delimiterPos - 1)
			);
		}
		setHasHistory(true);
	}

	return true;
}

bool Options::save() const
{
	// Recent books.
	QJsonArray recentBooks;
	for (auto book = _recentBooks.data().crbegin();
	     book != _recentBooks.data().crend(); ++book)
		recentBooks.push_back(book->first + "|" + book->second);
	QJsonObject data;
	data["recentBooks"] = recentBooks;

	QFile configFile(QApplication::applicationDirPath() + "/ImgView.ini");
	if (!configFile.open(QIODevice::WriteOnly)) {
		gError() << "cannot open config file ImgView.ini.";
		return false;
	}
	configFile.write(QJsonDocument(data).toJson());
	return true;
}

bool Options::show() const { return _show; }

bool Options::hasHistory() const { return _hasHistory; }

LruCache<QString, QString>& Options::recentBooks() { return _recentBooks; }

bool Options::showLibrary() const { return _showLibrary; }

bool Options::showPageList() const { return _showPageList; }

bool Options::showHistory() const { return _showHistory; }

bool Options::showInfo() const { return _showInfo; }

bool Options::showAction() const { return _showAction; }

bool Options::showSideBar() const { return _showSideBar; }

ImageSize Options::imageSize() const { return _imageSize; }

const char* Options::imageBgColor() const { return _imageBgColor; }

bool Options::isFirstPage() const { return _isFirstPage; }

bool Options::isLastPage() const { return _isLastPage; }

bool Options::isFirstBook() const { return _isFirstBook; }

bool Options::isLastBook() const { return _isLastBook; }

bool Options::isFirstLoc() const { return _isFirstLoc; }

bool Options::isLastLoc() const { return _isLastLoc; }

PageNum Options::pageNum() const { return _pageNum; }

ReadDirection Options::readDirection() const { return _readDirection; }

bool Options::splitWidePage() const { return _splitWidePage; }

bool Options::firstPageAlone() const { return _firstPageAlone; }

bool Options::lastPageAlone() const { return _lastPageAlone; }

Sort Options::sortPage() const { return _sortPage; }

ListStyle Options::pageListStyle() const { return _pageListStyle; }

Sort Options::sortBook() const { return _sortBook; }

ListStyle Options::bookListStyle() const { return _bookListStyle; }

bool Options::minImage() const { return _minImage; }

bool Options::maxImage() const { return _maxImage; }

bool Options::keepScale() const { return _keepScale; }

void Options::setShow(const bool show)
{
	if (show != _show) {
		_show = show;
		emit showChanged();
	}
}

void Options::setHasHistory(const bool hasHistory)
{
	if (hasHistory != _hasHistory) {
		_hasHistory = hasHistory;
		emit hasHistoryChanged();
	}
}

void Options::setShowLibrary(const bool showLibrary)
{
	if (showLibrary != _showLibrary) {
		_showLibrary = showLibrary;
		emit showLibraryChanged();
	}
}

void Options::setShowPageList(const bool showPageList)
{
	if (showPageList != _showPageList) {
		_showPageList = showPageList;
		emit showPageListChanged();
	}
}

void Options::setShowHistory(const bool showHistory)
{
	if (showHistory != _showHistory) {
		_showHistory = showHistory;
		emit showHistoryChanged();
	}
}

void Options::setShowInfo(const bool showInfo)
{
	if (showInfo != _showInfo) {
		_showInfo = showInfo;
		emit showInfoChanged();
	}
}

void Options::setShowAction(const bool showAction)
{
	if (showAction != _showAction) {
		_showAction = showAction;
		emit showActionChanged();
	}
}

void Options::setShowSideBar(const bool showSideBar)
{
	if (showSideBar != _showSideBar) {
		_showSideBar = showSideBar;
		emit showSideBarChanged();
	}
}

void Options::setImageSize(const ImageSize& imageSize)
{
	if (imageSize != _imageSize) {
		_imageSize = imageSize;
		emit imageSizeChanged();
	}
}

void Options::setImageBgColor(const char* imageBgColor)
{
	if (strcmp(imageBgColor, _imageBgColor)) {
		strcpy(_imageBgColor, imageBgColor);
		emit imageBgColorChanged();
	}
}

void Options::setIsFirstPage(const bool isFirstPage)
{
	if (isFirstPage != _isFirstPage) {
		_isFirstPage = isFirstPage;
		emit isFirstPageChanged();
	}
}

void Options::setIsLastPage(const bool isLastPage)
{
	if (isLastPage != _isLastPage) {
		_isLastPage = isLastPage;
		emit isLastPageChanged();
	}
}

void Options::setIsFirstBook(const bool isFirstBook)
{
	if (isFirstBook != _isFirstBook) {
		_isFirstBook = isFirstBook;
		emit isFirstBookChanged();
	}
}

void Options::setIsLastBook(const bool isLastBook)
{
	if (isLastBook != _isLastBook) {
		_isLastBook = isLastBook;
		emit isLastBookChanged();
	}
}

void Options::setIsFirstLoc(const bool isFirstLocation)
{
	if (isFirstLocation != _isFirstLoc) {
		_isFirstLoc = isFirstLocation;
		emit isFirstLocChanged();
	}
}

void Options::setIsLastLoc(const bool isLastLocation)
{
	if (isLastLocation != _isLastLoc) {
		_isLastLoc = isLastLocation;
		emit isLastLocChanged();
	}
}

void Options::setPageNum(const PageNum& pageNum)
{
	if (pageNum != _pageNum) {
		_pageNum = pageNum;
		emit pageNumChanged();
	}
}

void Options::setReadDirection(const ReadDirection& readDirection)
{
	if (readDirection != _readDirection) {
		_readDirection = readDirection;
		emit readDirectionChanged();
	}
}

void Options::setSplitWidePage(const bool splitWidePage)
{
	if (splitWidePage != _splitWidePage) {
		_splitWidePage = splitWidePage;
		emit splitWidePageChanged();
	}
}

void Options::setFirstPageAlone(const bool firstPageAlone)
{
	if (firstPageAlone != _firstPageAlone) {
		_firstPageAlone = firstPageAlone;
		emit firstPageAloneChanged();
	}
}

void Options::setLastPageAlone(const bool lastPageAlone)
{
	if (lastPageAlone != _lastPageAlone) {
		_lastPageAlone = lastPageAlone;
		emit lastPageAloneChanged();
	}
}

void Options::setSortPage(const Sort& sortPage)
{
	if (sortPage != _sortPage) {
		_sortPage = sortPage;
		emit sortPageChanged();
	}
}

void Options::setPageListStyle(const ListStyle& pageListStyle)
{
	if (pageListStyle != _pageListStyle) {
		_pageListStyle = pageListStyle;
		emit pageListStyleChanged();
	}
}

void Options::setSortBook(const Sort& sortBook)
{
	if (sortBook != _sortBook) {
		_sortBook = sortBook;
		emit sortBookChanged();
	}
}

void Options::setBookListStyle(const ListStyle& bookListStyle)
{
	if (bookListStyle != _bookListStyle) {
		_bookListStyle = bookListStyle;
		emit bookListStyleChanged();
	}
}

void Options::setMinImage(const bool minImage)
{
	if (minImage != _minImage) {
		_minImage = minImage;
		emit minImageChanged();
	}
}

void Options::setMaxImage(const bool maxImage)
{
	if (maxImage != _maxImage) {
		_maxImage = maxImage;
		emit maxImageChanged();
	}
}

void Options::setKeepScale(const bool keepScale)
{
	if (keepScale != _keepScale) {
		_keepScale = keepScale;
		emit keepScaleChanged();
	}
}

} // namespace img_view
