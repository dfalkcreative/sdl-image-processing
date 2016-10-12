//==========================================================================================//
//								Image Segmentation Class									//
//------------------------------------------------------------------------------------------//
//	Usage:																					//
//		# Method A (Manual):																//
//			1.) Load image as surface in main program using IMG_Load().						//
//				*Recommendation: Duplicate specified image surface to a filtering			//
//				surface	using 'filtered = CopySurface(image)'.								//
//																							//
//			2.) First, blur the surface slightly to remove noise.							//
//				*Recommendation: 3-5 passes using function FilterBlur()						//
//																							//
//			3.) Afterwards, pass the surface through a threshold filter						//
//			to convert to binary image.														//
//				*Recommendation: Max threshold of 100-150 using function Threshold().		//
//																							//
//			4.) Store Vec2i values of surface in a vector array using the					//
//			function PointTrace().															//
//				*Example:																	//
//					std::vector<Vec2i> points = PointTrace(image);							//
//																							//
//			5.) Create a QuadTree to store points array in (for much faster					//
//			search algorithms.)																//
//				*Recommendation: Use an even number for QuadTree indices. I typically		//
//					use values between 8 and 16.											//
//				*Example:																	//
//					QuadTree tree(points, 8);												//
//																							//
//			6.) Pass function SearchPolygons() to filtering surface. 'Samples'				//
//			describes the number of points used to 'sample' each slice of the QuadTree		//
//			to search the points array for polygons. Use a higher number for increased		//
//			accuracy at the expense of performance.											//
//				*Example:																	//
//					SearchPolygons(tree, index, Vec2i( 8, 2 ));								//
//__________________________________________________________________________________________//

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "../Transform.h"
#include "Segment.h"

using namespace std;
using namespace transform;

Quad::Quad(){
	pos = CreateVec2i(0, 0);
	points.clear();
	points.resize(0);
	rect = CreateRect(0, 0, 0, 0);
}

Quad::Quad(vector<Vec2i> data){
	pos = CreateVec2i(0, 0);
	points = data;
	rect = getRect();
}

Quad::Quad(vector<Vec2i> data, Vec2i position){
	pos = position;
	points = data;
	rect = getRect();
}

inline SDL_Rect Quad::getRect(){
	Vec2i iter;
	int minX = points[0].x, minY = points[0].y,
		maxX = minX, maxY = minY;

	for (int a = 0; a < points.size(); a++){
		iter = CreateVec2i(points[a].x, points[a].y);

		if (iter.x < minX)
			minX = iter.x;

		if (iter.x > maxX)
			maxX = iter.x;

		if (iter.y < minY)
			minY = iter.y;

		if (iter.y > maxY)
			maxY = iter.y;
	}

	return CreateRect(minX, minY, maxX, maxY);
}

QuadTree::QuadTree(){
	quads.clear();
	quads.resize(0);
}

QuadTree::QuadTree(vector<Vec2i> data){
	if (data.size() < 4) data.resize(4);

	int step = data.size() / 4; 
	Quad temp;

	for (int a = 0; a < 4; a++){
		//Clear temporary Quad array
		temp.points.clear(); temp.points.resize(0);

		//Add point data per step size
		for (int b = 0; b < step; b++){
			int id = b + (step*a);

			if (data.size() > id)
				temp.points.push_back(data[b + (step*a)]);
		}

		//Update temporary Quad index
		temp.pos = CreateVec2i(a, 0);
		temp.getRect();
		quads.push_back(temp);
	}

	rect = getRect();

	for (int b = 0; b < quads.size(); b++){
		quads[b].rect = quads[b].getRect();
	}
}

QuadTree::QuadTree(vector<Vec2i> data, int indices){
	if (data.size() < indices) data.resize(indices);

	int step = data.size() / indices;
	Quad temp;

	for (int a = 0; a < indices; a++){
		//Clear temporary Quad array
		temp.points.clear(); temp.points.resize(0);

		//Add point data per step size
		for (int b = 0; b < step; b++){
			int id = b + (step*a);

			if (data.size() > id)
				temp.points.push_back(data[id]);
		}

		//Update temporary Quad index
		temp.pos = CreateVec2i(a, 0);
		quads.push_back(temp);
	}

	rect = getRect();

	for (int b = 0; b < quads.size(); b++){
		quads[b].rect = quads[b].getRect();
	}
}

inline vector<Vec2i>* QuadTree::getPoints(int index){
	if (index < quads.size())
		return &quads[index].points;
	else
		return NULL;
}

inline int QuadTree::getSize(){
	int sum;

	for (int a = 0; a < quads.size(); a++){
		sum += quads[a].points.size();
	}

	return sum;
}

inline int QuadTree::getSize(int index){
	if (index < quads.size())
		return quads[index].points.size();
	else
		return 0;
}

