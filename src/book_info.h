/**
 * book_info.h
 *
 * Created by vamirio on 2022 Jul 18
 */
#ifndef BOOK_INFO_H
#define BOOK_INFO_H

#include <QString>

namespace img_view
{

class BookInfo
{
  public:
	BookInfo() = default;
	~BookInfo() = default;

	/**
	 * @brief Browse a book and get its information.
	 *
	 * @param book The directory path to browse
	 *
	 * @return True when succeeded, false when the path is not exist, is not
	 * a directory, or you don't have the permission to open it
	 */
	bool browse(const QString& book);

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
	QByteArray _absPath;
	QByteArray _bookname;
	/* The cover file is the first image file in the book (name ascending).
	 */
	QByteArray _coverFilepath;
	QByteArray _coverFilename;
	qint64 _lastModified = 0;
};

} // namespace img_view

#endif /* BOOK_INFO_H */
