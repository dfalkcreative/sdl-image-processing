#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <complex>
#include <math.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Transform.h"
#include "Util/Segment.h"
//#include "FFT/kiss_fft.h"

using namespace transform;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------//
//												 Constructor Functions:													//
//----------------------------------------------------------------------------------------------------------------------//

Vec2i::Vec2i(){
	x = 0; y = 0;
}

inline SDL_Rect CreateRect(int x, int y, int w, int h){
	SDL_Rect temp;
	temp.x = x; temp.y = y; temp.w = w; temp.h = h;
	return temp;
}

inline SDL_Rect CreateRect(double x, double y, double w, double h){
	SDL_Rect temp;
	temp.x = int(x); temp.y = int(y); temp.w = int(w); temp.h = int(h);
	return temp;
}

inline SDL_Rect CreateRect(float x, float y, float w, float h){
	SDL_Rect temp;
	temp.x = int(x); temp.y = int(y); temp.w = int(w); temp.h = int(h);
	return temp;
}

inline SDL_Color CreateColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
	SDL_Color temp;
	temp.r = r; temp.g = g; temp.b = b; temp.a = a;
	return temp;
}

inline SDL_Color CreateColor(int r, int g, int b, int a){
	SDL_Color temp;
	temp.r = Uint8(r); temp.g = Uint8(g); temp.b = Uint8(b); temp.a = Uint8(a);
	return temp;
}

inline SDL_Color CreateColor(double r, double g, double b, double a){
	SDL_Color temp;
	temp.r = Uint8(r); temp.g = Uint8(g); temp.b = Uint8(b); temp.a = Uint8(a);
	return temp;
}

inline SDL_Color CreateColor(float r, float g, float b, float a){
	SDL_Color temp;
	temp.r = Uint8(r); temp.g = Uint8(g); temp.b = Uint8(b); temp.a = Uint8(a);
	return temp;
}

inline Vec2i CreateVec2i(int x, int y){
	Vec2i temp(x, y); return temp;
}

inline Vec2i CreateVec2i(double x, double y){
	int iX = int(x); int iY = int(y);

	Vec2i temp(iX, iY); return temp;
}

inline Vec2i CreateVec2i(float x, float y){
	int iX = int(x); int iY = int(y);

	Vec2i temp(iX, iY); return temp;
}

inline Vec2d CreateVec2d(double x, double y){
	Vec2d temp(x, y); return temp;
}

inline Vec2d CreateVec2d(int x, int y){
	double dX = double(x); double dY = double(y);

	Vec2d temp(dX, dY); return temp;
}

inline Vec2d CreateVec2d(float x, float y){
	double dX = double(x); double dY = double(y);

	Vec2d temp(dX, dY); return temp;
}

//----------------------------------------------------------------------------------------------------------------------//
//												   Assist Functions:													//
//----------------------------------------------------------------------------------------------------------------------//

Vec2i getCenter(SDL_Surface* surface){
	Vec2i temp(surface->w / 2, surface->h / 2);
	return temp;
}

SDL_Color getColor(flag id){
	//Source:
	//	RGB values borrowed from: http://www.tayloredmktg.com/rgb/

	switch (id){
		case COLOR_WHITE:
			return CreateColor(255,255,255,255);
		case COLOR_GRAY:
			return CreateColor(127, 127, 127, 255);
		case COLOR_BLACK:
			return CreateColor(0, 0, 0, 255);
		case COLOR_RED:
			return CreateColor(255, 0, 0, 255);
		case COLOR_ORANGE:
			return CreateColor(255, 165, 0, 255);
		case COLOR_YELLOW:
			return CreateColor(255,255,0,255);
		case COLOR_GREEN:
			return CreateColor(0,255,0,255);
		case COLOR_BLUE:
			return CreateColor(0, 0, 255, 255);
		case COLOR_INDIGO:
			return CreateColor(147, 112, 219, 255);
		case COLOR_PURPLE:
			return CreateColor(160, 32, 240, 255);
		case COLOR_SALMON:
			return CreateColor(250, 128, 114, 255);
		case COLOR_BROWN:
			return CreateColor(139, 69, 19, 255);
		case COLOR_ORANGE_RED:
			return CreateColor(255,69,0,255);
		case COLOR_DARK_ORANGE:
			return CreateColor(255, 140, 0, 255);
		case COLOR_BEIGE:
			return CreateColor(245,245,220,255);
		case COLOR_TAN:
			return CreateColor(210, 180, 140, 255);
		default:
			return CreateColor(255, 255, 255, 255);
	}
}

SDL_Color getTemperatureColor(double kelvin){
	//Code translated from http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
	//Credit to Tanner Helland for conversion math
	if (kelvin < 1000) kelvin = 1000; else if (kelvin > 40000) kelvin = 40000;

	kelvin /= 100;
	double r, g, b, calc;
	SDL_Color out;

	//Red
	if (kelvin <= 66)
		r = 255;
	else{
		//'Note: the R-squared value for this approximation is .988
		calc = kelvin - 60;
		calc = 329.698727446 * pow(calc, -0.1332047592);
		r = calc;

		if (r < 0) r = 0; else if (r > 255) r = 255;
	}

	//Green
	if (kelvin <= 66){
		//'Note: the R-squared value for this approximation is .996
		calc = kelvin;
		calc = (99.4708025861 * log(calc)) - 161.1195681661;
		g = calc;

		if (g < 0) g = 0; else if (g > 255) g = 255;
	}
	else{
		//'Note: the R-squared value for this approximation is .987
		calc = kelvin - 60;
		calc = 288.1221695283 * pow(calc, -0.0755148492);
		g = calc;

		if (g < 0) g = 0; else if (g > 255) g = 255;
	}

	//Blue
	if (kelvin >= 66)
		b = 255;
	else if (kelvin <= 19)
		b = 0;
	else{
		//'Note: the R-squared value for this approximation is .998
		calc = kelvin - 10;
		calc = 138.5177312231 * log(calc) - 305.0447927307;

		b = calc;

		if (b < 0) b = 0; else if (b > 255) b = 255;
	}

	out = CreateColor(Uint8(r), Uint8(g), Uint8(b), Uint8(255));
	//out = correctGamma(out, 0.454545);

	return out;
}

SDL_Color grabColor(SDL_Surface* surface, int x, int y){
	Uint32 color = GetPixel(surface, x, y); SDL_Color rgba;
	SDL_GetRGBA(color, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
	return rgba;
}

SDL_Color correctGamma(SDL_Color color, double amount){
	double correction = 1 / amount;

	double r = 255 * pow(double(color.r) / 255, correction);
	double g = 255 * pow(double(color.g) / 255, correction);
	double b = 255 * pow(double(color.b) / 255, correction);

	if (r < 0) r = 0; else if (r > 255) r = 255;
	if (g < 0) g = 0; else if (g > 255) g = 255;
	if (b < 0) b = 0; else if (b > 255) b = 255;

	return CreateColor(Uint8(r), Uint8(g), Uint8(b), color.a);
}

int getColors(SDL_Surface* surface){
	vector<SDL_Color> colors;

	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 color = GetPixel(surface, x, y); SDL_Color rgba;
			SDL_GetRGBA(color, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
			bool found = false;

			for (int a = 0; a < signed(colors.size()); a++){
				if (rgba.r == colors[a].r && rgba.g == colors[a].g && rgba.b == colors[a].b)
					found = true;
			}

			if (found != true)
				colors.push_back(rgba);
		}
	}

	return colors.size();
}

bool hasPoint(SDL_Surface* surface, Vec2i point){
	if (point.x >= 0 && point.x < surface->w && point.y >= 0 && point.y < surface->h)
		return true;
	else
		return false;
}

int findMax(vector<int> vec){
	int max = 0;

	for (int a = 0; a < signed(vec.size()); a++){
		if (max < vec[a])
			max = vec[a];
	}

	return max;
}

//----------------------------------------------------------------------------------------------------------------------//
//												Manipulation Functions:													//
//----------------------------------------------------------------------------------------------------------------------//

Uint32 GetPixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;

	case 2:
		return *(Uint16 *)p;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;

	case 4:
		return *(Uint32 *)p;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void SetPixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16 *)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}

SDL_Surface* EmptySurface(SDL_Surface* surface){
	Uint32 rmask, gmask, bmask, amask;

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	SDL_Surface* temp = SDL_CreateRGBSurface(NULL, surface->w, surface->h, surface->format->BitsPerPixel, rmask, gmask, bmask, amask);
	return temp;
}

SDL_Surface* EmptySurface(SDL_Surface* surface, int wid, int hei){
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	SDL_Surface* temp = SDL_CreateRGBSurface(NULL, wid, hei, surface->format->BitsPerPixel, rmask, gmask, bmask, amask);
	return temp;
}

SDL_Surface* EmptySurface(int wid, int hei){
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	SDL_Surface* temp = SDL_CreateRGBSurface(NULL, wid, hei, 32, rmask, gmask, bmask, amask);
	return temp;
}

SDL_Surface* CopySurface(SDL_Surface* surface){
	SDL_Surface* temp = EmptySurface(surface);

	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 col = GetPixel(surface, x, y);
			SetPixel(temp, x, y, col);
		}
	}

	return temp;
}

void CopyRegion(SDL_Surface* surfaceA, SDL_Surface* surfaceB, SDL_Rect rectA, SDL_Rect rectB){

}

SDL_Surface* ScaleSurface(SDL_Surface* surface, double scale){
	SDL_Surface* temp = EmptySurface(surface, int(surface->w*scale), int(surface->h*scale));

	for (int y = 0; y < temp->h; y++){
		for (int x = 0; x < temp->w; x++){
			SetPixel(temp, x, y, GetPixel(surface, int(x / scale), int(y / scale)));
		}
	}

	return temp;
}

