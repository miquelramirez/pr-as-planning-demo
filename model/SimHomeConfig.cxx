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
