#ifndef TRANSFORM_H
#define TRANSFORM_H

typedef int flag;

namespace transform{
	const int FILTER_NONE			= 0;
	const int FILTER_BLUR			= 1;
	const int FILTER_SHARPEN		= 2;
	const int FILTER_EDGE			= 3;
	const int FILTER_EMBOSS			= 4;
	const int FILTER_SOBEL_LEFT		= 5;
	const int FILTER_SOBEL_RIGHT	= 6;
	const int FILTER_SOBEL_TOP		= 7;
	const int FILTER_SOBEL_BOTTOM	= 8;
	const int FILTER_UNSHARP_MASK	= 9;
	const int EDGE_CROP_WHITE		= 0;		//For kernel operations: Replaces out-of-boundary coordinate color values with NULL values rgb(0,0,0).
	const int EDGE_CROP_BLACK		= 1;
	const int EDGE_EXTEND			= 2;		//For kernel operations: Replaces out-of-boundary coordinate color values with last used value along image edge.
	const int EDGE_WRAP				= 3;		//For kernel operations: Replaces out-of-boundary coordinate color values with respective value from opposite edge (for seamless images).
	const int GRAY_AVERAGE			= 0;		//(R +G + B) / 3
	const int GRAY_LUMINANCE		= 1;		//0.72R + 0.21G + 0.07B
	const int GRAY_LIGHTNESS		= 2;		//(min(R, G, B) + max(R, G, B)) / 2
	const int GRAY_RED				= 3;		//Set grayscale equal to red channel color values
	const int GRAY_GREEN			= 4;		//Set grayscale equal to green channel color values
	const int GRAY_BLUE				= 5;		//Set grayscale equal to blue channel color values
	const int CHANNEL_ALL			= 0;
	const int CHANNEL_RED			= 1;
	const int CHANNEL_GREEN			= 2;
	const int CHANNEL_BLUE			= 3;
	const int CHANNEL_HSV			= 4;
	const int CHANNEL_HUE			= 5;
	const int CHANNEL_SATURATION	= 6;
	const int CHANNEL_VALUE			= 7;

	//For use with 'GetColor' function
	const int COLOR_WHITE			= 0;
	const int COLOR_GRAY			= 1;
	const int COLOR_BLACK			= 2;
	const int COLOR_RED				= 3;
	const int COLOR_ORANGE			= 4;
	const int COLOR_YELLOW			= 5;
	const int COLOR_GREEN			= 6;
	const int COLOR_BLUE			= 7;
	const int COLOR_INDIGO			= 8;
	const int COLOR_PURPLE			= 9;
	const int COLOR_SALMON			= 10;
	const int COLOR_BROWN			= 11;
	const int COLOR_ORANGE_RED		= 12;
	const int COLOR_DARK_ORANGE		= 13;
	const int COLOR_BEIGE			= 14;
	const int COLOR_TAN				= 15;

	//Histogram and Graph Styling
	const int GRAPH_DOTTED			= 0;
	const int GRAPH_LINEAR			= 1;
	const int GRAPH_FILLED			= 2;
	const int GRID_DEFAULT			= 0;
	const int GRID_HORIZONTAL		= 1;
	const int GRID_VERTICAL			= 2;

	//Noise Styling
	const int NOISE_GRAYSCALE = 0;
	const int NOISE_COLORED = 1;
	const int NOISE_RED = 2;
	const int NOISE_GREEN = 3;
	const int NOISE_BLUE = 4;

	//Posterize Styling
	const int POST_AVERAGE			= 0;
	const int POST_NONE				= 1;
	const int POST_HIGH_C			= 2;
	const int POST_BW				= 3;

	//Interlace Styling
	const int IL_DEFAULT			= 0;
	const int IL_HORIZONTAL			= 1;
	const int IL_VERTICAL			= 2;
	const int IL_WEAVE				= 3;
	const int IL_DOTTED				= 4;

	//Blend Flags
	const int BLEND_DEFAULT			= 0;
	const int BLEND_ADD				= 1;
	const int BLEND_SUBTRACT		= 2;
	const int BLEND_MULTIPLY		= 3;
	const int BLEND_DIVIDE			= 4;

	//Math Variables
	const double PI					= 3.141592;

	//GUI Constants
	const int WINPOS_CENTERED		= 0;
	const int WINPOS_TOP_L			= 1;
	const int WINPOS_TOP_R			= 2;
	const int WINPOS_BOTTOM_L		= 3;
	const int WINPOS_BOTTOM_R		= 4;

	//GUI::Window States
	const int GUI_STATE_DEFAULT = 0;
	const int GUI_STATE_HOVER = 1;
	const int GUI_STATE_DRAG = 2;
	const int GUI_STATE_RESIZE = 3;
}	

