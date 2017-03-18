#ifndef _XMLWRITER_H_
#define _XMLWRITER_H_

#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "stdafx.h"

class XMLWriter
{
	public:
		XMLWriter(std::string of);
		~XMLWriter();

		void openTag(std::string tagName);
		void addValue(std::string value);
		void closeTag();
		void initXML(std::string version, std::string encoding, std::string standalone);
		void openTagWithKey(std::string tagName, std::string keyName);
		void openTagWithKey(std::string tagName, int keyName);
		void openTagWithValue(std::string tagName, std::string value);
		void openTagWithValue(std::string tagName, int value);
		void openTagWithValue(std::string tagName, double value);
		void printXMLToFile();
		void openTagWithKeyValue(std::string tagName, std::string keyName, std::string value);

	private:
		HMODULE m_XMLWriter;
		int tagIndentation = 0;
		std::string tags[10];
		std::string stringBuilder;
		std::string outputFile;
		const char tabChar = char(9);
};
#endif