SDL_Surface* ScaleSurface(SDL_Surface* surface, double scaleX, double scaleY){
	SDL_Surface* temp = EmptySurface(surface, int(surface->w*scaleX), int(surface->h*scaleY));

	for (int y = 0; y < temp->h; y++){
		for (int x = 0; x < temp->w; x++){
			SetPixel(temp, x, y, GetPixel(surface, int(x / scaleX), int(y / scaleY)));
		}
	}

	return temp;
}

SDL_Surface* ResizeSurface(SDL_Surface* surface, int wid, int hei){
	SDL_Surface* temp = EmptySurface(surface, wid, hei);

	double dw = double(wid)/surface->w;
	double dh = double(hei)/surface->h;

	for (int y = 0; y < temp->h; y++){
		for (int x = 0; x < temp->w; x++){
			SetPixel(temp, x, y, GetPixel(surface,int(x/dw),int(y/dh)));
		}
	}

	return temp;
}

SDL_Surface* ScaleRegion(SDL_Surface* surface, SDL_Rect rect){
	//Note: For some really wild behavior, change the step scalar ( x * dw ) & (y * dh ) to division rather than multiplication.
	SDL_Surface* temp = EmptySurface(surface);

	double dw = double(rect.w) / surface->w;
	double dh = double(rect.h) / surface->h;

	for (int y = 0; y < temp->h; y++){
		for (int x = 0; x < temp->w; x++){
			SetPixel(temp, x, y, GetPixel(surface, int(rect.x + ( x * dw)), int(rect.y + ( y * dh))));
		}
	}

	return temp;
}

//----------------------------------------------------------------------------------------------------------------------//
//												   Kernel Functions:													//
//----------------------------------------------------------------------------------------------------------------------//

void Kernel3x3::Normalize(){
	double sum = 0;

	//Find sum of all values within matrix
	for (int col = 0; col < 3; col++){
		for (int row = 0; row < 3; row++){
			sum += val[col][row];
		}
	}

	//Divide each value by the sum
	for (int col = 0; col < 3; col++){
		for (int row = 0; row < 3; row++){
			val[col][row] = val[col][row] / sum;
		}
	}
}

void Kernel3x3::Scale(double amount){
	for (int col = 0; col < 3; col++){
		for (int row = 0; row < 3; row++){
			val[col][row] *= amount;
		}
	}
}

void ApplyKernel(SDL_Surface* srcSurface, SDL_Surface* dstSurface, Kernel3x3 kernel, flag edge){
	for (int y = 0; y < srcSurface->h; y++){
		for (int x = 0; x < srcSurface->w; x++){
			//Create temporary variables
			UMatrix3x3	r, g, b;				//Color matrix (stores initial color values)
			DMatrix3x3	nR, nG, nB;				//New values after kernel multiplication
			double addR, addG, addB;			//Sums of all values after kernel operation
			addR = 0; addG = 0; addB = 0;

			//Fill matrices with image color values
			for (int col = 0; col < 3; col++){
				for (int row = 0; row < 3; row++){
					//Store color values
					int locX = x - 1 + row;
					int locY = y - 1 + col;
					Uint32 color;

					//Check if position is an edge
					if (locX < 0 || locX > srcSurface->w-1 || locY < 0 || locY > srcSurface->h-1){
						switch (edge){
							case EDGE_CROP_WHITE:
								color = SDL_MapRGB(srcSurface->format, 255, 255, 255);
								SDL_GetRGB(color, srcSurface->format, &r.val[col][row], &g.val[col][row], &b.val[col][row]);
								break;

							case EDGE_CROP_BLACK:
								color = SDL_MapRGB(srcSurface->format, 0, 0, 0);
								SDL_GetRGB(color, srcSurface->format, &r.val[col][row], &g.val[col][row], &b.val[col][row]);
								break;

							case EDGE_EXTEND:
								if (locX < 0) locX = 0;
								else if (locX > srcSurface->w-1) locX = srcSurface->w-1;
								else if (locY < 0) locY = 0;
								else if (locY > srcSurface->h-1) locY = srcSurface->h-1;

								color = GetPixel(srcSurface, locX, locY);
								SDL_GetRGB(color, srcSurface->format, &r.val[col][row], &g.val[col][row], &b.val[col][row]);
								break;

							case EDGE_WRAP:
								if (locX < 0) locX = srcSurface->w-1;
								else if (locX > srcSurface->w-1) locX = 0;
								else if (locY < 0) locY = srcSurface->h-1;
								else if (locY > srcSurface->h-1) locY = 0;

								color = GetPixel(srcSurface, locX, locY);
								SDL_GetRGB(color, srcSurface->format, &r.val[col][row], &g.val[col][row], &b.val[col][row]);
								break;
						}
					}
					else{
						color = GetPixel(srcSurface, locX, locY);
						SDL_GetRGB(color, srcSurface->format, &r.val[col][row], &g.val[col][row], &b.val[col][row]);
					}

					//Multiply color values by normalized kernel
					nR.val[col][row] = r.val[col][row] * kernel.val[col][row];
					nG.val[col][row] = g.val[col][row] * kernel.val[col][row];
					nB.val[col][row] = b.val[col][row] * kernel.val[col][row];

					//Add sums of new values
					addR = addR + nR.val[col][row];
					addG = addG + nG.val[col][row];
					addB = addB + nB.val[col][row];
				}
			}

			//Wrap calculated values between 0 and 255
			if (addR > 255)	addR = 255; else if (addR < 0)	addR = 0;
			if (addG > 255)	addG = 255; else if (addG < 0)	addG = 0;
			if (addB > 255)	addB = 255; else if (addB < 0)	addB = 0;

			Uint32 newCol = SDL_MapRGB(dstSurface->format, Uint8(addR), Uint8(addG), Uint8(addB));
			SetPixel(dstSurface, x, y, newCol);
		}
	}
}

SDL_Surface* FilterSurface(SDL_Surface* surface, flag method, flag edge){
	Kernel3x3		kernel;
	SDL_Surface*	temp = EmptySurface(surface);

	switch (method){
		case FILTER_BLUR:
			kernel.Blur();
			ApplyKernel(surface, temp, kernel, edge);
			break;

		case FILTER_SHARPEN:
			kernel.Sharpen();
			ApplyKernel(surface, temp, kernel, edge);
			break;

		case FILTER_EDGE:
			kernel.Edge();
			ApplyKernel(surface, temp, kernel, edge);
			break;

		case FILTER_EMBOSS:
			kernel.Emboss();
			ApplyKernel(surface, temp, kernel, edge);
			break;

		case FILTER_NONE:
			break;
	}

	return temp;
}

SDL_Surface* FilterBlur(SDL_Surface* surface, int passes, flag edge){
	SDL_Surface* post = EmptySurface(surface);

	for (int a = 0; a < passes; a++){
		post = FilterSurface(surface, FILTER_BLUR, edge);
		surface = CopySurface(post);
	}

	return surface;
}

SDL_Surface* FilterOffset(SDL_Surface* surface, int x, int y){
	SDL_Surface* temp = EmptySurface(surface);

	for (int cy = 0; cy < surface->h; cy++){
		for (int cx = 0; cx < surface->w; cx++){
			int targetx, targety;

			if (cx + x < 0)
				targetx = surface->w - (x + cx);
			else if (cy + x > surface->w - 1)
				targetx = cx + x - surface->w;
			else
				targetx = cx + x;

			if (cy + y < 0)
				targety = surface->h - (y + cy);
			else if (cy + y > surface->h - 1)
				targety = cy + y - surface->h;
			else
				targety = cy + y;

			Uint32 color = GetPixel(surface, targetx, targety);
			SetPixel(temp, cx, cy, color);
		}
	}

	return temp;
}

SDL_Surface* FlipHorizontal(SDL_Surface* surface){
	return FilterOffset(surface, -(surface->w-1), 0);
}

SDL_Surface* FlipVertical(SDL_Surface* surface){
	return FilterOffset(surface, 0, -(surface->h-1));
}

//----------------------------------------------------------------------------------------------------------------------//
//											    Image Adjustment Functions:												//
//----------------------------------------------------------------------------------------------------------------------//