struct Vec2i{
	int x, y;

	Vec2i();
	Vec2i(int X, int Y):x(X), y(Y){}
};

struct Vec2d{
	double x, y;

	Vec2d(double X, double Y) :x(X), y(Y){}
};

struct UMatrix3x3{
	Uint8 val[3][3];
};

struct DMatrix3x3{
	double val[3][3];
};

struct UMatrix5x5{
	Uint8 val[5][5];
};

struct DMatrix5x5{
	double val[5][5];
};

struct Kernel3x3: public DMatrix3x3{
	//Kernel operations
	void Normalize();
	void Scale(double);

	//Kernel presets
	void Blur(){
		val[0][0] = 0.0625; val[0][1] = 0.125;	val[0][2] = 0.0625;
		val[1][0] = 0.125;  val[1][1] = 0.25;	val[1][2] = 0.125;
		val[2][0] = 0.0625; val[2][1] = 0.125;	val[2][2] = 0.0625;
	}
	void Sharpen(){
		val[0][0] = 0.0;	val[0][1] = -1.0;	val[0][2] = 0.0;
		val[1][0] = -1.0;	val[1][1] = 5.0;	val[1][2] = -1.0;
		val[2][0] = 0.0;	val[2][1] = -1.0;	val[2][2] = 0.0;
	}
	void Emboss(){
		val[0][0] = -2.0;	val[0][1] = -1.0;	val[0][2] = 0.0;
		val[1][0] = -1.0;	val[1][1] = 1.0;	val[1][2] = 1.0;
		val[2][0] = 0.0;	val[2][1] = 1.0;	val[2][2] = 2.0;
	}
	void Edge(){
		val[0][0] = 0.0;	val[0][1] = -1.0;	val[0][2] = 0.0;
		val[1][0] = -1.0;	val[1][1] = 4.0;	val[1][2] = -1.0;
		val[2][0] = 0.0;	val[2][1] = -1.0;	val[2][2] = 0.0;
	}
	void Sobel(int);
};

struct Kernel5x5 : public DMatrix5x5{
	void Normalize();
	void Scale(double);

	void Identity();
	void Blur();
	void Sharpen();
	void Emboss();
	void Edge();
	void Sobel(int);
};

//----------------------------------------------------------------------------------------------------------------------//
//												 Constructor Functions:													//
//----------------------------------------------------------------------------------------------------------------------//

//'SDL_Rect' constructor function
inline SDL_Rect CreateRect(int x, int y, int w, int h);

//'SDL_Rect' constructor function
inline SDL_Rect CreateRect(double x, double y, double w, double h);

//'SDL_Rect' constructor function
inline SDL_Rect CreateRect(float x, float y, float w, float h);

//'SDL_Color' constructor function
inline SDL_Color CreateColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

//'SDL_Color' constructor function
inline SDL_Color CreateColor(int r, int g, int b, int a);

//'SDL_Color' constructor function
inline SDL_Color CreateColor(double r, double g, double b, double a);

//'SDL_Color' constructor function
inline SDL_Color CreateColor(float r, float g, float b, float a);

//'Vec2i' constructor function
inline Vec2i CreateVec2i(int x, int y);

//'Vec2i' constructor function
inline Vec2i CreateVec2i(double x, double y);

//'Vec2i' constructor function
inline Vec2i CreateVec2i(float x, float y);

//'Vec2d' constructor function
inline Vec2d CreateVec2d(double x, double y);

//'Vec2d' constructor function
inline Vec2d CreateVec2d(int x, int y);

//'Vec2d' constructor function
inline Vec2d CreateVec2d(float x, float y);

//----------------------------------------------------------------------------------------------------------------------//
//												   Assist Functions:													//
//----------------------------------------------------------------------------------------------------------------------//

//Returns center position of specified surface as Vec2i
Vec2i getCenter(SDL_Surface* surface);

//Returns SDL_Color for basic colors identifiers (COLOR_WHITE, COLOR_GRAY, etc.)
SDL_Color getColor(flag id);

//Returns SDL_Color at specified point on surface
SDL_Color grabColor(SDL_Surface* surface, int x, int y);

//Applies gamma correction to specified color
SDL_Color correctGamma(SDL_Color color, double amount);

//Returns the number of colors found on a surface
int getColors(SDL_Surface* surface);

//Returns true if point is within surface boundary
bool hasPoint(SDL_Surface* surface, Vec2i point);

//Returns max value found within numerical (integer) vector array
int findMax(std::vector<int> vec);

//Returns max value found within numerical (double) vector array
double findMax(std::vector<double> vec);

