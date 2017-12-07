#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"

#define CURSOR_WIDTH 2

struct _TTF_Font;
struct SDL_Texture;
class UI_element;
class Text;
class Image;
class Button;
class InputBox;
class Window;
class Slider;
class Chrono;

enum event_type
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_RIGHT_CLICK,
	MOUSE_RIGHT_RELEASE,
	MOUSE_LEFT_CLICK,
	MOUSE_LEFT_RELEASE
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void UIDebugDraw();
	// Gui creation functions
	const SDL_Texture* GetAtlas() const;
	Text* createText(char* text, int x, int y, _TTF_Font* font, SDL_Color color = { 255, 255, 255, 255 }, j1Module* callback = nullptr);
	Chrono* createChrono(int x, int y, _TTF_Font* font, SDL_Color color, j1Module* callback = nullptr);
	Image* createImage(int x, int y, SDL_Texture* texture, j1Module* callback = nullptr);
	Image* createImageFromAtlas(int x, int y, SDL_Rect section, j1Module* callback = nullptr);
	//NULL texture to use atlas
	Button* createButton(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, j1Module* callback = nullptr);
	//NULL texture to use atlas
	Button* createSwitch(int x, int y, SDL_Texture* texture, SDL_Rect standbyUnactive, SDL_Rect OnMouseUnactive, SDL_Rect standbyActive, SDL_Rect OnMouseActive, j1Module* callback = nullptr);
	//NULL texture to use atlas
	InputBox* createInputBox(_TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect section, j1Module* callback = nullptr);
	//NULL texture to use atlas
	Window* createWindow(int x, int y, SDL_Texture* texture, SDL_Rect section, j1Module* callback = nullptr);
	//NULL texture to use atlas
	Slider* createSlider(int x, int y, SDL_Texture* texture, SDL_Rect empty, SDL_Rect full, Button* button, _TTF_Font* text_font, SDL_Color text_color, int default_progress = 50, j1Module* callback = nullptr, char* text = "");

public:
	bool UI_Debug = false;
	float UI_scale;
	j1Timer chrono;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
	p2List<UI_element*> UI_elements;
	UI_element* draggingElement = nullptr;
};

#endif // __j1GUI_H__