void Grayscale(SDL_Surface* surface, flag method){
	switch (method){
	case GRAY_AVERAGE:
		for (int y = 0; y < surface->h; y++){
			for (int x = 0; x < surface->w; x++){
				Uint32 col = GetPixel(surface, x, y);
				Uint8 r, g, b, a; int val;

				SDL_GetRGBA(col, surface->format, &r, &g, &b, &a);
				val = (int(r) + int(g) + int(b)) / 3;

				Uint32 newCol = SDL_MapRGBA(surface->format, Uint8(val), Uint8(val), Uint8(val), a);
				SetPixel(surface, x, y, newCol);
			}
		}
		break;

	case GRAY_LUMINANCE:
		for (int y = 0; y < surface->h; y++){
			for (int x = 0; x < surface->w; x++){
				Uint32 col = GetPixel(surface, x, y);
				Uint8 r, g, b, a; double val;

				SDL_GetRGBA(col, surface->format, &r, &g, &b, &a);
				val = (int(r)*0.72) + (int(g)*0.21) + (int(b)*0.07);

				Uint32 newCol = SDL_MapRGBA(surface->format, Uint8(val), Uint8(val), Uint8(val), a);
				SetPixel(surface, x, y, newCol);
			}
		}
		break;

	case GRAY_LIGHTNESS:
		//(min(R, G, B) + max(R, G, B)) / 2
		for (int y = 0; y < surface->h; y++){
			for (int x = 0; x < surface->w; x++){
				Uint32 col = GetPixel(surface, x, y);
				Uint8 r, g, b, a; double val;

				SDL_GetRGBA(col, surface->format, &r, &g, &b, &a);
				val = (min(int(r), min(int(g), int(b))) + max(int(r), max(int(g), int(b)))) / 2;

				Uint32 newCol = SDL_MapRGBA(surface->format, Uint8(val), Uint8(val), Uint8(val), a);
				SetPixel(surface, x, y, newCol);
			}
		}
		break;

	case GRAY_RED:
		for (int y = 0; y < surface->h; y++){
			for (int x = 0; x < surface->w; x++){
				Uint32 col = GetPixel(surface, x, y);
				Uint8 r, g, b, a; int val;

				SDL_GetRGBA(col, surface->format, &r, &g, &b, &a);
				val = int(r);

				Uint32 newCol = SDL_MapRGBA(surface->format, Uint8(val), Uint8(val), Uint8(val), a);
				SetPixel(surface, x, y, newCol);
			}
		}
		break;

	case GRAY_GREEN:
		for (int y = 0; y < surface->h; y++){
			for (int x = 0; x < surface->w; x++){
				Uint32 col = GetPixel(surface, x, y);
				Uint8 r, g, b, a; int val;

				SDL_GetRGBA(col, surface->format, &r, &g, &b, &a);
				val = int(g);

				Uint32 newCol = SDL_MapRGBA(surface->format, Uint8(val), Uint8(val), Uint8(val), a);
				SetPixel(surface, x, y, newCol);
			}
		}
		break;

	case GRAY_BLUE:
		for (int y = 0; y < surface->h; y++){
			for (int x = 0; x < surface->w; x++){
				Uint32 col = GetPixel(surface, x, y);
				Uint8 r, g, b, a; int val;

				SDL_GetRGBA(col, surface->format, &r, &g, &b, &a);
				val = int(b);

				Uint32 newCol = SDL_MapRGBA(surface->format, Uint8(val), Uint8(val), Uint8(val), a);
				SetPixel(surface, x, y, newCol);
			}
		}
		break;
	}
}

void Desaturate(SDL_Surface* surface, double value){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 col = GetPixel(surface, x, y);
			Uint8 r, g, b, a; int average, dR, dG, dB;

			SDL_GetRGBA(col, surface->format, &r, &g, &b, &a);

			//Calculate average at pixel, as well as difference between calculated average and the original value per color channel
			average = (int(r) + int(g) + int(b)) / 3;
			dR = average - int(r); dG = average - int(g); dB = average - int(b);

			r = Uint8(r + (value*dR)); g = Uint8(g + (value*dG)); b = Uint8(b + (value*dB));

			if (r < 0) r = 0; else if (r > 255) r = 255;
			if (g < 0) g = 0; else if (g > 255) g = 255;
			if (b < 0) b = 0; else if (b > 255) b = 255;

			Uint32 newCol = SDL_MapRGBA(surface->format, r, g, b, a);
			SetPixel(surface, x, y, newCol);
		}
	}
}

void Threshold(SDL_Surface* surface, flag channel, int max){
	switch (channel){
		case CHANNEL_RED:
			for (int y = 0; y < surface->h; y++){
				for (int x = 0; x < surface->w; x++){
					Uint32 col = GetPixel(surface, x, y);
					Uint8 r, g, b; int val;

					SDL_GetRGB(col, surface->format, &r, &g, &b);
					if (int(r) < max) val = 0; else if (int(r) >= max) val = 255;

					Uint32 newCol = SDL_MapRGB(surface->format, Uint8(val), Uint8(val), Uint8(val));
					SetPixel(surface, x, y, newCol);
				}
			}
			break;

		case CHANNEL_GREEN:
			for (int y = 0; y < surface->h; y++){
				for (int x = 0; x < surface->w; x++){
					Uint32 col = GetPixel(surface, x, y);
					Uint8 r, g, b; int val;

					SDL_GetRGB(col, surface->format, &r, &g, &b);
					if (int(g) < max) val = 0; else if (int(g) >= max) val = 255;

					Uint32 newCol = SDL_MapRGB(surface->format, Uint8(val), Uint8(val), Uint8(val));
					SetPixel(surface, x, y, newCol);
				}
			}
			break;

		case CHANNEL_BLUE:
			for (int y = 0; y < surface->h; y++){
				for (int x = 0; x < surface->w; x++){
					Uint32 col = GetPixel(surface, x, y);
					Uint8 r, g, b; int val;

					SDL_GetRGB(col, surface->format, &r, &g, &b);
					if (int(b) < max) val = 0; else if (int(b) >= max) val = 255;

					Uint32 newCol = SDL_MapRGB(surface->format, Uint8(val), Uint8(val), Uint8(val));
					SetPixel(surface, x, y, newCol);
				}
			}
			break;
	}
}

void ReplaceColor(SDL_Surface* surface, SDL_Color srcColor, SDL_Color dstColor, int threshold){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 color = GetPixel(surface, x, y); SDL_Color values;
			SDL_GetRGBA(color, surface->format, &values.r, &values.g, &values.b, &values.a);

			if (values.r > srcColor.r - threshold && values.r < srcColor.r + threshold &&
				values.g > srcColor.g - threshold && values.g < srcColor.g + threshold &&
				values.b > srcColor.b - threshold && values.b < srcColor.b + threshold)
				SetPixel(surface, x, y, SDL_MapRGBA(surface->format, dstColor.r, dstColor.g, dstColor.b, dstColor.a));
		}
	}
}