//Returns max value found within numerical (float) vector array
float findMax(std::vector<float> vec);

//Returns all points whose color values fall between specified color and a given threshold
std::vector<Vec2i> extractPoints(SDL_Surface* surface, SDL_Color color, Uint8 threshold);

//Returns all points whose hue values fall between specified angles
std::vector<Vec2i> extractHues(SDL_Surface* surface);

//----------------------------------------------------------------------------------------------------------------------//
//												Manipulation Functions:													//
//----------------------------------------------------------------------------------------------------------------------//

//Get pixel data from specified surface at point x, y
Uint32 GetPixel(SDL_Surface* surface, int x, int y);

//Replace individual pixel on surface with target Uint32 color
void SetPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

//Create empty surface with another surface's size & format information
SDL_Surface* EmptySurface(SDL_Surface* surface);

//Create empty surface of a custom size with another surface's format
SDL_Surface* EmptySurface(SDL_Surface* surface, int wid, int hei);

//Create empty surface of default format with specified size
SDL_Surface* EmptySurface(int wid, int hei);

//Copy surface contents to another surface
SDL_Surface* CopySurface(SDL_Surface* surface);

//Copy region of surface to another surface at specified region
void CopyRegion(SDL_Surface* srcSurface, SDL_Surface* dstSurface, SDL_Rect srcRect, SDL_Rect dstRect);

//Scale surface in both directions by scalar amount
SDL_Surface* ScaleSurface(SDL_Surface* surface, double scale);

//Scale surface to specified x scale and y scale
SDL_Surface* ScaleSurface(SDL_Surface* surface, double scaleX, double scaleY);

//Resize surface to a specified width and height
SDL_Surface* ResizeSurface(SDL_Surface* surface, int wid, int hei);

SDL_Surface* ScaleRegion(SDL_Surface* surface, SDL_Rect rect);

//Add surface pixels scaled to opacity (between 0.0 and 1.0) to target
void AddSurface(SDL_Surface* target, SDL_Surface* source, double opacity);

//Subtract surface pixels scaled to opacity (between 0.0 and 1.0) from target
void SubtractSurface(SDL_Surface* target, SDL_Surface* source, double opacity);

//----------------------------------------------------------------------------------------------------------------------//
//											  Kernel & Filter Functions:												//
//----------------------------------------------------------------------------------------------------------------------//

//Apply pre-created convolution matrix to surface
void ApplyKernel(SDL_Surface* srcSurface, SDL_Surface* dstSurface, Kernel3x3 kernel);

//Specify and handle covolution matrix filter
SDL_Surface* FilterSurface(SDL_Surface* surface, flag method, flag edge);

//Apply blur kernel to surface specified number of passes
SDL_Surface* FilterBlur(SDL_Surface* surface, int passes, flag edge);

//Offset image in either direction
SDL_Surface* FilterOffset(SDL_Surface* surface, int x, int y);

//Apply blur and subtract from original if difference is above set threshold
void FilterUnsharpMask();

//Flip surface horizontally
SDL_Surface* FlipHorizontal(SDL_Surface* surface);

//Flip surface vertically
SDL_Surface* FlipVertical(SDL_Surface* surface);

//----------------------------------------------------------------------------------------------------------------------//
//											    Image Adjustment Functions:												//
//----------------------------------------------------------------------------------------------------------------------//

//Set surface to grayscale using various gray algorithms
void Grayscale(SDL_Surface* surface, flag method);

//Percentage-based desaturation (1.0 = Full Color, 0.0 = Grayscale)
void Desaturate(SDL_Surface* surface, double value);

//Convert image to binary based on specified color channel
void Threshold(SDL_Surface* surface, flag channel, int max);

//Use to replace a color (within a set threshold) to another color.
void ReplaceColor(SDL_Surface* surface, SDL_Color srcColor, SDL_Color dstColor, int threshold);

//Removes every (amount) pixel from surface
void Interlace(SDL_Surface* surface, int amount);

//Replaces every (amount) pixel from surface to target color
void Interlace(SDL_Surface* surface, int amount, SDL_Color color);

//Replaces every (amount) pixel from surface to target color through any IL_* flag (IL_HORIZONTAL, IL_VERTICAL, etc.)
void Interlace(SDL_Surface* surface, int amount, SDL_Color color, flag style);

//Use to replace or blend every (amount) pixel from surface to target color through any IL_* flag (IL_HORIZONTAL, IL_VERTICAL, etc.). 
//Blend modes include BLEND_ADD, BLEND_SUBTRACT, and BLEND_DEFAULT
void Interlace(SDL_Surface* surface, int amount, SDL_Color color, flag style, flag blend);

