/**
 * options.cc
 *
 * Created by vamirio on 2022 Jun 27
 */
#include "options.h"

#include <cstring>

namespace img_view
{

Options::Options()
{
	imageBgColor = new char[7];
	strcpy(imageBgColor, "000000");  /* White. */
}

Options::~Options()
{
	delete[] imageBgColor;
}

} /* img_view */
