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
