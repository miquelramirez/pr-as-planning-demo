/*
Plan Recognition as Planning Demo: Home domain
Copyright (C) 2012
Miquel Ramirez <miquel.ramirez@rmit.edu.au>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __GRAPHICS_FACTORY__
#define __GRAPHICS_FACTORY__

#include <QImage>
#include <QPixmap>
#include <string>
#include <map>

namespace UI
{

class GraphicsFactory
{
public:

	~GraphicsFactory();

	static GraphicsFactory& instance();

	QImage* get_image( std::string key )
	{
		return m_image_lib[key];
	}

	QPixmap* get_pixmap( std::string key )
	{
		return m_pixmap_lib[key];
	}

protected:

	GraphicsFactory();
	
	void load_graphics();

protected:

	std::map< std::string, QImage* >	m_image_lib;
	std::map< std::string, QPixmap* >	m_pixmap_lib;
};

}

#endif
