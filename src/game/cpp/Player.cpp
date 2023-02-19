#include "Player.h"

using namespace tinyxml2;
using namespace std;

Player::Player(){
    current_state = "idle";
    health = 100;
    is_hurt       = false;
    index         = 0;
    current = 0;
    is_skill = false;
    is_skill_finish = false;
}

Player::Player(const Player &p){
    x_pos = p.x_pos;
    y_pos = p.y_pos;
    alpha = p.alpha;
    scale = p.scale;
    health = p.health;

    x_initial     = p.x_initial;
    y_initial     = p.y_initial;
    alpha_initial = p.alpha_initial;
    scale_initial = p.scale_initial;

    type     = p.type;
    inverted = p.inverted;
    is_hurt  = is_hurt;
    index    = index;
    is_skill_finish = false;
    is_skill = false;
    current = 0;

    current_state  = p.current_state;
    previous_state = p.previous_state;

    states     = p.states;
    next_state = p.next_state;
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

string Player::getTrigger(){
    return animations[current_state]->getTrigger();
}

int Player::getHealth(){
    return health;
}

bool Player::isInverted(){
    return inverted;
}

bool Player::isHurt(){
    return is_hurt;
}

bool Player::initialize(std::string character, std::string skill, std::string skill_finish, bool player_one, float x_pos, float y_pos){
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

    doc.LoadFile(character.c_str());
    XMLElement* character_node = doc.FirstChildElement( "character");
    
    // position_node  = findChild(root_element, "position");
    XMLElement* position_node = character_node->FirstChildElement("position");
    if(position_node != 0){
        y_initial     = position_node->IntAttribute( "y_pos");
        x_initial     = position_node->IntAttribute( "x_pos");
        alpha_initial = position_node->FloatAttribute( "alpha");
        scale_initial = position_node->FloatAttribute( "scale");

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
        if( animation_node->FirstChildElement("position") != 0){
            int x_pos_animation = animation_node->FirstChildElement("position")->IntAttribute("x_pos");
            animation->setXPos( x_pos_animation);
        }

        if( animation_node->FirstChildElement("trigger") != 0){
            string trigger_value = animation_node->FirstChildElement("trigger")->Attribute( "value");
            animation->setTrigger( trigger_value);
        }
        
        XMLElement* frame_node = animation_node->FirstChildElement("frame");
        while( frame_node != 0){
            int y_initial_animation = 0;
            string path  = frame_node->Attribute("path");
            float width  = frame_node->IntAttribute("width");
            float height = frame_node->IntAttribute("height");
            float delay  = frame_node->FloatAttribute("delay");
            XMLElement* move_node = frame_node->FirstChildElement("move");
            if( move_node != 0){
                y_initial_animation = int(move_node->FloatAttribute( "y_vel")*20);
                std::string y_vel = move_node->Attribute( "y_vel");
                if( y_vel.find("-") == 0 && y_initial_animation > 0){
                    y_initial_animation = y_initial_animation*-1;
                }
            }
            animation->addFrame( path, width, height, delay, y_initial_animation);
            frame_node = frame_node->NextSiblingElement("frame");
        }
        animations.insert( { name, animation});
        animation_node = animation_node->NextSiblingElement("animation");
    }
    doc.Clear();

    // Load Skill
    doc.LoadFile( skill.c_str());
    int x_initial_skill = 0;
    int x_pos_skill = 0;
    int y_pos_skill = 0;
    XMLElement* character_node_skill = doc.FirstChildElement( "character");

    XMLElement* position_node_skill = character_node_skill->FirstChildElement("position");
    if( position_node_skill != 0){
        x_initial_skill = position_node_skill->IntAttribute("x_pos");
        x_pos_skill = position_node_skill->IntAttribute("x_pos");
        y_pos_skill = position_node_skill->IntAttribute("y_pos");
    }
    XMLElement* animation_node_skill = character_node_skill->FirstChildElement("animation");
    if( animation_node_skill != 0){
        string name       = "skill";
        Animation* animation = new Animation(true, true, x_pos_skill, y_pos_skill, x_initial_skill);
        if( character_node_skill->FirstChildElement("trigger") != 0){
            string trigger_skill = character_node_skill->FirstChildElement("trigger")->Attribute( "value");
            animation->setTrigger( trigger_skill);
        }
        XMLElement* frame_node = animation_node_skill->FirstChildElement("frame");
        while( frame_node != 0){
            string path  = frame_node->Attribute("path");
            float width  = frame_node->IntAttribute("width");
            float height = frame_node->IntAttribute("height");
            float delay  = frame_node->FloatAttribute("delay");
            animation->addFrame( path, width, height, delay, 0);
            frame_node = frame_node->NextSiblingElement("frame");
        }
        animations.insert( { name, animation});
    }
    doc.Clear();

    // Load Skill finish
    doc.LoadFile( skill_finish.c_str());
    int x_initial_finish = 0;
    int x_pos_finish = 0;
    int y_pos_finish = 0;
    XMLElement* character_node_finish = doc.FirstChildElement( "character");
    XMLElement* position_node_finish = character_node_finish->FirstChildElement("position");
    if( position_node_finish != 0){
        x_pos_finish = position_node_finish->IntAttribute("x_pos");
        y_pos_finish = position_node_finish->IntAttribute("y_pos")*-1;
    }
    XMLElement* animation_node_finish = character_node_finish->FirstChildElement("animation");
    if( animation_node_finish != 0){
        string name       = "skill_finish";
        Animation* animation = new Animation(true, true, x_pos_finish, y_pos_finish, x_initial_finish);
        if( character_node_finish->FirstChildElement("trigger") != 0){
            string trigger_finish = character_node_finish->FirstChildElement("trigger")->Attribute( "value");
            animation->setTrigger( trigger_finish);
        }
        XMLElement* frame_node = animation_node_finish->FirstChildElement("frame");
        while( frame_node != 0){
            string path  = frame_node->Attribute("path");
            float width  = frame_node->IntAttribute("width");
            float height = frame_node->IntAttribute("height");
            float delay  = frame_node->FloatAttribute("delay");
            animation->addFrame( path, width, height, delay, 0);
            frame_node = frame_node->NextSiblingElement("frame");
        }
        animations.insert( { name, animation});
    }
    doc.Clear();
    return true;
}

void Player::moveXpos( bool forward)
{   
    int move;
    if( forward){
        move = x_initial;
    }else{
        move = x_initial*(-1);
    }
    if(inverted)
        x_pos -= move;
    else
        x_pos += move;
}

void Player::draw( SDL_Renderer* screen){
    current++;
    bool change_state = animations[current_state]->draw( screen, x_pos, y_pos, scale, inverted);
    std::string state_predict;
    if( change_state){
        if( current_state.compare("fireball") == 0){
            current = 0;
            is_skill = true;
            x_pos_skill = x_pos;
            y_pos_skill = y_pos;
        }
        if( next_state[current_state].compare( "previous") == 0){
            state_predict = previous_state;
        }else{
            state_predict = next_state[current_state];
        }
        previous_state = current_state;
        current_state = state_predict;
    }

    if( is_skill){
        bool skill = animations["skill"]->draw( screen, x_pos_skill, y_pos_skill, scale, inverted);
        if( skill){
            is_skill = false;
            is_skill_finish = true;
            x_pos_skill_finish = animations["skill"]->getXFinish();
            y_pos_skill_finish = animations["skill"]->getYFinish();
        }
    }
    if( is_skill_finish){
        bool finish_skill = animations["skill_finish"]->draw( screen, x_pos_skill_finish, y_pos_skill_finish, scale, inverted);
        if( finish_skill){
            is_skill_finish = false;
        }
    }

}

void Player::updateState( std::string trigger){
    if( states.find(current_state)->second.find( trigger) != states.find(current_state)->second.end()){
        if( !(current_state.compare("hurt_low") == 0 && is_hurt)){
            previous_state = current_state;
            current_state = states[ current_state][trigger];
            if( current_state.compare("hurt_low") == 0){
                is_hurt = true;
                health = health - 5;
            }else{
                is_hurt = false;
            }
        }
        
        // animations[current_state]->reset();
    }
}


void Player::DoPlayer( bool is_collison){
    if(!is_collison){
        if( current_state.compare("walk_forward") == 0 || current_state.compare("jump_forward") == 0){
            moveXpos( true);
        }
    }
    if( current_state.compare("walk_back") == 0 || current_state.compare("jump_back") == 0){
        moveXpos( false);
    }

    if( current_state.find( "jump") == 0){
        y_pos = y_pos - animations[current_state]->getYInitial();
    }else{
        if( y_pos + int(animations[current_state]->getHeight()*scale) < GROUND ){
            y_pos += y_initial;
        }
    }
    // printf("%d\n", x_pos);
    if( x_pos < 0){
        x_pos = 0;
    }else if( x_pos > SCREEN_WIDTH){
        x_pos = SCREEN_WIDTH;
    }
}

Collision Player::getCollision(){
    return animations[current_state]->getCollision();
}

bool Player::checkCollision( Collision temp){
    return animations[current_state]->checkCollision( temp);
}