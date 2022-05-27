/**
 * book_info.cc
 *
 * Created by vamirio on 2022 May 14
 */
#include "book_info.h"

#include <QDir>
#include <QFileInfo>

namespace img_view {

BookInfo::BookInfo(const QString &dirpath, const QString &author,
		const QString &cover_filepath)
	: m_path(dirpath), m_author(author), m_coverFile(cover_filepath)
{
	init();
}

BookInfo::~BookInfo()
{
}

void BookInfo::init()
{
	if (m_path.isEmpty())
		return;

	QDir dir(m_path);
	if (!dir.isAbsolute())
		m_path = dir.absolutePath();

	QFileInfo info(m_path);
	m_modifyTime = info.lastModified().toMSecsSinceEpoch();

	if (!m_coverFile.isEmpty())
		return;
	for (const auto &file : dir.entryList(QDir::Files)) {

	}
}

}  /* namespace img_view */
