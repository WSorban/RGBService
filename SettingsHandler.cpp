#include "SettingsHandler.h"

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