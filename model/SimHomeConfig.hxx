#ifndef __SIM_HOME_CONFIG__
#define __SIM_HOME_CONFIG__

#include <string>
#include <QObject>
#include <QDomDocument>

namespace Application
{

class	SimHomeConfig : public QObject
{
	Q_OBJECT

public:

	static SimHomeConfig&	instance();
	~SimHomeConfig();

	const std::string&	xmlDocPath() const { return mXmlPath; }
	void			loadXmlConfig();

protected:

	SimHomeConfig( std::string path );



	std::string		mXmlPath;	
};

}


#endif // SimHomeConfig.hxx
