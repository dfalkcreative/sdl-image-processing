#ifndef GUI_H
#define GUI_H

struct GUI_Graphic{
	SDL_Texture*				texture;
	std::vector<SDL_Rect>		frames;
};

struct GUI_Event{
	flag						type;					//Specifies type of GUI_Event
	int							id;						//Specifies element ID of GUI_Event (If applicable)
};

struct GUI_Element{
	static int					count;			//Total object count

	//Properties
	int							id;
	std::vector<SDL_Rect>		states;			//Source rects for drawing
	flag						status;			//Current element states
	SDL_Rect					rect;
	std::string					title;

	//Controls
	bool isVisible;
	bool isActive;

	//Styling (if applicable)
	int margin, padding;
	int shadowAngle;
	int shadowDistance;
	int shadowAlpha;

	void Create(std::string Title, SDL_Rect dstRect);
};

struct GUI_Button : public GUI_Element{
	void Create();
};

struct GUI_RadioButton : public GUI_Element{

};

struct GUI_Checkbox : public GUI_Element{

};

struct GUI_MenuItem : public GUI_Element{

};

struct GUI_Menu : public GUI_Element{

};

struct GUI_Container : public GUI_Element{
	bool isGraphical;

	std::vector<GUI_RadioButton> rbuttons;
	std::vector<GUI_Checkbox> cboxes;

	SDL_Surface* content;
};

class GUI_Window{
private:
	static Vec2i				mouse; Vec2i mouseOld;
	static int					count;
	int							id;
	SDL_Rect					rect;
	const char*					title;
	flag						state;
	Uint8						alpha;

	int							headerH;
	int							gfxSize;				//The size (in pixels) of the individual tiles that 'build' the GUI representation
	SDL_Color					gfxKey;

	std::vector<GUI_Graphic>	gfx;
	std::vector<GUI_Event>		events;

	//Window elements
	std::vector<GUI_Button>		buttons;
	std::vector<GUI_Checkbox>	checkboxes;
	std::vector<GUI_Container>	containers;

	//Generation functions (For use with function 'Create()')
	SDL_Surface*				gen_gfxW();				//This generates required window media if missing
	SDL_Surface*				gen_gfxWB();			//This generates required window button media if missing
	SDL_Surface*				gen_gfxB();				//This generates required button media if missing
	SDL_Surface*				gen_gfxS();				//This generates required slider media if missing
	SDL_Surface*				gen_gfxM();				//This generates required menu media if missing
	SDL_Surface*				gen_gfxRB();			//This generates required radio button media if missing
	SDL_Surface*				gen_gfxCB();			//This generates required checkbox media if missing

public:
	//Constructors
	GUI_Window();

	//Main functions
	void Create(const char* wTitle, SDL_Renderer* renderTarget, int X, int Y, int W, int H);					//Create empty window
	void Retrieve();																							//Get information about status changes to window
	void Update(SDL_Event* targetEvent);																		//Handle window controls
	void Draw(SDL_Renderer* renderTarget);																		//Draw window

	//Add elements
	void addMenu();
	void addMenuItem();
	void addButton();
	void addTextbox();
	void addContainer();			//Add empty container for text, radio buttons, checkboxes, etc.
	void addRadioButton();			//Add radio button to target container
	void addCheckbox();				//Add checkbox to target container
	void addSlider();				//Add slider to window
	void addSurface();				//Add surface to container

	//Data
	int getX(); int getY();
	int getW(); int getH();

	bool mouseHoverRegion(SDL_Rect region);			//Returns true if mouse is over of specified region
	bool mouseHoverWindow();						//Returns true if mouse is over window and/or header
	bool mouseHoverHeader();						//Returns true if mouse is over header

	bool buttonPressed();			//Returns true if button has been pressed on window
	int getButton();				//Returns the id of the button that has been pressed
};

#endif