void Interlace(SDL_Surface* surface, int amount){
	for (int y = 0; y < surface->h/amount; y++){
		for (int x = 0; x < surface->w/amount; x++){
			SetPixel(surface, x*amount, y*amount, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
			if(hasPoint(surface,CreateVec2i((x*amount)+amount/2,(y*amount)+amount/2)) != false ) 
				SetPixel(surface, (x*amount)+amount/2, (y*amount)+amount/2, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
		}
	}
}

void Interlace(SDL_Surface* surface, int amount, SDL_Color color){
	for (int y = 0; y < surface->h / amount; y++){
		for (int x = 0; x < surface->w / amount; x++){
			SetPixel(surface, x*amount, y*amount, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
			if (hasPoint(surface, CreateVec2i((x*amount) + amount / 2, (y*amount) + amount / 2)) != false)
				SetPixel(surface, (x*amount) + amount/2, (y*amount) + amount/2, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
		}
	}
}

void Interlace(SDL_Surface* surface, int amount, SDL_Color color, flag style){
	switch (style){
		case IL_DEFAULT:			//IL_DEFAULT and IL_WEAVE perform same operation, this case layout is known as 'falling through' (notice no break statement under IL_DEFAULT case)
		case IL_WEAVE:
			for (int y = 0; y < surface->h / amount; y++){
				for (int x = 0; x < surface->w / amount; x++){
					SetPixel(surface, x*amount, y*amount, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
					if (hasPoint(surface, CreateVec2i((x*amount) + amount / 2, (y*amount) + amount / 2)) != false)
						SetPixel(surface, (x*amount) + amount / 2, (y*amount) + amount / 2, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
				}
			}
			break;
		case IL_HORIZONTAL:
			for (int y = 0; y < surface->h / amount; y++){
				for (int x = 0; x < surface->w; x++){
					SetPixel(surface, x, y*amount + amount / 2, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
				}
			}
			break;
		case IL_VERTICAL:
			for (int x = 0; x < surface->w / amount; x++){
				for (int y = 0; y < surface->h; y++){
					SetPixel(surface, x*amount + amount/2, y, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
				}
			}
			break;
		case IL_DOTTED:
			for (int y = 0; y < surface->h / amount; y++){
				for (int x = 0; x < surface->w / amount; x++){
					SetPixel(surface, x*amount + amount/2, y*amount + amount/2, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
				}
			}
			break;
	}
}

void Interlace(SDL_Surface* surface, int amount, SDL_Color color, flag style, flag blend){
	switch (style){
	case IL_DEFAULT:			//IL_DEFAULT and IL_WEAVE perform same operation, this case layout is known as 'falling through' (notice no break statement under IL_DEFAULT case)
	case IL_WEAVE:
		for (int y = 0; y < surface->h / amount; y++){
			for (int x = 0; x < surface->w / amount; x++){
				Uint32 col = GetPixel(surface, x*amount, y*amount); SDL_Color rgba;
				SDL_GetRGBA(col, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

				switch (blend){
					case BLEND_DEFAULT:
						SetPixel(surface, x*amount, y*amount, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));

						if (hasPoint(surface, CreateVec2i((x*amount) + amount / 2, (y*amount) + amount / 2)) != false)
							SetPixel(surface, (x*amount) + amount / 2, (y*amount) + amount / 2, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
						break;

					case BLEND_ADD:
						SetPixel(surface, x*amount, y*amount, SDL_MapRGBA(surface->format, color.r + rgba.r <= 255 ? color.r + rgba.r : 255, 
							color.g + rgba.g <= 255 ? color.g + rgba.g : 255, 
							color.b + rgba.b <= 255 ? color.b + rgba.b : 255, 
							color.a + rgba.a <= 255 ? color.a + rgba.a : 255));

						if (hasPoint(surface, CreateVec2i((x*amount) + amount / 2, (y*amount) + amount / 2)) != false)
							SetPixel(surface, (x*amount) + amount / 2, (y*amount) + amount / 2, SDL_MapRGBA(surface->format, color.r + rgba.r <= 255 ? color.r + rgba.r : 255, 
							color.g + rgba.g <= 255 ? color.g + rgba.g : 255,
							color.b + rgba.b <= 255 ? color.b + rgba.b : 255,
							color.a + rgba.a <= 255 ? color.a + rgba.a : 255));
						break;

					case BLEND_SUBTRACT:
						SetPixel(surface, x*amount, y*amount, SDL_MapRGBA(surface->format, color.r - rgba.r >= 0 ? color.r - rgba.r : 0,
							color.g - rgba.g >= 0 ? color.g - rgba.g : 0,
							color.b - rgba.b >= 0 ? color.b - rgba.b : 0,
							color.a - rgba.a >= 0 ? color.a - rgba.a : 0));

						if (hasPoint(surface, CreateVec2i((x*amount) + amount / 2, (y*amount) + amount / 2)) != false)
							SetPixel(surface, (x*amount) + amount / 2, (y*amount) + amount / 2, SDL_MapRGBA(surface->format, color.r - rgba.r >= 0 ? color.r - rgba.r : 0,
							color.g - rgba.g >= 0 ? color.g - rgba.g : 0,
							color.b - rgba.b >= 0 ? color.b - rgba.b : 0,
							color.a - rgba.a >= 0 ? color.a - rgba.a : 0));
						break;

						/*
					case BLEND_MULTIPLY:
						SetPixel(surface, x*amount, y*amount, SDL_MapRGBA(surface->format, color.r * rgba.r <= 255 ? color.r * rgba.r : 255,
							color.g * rgba.g <= 255 ? color.g * rgba.g : 255,
							color.b * rgba.b <= 255 ? color.b * rgba.b : 255,
							color.a * rgba.a <= 255 ? color.a * rgba.a : 255));

						if (hasPoint(surface, CreateVec2i((x*amount) + amount / 2, (y*amount) + amount / 2)) != false)
							SetPixel(surface, (x*amount) + amount / 2, (y*amount) + amount / 2, SDL_MapRGBA(surface->format, color.r * rgba.r <= 255 ? color.r * rgba.r : 255,
							color.g * rgba.g <= 255 ? color.g * rgba.g : 255,
							color.b * rgba.b <= 255 ? color.b * rgba.b : 255,
							color.a * rgba.a <= 255 ? color.a * rgba.a : 255));
						break;

					case BLEND_DIVIDE:
						SetPixel(surface, x*amount, y*amount, SDL_MapRGBA(surface->format, color.r > 0 ? color.r/rgba.r : 0, color.g > 0 ? color.g/rgba.g:0, color.b > 0 ? color.b/rgba.b : 0, color.a > 0 ? color.a/rgba.a : 0));

						if (hasPoint(surface, CreateVec2i((x*amount) + amount / 2, (y*amount) + amount / 2)) != false)
							SetPixel(surface, (x*amount) + amount / 2, (y*amount) + amount / 2, SDL_MapRGBA(surface->format, color.r > 0 ? color.r/rgba.r : 0, color.g > 0 ? color.g/rgba.g : 0, color.b > 0 ? color.b/rgba.b : 0, color.a > 0 ? color.a/rgba.a : 0));
						break;
						*/
				}
			}
		}
		break;
	case IL_HORIZONTAL:
		for (int y = 0; y < surface->h / amount; y++){
			for (int x = 0; x < surface->w; x++){
				Uint32 col = GetPixel(surface, x, y*amount + amount/2); SDL_Color rgba;
				SDL_GetRGBA(col, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

				switch (blend){
					case BLEND_DEFAULT:
						SetPixel(surface, x, y*amount + amount / 2, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
						break;

					case BLEND_ADD:
						SetPixel(surface, x, y*amount + amount / 2, SDL_MapRGBA(surface->format, color.r + rgba.r <= 255 ? color.r + rgba.r : 255, 
							color.g + rgba.g <= 255 ? color.g + rgba.g : 255,
							color.b + rgba.b <= 255 ? color.b + rgba.b : 255,
							color.a + rgba.a <= 255 ? color.a + rgba.a : 255));
						break;

					case BLEND_SUBTRACT:
						SetPixel(surface, x, y*amount + amount / 2, SDL_MapRGBA(surface->format, color.r - rgba.r >= 0 ? color.r - rgba.r : 0,
							color.g - rgba.g >= 0 ? color.g - rgba.g : 0,
							color.b - rgba.b >= 0 ? color.b - rgba.b : 0,
							color.a - rgba.a >= 0 ? color.a - rgba.a : 0));
						break;
				}
			}
		}
		break;
	case IL_VERTICAL:
		for (int x = 0; x < surface->w / amount; x++){
			for (int y = 0; y < surface->h; y++){
				Uint32 col = GetPixel(surface, x*amount + amount/2, y); SDL_Color rgba;
				SDL_GetRGBA(col, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

				switch (blend){
					case BLEND_DEFAULT:
						SetPixel(surface, x*amount + amount / 2, y, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
						break;
					case BLEND_ADD:
						SetPixel(surface, x*amount + amount / 2, y, SDL_MapRGBA(surface->format, color.r + rgba.r <= 255 ? color.r + rgba.r : 255,
							color.g + rgba.g <= 255 ? color.g + rgba.g : 255,
							color.b + rgba.b <= 255 ? color.b + rgba.b : 255,
							color.a + rgba.a <= 255 ? color.a + rgba.a : 255));
						break;
					case BLEND_SUBTRACT:
						SetPixel(surface, x*amount + amount / 2, y, SDL_MapRGBA(surface->format, color.r - rgba.r >= 0 ? color.r - rgba.r : 0,
							color.g - rgba.g >= 0 ? color.g - rgba.g : 0,
							color.b - rgba.b >= 0 ? color.b - rgba.b : 0,
							color.a - rgba.a >= 0 ? color.a - rgba.a : 0));
						break;
				}
			}
		}
		break;
	case IL_DOTTED:
		for (int y = 0; y < surface->h / amount; y++){
			for (int x = 0; x < surface->w / amount; x++){
				Uint32 col = GetPixel(surface, x*amount + amount/2, y*amount + amount / 2); SDL_Color rgba;
				SDL_GetRGBA(col, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

				switch (blend){
					case BLEND_DEFAULT:
						SetPixel(surface, x*amount + amount / 2, y*amount + amount / 2, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
						break;
					case BLEND_ADD:
						SetPixel(surface, x*amount + amount / 2, y*amount + amount / 2, SDL_MapRGBA(surface->format, color.r + rgba.r <= 255 ? color.r + rgba.r : 255,
							color.g + rgba.g <= 255 ? color.g + rgba.g : 255,
							color.b + rgba.b <= 255 ? color.b + rgba.b : 255,
							color.a + rgba.a <= 255 ? color.a + rgba.a : 255));
						break;
					case BLEND_SUBTRACT:
						SetPixel(surface, x*amount + amount / 2, y*amount + amount / 2, SDL_MapRGBA(surface->format, color.r - rgba.r >= 0 ? color.r - rgba.r : 0,
							color.g - rgba.g >= 0 ? color.g - rgba.g : 0,
							color.b - rgba.b >= 0 ? color.b - rgba.b : 0,
							color.a - rgba.a >= 0 ? color.a - rgba.a : 0));
						break;
				}
			}
		}
		break;
	}
}

void Posterize(SDL_Surface* surface, int colors){
	//'colors' describes the max number of colors per channel
	int margin = 256 / colors;

	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 value = GetPixel(surface, x, y); SDL_Color rgba;
			SDL_GetRGBA(value, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

			bool foundR = false, foundG = false, foundB = false;

			//Find closest palette
			for (int m = 0; m < colors; m++){
				double dvalue = m*margin;
				double nvalue = m < colors-1 ? (m + 1)*margin : 256;

				if (rgba.r >= dvalue && rgba.r < nvalue){
					if (foundR != true){
						rgba.r = Uint8(m*margin+(margin/2));
						foundR = true;
					}
				}

				if (rgba.g >= dvalue && rgba.g < nvalue){
					if (foundG != true){
						rgba.g = Uint8(m*margin + (margin / 2));
						foundG = true;
					}
				}
					

				if (rgba.b >= dvalue && rgba.b < nvalue){
					if (foundB != true){
						rgba.b = Uint8(m*margin + (margin / 2));
						foundB = true;
					}
				}
			}

			if (rgba.r < 0) rgba.r = 0; else if (rgba.r > 255) rgba.r = 255;
			if (rgba.g < 0) rgba.g = 0; else if (rgba.g > 255) rgba.g = 255;
			if (rgba.b < 0) rgba.b = 0; else if (rgba.b > 255) rgba.b = 255;

			if (foundR != true || foundG != true || foundB != true)
				cout << "Failed to calculate at X: " << x << " Y: " << y << endl;

			SetPixel(surface, x, y, SDL_MapRGBA(surface->format, rgba.r, rgba.g, rgba.b, rgba.a));
		}
	}
}

void Posterize(SDL_Surface* surface, int colors, flag style){
	//'colors' describes the number of colors per channel
	int margin = 256 / colors;

	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 value = GetPixel(surface, x, y); SDL_Color rgba;
			SDL_GetRGBA(value, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

			bool foundR = false, foundG = false, foundB = false;

			//Find closest palette
			for (int m = 0; m < colors; m++){
				double dvalue = m*margin;
				double nvalue = m < colors - 1 ? (m + 1)*margin : 256;

				if (rgba.r >= dvalue && rgba.r < nvalue){
					if (foundR != true){
						rgba.r = Uint8(m*margin + (style != POST_NONE ? margin / 2 : 0));
						foundR = true;
					}
				}

				if (rgba.g >= dvalue && rgba.g < nvalue){
					if (foundG != true){
						rgba.g = Uint8(m*margin + (style != POST_NONE ? margin / 2 : 0));
						foundG = true;
					}
				}


				if (rgba.b >= dvalue && rgba.b < nvalue){
					if (foundB != true){
						rgba.b = Uint8(m*margin + (style != POST_NONE ? margin / 2 : 0));
						foundB = true;
					}
				}
			}

			if (rgba.r < 0) rgba.r = 0; else if (rgba.r > 255) rgba.r = 255;
			if (rgba.g < 0) rgba.g = 0; else if (rgba.g > 255) rgba.g = 255;
			if (rgba.b < 0) rgba.b = 0; else if (rgba.b > 255) rgba.b = 255;

			if (foundR != true || foundG != true || foundB != true)
				cout << "Failed to calculate at X: " << x << " Y: " << y << endl;

			SetPixel(surface, x, y, SDL_MapRGBA(surface->format, rgba.r, rgba.g, rgba.b, rgba.a));
		}
	}

	if (style == POST_BW){
		Desaturate(surface, 1.0);
	}
	else if (style == POST_HIGH_C){
		AdjustContrast(surface, 80);
	}
}

SDL_Surface* Erode(SDL_Surface* surface){
	//Erode and Dilate functions use premise found here: http://blog.ostermiller.org/dilate-and-erode

	//Image has to be converted to binary using 'Threshold' before this function can be used
	SDL_Surface* temp = CopySurface(surface);
	//SDL_Surface* temp = SDL_CreateRGBSurface(NULL, surface->w, surface->h, 32, 0, 0, 0, 0);;

	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			UMatrix3x3 neighbors;

			//Fill matrix with local color values
			for (int col = 0; col < 3; col++){
				for (int row = 0; row < 3; row++){
					int locX = x - 1 + row;
					int locY = y - 1 + col;

					Uint32 color; Uint8 g, b;

					if (locX < 0 || locX > surface->w - 1 || locY < 0 || locY > surface->h - 1)
						color = GetPixel(surface, x, y);
					else
						color = GetPixel(surface, locX, locY);
					
					SDL_GetRGB(color, surface->format, &neighbors.val[col][row], &g, &b);
				}
			}

			//Check center value. If pixel is black,
			if (neighbors.val[1][1] == 0){
				//Check pixel values in all directions. If any neighbors are white, fill [1][1] with white
				if (int(neighbors.val[0][1]) != 0 || neighbors.val[1][0] != 0 || neighbors.val[1][2] != 0 || neighbors.val[2][1] != 0){
					Uint32 newCol = SDL_MapRGB(surface->format, 255, 255, 255);
					SetPixel(temp, x, y, newCol);
				}
			}
		}
	}
	//Dilate should turn on any pixel that is touching a pixel in the north, east, south, or west direction (no diagonals) that is already turned on in the input.
	return temp;
}

SDL_Surface* Dilate(SDL_Surface* surface){
	//Erode and Dilate functions use premise found here: http://blog.ostermiller.org/dilate-and-erode

	//Image has to be converted to binary using 'Threshold' before this function can be used
	SDL_Surface* temp = CopySurface(surface);
	//SDL_Surface* temp = SDL_CreateRGBSurface(NULL, surface->w, surface->h, 32, 0, 0, 0, 0);;

	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			UMatrix3x3 neighbors;

			//Fill matrix with local color values
			for (int col = 0; col < 3; col++){
				for (int row = 0; row < 3; row++){
					int locX = x - 1 + row;
					int locY = y - 1 + col;

					Uint32 color; Uint8 g, b;

					if (locX < 0 || locX > surface->w - 1 || locY < 0 || locY > surface->h - 1)
						color = GetPixel(surface, x, y);
					else
						color = GetPixel(surface, locX, locY);

					SDL_GetRGB(color, surface->format, &neighbors.val[col][row], &g, &b);
				}
			}

			//Check center value. If pixel isn't black,
			if (neighbors.val[1][1] != 0){
				//Check pixel values in all directions. If any neighbors are black, fill [1][1] with black
				if (int(neighbors.val[0][1]) == 0 || neighbors.val[1][0] == 0 || neighbors.val[1][2] == 0 || neighbors.val[2][1] == 0){
					Uint32 newCol = SDL_MapRGB(surface->format, 0, 0, 0);
					SetPixel(temp, x, y, newCol);
				}
			}
		}
	}
	//Dilate should turn on any pixel that is touching a pixel in the north, east, south, or west direction (no diagonals) that is already turned on in the input.
	return temp;
}

SDL_Surface* Trace(SDL_Surface* surface, int max, SDL_Color Col){
	Grayscale(surface, GRAY_AVERAGE);
	Threshold(surface, CHANNEL_RED, max);

	SDL_Surface* temp = EmptySurface(surface);

	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			UMatrix3x3 neighbors;

			//Fill matrix with local color values
			for (int col = 0; col < 3; col++){
				for (int row = 0; row < 3; row++){
					int locX = x - 1 + row;
					int locY = y - 1 + col;

					Uint32 color; Uint8 g, b;

					if (locX < 0 || locX > surface->w-1 || locY < 0 || locY > surface->h-1)
						color = SDL_MapRGBA(surface->format, 1, 1, 1, 255);
					else
						color = GetPixel(surface, locX, locY); 
					
					SDL_GetRGB(color, surface->format, &neighbors.val[col][row], &g, &b);
				}
			}

			//Check center value. If pixel isn't black,
			if (neighbors.val[1][1] != 0){
				//Check pixel values in all directions. If any neighbors are black, fill [1][1] with color
				if (int(neighbors.val[0][1]) == 0 || neighbors.val[1][0] == 0 || neighbors.val[1][2] == 0 || neighbors.val[2][1] == 0){
					Uint32 newCol = SDL_MapRGB(surface->format, Col.r, Col.g, Col.b);
					SetPixel(temp, x, y, newCol);
				}
			}
		}
	}

	return temp;
}

void Invert(SDL_Surface* surface){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 col = GetPixel(surface, x, y);
			Uint8 r, g, b, a; int nR, nG, nB;
			SDL_GetRGBA(col, surface->format, &r, &g, &b, &a);

			nR = int(a) - int(r);
			nG = int(a) - int(g);
			nB = int(a) - int(b);

			Uint32 newCol = SDL_MapRGBA(surface->format, Uint8(nR), Uint8(nG), Uint8(nB), a);
			SetPixel(surface, x, y, newCol);
		}
	}
}

void AdjustRed(SDL_Surface* surface, int offset){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 col = GetPixel(surface, x, y);

			Uint8 red, green, blue, alpha; int nR;
			SDL_GetRGBA(col, surface->format, &red, &green, &blue, &alpha);

			nR = int(red) + offset;
			if (nR > 255) nR = 255; else if (nR < 0) nR = 0;

			Uint32 newCol = SDL_MapRGBA(surface->format, Uint8(nR), green, blue, alpha);
			SetPixel(surface, x, y, newCol);
		}
	}
}

void AdjustGreen(SDL_Surface* surface, int offset){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 col = GetPixel(surface, x, y);

			Uint8 red, green, blue, alpha; int nG;
			SDL_GetRGBA(col, surface->format, &red, &green, &blue, &alpha);

			nG = int(green) + offset;
			if (nG > 255) nG = 255; else if (nG < 0) nG = 0;

			Uint32 newCol = SDL_MapRGBA(surface->format, red, Uint8(nG), blue, alpha);
			SetPixel(surface, x, y, newCol);
		}
	}
}

void AdjustBlue(SDL_Surface* surface, int offset){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 col = GetPixel(surface, x, y);
			Uint8 red, green, blue, alpha; int nB;
			SDL_GetRGBA(col, surface->format, &red, &green, &blue, &alpha);

			nB = int(blue) + offset;
			if (nB > 255) nB = 255; else if (nB < 0) nB = 0;

			Uint32 newCol = SDL_MapRGBA(surface->format, red, green, Uint8(nB), alpha);
			SetPixel(surface, x, y, newCol);
		}
	}
}

