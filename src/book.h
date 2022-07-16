/**
 * book.h
 *
 * Created by vamirio on 2022 May 13
 */
#ifndef BOOK_H
#define BOOK_H

#include <QList>
#include <QImage>

#include "image_info.h"

namespace img_view {

class BookInfo {
public:
	BookInfo() = default;
	~BookInfo() = default;

	/**
	 * @brief Browse a book and get its information.
	 *
	 * @param book The directory path to browse
	 *
	 * @return True when succeeded, false when the path is not exist, is not a
	 *         directory, or you don't have the permission to open it
	 */
	bool browse(const QString &book);

	/**
	 * @brief Check whether is no book has been browsed now.
	 *
	 * @return True when no book has been browsed
	 */
	bool empty() const;

	/**
	 * @brief Get the absolute path of the book.
	 */
	QString absPath() const;

	/**
	 * @brief Get the book basename.
	 */
	QString bookName() const;

	/**
	 * @brief Get the absolute path of the cover file, it is an empty string
	 *        when there are no image files in the current book.
	 */
	QString coverFilepath() const;

	/**
	 * @brief Get the cover file basename.
	 */
	QString coverFilename() const;

	/**
	 * @brief Get the last modified timestamp (milliseconds since
	 *        1970/01/01T00:00:00.000) of the book.
	 */
	qint64 lastModified() const;

private:
	QByteArray m_absPath;
	QByteArray m_bookname;
	/* The cover file is the first image file in the book (name ascending). */
	QByteArray m_coverFilepath;
	QByteArray m_coverFilename;
	qint64 m_lastModified = 0;
};

/*
 * A directory is considered as a book, all images in it are considered as
 * pages.
 */
class Book {
public:
	Book();
	~Book();

	/**
	 * @brief Open the specified book (directory).
	 *
	 * NOTE: remember to call close() before open a new book.
	 *
	 * @return True when success
	 */
	bool open(const QString &book);

	/**
	 * @brief Close book.
	 */
	void close();

	/**
	 * @brief Check if a book is empty (has no pages or hasn't open any books)
	 *
	 * @return True when a book is empty
	 */
	bool empty() const;

	/**
	 * @brief Get the book name, last part in path
	 */
	QString bookName() const;

	/**
	 * @brief Get the absolute path of the book
	 */
	QString absPath() const;

	/**
	 * @brief The cover file's basename.
	 */
	QString coverFilename() const;

	/**
	 * @brief The cover file's absolute path.
	 */
	QString coverFilepath() const;

	/**
	 * @brief The last modified timestamp of the book as the number of
	 *        milliseconds that have passed since 1970/01/01T00:00:00.000
	 */
	qint64 lastModified() const;

	/**
	 * @brief The information of the current page (image)
	 */
	const ImageInfo &curPage() const;

	/**
	 * @brief Set the current page (image file) to specified page
	 *
	 * @param pagename File basename of the page
	 *
	 * @return True when the page found in the book
	 */
	bool setCurPage(const QString &pagename);

	/**
	 * @brief Get the previous page's information.
	 *
	 * @return The previous page's information, if the current page is the
	 *         first page, return its information.
	 */
	const ImageInfo &prevPage() const;

	/**
	 * @brief Get the next page's information.
	 *
	 * @return The next page's information, if the current page is the last
	 *         page, return its information.
	 */
	const ImageInfo &nextPage() const;

	/**
	 * @brief Get at most NUM pages' information before the current page.
	 *
	 * @param num The number of pages to get.
	 *
	 * @return Information of the pages before the current page, if the current
	 *         page is the first page, return an empty list.
	 */
	QList<ImageInfo> prevPages(int num) const;

	/**
	 * @brief Get at most NUM pages' information after the current page.
	 *
	 * @param num The number of pages to get.
	 *
	 * @return Information of the pages after the current page, if the current
	 *         page is the last page, return an empty list.
	 */
	QList<ImageInfo> nextPages(int num) const;

	/**
	 * @brief Move to the previous page(image), if the current page is
	 *        the first page, do nothing
	 *
	 * If the book has no page, the function behave undefined
	 *
	 * @return The information of the previous page, or of the current page
	 *         if it is the first page
	 */
	const ImageInfo &toPrevPage();

	/**
	 * @brief Move to the next page(image), if the current page is the
	 *        last page, do nothing
	 *
	 * If the book has no page, the function behave undefined
	 *
	 * @return The information of the next page, or of the current page if
	 *         it is the last page
	 */
	const ImageInfo &toNextPage();

	/**
	 * @brief Move to the NUMth page, or the first page if NUM is negative and
	 *        the last page if NUM is larger than (the number of page - 1).
	 *
	 * @param num The page number.
	 *
	 * @return The information of the NUMth page.
	 */
	const ImageInfo &toPage(int num);

private:

private:
	BookInfo m_info;  /* This book's information. */
	/* Information of all pages inside this book. */
	QList<ImageInfo> m_pageList;
	int m_pageNum = -1;
};

}  /* img_view */

#endif /* ifndef BOOK_H */