//Use to reduce the amount of colors in a surface. "colors" indicates the max number of colors per channel (Total colors can range from 3^3^3)
void Posterize(SDL_Surface* surface, int colors);

//Use to reduce the amount of colors in a surface. "colors" indicates the max number of colors per channel (Total colors can range from 3^3^3). 
//The "style" flag can be used to introduce effects inline through POST_* constants (POST_BW, POST_HIGH_C, etc.)
void Posterize(SDL_Surface* surface, int colors, flag style);

//Shrink binary image
SDL_Surface* Erode(SDL_Surface* surface);

//Expand binary image
SDL_Surface* Dilate(SDL_Surface* surface);

//Trace edge of binary image (Recommended to blur first using either 'FilterBlur' or 'FilterSurface(Blur)'
SDL_Surface* Trace(SDL_Surface* surface, int max, SDL_Color col);

//Invert colors of a specified surface
void Invert(SDL_Surface* surface);													

//Add amount to surface red channel
void AdjustRed(SDL_Surface* surface, int offset);									

//Add amount to surface 'green' channel
void AdjustGreen(SDL_Surface* surface, int offset);									

//Add amount to surface 'blue' channel
void AdjustBlue(SDL_Surface* surface, int offset);									

//Adds amounts to individual color channels within a single function
void AdjustMulti(SDL_Surface* surface, int rOffset, int gOffset, int bOffset);		

//Adds uniform amount to all color channels	
void AdjustBrightness(SDL_Surface* surface, int amount);							

//Adjust surface dynamic range (recommended amounts between 0.0 and 100.0)
void AdjustContrast(SDL_Surface* surface, double amount);

//Adjust highlight amount on surface (recommended amounts between 0.0 and 100.0)
void AdjustHighlights(SDL_Surface* surface, double amount);

//Adjust shadow amount on surface (recommended amounts between 0.0 and -100.0)
void AdjustShadows(SDL_Surface* surface, double amount);

//Adjust gamma level of surface (recommended amounts between 0.0 to 8.0)
void AdjustGamma(SDL_Surface* surface, double amount);

//Adjust color tempurature of surface
void AdjustTemperature(SDL_Surface* surface, double kelvin, double strength);

//Generate temperature spectrum to SDL_Surface
SDL_Surface* TemperatureSpectrum(double kelvinA, double kelvinB, int height);

//Draw gradient or solid line
void DrawLine(SDL_Surface* surface, Vec2i srcPos, Vec2i dstPos, SDL_Color srcCol, SDL_Color dstCol);

//Draw basic circle to surface
void DrawCircle(SDL_Surface* surface, Vec2i pos, double radius, SDL_Color outline, SDL_Color fill, bool filled, bool dotted);

//Draw rectangle to surface
void DrawRect(SDL_Surface* surface, Vec2i srcPos, Vec2i dstPos, SDL_Color outline, SDL_Color fill, bool filled);

//Generate cone gradient on target surface
void GradientCone();

//Generate radial gradient on target surface
void GradientRadial(SDL_Surface* surface, Vec2i pos, double radius, SDL_Color srcCol, SDL_Color dstCol);

//Generate linear gradient on target surface
void GradientLinear(Vec2i srcPos, Vec2i dstPos, SDL_Color srcCol, SDL_Color dstCol);

//Generate basic noise
void GenerateNoise(SDL_Surface* surface, flag style, Uint8 rangeMin, Uint8 rangeMax);

//Generate difference clouds
void GenerateClouds();

//Fills in specified surface with solid color
void Fill(SDL_Surface* surface, SDL_Color color);

//----------------------------------------------------------------------------------------------------------------------//
//										    Graphing & Visualization Functions:											//
//----------------------------------------------------------------------------------------------------------------------//

void DrawPoints(SDL_Surface* surface, SDL_Color color, std::vector<Vec2i> points);

void DrawGridlines(SDL_Surface* surface, SDL_Color color, flag style, int padding, double cellsize, double maxX, double maxY);

void Histogram(SDL_Surface* srcSurface, SDL_Surface* dstSurface, flag channel, flag style, int padding, int desample);

void Histogram(SDL_Surface* srcSurface, SDL_Surface* dstSurface, flag channel, flag style, int padding, int desample, int maxY);

void FFT(SDL_Surface* source, SDL_Surface* destination, double samples);

//----------------------------------------------------------------------------------------------------------------------//
//											    Color Conversion Functions:												//
//----------------------------------------------------------------------------------------------------------------------//

void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v);

void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v);

//----------------------------------------------------------------------------------------------------------------------//
//											    Color Conversion Functions:												//
//----------------------------------------------------------------------------------------------------------------------//

#endif