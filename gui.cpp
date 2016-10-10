#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Transform.h"
#include "GUI.h"

using namespace std;
using namespace transform;

void GUI_Element::Create(string Title, SDL_Rect dstRect){
	rect.x = dstRect.x; rect.y = dstRect.y;
	rect.w = dstRect.w; rect.h = dstRect.h;
}

SDL_Surface* GUI_Window::gen_gfxW(){
	//Window graphical media generated to following vertical template:
	//1st 3x3: window title and control region style
	//2nd 3x3: window background style
	//3rd 3x3: window container style

	SDL_Surface* temp = EmptySurface(3 * gfxSize, 9 * gfxSize);

	for (int y = 0; y < 9; y++){
		for (int x = 0; x < 3; x++){
			for (int py = 0; py < gfxSize; py++){
				for (int px = 0; px < gfxSize; px++){
					if (y == 0 || y == 3 || y == 6){				//Tops
						if (x == 0){		//Top left
							switch (y){
							case 0: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 210, 210, 210, 255)); break;
							case 3: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 150, 150, 150, 255)); break;
							case 6: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 150, 150, 150, 255)); break;
							}
						}
						else if (x == 1){	//Top center
							switch (y){
							case 0: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 200, 200, 200, 255)); break;
							case 3: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 180, 180, 180, 255)); break;
							case 6: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 180, 180, 180, 255)); break;
							}
						}
						else if (x == 2){	//Top right
							switch (y){
							case 0: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 190, 190, 190, 255)); break;
							case 3: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 210, 210, 210, 255)); break;
							case 6: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 210, 210, 210, 255)); break;
							}
						}
					}
					else if (y == 1 || y == 4 || y == 7){			//Centers
						if (x == 0){		//Center left
							switch (y){
							case 1: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 200, 200, 200, 255)); break;
							case 4: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 180, 180, 180, 255)); break;
							case 7: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 180, 180, 180, 255)); break;
							}
						}
						else if (x == 1){	//Center
							switch (y){
							case 1: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 220, 220, 220, 255)); break;
							case 4: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 120, 120, 120, 255)); break;
							case 7: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 120, 120, 120, 255)); break;
							}
						}
						else if (x == 2){	//Center right
							switch (y){
							case 1: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 200, 200, 200, 255)); break;
							case 4: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 180, 180, 180, 255)); break;
							case 7: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 180, 180, 180, 255)); break;
							}
						}
					}
					else{			//Bottoms
						if (x == 0){		//Bottom left
							switch (y){
							case 2: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 190, 190, 190, 255)); break;
							case 5: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 210, 210, 210, 255)); break;
							case 8: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 210, 210, 210, 255)); break;
							}
						}
						else if (x == 1){	//Bottom center
							switch (y){
							case 2: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 200, 200, 200, 255)); break;
							case 5: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 180, 180, 180, 255)); break;
							case 8: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 180, 180, 180, 255)); break;
							}
						}
						else if (x == 2){	//Bottom right
							switch (y){
							case 2: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 210, 210, 210, 255)); break;
							case 5: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 150, 150, 150, 255)); break;
							case 8: SetPixel(temp, px + (x*gfxSize), py + (y*gfxSize), SDL_MapRGBA(temp->format, 150, 150, 150, 255)); break;
							}
						}
					}

				}
			}
		}
	}

	IMG_SavePNG(temp, "gfxW.png");

	return temp;
}

SDL_Surface* GUI_Window::gen_gfxB(){
	SDL_Surface* temp = EmptySurface(1, 1);
	return temp;
}

SDL_Surface* GUI_Window::gen_gfxS(){
	SDL_Surface* temp = EmptySurface(1, 1);
	return temp;
}

SDL_Surface* GUI_Window::gen_gfxM(){
	SDL_Surface* temp = EmptySurface(1, 1);
	return temp;
}

SDL_Surface* GUI_Window::gen_gfxRB(){
	SDL_Surface* temp = EmptySurface(1, 1);
	return temp;
}