void AdjustMulti(SDL_Surface* surface, int rOffset, int gOffset, int bOffset){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 col = GetPixel(surface, x, y);
			Uint8 red, green, blue, alpha; int nR, nG, nB;
			SDL_GetRGBA(col, surface->format, &red, &green, &blue, &alpha);

			nR = int(red) + rOffset;
			nG = int(green) + gOffset;
			nB = int(blue) + bOffset;

			if (nR > 255) nR = 255; else if (nR < 0) nR = 0;
			if (nG > 255) nG = 255; else if (nG < 0) nG = 0;
			if (nB > 255) nB = 255; else if (nB < 0) nB = 0;

			Uint32 newCol = SDL_MapRGBA(surface->format, Uint8(nR), Uint8(nG), Uint8(nB), alpha);
			SetPixel(surface, x, y, newCol);
		}
	}
}

void AdjustBrightness(SDL_Surface* surface, int amount){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 col = GetPixel(surface, x, y);
			Uint8 red, green, blue, alpha; int nR, nG, nB;
			SDL_GetRGBA(col, surface->format, &red, &green, &blue, &alpha);

			nR = int(red) + amount;
			nG = int(green) + amount;
			nB = int(blue) + amount;

			if (nR > 255) nR = 255; else if (nR < 0) nR = 0;
			if (nG > 255) nG = 255; else if (nG < 0) nG = 0;
			if (nB > 255) nB = 255; else if (nB < 0) nB = 0;

			Uint32 newCol = SDL_MapRGBA(surface->format, Uint8(nR), Uint8(nG), Uint8(nB), alpha);
			SetPixel(surface, x, y, newCol);
		}
	}
}

