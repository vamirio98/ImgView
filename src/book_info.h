/**
 * book_info.h
 *
 * Created by vamirio on 2022 May 14
 */
#ifndef BOOK_INFO_H
#define BOOK_INFO_H

#include <QString>

namespace img_view {

class BookInfo {
public:
	/* Note: the @dirpath must be empty or an existing directory. */
	BookInfo(const QString &dirpath = "", const QString &author = "",
			const QString &cover_filepath = "");
	~BookInfo();

private:
	void init();

private:
	QString m_path;
	QString m_author;
	qint64 m_modifyTime = 0;
	QString m_coverFile;
};

}  /* namespace img_view */

#endif /* ifndef BOOK_INFO_H */
