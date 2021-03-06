#include "SettingsHandler.h"
#include "XMLWriter.h"
#include "ColorOperations.h"


void setMBPart(int part, int color, double speed, int direction, XMLWriter* m_XMLWriter)
{
	double hue = getHueFromRGB(color);
	double saturation = getSaturationFromRGB(color);
	double lightness = getLightnessFromRGB(color);

	/*double hue, saturation, lightness;

	RGBtoHSL(color, hue, saturation, lightness);*/

	m_XMLWriter->openTagWithKey("led", part);
		m_XMLWriter->openTagWithValue("color", color);
		m_XMLWriter->openTagWithValue("r", GetBValue(color));
		m_XMLWriter->openTagWithValue("g", GetGValue(color));
		m_XMLWriter->openTagWithValue("b", GetRValue(color));
		m_XMLWriter->openTagWithValue("hue", hue);
		m_XMLWriter->openTagWithValue("saturation", saturation);
		m_XMLWriter->openTagWithValue("lightness", lightness);
		m_XMLWriter->openTagWithValue("speed", speed);
		m_XMLWriter->openTagWithValue("direction", direction);
	m_XMLWriter->closeTag();
}

void saveMBAura(int color)
{
	XMLWriter* m_XMLWriter = new XMLWriter("C:\\Program Files (x86)\\ASUS\\AURA\\LastProfile.xml");

	m_XMLWriter->initXML("1.0", "UTF-8", "no");
	m_XMLWriter->openTag("root");
		m_XMLWriter->openTagWithValue("header","ASUS_AURA");
		m_XMLWriter->openTagWithValue("version", "");
		m_XMLWriter->openTagWithValue("funcid", 0);
		m_XMLWriter->openTagWithKeyValue("connecteddevice","Mainboard_Master", "0");
		m_XMLWriter->openTagWithKeyValue("ingroupdevice", "Mainboard_Master","0");
		m_XMLWriter->openTagWithKey("device","Group");
			m_XMLWriter->openTagWithValue("isenabled", 1);
			m_XMLWriter->openTagWithKey("scene", "S0");
				m_XMLWriter->openTagWithValue("state", "S0");
				m_XMLWriter->openTagWithKey("mode", 1);
					setMBPart(0, color, 0, 0, m_XMLWriter);
					setMBPart(1, color, 0, 0, m_XMLWriter);
					setMBPart(2, color, 0, 0, m_XMLWriter);
					setMBPart(3, color, 0, 0, m_XMLWriter);
					setMBPart(4, color, 0, 0, m_XMLWriter);
					m_XMLWriter->openTagWithValue("color_type", "Plain");
					m_XMLWriter->openTagWithValue("start_end_color_cycle_start", 0.000);
					m_XMLWriter->openTagWithValue("start_end_color_cycle_range", 0.75);
					m_XMLWriter->openTagWithValue("start_end_color_cycle_end", 0.75);
					m_XMLWriter->openTagWithValue("start_to_end_or_end_to_start", 1);
					m_XMLWriter->openTag("effect_path_order");
						m_XMLWriter->openTagWithKeyValue("order", "Mainboard", "0");
						m_XMLWriter->openTagWithKeyValue("order", "Vga", "-1");
						m_XMLWriter->openTagWithKeyValue("order", "Keyboard", "-1");
						m_XMLWriter->openTagWithKeyValue("order", "Mouse", "-1");
						m_XMLWriter->openTagWithKeyValue("order", "GSkillDram", "-1");
						m_XMLWriter->openTagWithKeyValue("order", "RogAudioDevices_Microphone", "-1");
					m_XMLWriter->closeTag(); //closing effect+path_order tag
					m_XMLWriter->openTagWithValue("effect_speed", 0);
					m_XMLWriter->openTagWithValue("music_index", 3);
				m_XMLWriter->closeTag(); //closing mode tag
			m_XMLWriter->closeTag(); //closing scene tag
		m_XMLWriter->closeTag(); //closing device tag
	m_XMLWriter->closeTag(); //closing root tag
	m_XMLWriter->printXMLToFile();
	delete m_XMLWriter;
}

void settingsBuilder(std::string* settings, std::string message)
{
	unsigned char DeviceType = 6;
	unsigned char MessageType = 0;

	MessageType = message[0];

	//if first byte is R, it's a change color command
	if (MessageType == 'R')
	{
		DeviceType = ((unsigned char)message[7]);
		switch (DeviceType)
		{
		case 1:
			settings[1] = message;
			break;
		case 2:
			settings[2] = message;
			break;
		case 3:
			settings[3] = message;
			break;
		case 4:
			settings[4] = message;
			break;
		case 5:
			settings[5] = message;
			break;
		case 6:
			settings[1] = message;
			settings[2] = message;
			settings[3] = message;
			settings[4] = message;
			settings[5] = message;
		}
	}
	//if first byte is 3, then it's effect mode
	else if (MessageType == 3)
	{
		DeviceType = ((unsigned char)message[1]);
		switch (DeviceType)
		{
		case 1:
			settings[1] = message;
			break;
		case 2:
			settings[2] = message;
			break;
		case 3:
			settings[3] = message;
			break;
		case 4:
			settings[4] = message;
			break;
		case 5:
			settings[5] = message;
			break;
		case 6:
			settings[1] = message;
			settings[2] = message;
			settings[3] = message;
			settings[4] = message;
			settings[5] = message;
		}
	}

}

void settingsSaver(std::string outputFile, std::string* settings, int strLen)
{
	std::ofstream myfile(outputFile);
	if (myfile.is_open())
	{
		for (int i = 0; i < strLen; i++)
		{
			myfile << settings[i] + "\n";
		}
		myfile.close();
	}
}

void settingsLoader(std::string inputFile, std::string* settings)
{
	std::string line;
	std::ifstream myfile(inputFile);
	if (myfile.is_open())
	{
		int i = 0;
		while (getline(myfile, line))
		{
			settings[i] = line;
			i++;
			//do not read more than the first 6 lines
			if (i > 5) break;
		}
		myfile.close();
	}
}

std::string ConvertCLIPointerToStdString(array<unsigned char>^ s)
{
	pin_ptr<unsigned char> p = &s[0];
	unsigned char *unmanagedP = p;
	std::string str(unmanagedP, unmanagedP + s->GetLength(0));
	return str;
}