void AdjustHighlights(SDL_Surface* surface, double amount){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 color = GetPixel(surface, x, y); SDL_Color rgba;
			SDL_GetRGBA(color, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

			//y = c*(x/255)^2    where: y = color offset (addition), c = highlight amount, x = old color
			//Formula by Daniel Falk (myself)

			double r = rgba.r + (amount*pow((double(rgba.r) / 255),2));
			double g = rgba.g + (amount*pow((double(rgba.g) / 255), 2));
			double b = rgba.b + (amount*pow((double(rgba.b) / 255), 2));

			if (r < 0) r = 0; else if (r > 255) r = 255;
			if (g < 0) g = 0; else if (g > 255) g = 255;
			if (b < 0) b = 0; else if (b > 255) b = 255;

			SetPixel(surface, x, y, SDL_MapRGBA(surface->format, Uint8(r), Uint8(g), Uint8(b), rgba.a));
		}
	}
}

void AdjustShadows(SDL_Surface* surface, double amount){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 color = GetPixel(surface, x, y); SDL_Color rgba;
			SDL_GetRGBA(color, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

			//y = c*((x-255)/255)^2			where: y = color offset, c = shadow amount, x = old color value
			//Formula by Daniel Falk (myself)

			double r = rgba.r + (amount*pow(((double(rgba.r)-255)/255), 2));
			double g = rgba.g + (amount*pow(((double(rgba.g)-255 )/255), 2));
			double b = rgba.b + (amount*pow(((double(rgba.b)-255)/255), 2));

			if (r < 0) r = 0; else if (r > 255) r = 255;
			if (g < 0) g = 0; else if (g > 255) g = 255;
			if (b < 0) b = 0; else if (b > 255) b = 255;

			SetPixel(surface, x, y, SDL_MapRGBA(surface->format, Uint8(r), Uint8(g), Uint8(b), rgba.a));
		}
	}
}

void AdjustContrast(SDL_Surface* surface, double amount){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 color = GetPixel(surface, x, y); SDL_Color rgba;
			SDL_GetRGBA(color, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

			//y =c*((x-128)/128)^2			a.k.a newcolor = amount*((oldcolor-128)/128)^2)
			//Formula by Daniel Falk (myself)

			double r, g, b;
			if (rgba.r < 128)
				r = rgba.r - (amount*pow((double(rgba.r) - 128) / 128, 2));
			else
				r = rgba.r + (amount*pow((double(rgba.r) - 128) / 128, 2));

			if (rgba.g < 128)
				g = rgba.g - (amount*pow((double(rgba.g) - 128) / 128, 2));
			else
				g = rgba.g + (amount*pow((double(rgba.g) - 128) / 128, 2));

			if (rgba.b < 128)
				b = rgba.b - (amount*pow((double(rgba.b) - 128) / 128, 2));
			else
				b = rgba.b + (amount*pow((double(rgba.b) - 128) / 128, 2));

			if (r < 0) r = 0; else if (r > 255) r = 255;
			if (g < 0) g = 0; else if (g > 255) g = 255;
			if (b < 0) b = 0; else if (b > 255) b = 255;

			SetPixel(surface, x, y, SDL_MapRGBA(surface->format, Uint8(r), Uint8(g), Uint8(b), rgba.a));
		}
	}
}

void AdjustGamma(SDL_Surface* surface, double amount){
	//Source: http://www.dfstudios.co.uk/articles/programming/image-programming-algorithms/image-processing-algorithms-part-6-gamma-correction/
	//Original Author: Francis G. Loch
	//
	//"The range of values used for gamma will depend on the application, but personally I tend to use a range of 0.01 to 7.99."
	double correction = 1 / amount;

	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			Uint32 color = GetPixel(surface, x, y); SDL_Color rgba;
			SDL_GetRGBA(color, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

			double r = 255 * pow(double(rgba.r) / 255, correction);
			double g = 255 * pow(double(rgba.g) / 255, correction);
			double b = 255 * pow(double(rgba.b) / 255, correction);

			if (r < 0) r = 0; else if (r > 255) r = 255;
			if (g < 0) g = 0; else if (g > 255) g = 255;
			if (b < 0) b = 0; else if (b > 255) b = 255;

			SetPixel(surface, x, y, SDL_MapRGBA(surface->format, Uint8(r), Uint8(g), Uint8(b), rgba.a));
		}
	}
}

void AdjustTemperature(SDL_Surface* surface, double kelvin, double strength){
	//Code translated from http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
	//Credit to Tanner Helland for conversion math

	if (kelvin < 1000) kelvin = 1000; else if (kelvin > 40000) kelvin = 40000;

	double temp = kelvin / 100;
	double r, g, b, calc;

	//Red
	if (temp <= 66)
		r = 255;
	else{
		//'Note: the R-squared value for this approximation is .988
		calc = temp - 60;
		calc = 329.698727446 * pow(calc, -0.1332047592);
		r = calc;

		if (r < 0) r = 0; else if (r > 255) r = 255;
	}

	//Green
	if (temp <= 66){
		//'Note: the R-squared value for this approximation is .996
		calc = temp;
		calc = 99.4708025861 * log(calc) - 161.1195681661;
		g = calc;

		if (g < 0) g = 0; else if (g > 255) g = 255;
	}
	else{
		//'Note: the R-squared value for this approximation is .987
		calc = temp - 60;
		calc = 288.1221695283 * pow(calc, -0.0755148492);
		g = calc;

		if (g < 0) g = 0; else if (g > 255) g = 255;
	}

	//Blue
	if (temp >= 66)
		b = 255;
	else if (temp <= 19)
		b = 0;
	else{
		//'Note: the R-squared value for this approximation is .998
		calc = temp - 10;
		calc = 138.5177312231 * log(calc) - 305.0447927307;

		b = calc;

		if (b < 0) b = 0; else if (b > 255) b = 255;
	}

	r *= strength; g *= strength; b *= strength;

	AdjustMulti(surface,int(r), int(g), int(b));
}

SDL_Surface* TemperatureSpectrum(double kelvinA, double kelvinB, int height){
	SDL_Surface* temp = EmptySurface(kelvinB - kelvinA, height);

	for (int x = 0; x < kelvinB - kelvinA; x++){
		SDL_Color color = getTemperatureColor(x + kelvinA);

		for (int y = 0; y < height; y++){
			SetPixel(temp, x, y, SDL_MapRGBA(temp->format, color.r, color.g, color.b, 255));
		}
	}

	return temp;
}

//----------------------------------------------------------------------------------------------------------------------//
//											         Drawing Functions:												    //
//----------------------------------------------------------------------------------------------------------------------//

void DrawLine(SDL_Surface* surface, Vec2i srcPos, Vec2i dstPos, SDL_Color srcCol, SDL_Color dstCol){
	double dr = 0, dg = 0, db = 0, da = 0;
	double dx = dstPos.x - srcPos.x; double dy = dstPos.y - srcPos.y;
	double length = SDL_sqrt(dx*dx + dy*dy);

	dx /= length; dy /= length;

	//Determine whether line is a gradient or not, and if so, determine value to add per iteration
	bool gradient = true;
	if (srcCol.r == dstCol.r && srcCol.g == dstCol.g && srcCol.b == dstCol.b && srcCol.a == dstCol.a)
		gradient = false;

	if (gradient != false){
		dr = (dstCol.r - srcCol.r) / length; dg = (dstCol.g - srcCol.g) / length;
		db = (dstCol.b - srcCol.b) / length; da = (dstCol.a - srcCol.a) / length;
	}

	for (int a = 0; a < length; a++){
		Vec2d out(srcPos.x + (dx*a), srcPos.y + (dy*a));

		if (hasPoint(surface, CreateVec2i(int(out.x), int(out.y))) != false){
			if (gradient != false)
				SetPixel(surface, int(out.x), int(out.y), SDL_MapRGBA(surface->format, Uint8(srcCol.r + (dr*a)), Uint8(srcCol.g + (dg*a)), Uint8(srcCol.b + (db*a)), Uint8(srcCol.a + (da*a))));
			else
				SetPixel(surface, int(out.x), int(out.y), SDL_MapRGBA(surface->format, srcCol.r, srcCol.g, srcCol.b, srcCol.a));
		}
	}
}

void DrawCircle(SDL_Surface* surface, Vec2i pos, double radius, SDL_Color outline, SDL_Color fill, bool filled, bool dotted){
	//7 == 5 ? 4 : 3		// evaluates to 3, since 7 is not equal to 5.
	//7 == 5 + 2 ? 4 : 3    // evaluates to 4, since 7 is equal to 5+2.
	//5 > 3 ? a : b			// evaluates to the value of a, since 5 is greater than 3.
	//a > b ? a : b			// evaluates to whichever is greater, a or b. 

	int oldtime = SDL_GetTicks();

	if (dotted == true && filled == true) filled = false;

	if (filled != false){
		for (double rad = 0; rad < radius; rad++){
			for (double ang = 0; ang < 360; ang += rad > 0 ? 16 / rad : 1){						//The angle step is determined by function: p = (16 / rad)
				int nx = int(pos.x + cos(ang)*rad), ny = int(pos.y + sin(ang)*rad);

				if (hasPoint(surface, CreateVec2i(nx,ny)) != false) SetPixel(surface,nx,ny,SDL_MapRGBA(surface->format,outline.r,outline.g,outline.b,outline.a));

				for (int a = -1; a <= 1; a += 2){
					if (hasPoint(surface, CreateVec2i(nx + a, ny)) != false) SetPixel(surface, nx + a, ny, SDL_MapRGBA(surface->format, outline.r, outline.g, outline.b, outline.a));
					
					if (hasPoint(surface, CreateVec2i(nx, ny + a)) != false) SetPixel(surface, nx, ny + a, SDL_MapRGBA(surface->format, outline.r, outline.g, outline.b, outline.a));
				}
			}
		}
	}
	else{
		for (double ang = 0; ang < 360; dotted == true ? ang += 2 : ang += 2 / double(radius)){
			int nx = int(pos.x + (cos(ang)*radius)), ny = int(pos.y + (sin(ang)*radius));

			if (hasPoint(surface, CreateVec2i(nx, ny)) != false) SetPixel(surface, int(nx), int(ny), SDL_MapRGBA(surface->format, outline.r, outline.g, outline.b, outline.a));
		}
	}

	cout << "Time elapsed: " << SDL_GetTicks() - oldtime << endl;
}

