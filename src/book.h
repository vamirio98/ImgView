/**
 * book.h
 *
 * Created by vamirio on 2022 May 13
 */
#ifndef BOOK_H
#define BOOK_H

#include <QList>
#include <QImage>

#include "img_info.h"

namespace img_view {

struct BookInfo {
	QString m_path = "";
	QString m_coverFilepath = "";
	qint64 m_lastModified = 0;

	BookInfo() = default;
	BookInfo(const QString &path);
	~BookInfo() = default;
};

/*
 * A directory is considered as a book, all images in it are considered as
 * pages.
 */
class Book {
public:
	Book();
	Book(const QString &path);
	~Book();

	/**
	 * @brief Set Book as the specified directory
	 *
	 * @return True when success
	 */
	bool setBook(const QString &path);

	/**
	 * @brief Get the book name, last part in path
	 */
	QString bookName() const;

	/**
	 * @brief Get the absolute path of the book
	 */
	QString absPath() const;

	/**
	 * @brief The last modified timestamp of the book as the number of
	 *        milliseconds that have passed since 1970-01-01T00:00:00.000
	 */
	qint64 lastModified() const;

	/**
	 * @brief The cover of the book, the first image in the directory
	 */
	QString coverFilepath() const;

	/**
	 * @brief Check whether there is no page in the current book(directory)
	 *
	 * @return True when there is no page in the current book
	 */
	bool noPage() const;

	/**
	 * @brief The information of the current page(image)
	 */
	const ImgInfo &currPageInfo() const;

	/**
	 * @brief Get the current page(image) filepath
	 *
	 * @return The filepath of the current page
	 */
	QString currPageFilepath() const;

	/**
	 * @brief Set the current page(image file) to specified page
	 *
	 * @return True when the page found in the book
	 */
	bool setCurrPage(const QString &filepath);

	/**
	 * @brief Move to the previous page(image), if the current page is
	 *        the first page, do nothing
	 *
	 * If the book has no page, the function behave undefined
	 *
	 * @return The filepath of the previous page, or of the current page
	 *         if it is the first page
	 */
	QString toPrevPage();

	/**
	 * @brief Move to the next page(image), if the current page is the
	 *        last page, do nothing
	 *
	 * If the book has no page, the function behave undefined
	 *
	 * @return The filepath of the next page, or of the current page if
	 *         it is the last page
	 */
	QString toNextPage();

private:

private:
	BookInfo m_info;
	QList<BookInfo> m_books;
	QList<ImgInfo> m_pages;
	QList<ImgInfo>::iterator m_currPage;
};

}  /* namespace img_view */

#endif /* ifndef BOOK_H */