inline int QuadTree::getX(int index, int item){
	if (index < quads.size()){
		if (item < quads[index].points.size())
			return quads[index].points[item].x;
		else{
			cout << "Failed to return X value for item " << item << " at index " << index << endl;
			return 0;
		}
	}
	else{
		cout << "Failed to return X value at index " << index << endl;
		return 0;
	}
}

inline int QuadTree::getY(int index, int item){
	if (index < quads.size()){
		if (item < quads[index].points.size())
			return quads[index].points[item].y;
		else{
			cout << "Failed to return Y value for item " << item << " at index " << index << endl;
			return 0;
		}
	}
	else{
		cout << "Failed to return Y value at index " << index << endl;
		return 0;
	}
}

inline Vec2i QuadTree::getVec2i(int index, int item){       
	if (index < quads.size()){
		if (item < quads[index].points.size())
			return quads[index].points[item];
		else{
			cout << "Failed to return item " << item << " at index " << index << endl;
			return Vec2i(0, 0);
		}
	}
	else{
		cout << "Failed to return item at index " << index << endl;
		return Vec2i(0, 0);
	}
}

inline SDL_Rect QuadTree::getRect(){
	Vec2i iter;
	int minX = getX(0, 0), minY = getY(0, 0),
		maxX = minX, maxY = minY;

	for (int index = 0; index < quads.size(); index++){
		for (int a = 0; a < getSize(index); a++){
			iter = CreateVec2i(getX(index, a), getY(index, a));

			if (iter.x < minX)
				minX = iter.x;

			if (iter.x > maxX)
				maxX = iter.x;

			if (iter.y < minY)
				minY = iter.y;

			if (iter.y > maxY)
				maxY = iter.y;
		}
	}

	return CreateRect(minX, minY, maxX, maxY);
}

inline SDL_Rect QuadTree::getRect(int index){
	Vec2i iter;
	int minX = getX(index, 0), minY = getY(index, 0), 
		maxX = minX, maxY = minY;

	for (int a = 0; a < getSize(index); a++){
		iter = CreateVec2i(getX(index, a), getY(index, a));

		if (iter.x < minX)
			minX = iter.x;

		if (iter.x > maxX)
			maxX = iter.x;

		if (iter.y < minY)
			minY = iter.y;

		if (iter.y > maxY)
			maxY = iter.y;
	}

	return CreateRect(minX, minY, maxX, maxY);
}

Segmented::Segmented(){
	surface = EmptySurface(64, 64);
	isValid = false;
}

Segmented::Segmented(SDL_Surface* source){
	surface = CopySurface(source);

	//Grayscale(surface, GRAY_AVERAGE);
	//Threshold(surface, CHANNEL_RED, 150);

	isValid = true;
}

Segmented::Segmented(SDL_Surface* source, Uint8 threshold){
	surface = CopySurface(source);

	Grayscale(surface, GRAY_AVERAGE);
	Threshold(surface, CHANNEL_RED, threshold);

	isValid = true;
}

vector<Vec2i> RotateData(vector<Vec2i> points){

	vector<Vec2i> out;
	return out;
}

inline SDL_Rect FindRect(vector<Vec2i> points){
	Vec2i iter;
	int minX = points[0].x, minY = points[0].y,
		maxX = minX, maxY = minY;

	for (int a = 0; a < points.size(); a++){
		iter = CreateVec2i(points[0].x, points[0].y);

		if (iter.x < minX)
			minX = iter.x;

		if (iter.x > maxX)
			maxX = iter.x;

		if (iter.y < minY)
			minY = iter.y;

		if (iter.y > maxY)
			maxY = iter.y;
	}

	return CreateRect(minX, minY, maxX, maxY);
}

inline int FindIndex(QuadTree tree, int y){
	int index = -1;

	for (int b = 0; b < tree.quads.size(); b++){
		if (y >= tree.quads[b].rect.y && y <= tree.quads[b].rect.h){
			index = b;
			break;
		}
	}

	return index;
}

inline bool PointExists(vector<Vec2i> points, Vec2i point, bool useQuad){
	if (useQuad != false){
		int segments = 8;

		if (points.size() < segments) points.resize(segments);
		QuadTree sub(points, segments);

		int index = -1;

		for (int b = 0; b < sub.quads.size(); b++){
			if (point.y >= sub.quads[b].rect.y && point.y <= sub.quads[b].rect.h){
				index = b;
				break;
			}
		}

		if (index > -1){
			for (int a = 0; a < sub.quads[index].points.size(); a++){
				if (sub.getX(index, a) == point.x && sub.getY(index, a) == point.y){
					return true;
					break;
				}
			}
			return false;
		}
		else
			return false;
	}
	else{
		for (int a = 0; a < points.size(); a++){
			if (point.x == points[a].x && point.y == points[a].y){
				return true;
				break;
			}
		}
		return false;
	}
}

