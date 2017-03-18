#include "XMLWriter.h"

XMLWriter::XMLWriter(std::string of) :m_XMLWriter(NULL)
{
	this->outputFile = of;
	this->tagIndentation = 0;
}

XMLWriter::~XMLWriter()
{
}

void XMLWriter::initXML(std::string version, std::string encoding, std::string standalone)
{
	stringBuilder = stringBuilder + "<?xml version=" + '"' + 
		version + '"' + " encoding=" + '"' + 
		encoding + '"' + " standalone=" + '"' + 
		standalone + '"' + " ?>" + char(13);
}

void XMLWriter::openTag(std::string tagName)
{
	for (int i = 0; i < tagIndentation; i++)
	{
		//append tabs, depending on the needed indentation
		stringBuilder = stringBuilder + tabChar;
	}
	stringBuilder = stringBuilder + '<' + tagName + '>' + char(13);

	tags[tagIndentation] = tagName;
	tagIndentation++;
}

void XMLWriter::openTagWithKey(std::string tagName, std::string keyName)
{
	for (int i = 0; i < tagIndentation; i++)
	{
		//append tabs, depending on the needed indentation
		stringBuilder = stringBuilder + tabChar;
	}
	stringBuilder = stringBuilder + '<' + tagName + ' ' + "key=" + '"' + keyName + '"' + '>' + char(13);

	tags[tagIndentation] = tagName;
	tagIndentation++;
}

void XMLWriter::openTagWithKey(std::string tagName, int keyName)
{
	for (int i = 0; i < tagIndentation; i++)
	{
		//append tabs, depending on the needed indentation
		stringBuilder = stringBuilder + tabChar;
	}
	stringBuilder = stringBuilder + '<' + tagName + ' ' + "key=" + '"' + std::to_string(keyName) + '"' + '>' + char(13);

	tags[tagIndentation] = tagName;
	tagIndentation++;
}

void XMLWriter::openTagWithValue(std::string tagName, std::string value)
{
	for (int i = 0; i < tagIndentation; i++)
	{
		//append tabs, depending on the needed indentation
		stringBuilder = stringBuilder + tabChar;
	}
	stringBuilder = stringBuilder + '<' + tagName + '>' + value + "</" + tagName + '>' + char(13);
}

void XMLWriter::openTagWithValue(std::string tagName, int value)
{
	int val = value;
	int something = 0;

	for (int i = 0; i < tagIndentation; i++)
	{
		//append tabs, depending on the needed indentation
		stringBuilder = stringBuilder + tabChar;
	}
	stringBuilder = stringBuilder + '<' + tagName + '>' + std::to_string(val) + "</" + tagName + '>' + char(13);
}

void XMLWriter::openTagWithValue(std::string tagName, double value)
{
	for (int i = 0; i < tagIndentation; i++)
	{
		//append tabs, depending on the needed indentation
		stringBuilder = stringBuilder + tabChar;
	}
	stringBuilder = stringBuilder + '<' + tagName + '>' + std::to_string(value) + "</" + tagName + '>' + char(13);
}

void XMLWriter::openTagWithKeyValue(std::string tagName, std::string keyName, std::string value)
{
	for (int i = 0; i < tagIndentation; i++)
	{
		//append tabs, depending on the needed indentation
		stringBuilder = stringBuilder + tabChar;
	}
	stringBuilder = stringBuilder + '<' + tagName + ' ' + "key=" + '"' + keyName + '"' + '>' + 
		value + 
		"</" + tagName + '>' + char(13);
}

void XMLWriter::closeTag()
{
	tagIndentation--;

	for (int i = 0; i < tagIndentation; i++)
	{
		//append tabs, depending on the needed indentation
		stringBuilder = stringBuilder + tabChar;
	}

	stringBuilder = stringBuilder + "</" + tags[tagIndentation] + '>' + char(13);
}

void XMLWriter::addValue(std::string value)
{
	for (int i = 0; i < tagIndentation; i++)
	{
		//append tabs, depending on the needed indentation
		stringBuilder = stringBuilder + tabChar;
	}
	stringBuilder = stringBuilder + value;
}

void XMLWriter::printXMLToFile()
{
	std::ofstream myfile(outputFile);
	if (myfile.is_open())
	{
		myfile << stringBuilder;
		myfile.close();
	}
}