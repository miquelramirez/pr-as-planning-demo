#include <widgets/GraphicsFactory.hxx>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>

namespace UI
{

GraphicsFactory::GraphicsFactory()
{
	load_graphics();
}

GraphicsFactory::~GraphicsFactory()
{
}

GraphicsFactory& GraphicsFactory::instance()
{
	static GraphicsFactory the_instance;

	return the_instance;
}

void GraphicsFactory::load_graphics()
{
	std::string path_to_bindings = "resources/graphics/bindings.txt";
	std::ifstream in( path_to_bindings.c_str() );
	
	if (in.fail())
	{
		std::cerr << "Whooops! couldn't find the graphic bindings description file!" << std::endl;
		std::exit(1);
	}
	char 			 line_buffer[256000];
	std::string 		 line;

	while ( !in.eof() )
	{
		in.getline( line_buffer, 255999, '\n' );
		line.assign( line_buffer );
		if (line.empty()) continue;
		std::string::iterator it = std::find( line.begin(), line.end(), '=');
		std::string key( line.begin(), it++ );
		std::string value( it, line.end() );
		QString path_to_image_file( "resources/graphics/%1" );
		QImage* new_image = new QImage;
		if (!new_image->load( path_to_image_file.arg( value.c_str() ) ))
		{
			std::cerr << "Could not open image file: " << path_to_image_file.toStdString() << "!!!" << std::endl;
			std::exit(1);
		}
		m_image_lib[key] = new_image;
		QPixmap* new_pixmap = new QPixmap;
		new_pixmap->load( path_to_image_file.arg( value.c_str() ) );
		m_pixmap_lib[key] = new_pixmap;
	}
	
}

}
