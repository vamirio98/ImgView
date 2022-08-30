/**
 * options.h
 *
 * Created by vamirio on 2022 Jun 27
 */
#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>

namespace img_view
{

	/*! \enum ImageSize
	 *
	 * Control how image scale.
	 */
	enum class ImageSize {
		OriginSize = 0,
		FitToWindow,
		FitHeightToWindow,
		FitWidthToWindow,
	};

	/*! \enum PageNum
	 *
	 *  Control how many pages shown in landscape.
	 */
	enum class PageNum { OnePage = 1, TwoPage };

	/*! \enum ReadDirection
	 *
	 *  Read direction.
	 */
	enum class ReadDirection { RightToLeft = 0, LeftToRight };

	/*! \enum Sort
	 *
	 *  How to sort.
	 */
	enum class Sort {
		NameAscending = 0,
		NameDescending,
		DateAscending,
		DateDescending,
		SizeAscending,
		SizeDescending,
		Shuffle
	};

	/*! \enum ListStyle
	 *
	 *  How to display list.
	 */
	enum class ListStyle { List = 0, Content, Thumbnail };

	class Options : public QObject
	{

		Q_OBJECT

	  public:
		Options();
		~Options();

	  public:
		bool show() const;
		bool hasHistory() const;
		bool showLibrary() const;
		bool showPageList() const;
		bool showHistory() const;
		bool showInfo() const;
		bool showAction() const;
		bool showSideBar() const;
		ImageSize imageSize() const;
		const char* imageBgColor() const;
		bool isFirstPage() const;
		bool isLastPage() const;
		bool isFirstBook() const;
		bool isLastBook() const;
		bool isFirstLoc() const;
		bool isLastLoc() const;
		PageNum pageNum() const;
		ReadDirection readDirection() const;
		bool splitWidePage() const;
		bool firstPageAlone() const;
		bool lastPageAlone() const;
		Sort sortPage() const;
		ListStyle pageListStyle() const;
		Sort sortBook() const;
		ListStyle bookListStyle() const;
		bool minImage() const;
		bool maxImage() const;
		bool keepScale() const;

		void setShow(const bool show);
		void setHasHistory(const bool hasHistory);
		void setShowLibrary(const bool showLibrary);
		void setShowPageList(const bool showPageList);
		void setShowHistory(const bool showHistory);
		void setShowInfo(const bool showInfo);
		void setShowAction(const bool showAction);
		void setShowSideBar(const bool showSideBar);
		void setImageSize(const ImageSize& imageSize);
		void setImageBgColor(const char* imageBgColor);
		void setIsFirstPage(const bool isFirstPage);
		void setIsLastPage(const bool isLastPage);
		void setIsFirstBook(const bool isFirstBook);
		void setIsLastBook(const bool isLastBook);
		void setIsFirstLoc(const bool isFirstLocation);
		void setIsLastLoc(const bool isLastLocation);
		void setPageNum(const PageNum& pageNum);
		void setReadDirection(const ReadDirection& readDirection);
		void setSplitWidePage(const bool splitWidePage);
		void setFirstPageAlone(const bool firstPageAlone);
		void setLastPageAlone(const bool lastPageAlone);
		void setSortPage(const Sort& sortPage);
		void setPageListStyle(const ListStyle& pageListStyle);
		void setSortBook(const Sort& sortBook);
		void setBookListStyle(const ListStyle& bookListStyle);
		void setMinImage(const bool minImage);
		void setMaxImage(const bool maxImage);
		void setKeepScale(const bool keepScale);

	  signals:
		void showChanged();
		void hasHistoryChanged();
		void showLibraryChanged();
		void showPageListChanged();
		void showHistoryChanged();
		void showInfoChanged();
		void showActionChanged();
		void showSideBarChanged();
		void imageSizeChanged();
		void imageBgColorChanged();
		void isFirstPageChanged();
		void isLastPageChanged();
		void isFirstBookChanged();
		void isLastBookChanged();
		void isFirstLocChanged();
		void isLastLocChanged();
		void pageNumChanged();
		void readDirectionChanged();
		void splitWidePageChanged();
		void firstPageAloneChanged();
		void lastPageAloneChanged();
		void sortPageChanged();
		void pageListStyleChanged();
		void sortBookChanged();
		void bookListStyleChanged();
		void minImageChanged();
		void maxImageChanged();
		void keepScaleChanged();

	  private:
		bool _show = false; /* Show image or not. */
		bool _hasHistory = false;

		/* Component visibility. */
		bool _showLibrary = false;
		bool _showPageList = false;
		bool _showHistory = false;
		bool _showInfo = false;
		bool _showAction = false;
		bool _showSideBar = false;

		ImageSize _imageSize = ImageSize::OriginSize;
		char* _imageBgColor = nullptr; /* Image background color. */

		bool _isFirstPage = true;
		bool _isLastPage = true;
		bool _isFirstBook = true;
		bool _isLastBook = true;
		bool _isFirstLoc = true;
		bool _isLastLoc = true;

		/* Pages options. */
		PageNum _pageNum = PageNum::OnePage;
		ReadDirection _readDirection = ReadDirection::RightToLeft;
		bool _splitWidePage = false;
		bool _firstPageAlone = false;
		bool _lastPageAlone = false;
		Sort _sortPage = Sort::NameAscending;
		ListStyle _pageListStyle = ListStyle::List;

		/* Books options. */
		Sort _sortBook = Sort::NameAscending;
		ListStyle _bookListStyle = ListStyle::List;

		/* Scale option. */
		bool _minImage = true;
		bool _maxImage = true;
		bool _keepScale = false;
	};

	extern Options gOpt;

} // namespace img_view

#endif /* ifndef OPTIONS_H */