void DrawRect(SDL_Surface* surface, Vec2i srcPos, Vec2i dstPos, SDL_Color outline, SDL_Color fill, bool filled){
	int wid = dstPos.x - srcPos.x;
	int hei = dstPos.y - srcPos.y;

	for (int y = 0; y < abs(hei); y++){
		for (int x = 0; x < abs(wid); x++){
			if (x == 0 || y == 0 || x == abs(wid) - 1 || y == abs(hei) - 1){
				if (hasPoint(surface, CreateVec2i(srcPos.x + x, srcPos.y + y)) != false)
					SetPixel(surface, srcPos.x + x, srcPos.y + y, SDL_MapRGBA(surface->format, outline.r, outline.g, outline.b, outline.a));
			}
			else{
				if (hasPoint(surface, CreateVec2i(srcPos.x + x, srcPos.y + y)) != false && filled != false)
					SetPixel(surface, srcPos.x + x, srcPos.y + y, SDL_MapRGBA(surface->format, fill.r, fill.g, fill.b, fill.a));
			}
		}
	}
}

void GradientRadial(SDL_Surface* surface, Vec2i pos, double radius, SDL_Color srcCol, SDL_Color dstCol){
	double dr, dg, db, da;

	//Determine whether circle is a gradient or not, and if so, determine value to add per iteration
	bool gradient = true;
	if (srcCol.r == dstCol.r && srcCol.g == dstCol.g && srcCol.b == dstCol.b && srcCol.a == dstCol.a)
		gradient = false;

	if (gradient != false){
		dr = (dstCol.r - srcCol.r) / radius; dg = (dstCol.g - srcCol.g) / radius;
		db = (dstCol.b - srcCol.b) / radius; da = (dstCol.a - srcCol.a) / radius;
	}

	Fill(surface, dstCol);
	
	for (double rad = 0; rad < radius; rad++){
		for (double ang = 0; ang < 360; ang += rad > 0 ? 16 / rad : 1){						//The angle step is determined by function: p = (16 / rad)
			int nx = int(pos.x + cos(ang)*rad), ny = int(pos.y + sin(ang)*rad);

			if (hasPoint(surface, CreateVec2i(nx, ny)) != false) SetPixel(surface, nx, ny, SDL_MapRGBA(surface->format, srcCol.r+Uint8(dr*rad), srcCol.g+Uint8(dg*rad), srcCol.b+Uint8(db*rad), srcCol.a+Uint8(da*rad)));

			for (int a = -1; a <= 1; a += 2){
				if (hasPoint(surface, CreateVec2i(nx + a, ny)) != false) SetPixel(surface, nx + a, ny, SDL_MapRGBA(surface->format, srcCol.r + Uint8(dr*rad), srcCol.g + Uint8(dg*rad), srcCol.b + Uint8(db*rad), srcCol.a + Uint8(da*rad)));

				if (hasPoint(surface, CreateVec2i(nx, ny + a)) != false) SetPixel(surface, nx, ny + a, SDL_MapRGBA(surface->format, srcCol.r + Uint8(dr*rad), srcCol.g + Uint8(dg*rad), srcCol.b + Uint8(db*rad), srcCol.a + Uint8(da*rad)));
			}
		}
	}
}

void GradientLinear(Vec2i srcPos, Vec2i dstPos, SDL_Color srcCol, SDL_Color dstCol){

}

