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

#include <model/SimHomeConfig.hxx>
#include <cstdlib>
#include <QFile>
#include <iostream>

namespace Application
{


SimHomeConfig::SimHomeConfig( std::string path )
	: mXmlPath( path )
{
	
}

SimHomeConfig::~SimHomeConfig()
{
}

SimHomeConfig& SimHomeConfig::instance()
{
	static SimHomeConfig theCfg("app-config.xml");
	return theCfg;
}

void	SimHomeConfig::loadXmlConfig()
{
	QFile file(xmlDocPath().c_str());

	if ( !file.open(QFile::ReadOnly|QFile::Text) )
	{
		std::cerr << "Could not open config file " << xmlDocPath() << std::endl; 
		std::exit(1);	
	}

	QDomDocument domDocument;
	QString errorStr;
	int errorLine, errorColumn;

	if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn) ) 
	{
		std::cerr << "Problem parsing " << xmlDocPath() << std::endl;
		QString msg = tr("Parse error at line %1, column %2:\n%3")
			.arg(errorLine)
			.arg(errorColumn)
			.arg(errorStr);
		std::cerr << msg.toStdString() << std::endl;
		std::exit(1);
	}	
}

}
