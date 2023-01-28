#include "Player.h"

using namespace tinyxml2;
using namespace std;

Player::Player(){
    current_state = "idle";
    opponent      = NULL;
    is_hurt       = false;
    index         = 0;
}

Player::Player(const Player &p){
    x_pos = p.x_pos;
    y_pos = p.y_pos;
    alpha = p.alpha;
    scale = p.scale;

    x_initial     = p.x_initial;
    y_initial     = p.y_initial;
    alpha_initial = p.alpha_initial;
    scale_initial = p.scale_initial;

    type     = p.type;
    inverted = p.inverted;
    is_hurt  = is_hurt;
    index    = index;

    current_state  = p.current_state;
    previous_state = p.previous_state;

    states     = p.states;
    next_state = p.next_state;
    opponent = p.opponent;
    state = p.state;
}

Player::~Player(){
}

bool Player::operator< (const Player& p) const{
    return this->index < p.index;
}

float Player::getXpos(){
    return x_pos;
}

bool Player::isInverted(){
    return inverted;
}

bool Player::isHurt(){
    return is_hurt;
}

bool Player::initialize(std::string file_name, bool player_one, float x_pos, float y_pos){
    XMLDocument doc;

    if(!player_one)
    {
        state["INVERT"] = true;
        inverted        = true;
    }
    else
    {
        state["INVERT"] = false;
        inverted        = false;
    }

    this->x_pos = x_pos;
    this->y_pos = y_pos;

    doc.LoadFile(file_name.c_str());
    XMLElement* character_node = doc.FirstChildElement( "character");
    
    // position_node  = findChild(root_element, "position");
    XMLElement* position_node = character_node->FirstChildElement("position");
    if(position_node != 0){
        y_initial     = position_node->IntAttribute( "y_pos");
        x_initial     = position_node->IntAttribute( "x_pos");
        alpha_initial = position_node->FloatAttribute( "alpha");
        scale_initial = position_node->FloatAttribute( "scale");

        moveXpos(x_initial);
        this->y_pos += y_initial;
        alpha = alpha_initial;
        scale = scale_initial;
    }

    XMLElement* action_node = character_node->FirstChildElement("action");
    while( action_node != 0){
        string name    = action_node->Attribute( "name");
        string trigger = action_node->Attribute("trigger");
        string from    = action_node->Attribute("from");
        string to      = action_node->Attribute("to");
        
        vector<string> from_tokens    = splitString(from, ", ");
        vector<string> trigger_tokens = splitString(trigger, ", ");

        for(unsigned int j = 0; j < from_tokens.size(); j++)
            for(unsigned int k = 0; k < trigger_tokens.size(); k++)
                states[from_tokens[j]][trigger_tokens[k]] = name;

        next_state[name] = to;

        action_node = action_node->NextSiblingElement("action");
        
    }

    XMLElement* animation_node = character_node->FirstChildElement("animation");
    while( animation_node != 0){
        string name       = animation_node->Attribute("name");
        bool is_combo     = animation_node->BoolAttribute("combo");
        bool is_continual = animation_node->BoolAttribute("continual");
        Animation* animation = new Animation(is_combo, is_continual);
        
        XMLElement* frame_node = animation_node->FirstChildElement("frame");
        while( frame_node != 0){
            string path  = frame_node->Attribute("path");
            float width  = frame_node->IntAttribute("width");
            float height = frame_node->IntAttribute("height");
            float delay  = frame_node->FloatAttribute("delay");
            animation->addFrame( path, width, height, delay);
            frame_node = frame_node->NextSiblingElement("frame");
        }
        animations.insert( { name, animation});
        animation_node = animation_node->NextSiblingElement("animation");
    }
    return true;
}

void Player::moveXpos(float value)
{
    if(inverted)
        x_pos -= value;
    else
        x_pos += value;
}

void Player::draw( SDL_Renderer* screen){
    bool change_state = animations[current_state]->draw( screen, x_pos, y_pos, scale);
    if( change_state){
        current_state = next_state[current_state];
    }
}