void Fill(SDL_Surface* surface, SDL_Color color){
	for (int y = 0; y < surface->h; y++){
		for (int x = 0; x < surface->w; x++){
			SetPixel(surface, x, y, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------//
//												   Graphing Functions:													//
//----------------------------------------------------------------------------------------------------------------------//

void DrawGridlines(SDL_Surface* surface, SDL_Color color, flag style, int padding, double cellsize, double maxX, double maxY){
	//cellsize should divide evenly into 256
	
	SDL_Rect out = CreateRect(padding, padding, surface->w - (padding * 2), surface->h - (padding * 2));

	double stepX = out.w / cellsize;
	double stepY = out.h / cellsize;

	if (style == GRID_DEFAULT || style == GRID_HORIZONTAL){
		for (int y = 0; y <= stepY; y++){
			DrawLine(surface,
				CreateVec2i(out.x, int(out.y + (y*cellsize))),
				CreateVec2i(out.x + out.w, int(out.y + (y*cellsize))),
				color, color);
		}
	}

	if (style == GRID_DEFAULT || style == GRID_VERTICAL){
		for (int x = 0; x <= stepX; x++){
			DrawLine(surface,
				CreateVec2i(out.x + int(x * cellsize), out.y),
				CreateVec2i(out.x + int(x * cellsize), out.y + out.h),
				color, color);
		}
	}
}

void Histogram(SDL_Surface* srcSurface, SDL_Surface* dstSurface, flag channel, flag style, int padding, int desample){
	//==============================================================================================================//
	//	Notes:																										//
	//--------------------------------------------------------------------------------------------------------------//
	//	Variables:																									//
	//		'srcSurface' represents the original image that you want to extract data from.							//
	//		'dstSurface' represents the target surface that you want the graph drawn to.							//
	//		'channel' is an enum representing the color channel you want to view information about.					//
	//		'padding' is the amount of pixels on each sides that you want to 'pad' the graph						//
	//			between (quite similar to CSS padding).																//
	//		'desample' is the amount of pixels you want to skip in order to increase speed.							//
	//			1 = slowest (no pixels skipped), best accuracy. Higher value = faster, less accurate.				//
	//			I recommend	using powers of 2 for desample amount (ex. 1, 2, 4, 8, etc.);							//
	//______________________________________________________________________________________________________________//

	if (desample < 1) desample = 1;

	double w = 256; //double h = 256; 
	double h = srcSurface->h;
	double range = 256 / w;

	vector<int> steps; steps.resize(256);
	int count = 0;

	//Store data
	for (int y = 0; y < srcSurface->h/desample; y++){
		for (int x = 0; x < srcSurface->w/desample; x++){
			Uint32 color = GetPixel(srcSurface, x*desample, y*desample); SDL_Color rgba;
			SDL_GetRGBA(color, srcSurface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

			int value = 0; //This value changes depending on what we're looking for

			switch (channel){
				case CHANNEL_ALL: value = (rgba.r + rgba.g + rgba.b) / 3; break;
				case CHANNEL_RED: value = rgba.r; break;
				case CHANNEL_GREEN: value = rgba.g; break;
				case CHANNEL_BLUE: value = rgba.b; break;
				default: value = (rgba.r + rgba.g + rgba.b) / 3; break;
			}

			for (int a = 0; a < signed(steps.size()); a++){
				if (value >= double(a*range) && value < double(a*range) + range){
					steps[a]++;
				}
			}
		}
	}

	//Change point color depending on graph type
	Uint32 ptCol; SDL_Color ptRGB; SDL_Color ptRGBa;
	switch (channel){
		case CHANNEL_ALL: ptCol = SDL_MapRGBA(dstSurface->format, 200, 200, 200, 255); 
			ptRGB = CreateColor(200, 200, 200, 255); ptRGBa = CreateColor(200, 200, 200, 50); break;

		case CHANNEL_RED: ptCol = SDL_MapRGBA(dstSurface->format, 200, 0, 0, 255); 
			ptRGB = CreateColor(200, 0, 0, 255); ptRGBa = CreateColor(200, 0, 0, 50); break;

		case CHANNEL_GREEN: ptCol = SDL_MapRGBA(dstSurface->format, 0, 200, 0, 255); 
			ptRGB = CreateColor(0, 200, 0, 255); ptRGBa = CreateColor(0, 200, 0, 50); break;

		case CHANNEL_BLUE: ptCol = SDL_MapRGBA(dstSurface->format, 0, 0, 200, 255); 
			ptRGB = CreateColor(0, 0, 200, 255); ptRGBa = CreateColor(0, 0, 200, 50); break;

		default: ptCol = SDL_MapRGBA(dstSurface->format, 200, 200, 200, 255); 
			ptRGB = CreateColor(200, 200, 200, 255); ptRGBa = CreateColor(200, 200, 200, 50); break;
	}

	//Plot data
	int max = findMax(steps);
	double rangeY = max / h;

	for (int a = 0; a < signed(steps.size()); a++){
		switch (style){
			case GRAPH_DOTTED:
				SetPixel(dstSurface, int(a*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h-((steps[a] / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding)), ptCol);
				break;

			case GRAPH_LINEAR:
				if (a + 1 != steps.size()){
					DrawLine(dstSurface,
						CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h-((steps[a] / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						CreateVec2i(int((a + 1)*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h-((steps[a + 1] / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						ptRGB, ptRGB);
				}
				break;

			case GRAPH_FILLED:
				if (a + 1 != steps.size()){
					DrawLine(dstSurface,
						CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h-((steps[a] / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						CreateVec2i(int((a + 1)*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h-((steps[a + 1] / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						ptRGB, ptRGB);
				}

				//Fill region under point
				DrawLine(dstSurface,
					CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), dstSurface->h-padding),
					CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), dstSurface->h-int(((steps[a] / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
					ptRGBa, ptRGBa);
		}
	}
}

void Histogram(SDL_Surface* srcSurface, SDL_Surface* dstSurface, flag channel, flag style, int padding, int desample, int maxY){
	//==============================================================================================================//
	//	Notes:																										//
	//--------------------------------------------------------------------------------------------------------------//
	//	Variables:																									//
	//		'srcSurface' represents the original image that you want to extract data from.							//
	//		'dstSurface' represents the target surface that you want the graph drawn to.							//
	//		'channel' is an enum representing the color channel you want to view information about.					//
	//		'padding' is the amount of pixels on each sides that you want to 'pad' the graph						//
	//			between (quite similar to CSS padding).																//
	//		'desample' is the amount of pixels you want to skip in order to increase speed.							//
	//			1 = slowest (no pixels skipped), best accuracy. Higher value = faster, less accurate.				//
	//			I recommend	using powers of 2 for desample amount (ex. 1, 2, 4, 8, etc.);							//
	//______________________________________________________________________________________________________________//

	if (desample < 1) desample = 1;

	double w = 256; //double h = 256; 
	double h = srcSurface->h;
	double range = 256 / w;

	vector<int> steps; steps.resize(256);
	int count = 0;

	//Store data
	for (int y = 0; y < srcSurface->h / desample; y++){
		for (int x = 0; x < srcSurface->w / desample; x++){
			Uint32 color = GetPixel(srcSurface, x*desample, y*desample); SDL_Color rgba;
			SDL_GetRGBA(color, srcSurface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);

			int value = 0; //This value changes depending on what we're looking for

			switch (channel){
			case CHANNEL_ALL: value = (rgba.r + rgba.g + rgba.b) / 3; break;
			case CHANNEL_RED: value = rgba.r; break;
			case CHANNEL_GREEN: value = rgba.g; break;
			case CHANNEL_BLUE: value = rgba.b; break;
			default: value = (rgba.r + rgba.g + rgba.b) / 3; break;
			}

			for (int a = 0; a < signed(steps.size()); a++){
				if (value >= double(a*range) && value < double(a*range) + range){
					steps[a]++;
				}
			}
		}
	}

	//Change point color depending on graph type
	Uint32 ptCol; SDL_Color ptRGB; SDL_Color ptRGBa;
	switch (channel){
	case CHANNEL_ALL: ptCol = SDL_MapRGBA(dstSurface->format, 200, 200, 200, 255);
		ptRGB = CreateColor(200, 200, 200, 255); ptRGBa = CreateColor(200, 200, 200, 50); break;

	case CHANNEL_RED: ptCol = SDL_MapRGBA(dstSurface->format, 200, 0, 0, 255);
		ptRGB = CreateColor(200, 0, 0, 255); ptRGBa = CreateColor(200, 0, 0, 50); break;

	case CHANNEL_GREEN: ptCol = SDL_MapRGBA(dstSurface->format, 0, 200, 0, 255);
		ptRGB = CreateColor(0, 200, 0, 255); ptRGBa = CreateColor(0, 200, 0, 50); break;

	case CHANNEL_BLUE: ptCol = SDL_MapRGBA(dstSurface->format, 0, 0, 200, 255);
		ptRGB = CreateColor(0, 0, 200, 255); ptRGBa = CreateColor(0, 0, 200, 50); break;

	default: ptCol = SDL_MapRGBA(dstSurface->format, 200, 200, 200, 255);
		ptRGB = CreateColor(200, 200, 200, 255); ptRGBa = CreateColor(200, 200, 200, 50); break;
	}

	//Plot data
	//int max = findMax(steps);
	int max = maxY;
	double rangeY = max / h;

	for (int a = 0; a < signed(steps.size()); a++){
		switch (style){
		case GRAPH_DOTTED:
			if (steps[a] <= maxY)
				SetPixel(dstSurface, int(a*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h - ((steps[a] / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding)), ptCol);
			else
				SetPixel(dstSurface, int(a*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h - ((maxY / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding)), ptCol);
			break;

		case GRAPH_LINEAR:
			if (a + 1 != steps.size()){
				if (steps[a] <= maxY){
					DrawLine(dstSurface,
						CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h - ((steps[a] / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						CreateVec2i(int((a + 1)*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h - (((steps[a + 1] > maxY ? maxY : steps[a+1]) / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						ptRGB, ptRGB);
				}
				else{
					DrawLine(dstSurface,
						CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h - ((maxY / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						CreateVec2i(int((a + 1)*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h - (((steps[a + 1] > maxY ? maxY : steps[a + 1]) / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						ptRGB, ptRGB);
				}
			}
			break;

		case GRAPH_FILLED:
			if (a + 1 != steps.size()){
				if (steps[a] <= maxY){
					DrawLine(dstSurface,
						CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h - ((steps[a] / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						CreateVec2i(int((a + 1)*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h - (((steps[a + 1] > maxY ? maxY : steps[a + 1]) / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						ptRGB, ptRGB);
				}
				else{
					DrawLine(dstSurface,
						CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h - ((maxY / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						CreateVec2i(int((a + 1)*((dstSurface->w - (padding * 2)) / w) + padding), int(dstSurface->h - (((steps[a + 1] > maxY ? maxY : steps[a + 1]) / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
						ptRGB, ptRGB);
				}
			}

			//Fill region under point
			if (steps[a] <= maxY){
				DrawLine(dstSurface,
					CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), dstSurface->h - padding),
					CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), dstSurface->h - int(((steps[a] / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
					ptRGBa, ptRGBa);
			}
			else{
				DrawLine(dstSurface,
					CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), dstSurface->h - padding),
					CreateVec2i(int(a*((dstSurface->w - (padding * 2)) / w) + padding), dstSurface->h - int(((maxY / rangeY)*((dstSurface->h - (padding * 2)) / h) + padding))),
					ptRGBa, ptRGBa);
			}
		}
	}
}

void FFT(SDL_Surface* source, SDL_Surface* destination, double samples){

}

//----------------------------------------------------------------------------------------------------------------------//
//											    Color Conversion Functions:												//
//----------------------------------------------------------------------------------------------------------------------//

void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v)
{
	//https://www.cs.rit.edu/~ncs/color/t_convert.html

	// r,g,b values are from 0 to 1
	// h = [0,360], s = [0,1], v = [0,1]
	//		if s == 0, then h = -1 (undefined)

	float minv, maxv, delta;
	minv = min(min(r, g), b);
	maxv = max(max(r, g), b);
	
	*v = maxv;						// v
	delta = maxv - minv;

	if (maxv != 0)
		*s = delta / maxv;			// s
	else {							// r = g = b = 0
									// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if (r == maxv)
		*h = (g - b) / delta;		// between yellow & magenta
	else if (g == maxv)
		*h = 2 + (b - r) / delta;	// between cyan & yellow
	else
		*h = 4 + (r - g) / delta;	// between magenta & cyan

	*h *= 60;						// degrees
	if (*h < 0) *h += 360;
}

void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
{
	int i;
	float f, p, q, t;

	if (s == 0) {
		*r = *g = *b = v;		// achromatic (grey)
		return;
	}

	h /= 60;			// sector 0 to 5
	i = int(floor(h));
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));

	switch (i) {
		case 0:
			*r = v; *g = t; *b = p;
			break;

		case 1:
			*r = q; *g = v; *b = p;
			break;

		case 2:
			*r = p; *g = v; *b = t;
			break;

		case 3:
			*r = p; *g = q; *b = v;
			break;

		case 4:
			*r = t; *g = p; *b = v;
			break;

		default:
			*r = v; *g = p; *b = q;
			break;
	}
}

//======================================================================================================================//
//	Current bugs, fixes, and feature ideas:																				//
//----------------------------------------------------------------------------------------------------------------------//
//	Known Bugs:																											//
//		9/28/2016: function FilterOffset() does not work for negative Y values.											//
//		9/29/2016: function DrawCircle() has wierd spacing issues. Look at dotted = true, filled = false.				//
//																														//
//	Tasks:																												//
//		# Implement within function Histogram() ability to view HSV values rather than RGB.								//
//		# Add ability to detect object within image.																	//
//		# Add simple method to add text to surface at specified position.												//
//		# Add segmentation functionality to binary surface.																//
//		# Implement binary surface type.																				//
//		# Implement overloads for filtering specified regions rather than full surfaces.								//
//		# Integrate kiss_FFT for 2D Fourier transform.																	//
//			% Implement high pass filtering, low pass filtering, and band pass filtering.								//
//		# Use sinusoids for gradient functions rather than linear interpolation.										//
//																														//
//	Changelog:																											//
//		9/27/2016 (added): Implement function Erode()																	//
//		9/28/2016 (fixed): functions Erode() and Dilate() don't take image edge into account.							//
//		9/28/2016 (fixed): function GradientRadial() only draws single gradient line to surface.						//
//		9/29/2016 (fixed): function DrawLine() only drawing single line with constant slope.							//
//		9/29/2016 (fixed): function Histogram() draws graph upside down.												//
//		9/29/2016 (added): Implement function Histogram()																//
//		9/29/2016 (added): Add ability to draw hollow rects.															//
//		9/29/2016 (added): Implement function DrawCircle()																//
//		10/1/2016: Replaced header comments for new functions. Cleaned up some code a bit.								//
//		10/1/2016: Added function Interlace() with overloads and styling. Looks cool.									//
//		10/2/2016 (fixed): function Posterize() inaccurately calculating pixel steps.									//
//		10/4/2016: Began work on GUI system.																			//
//		10/7/2016: Added function AdjustTemperature() as well as several assist functions to handle visualization.		//
//______________________________________________________________________________________________________________________//