SDL_Surface* GUI_Window::gen_gfxCB(){
	SDL_Surface* temp = EmptySurface(1, 1);
	return temp;
}

GUI_Window::GUI_Window(){
	rect = CreateRect(0, 0, gfxSize * 2, gfxSize * 4);
	id = count; count++;
	mouse.x = 0;
	mouse.y = 0;
}

bool GUI_Window::mouseHoverRegion(SDL_Rect region){
	if (mouse.x >= region.x && mouse.y >= region.y && mouse.x <= region.x + region.w && mouse.y <= region.y + region.h)
		return true;
	else
		return false;
}

bool GUI_Window::mouseHoverHeader(){
	return mouseHoverRegion(CreateRect(rect.x, rect.y, rect.w, rect.y + headerH));
}

bool GUI_Window::mouseHoverWindow(){
	return mouseHoverRegion(CreateRect(rect.x, rect.y, rect.w, rect.h));
}

void GUI_Window::Create(const char* wTitle, SDL_Renderer* renderTarget, int X, int Y, int W, int H){
	//Store defaults
	title = wTitle;
	rect.x = X; rect.y = Y; rect.w = W; rect.h = H;
	gfxSize = 12; gfxKey = CreateColor(0, 255, 255, 255);
	headerH = 24; alpha = 200;

	//Load media
	SDL_Surface* surW = IMG_Load("gfxW.png");
	SDL_Surface* surB = IMG_Load("gfxB.png");
	SDL_Surface* surS = IMG_Load("gfxS.png");
	SDL_Surface* surM = IMG_Load("gfxM.png");
	SDL_Surface* surRB = IMG_Load("gfxRB.png");
	SDL_Surface* surCB = IMG_Load("gfxCB.png");

	//Generate missing media
	if (!surW) surW = gen_gfxW();
	if (!surB) surB = gen_gfxB();
	if (!surS) surS = gen_gfxS();
	if (!surM) surM = gen_gfxM();
	if (!surRB) surRB = gen_gfxRB();
	if (!surCB) surCB = gen_gfxCB();

	//Convert to drawable texture
	GUI_Graphic gfxW, gfxB, gfxS, gfxM, gfxRB, gfxCB;
	gfxW.texture = SDL_CreateTextureFromSurface(renderTarget, surW); gfx.push_back(gfxW);
	gfxB.texture = SDL_CreateTextureFromSurface(renderTarget, surB); gfx.push_back(gfxB);
	gfxS.texture = SDL_CreateTextureFromSurface(renderTarget, surS); gfx.push_back(gfxS);
	gfxM.texture = SDL_CreateTextureFromSurface(renderTarget, surM); gfx.push_back(gfxM);
	gfxRB.texture = SDL_CreateTextureFromSurface(renderTarget, surRB); gfx.push_back(gfxRB);
	gfxCB.texture = SDL_CreateTextureFromSurface(renderTarget, surCB); gfx.push_back(gfxCB);

	//Store rect data
	for (int a = 0; a < gfx.size(); a++){
		int w, h;
		SDL_QueryTexture(gfx[a].texture, NULL, NULL, &w, &h);

		for (int y = 0; y < h / gfxSize; y++){
			for (int x = 0; x < w / gfxSize; x++){
				gfx[a].frames.push_back(CreateRect(x*gfxSize, y*gfxSize, gfxSize, gfxSize));
			}
		}
	}

	cout << gfx[0].frames.size() << endl;

	//Clean up
	SDL_FreeSurface(surW);
	SDL_FreeSurface(surB);
	SDL_FreeSurface(surS);
	SDL_FreeSurface(surM);
	SDL_FreeSurface(surRB);
	SDL_FreeSurface(surCB);
}