inline bool PointExists(QuadTree tree, Vec2i point){
	int index = -1;

	for (int b = 0; b < tree.quads.size(); b++){
		if (point.y >= tree.quads[b].rect.y && point.y <= tree.quads[b].rect.h){
			index = b;
			break;
		}
	}

	if (index > -1){
		for (int a = 0; a < tree.quads[index].points.size(); a++){
			if (tree.getX(index, a) == point.x && tree.getY(index, a) == point.y){
				return true;
			}
		}
		return false;
	}
	else
		return false;
}

inline bool PointExists(QuadTree tree, Vec2i point, int* id){
	int index = -1;

	for (int b = 0; b < tree.quads.size(); b++){
		if (point.y >= tree.quads[b].rect.y && point.y <= tree.quads[b].rect.h){
			index = b;
			break;
		}
	}

	if (index > -1){
		for (int a = 0; a < tree.quads[index].points.size(); a++){
			if (tree.getX(index, a) == point.x && tree.getY(index, a) == point.y){
				*id = index;
				return true;
			}
		}
		return false;
	}
	else
		return false;
}

inline bool PointInRect(SDL_Rect rect, Vec2i pt){
	//Notes: I might not need to add rect.x to rect.w & rect.y to rect.h
	//Need to test more.

	if (pt.x >= rect.x && pt.y >= rect.y &&
		pt.x <= rect.w && pt.y <= rect.h)
		return true;
	else
		return false;
}

inline bool PointInPoly(vector<Vec2i> points, Vec2i point, SDL_Rect boundary){
	int count = 0;

	for (int x = point.x; x < boundary.x + boundary.w; x++){
		if (PointExists(points, CreateVec2i(point.x + x, point.y), true) != false){
			count++;
		}
	}

	if (count % 2 == 0)
		return false;
	else
		return true;
}

inline bool PointInPoly(QuadTree tree, Vec2i point){
	int count = 0; int id = FindIndex(tree, point.y);

	if (id > -1){
		QuadTree sub(tree.quads[id].points, 8);
		id = FindIndex(sub, point.y);

		for (int x = point.x; x < tree.rect.w; x++){
			if (PointExists(sub.quads[id].points, CreateVec2i(point.x + x, point.y), true) != false){
				count++;
			}
		}

		if (count % 2 == 0)
			return false;
		else
			return true;
	}
	else
		return false;
}

void MapPNG(string filename, vector<Vec2i> points){

}

void MapPNG(string filename, QuadTree tree){

}

void MapPNG(string filename, QuadTree qt, int index){
	SDL_Rect rect = qt.getRect(index);
	SDL_Surface* temp = EmptySurface(rect.w - rect.x, rect.h - rect.y);
	
	for (int a = 0; a < qt.getSize(index); a++){
		if (hasPoint(temp, CreateVec2i(qt.getX(index,a), qt.getY(index,a))) != false)
			SetPixel(temp, qt.getX(index, a), qt.getY(index, a), SDL_MapRGBA(temp->format, 255, 255, 255, 255));
	}

	IMG_SavePNG(temp, filename.c_str());
}

vector<Vec2i> PointTrace(SDL_Surface* surface){
	vector<Vec2i> points;
	points.reserve(int(surface->w * surface->h));

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
					points.push_back(CreateVec2i(x,y));
				}
			}
		}
	}

	return points;
}

void SearchPolygons(QuadTree tree, int index, Vec2i samples){
	SDL_Surface* temp = EmptySurface(tree.rect.w, tree.rect.h);
	SDL_Rect rect = tree.quads[index].rect;

	int w = rect.w - rect.x;
	int h = rect.h - rect.y;

	double dx = w / samples.x;
	double dy = h / samples.y;

	Vec2i loc;

	for (int y = 0; y < samples.y; y++){
		for (int x = 0; x < samples.x; x++){
			loc = CreateVec2i(rect.x + int(x * dx), rect.y + int(y * dy));
			
			if (PointInPoly(tree.quads[index].points, loc) != false){
				SetPixel(temp, loc.x, loc.y, SDL_MapRGBA(temp->format, 0, 0, 255, 255));
			}
			else
				SetPixel(temp, loc.x, loc.y, SDL_MapRGBA(temp->format, 255, 0, 0, 255));
		}
	}

	IMG_SavePNG(temp, "polygons.png");
}




//======================================================================================================================//
//	Current bugs, fixes, and feature ideas:																				//
//----------------------------------------------------------------------------------------------------------------------//
//	Tasks:																												//
//		# To ensure highest accuracy, QuadTree input points in constructor should be divisible by total step size.		//
//			Implement a strategy to avoid losing data.																	//
//		# 
//______________________________________________________________________________________________________________________//