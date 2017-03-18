#include <cmath>
#include "ColorOperations.h"

//http://stackoverflow.com/questions/3732046/how-do-you-get-the-hue-of-a-xxxxxx-colour
double getLightnessFromRGB(int color)
{
	double r = GetBValue(color);
	double g = GetGValue(color);
	double b = GetRValue(color);

	double lightness;
	//converting RGB values to the range of 0-1
	const double R = r / 255.0;
	const double G = g / 255.0;
	const double B = b / 255.0;

	//Find the minimum and maximum values of R, G and B
	const double maxRGB = fmax(fmax(R, G), fmax(G, B));
	const double minRGB = fmin(fmin(R, G), fmin(G, B));

	lightness = (maxRGB + minRGB) / 2.0;

	return lightness;
}

//http://stackoverflow.com/questions/3732046/how-do-you-get-the-hue-of-a-xxxxxx-colour


double getSaturationFromRGB(int color)
{
	double r = GetBValue(color);
	double g = GetGValue(color);
	double b = GetRValue(color);

	double saturation;
	double delta;

	//converting RGB values to the range of 0-1
	const double R = r / 255.0;
	const double G = g / 255.0;
	const double B = b / 255.0;

	//Find the minimum and maximum values of R, G and B
	const double maxRGB = fmax(fmax(R, G), fmax(G, B));
	const double minRGB = fmin(fmin(R, G), fmin(G, B));

	delta = maxRGB - minRGB;

	if (delta == 0.0) //achromatic
	{
		saturation = 0.0;
	}
	else
	{
		const double lightness = getLightnessFromRGB(color);
		saturation = delta;
		if (lightness <= 0.5)
		{
			saturation /= (maxRGB + minRGB);
		}
		else
		{
			saturation /= 2.0 - maxRGB - minRGB;
		}
	}
	return saturation;
}

//http://stackoverflow.com/questions/3732046/how-do-you-get-the-hue-of-a-xxxxxx-colour
double getHueFromRGB(int color)
{
	double r = GetBValue(color);
	double g = GetGValue(color);
	double b = GetRValue(color);

	double hue;
	double delta;

	//converting RGB values to the range of 0-1
	const double R = r / 255.0;
	const double G = g / 255.0;
	const double B = b / 255.0;
	//Find the minimum and maximum values of R, G and B
	const double maxRGB = fmax(fmax(R, G), fmax(G, B));
	const double minRGB = fmin(fmin(R, G), fmin(G, B));

	delta = maxRGB - minRGB;

	//Depending on what RGB color channel is the max value. The three different formulas are	
	if (delta == 0.0) //achromatic
	{
		hue = 0.0;
	}
	else
	{
		if (maxRGB == R)
		{
			hue = fmod((G - B) / delta,6.0);
		}
		else if (maxRGB == G)
		{
			hue = 2.0 + (B - R) / delta;
		}
		else if (maxRGB == B)
		{
			hue = 4.0 + (R - G) / delta;
		}
		hue = hue *60;
	}

	if (hue < 0)
	{
		hue = hue + 360;
	}
	hue = hue / 360;

	return hue;
}