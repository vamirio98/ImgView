/**
 * options.cc
 *
 * Created by vamirio on 2022 Jun 27
 */
#include "options.h"

#include <cstring>

namespace img_view
{

Options gOpt;

Options::Options()
{
	_imageBgColor = new char[7];
	strcpy(_imageBgColor, "000000");  /* White. */
}

Options::~Options()
{
	delete[] _imageBgColor;
}

bool Options::show() const
{
	return _show;
}

bool Options::hasHistory() const
{
	return _hasHistory;
}

bool Options::showLibrary() const
{
	return _showLibrary;
}

bool Options::showPageList() const
{
	return _showPageList;
}

bool Options::showHistory() const
{
	return _showHistory;
}

bool Options::showInfo() const
{
	return _showInfo;
}

bool Options::showAction() const
{
	return _showAction;
}

bool Options::showSideBar() const
{
	return _showSideBar;
}

ImageSize Options::imageSize() const
{
	return _imageSize;
}

const char* Options::imageBgColor() const
{
	return _imageBgColor;
}

bool Options::isFirstPage() const
{
	return _isFirstPage;
}

bool Options::isLastPage() const
{
	return _isLastPage;
}

bool Options::isFirstBook() const
{
	return _isFirstBook;
}

bool Options::isLastBook() const
{
	return _isLastBook;
}

bool Options::isFirstLocation() const
{
	return _isFirstLocation;
}

bool Options::isLastLocation() const
{
	return _isLastLocation;
}

PageNum Options::pageNum() const
{
	return _pageNum;
}

ReadDirection Options::readDirection() const
{
	return _readDirection;
}

bool Options::splitWidePage() const
{
	return _splitWidePage;
}

bool Options::firstPageAlone() const
{
	return _firstPageAlone;
}

bool Options::lastPageAlone() const
{
	return _lastPageAlone;
}

Sort Options::sortPage() const
{
	return _sortPage;
}

ListStyle Options::pageListStyle() const
{
	return _pageListStyle;
}

Sort Options::sortBook() const
{
	return _sortBook;
}

ListStyle Options::bookListStyle() const
{
	return _bookListStyle;
}

bool Options::minImage() const
{
	return _minImage;
}

bool Options::maxImage() const
{
	return _maxImage;
}

bool Options::keepScale() const
{
	return _keepScale;
}

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

void Options::setIsFirstLocation(const bool isFirstLocation)
{
	if (isFirstLocation != _isFirstLocation) {
		_isFirstLocation = isFirstLocation;
		emit isFirstLocationChanged();
	}
}

void Options::setIsLastLocation(const bool isLastLocation)
{
	if (isLastLocation != _isLastLocation) {
		_isLastLocation = isLastLocation;
		emit isLastLocationChanged();
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

} /* img_view */
