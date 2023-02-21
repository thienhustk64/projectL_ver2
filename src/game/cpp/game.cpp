#include "game.h"

game::game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	/*IMG_Init(IMG_INIT_PNG)& IMG_INIT_PNG;
	TTF_Init();*/
}

game::~game()
{
}

void game::initWindow()
{
	window = SDL_CreateWindow("WereWolf Deluxo Edition", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
}

void game::initStates()
{
	this->states->push(new menu_state(this->window, this->states));
}

void game::update()
{
	if (!this->states->empty())
	{
		this->states->top()->update();

		if (this->states->top()->getQuit())
		{
			this->states->top()->endState();
			delete this->states->top();
			this->states->pop();
		}
	}
	//Application end
	else
	{
		this->window == NULL;
	}
}

void game::render()
{
	//Render items
	if (!this->states->empty())
		this->states->top()->render();

	this->renderWindow();
}

void game::renderWindow()
{
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}
