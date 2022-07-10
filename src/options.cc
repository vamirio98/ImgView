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
	m_imageBgColor = new char[7];
	strcpy(m_imageBgColor, "000000");  /* White. */
}

Options::~Options()
{
	delete[] m_imageBgColor;
}

bool Options::show() const
{
	return m_show;
}

bool Options::hasHistory() const
{
	return m_hasHistory;
}

bool Options::showLibrary() const
{
	return m_showLibrary;
}

bool Options::showPageList() const
{
	return m_showPageList;
}

bool Options::showHistory() const
{
	return m_showHistory;
}

bool Options::showInfo() const
{
	return m_showInfo;
}

bool Options::showAction() const
{
	return m_showAction;
}

bool Options::showSideBar() const
{
	return m_showSideBar;
}

ImageSize Options::imageSize() const
{
	return m_imageSize;
}

const char *Options::imageBgColor() const
{
	return m_imageBgColor;
}

bool Options::isFirstPage() const
{
	return m_isFirstPage;
}

bool Options::isLastPage() const
{
	return m_isLastPage;
}

bool Options::isFirstBook() const
{
	return m_isFirstBook;
}

bool Options::isLastBook() const
{
	return m_isLastBook;
}

bool Options::isFirstLocation() const
{
	return m_isFirstLocation;
}

bool Options::isLastLocation() const
{
	return m_isLastLocation;
}

PageNum Options::pageNum() const
{
	return m_pageNum;
}

ReadDirection Options::readDirection() const
{
	return m_readDirection;
}

bool Options::splitWidePage() const
{
	return m_splitWidePage;
}

bool Options::firstPageAlone() const
{
	return m_firstPageAlone;
}

bool Options::lastPageAlone() const
{
	return m_lastPageAlone;
}

Sort Options::sortPage() const
{
	return m_sortPage;
}

ListStyle Options::pageListStyle() const
{
	return m_pageListStyle;
}

Sort Options::sortBook() const
{
	return m_sortBook;
}

ListStyle Options::bookListStyle() const
{
	return m_bookListStyle;
}

bool Options::minImage() const
{
	return m_minImage;
}

bool Options::maxImage() const
{
	return m_maxImage;
}

bool Options::keepScale() const
{
	return m_keepScale;
}

void Options::setShow(const bool show)
{
	if (show != m_show) {
		m_show = show;
		emit showChanged();
	}
}

void Options::setHasHistory(const bool hasHistory)
{
	if (hasHistory != m_hasHistory) {
		m_hasHistory = hasHistory;
		emit hasHistoryChanged();
	}
}

void Options::setShowLibrary(const bool showLibrary)
{
	if (showLibrary != m_showLibrary) {
		m_showLibrary = showLibrary;
		emit showLibraryChanged();
	}
}

void Options::setShowPageList(const bool showPageList)
{
	if (showPageList != m_showPageList) {
		m_showPageList = showPageList;
		emit showPageListChanged();
	}
}

void Options::setShowHistory(const bool showHistory)
{
	if (showHistory != m_showHistory) {
		m_showHistory = showHistory;
		emit showHistoryChanged();
	}
}

void Options::setShowInfo(const bool showInfo)
{
	if (showInfo != m_showInfo) {
		m_showInfo = showInfo;
		emit showInfoChanged();
	}
}

void Options::setShowAction(const bool showAction)
{
	if (showAction != m_showAction) {
		m_showAction = showAction;
		emit showActionChanged();
	}
}

void Options::setShowSideBar(const bool showSideBar)
{
	if (showSideBar != m_showSideBar) {
		m_showSideBar = showSideBar;
		emit showSideBarChanged();
	}
}

void Options::setImageSize(const ImageSize &imageSize)
{
	if (imageSize != m_imageSize) {
		m_imageSize = imageSize;
		emit imageSizeChanged();
	}
}

void Options::setImageBgColor(const char *imageBgColor)
{
	if (strcmp(imageBgColor, m_imageBgColor)) {
		strcpy(m_imageBgColor, imageBgColor);
		emit imageBgColorChanged();
	}
}

void Options::setIsFirstPage(const bool isFirstPage)
{
	if (isFirstPage != m_isFirstPage) {
		m_isFirstPage = isFirstPage;
		emit isFirstPageChanged();
	}
}

void Options::setIsLastPage(const bool isLastPage)
{
	if (isLastPage != m_isLastPage) {
		m_isLastPage = isLastPage;
		emit isLastPageChanged();
	}
}

void Options::setIsFirstBook(const bool isFirstBook)
{
	if (isFirstBook != m_isFirstBook) {
		m_isFirstBook = isFirstBook;
		emit isFirstBookChanged();
	}
}

void Options::setIsLastBook(const bool isLastBook)
{
	if (isLastBook != m_isLastBook) {
		m_isLastBook = isLastBook;
		emit isLastBookChanged();
	}
}

void Options::setIsFirstLocation(const bool isFirstLocation)
{
	if (isFirstLocation != m_isFirstLocation) {
		m_isFirstLocation = isFirstLocation;
		emit isFirstLocationChanged();
	}
}

void Options::setIsLastLocation(const bool isLastLocation)
{
	if (isLastLocation != m_isLastLocation) {
		m_isLastLocation = isLastLocation;
		emit isLastLocationChanged();
	}
}

void Options::setPageNum(const PageNum &pageNum)
{
	if (pageNum != m_pageNum) {
		m_pageNum = pageNum;
		emit pageNumChanged();
	}
}

void Options::setReadDirection(const ReadDirection &readDirection)
{
	if (readDirection != m_readDirection) {
		m_readDirection = readDirection;
		emit readDirectionChanged();
	}
}

void Options::setSplitWidePage(const bool splitWidePage)
{
	if (splitWidePage != m_splitWidePage) {
		m_splitWidePage = splitWidePage;
		emit splitWidePageChanged();
	}
}

void Options::setFirstPageAlone(const bool firstPageAlone)
{
	if (firstPageAlone != m_firstPageAlone) {
		m_firstPageAlone = firstPageAlone;
		emit firstPageAloneChanged();
	}
}

void Options::setLastPageAlone(const bool lastPageAlone)
{
	if (lastPageAlone != m_lastPageAlone) {
		m_lastPageAlone = lastPageAlone;
		emit lastPageAloneChanged();
	}
}

void Options::setSortPage(const Sort &sortPage)
{
	if (sortPage != m_sortPage) {
		m_sortPage = sortPage;
		emit sortPageChanged();
	}
}

void Options::setPageListStyle(const ListStyle &pageListStyle)
{
	if (pageListStyle != m_pageListStyle) {
		m_pageListStyle = pageListStyle;
		emit pageListStyleChanged();
	}
}

void Options::setSortBook(const Sort &sortBook)
{
	if (sortBook != m_sortBook) {
		m_sortBook = sortBook;
		emit sortBookChanged();
	}
}

void Options::setBookListStyle(const ListStyle &bookListStyle)
{
	if (bookListStyle != m_bookListStyle) {
		m_bookListStyle = bookListStyle;
		emit bookListStyleChanged();
	}
}

void Options::setMinImage(const bool minImage)
{
	if (minImage != m_minImage) {
		m_minImage = minImage;
		emit minImageChanged();
	}
}

void Options::setMaxImage(const bool maxImage)
{
	if (maxImage != m_maxImage) {
		m_maxImage = maxImage;
		emit maxImageChanged();
	}
}

void Options::setKeepScale(const bool keepScale)
{
	if (keepScale != m_keepScale) {
		m_keepScale = keepScale;
		emit keepScaleChanged();
	}
}

} /* img_view */
