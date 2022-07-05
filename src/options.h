/**
 * options.h
 *
 * Created by vamirio on 2022 Jun 27
 */
#ifndef OPTIONS_H
#define OPTIONS_H

namespace img_view
{

/*! \enum ImageScaleOption
 *
 * Control how image scale.
 */
enum class ImageScaleOption {
	OriginSize = 0,
	FitToWindow,
	FitHeightToWindow,
	FitWidthToWindow,
};

/*! \enum PageNum
 *
 *  Control how many pages shown in landscape.
 */
enum class PageNum {
	OnePage = 1,
	TwoPage
};

/*! \enum ReadDirection
 *
 *  Read direction.
 */
enum class ReadDirection {
	RightToLeft = 0,
	LeftToRight
};

/*! \enum SortOption
 *
 *  How to sort.
 */
enum class SortOption {
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
enum class ListStyle {
	List = 0,
	Content,
	Thumbnail
};

struct Options {
	/* Component visibility. */
	bool showLibrary = false;
	bool showPageList = false;
	bool showHistory = false;
	bool showInfo = false;
	bool showAction = false;
	bool showSideBar = true;

	ImageScaleOption imageScaleOption = ImageScaleOption::OriginSize;
	/* Image background color. */
	char *imageBgColor = nullptr;

	/* Actions' possibility. */
	bool hasPrevPage = false;
	bool hasNextPage = false;
	bool hasFirstPage = false;
	bool hasLastPage = false;
	bool hasPrevBook = false;
	bool hasNextBook = false;
	bool hasPrevLoc = false;
	bool hasNextLoc = false;

	/* Pages options. */
	PageNum pageNum = PageNum::OnePage;
	ReadDirection readDirection = ReadDirection::RightToLeft;
	bool splitWidePage = false;
	bool firstPageAlone = false;
	bool lastPageAlone = false;
	SortOption sortPageBy = SortOption::NameAscending;
	ListStyle pageListStyle = ListStyle::List;

	/* Books options. */
	SortOption sortBookBy = SortOption::NameAscending;
	ListStyle bookListStyle = ListStyle::List;

	/* Scale option. */
	bool canZoomIn = true;
	bool canZoomOut = true;
	bool keepScale = false;

	Options();
	~Options();
};

} /* img_view */

#endif /* ifndef OPTIONS_H */
