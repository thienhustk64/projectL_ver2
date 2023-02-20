#ifndef STATE_H
#define STATE_H

#include "lib.h"

using namespace std;

class State
{
public:
	State(SDL_Window* window, stack<State*>* states);
	~State();

	const bool& getQuit() const;

	virtual void update() = 0;
	virtual void endState() = 0;
	virtual void render() = 0;
private:
	stack<State*>* states;
	SDL_Window* window;
	bool quit;
};


#endif 