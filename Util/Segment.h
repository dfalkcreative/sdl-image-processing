#ifndef SEGMENT_H
#define SEGMENT_H

struct Polygon{
	std::vector<Vec2i> points;
};

struct Quad{
	Vec2i pos;
	SDL_Rect rect;
	std::vector<Vec2i> points;

	Quad();
	Quad(std::vector<Vec2i> data);
	Quad(std::vector<Vec2i> data, Vec2i position);

	inline SDL_Rect getRect();
};

struct QuadTree{
	std::vector<Quad> quads;
	SDL_Rect rect;

	QuadTree();
	QuadTree(std::vector<Vec2i> data);
	QuadTree(std::vector<Vec2i> data, int indices);

	inline std::vector<Vec2i>* getPoints(int index);

	inline int getX(int index, int item);
	inline int getY(int index, int item);

	inline int getSize();
	inline int getSize(int index);

	inline SDL_Rect getRect();
	inline SDL_Rect getRect(int index);

	inline Vec2i getVec2i(int index, int item);
};

class Segmented{
	private:
		bool isValid;

	public:
		SDL_Surface* surface;

		Segmented();
		Segmented(SDL_Surface* source);
		Segmented(SDL_Surface* source, Uint8 threshold);
};

std::vector<Vec2i> RotateData(std::vector<Vec2i> points);

inline SDL_Rect FindRect(std::vector<Vec2i> points);

inline bool PointExists(std::vector<Vec2i> points, Vec2i point, bool useQuad);

inline bool PointExists(QuadTree tree, Vec2i point);

inline bool PointExists(QuadTree tree, Vec2i point, int* id);

inline bool PointInRect(SDL_Rect rect, Vec2i pt);

inline bool PointInPoly(std::vector<Vec2i> points, Vec2i point, SDL_Rect boundary);

void MapPNG(std::string filename, std::vector<Vec2i> points);

void MapPNG(std::string filename, QuadTree qt, int index);

std::vector<Vec2i> PointTrace(SDL_Surface* surface);

void SearchPolygons(QuadTree tree, int index, Vec2i samples);

//void RemoveNoise(SDL_Surface* surface, std::vector<Vec2i> points, int distance);

#endif