void GUI_Window::Update(SDL_Event* targetEvent){
	SDL_GetMouseState(&mouse.x, &mouse.y);

	if (mouseHoverWindow() != false){
		while (SDL_PollEvent(targetEvent) != 0){
			switch (targetEvent->type){
			case SDL_BUTTON_LEFT:
				cout << "gfa" << endl;
				break;
			}
		}
	}
	else if (SDL_MouseButtonEvent().state == SDL_PRESSED){
		if (state == GUI_STATE_RESIZE){
			rect.w = rect.w + (mouse.x - mouseOld.x);
			rect.h = rect.h + (mouse.y - mouseOld.y);
		}
		cout << mouse.x << " " << mouse.y << endl;
	}
	else if (SDL_MouseButtonEvent().state == SDL_RELEASED){
		if (state != GUI_STATE_DEFAULT) state = GUI_STATE_DEFAULT;
	}
}

void GUI_Window::Draw(SDL_Renderer* renderTarget){
	//Fix limitations
	if (headerH < gfxSize * 2)	headerH = gfxSize * 2;
	if (rect.w < gfxSize * 2)	rect.w = gfxSize * 2;
	if (rect.h < gfxSize * 4)	rect.h = gfxSize * 4;

	//Draw window header
	SDL_SetTextureAlphaMod(gfx[0].texture, 255);
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[0], &CreateRect(rect.x, rect.y, gfxSize, gfxSize));								//Top left corner
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[1], &CreateRect(rect.x + gfxSize, rect.y, rect.w - (gfxSize * 2), gfxSize));	//Top center border
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[2], &CreateRect(rect.x + rect.w - gfxSize, rect.y, gfxSize, gfxSize));			//Top right corner
	if (headerH > gfxSize * 2){
		SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[3], &CreateRect(rect.x, rect.y + gfxSize, gfxSize, headerH - (gfxSize * 2)));
		SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[4], &CreateRect(rect.x + gfxSize, rect.y + gfxSize, rect.w - (gfxSize * 2), headerH - (gfxSize * 2)));
		SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[5], &CreateRect(rect.x + rect.w - gfxSize, rect.y + gfxSize, gfxSize, headerH - (gfxSize * 2)));
	}
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[6], &CreateRect(rect.x, rect.y + headerH - gfxSize, gfxSize, gfxSize));
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[7], &CreateRect(rect.x + gfxSize, rect.y + headerH - gfxSize, rect.w - (gfxSize * 2), gfxSize));
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[8], &CreateRect(rect.x + rect.w - gfxSize, rect.y + headerH - gfxSize, gfxSize, gfxSize));

	//Draw main window region
	SDL_SetTextureAlphaMod(gfx[0].texture, alpha);
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[9], &CreateRect(rect.x, rect.y + headerH, gfxSize, gfxSize));								//Top left corner
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[10], &CreateRect(rect.x + gfxSize, rect.y + headerH, rect.w - (gfxSize * 2), gfxSize));	//Top center border
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[11], &CreateRect(rect.x + rect.w - gfxSize, rect.y + headerH, gfxSize, gfxSize));			//Top right corner
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[12], &CreateRect(rect.x, rect.y + headerH + gfxSize, gfxSize, rect.h - headerH - (gfxSize * 2)));
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[13], &CreateRect(rect.x + gfxSize, rect.y + headerH + gfxSize, rect.w - (gfxSize)* 2, rect.h - headerH - (gfxSize * 2)));
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[14], &CreateRect(rect.x + rect.w - gfxSize, rect.y + headerH + gfxSize, gfxSize, rect.h - headerH - (gfxSize * 2)));
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[15], &CreateRect(rect.x, rect.y + rect.h - gfxSize, gfxSize, gfxSize));
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[16], &CreateRect(rect.x + gfxSize, rect.y + rect.h - gfxSize, rect.w - (gfxSize * 2), gfxSize));
	SDL_RenderCopy(renderTarget, gfx[0].texture, &gfx[0].frames[17], &CreateRect(rect.x + rect.w - gfxSize, rect.y + rect.h - gfxSize, gfxSize, gfxSize));
}

int GUI_Window::getX(){
	return rect.x;
}

int GUI_Window::getY(){
	return rect.y;
}

int GUI_Window::getW(){
	return rect.w;
}

int GUI_Window::getH(){
	return rect.h;
}

//Init static members
int GUI_Window::count = 0;

Vec2i GUI_Window::mouse = CreateVec2i(0, 0);
