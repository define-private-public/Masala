// State of a key on a keyboard

#ifndef M_KEY_STATE_H
#define M_KEY_STATE_H


class MKeyState {
public:
    bool down = false;
    bool isAutoRepeat = false;
    
    // Modifiers
    bool shift = false;
    bool control = false;
    bool alt = false;


    MKeyState() { };
    MKeyState(const MKeyState &other):
        down(other.down)
    { }
    MKeyState &operator= (const MKeyState &other) {
        if (other != *this) {
            // Copy state over
            this->down = other.down;
            this->isAutoRepeat = other.isAutoRepeat;
            this->shift = other.shift;
            this->control = other.control;
            this->alt = other.alt;
        }

        return *this;
    }

    bool operator==(const MKeyState &other) const {
        return ((this->down == other.down) &&
                (this->isAutoRepeat == other.isAutoRepeat) &&
                (this->shift == other.shift) &&
                (this->control == other.control) &&
                (this->alt == other.alt));
    }

    bool operator!=(const MKeyState &other) const {
        return !(*this == other);
    }

    
    inline bool isDown() { return down; }
    inline bool isUp() { return !down; }
};


#endif // M_KEY_STATE_H

