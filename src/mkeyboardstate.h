// A collection of keys with state

#ifndef M_KEYBOARD_STATE_H
#define M_KEYBOARD_STATE_H


#include "mkeystate.h"
#include <cstring>


class MKeyboardState {
public:

    MKeyState zero;
    MKeyState one;
    MKeyState two;
    MKeyState three;
    MKeyState four;
    MKeyState five;
    MKeyState six;
    MKeyState seven;
    MKeyState eight;
    MKeyState nine;
    MKeyState a;
    MKeyState b;
    MKeyState c;
    MKeyState d;
    MKeyState e;
    MKeyState f;
    MKeyState g;
    MKeyState h;
    MKeyState i;
    MKeyState j;
    MKeyState k;
    MKeyState l;
    MKeyState m;
    MKeyState n;
    MKeyState o;
    MKeyState p;
    MKeyState q;
    MKeyState r;
    MKeyState s;
    MKeyState t;
    MKeyState u;
    MKeyState v;
    MKeyState w;
    MKeyState x;
    MKeyState y;
    MKeyState z;
    MKeyState F1;
    MKeyState F2;
    MKeyState F3;
    MKeyState F4;
    MKeyState F5;
    MKeyState F6;
    MKeyState F7;
    MKeyState F8;
    MKeyState F9;
    MKeyState F10;
    MKeyState F11;
    MKeyState F12;

    MKeyState escape;
    MKeyState home;
    MKeyState end;
    MKeyState insert;
    MKeyState delete_;      // keyword is taken in C++
    MKeyState minus;
    MKeyState equal;
    MKeyState backspace;
    MKeyState tab;
    MKeyState leftBracket;
    MKeyState rightBracket;
    MKeyState backSlash;
    MKeyState semicolon;
    MKeyState apostrophe;
    MKeyState enter;
    MKeyState comma;
    MKeyState period;
    MKeyState forwardSlash;

    MKeyState up;
    MKeyState down;
    MKeyState left;
    MKeyState right;
    MKeyState pageUp;
    MKeyState pageDown;


    MKeyboardState() { }
    MKeyboardState(const MKeyboardState &other) {
        // Use the assignment operator
        *this = other;
    }
    MKeyboardState &operator= (const MKeyboardState &other) {
        if (other != *this) {
            this->zero = other.zero;
            this->one = other.one;
            this->two = other.two;
            this->three = other.three;
            this->four = other.four;
            this->five = other.five;
            this->six = other.six;
            this->seven = other.seven;
            this->eight = other.eight;
            this->nine = other.nine;
            this->a = other.a;
            this->b = other.b;
            this->c = other.c;
            this->d = other.d;
            this->e = other.e;
            this->f = other.f;
            this->g = other.g;
            this->h = other.h;
            this->i = other.i;
            this->j = other.j;
            this->k = other.k;
            this->l = other.l;
            this->m = other.m;
            this->n = other.n;
            this->o = other.o;
            this->p = other.p;
            this->q = other.q;
            this->r = other.r;
            this->s = other.s;
            this->t = other.t;
            this->u = other.u;
            this->v = other.v;
            this->w = other.w;
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->F1 = other.F1;
            this->F2 = other.F2;
            this->F3 = other.F3;
            this->F4 = other.F4;
            this->F5 = other.F5;
            this->F6 = other.F6;
            this->F7 = other.F7;
            this->F8 = other.F8;
            this->F9 = other.F9;
            this->F10 = other.F10;
            this->F11 = other.F11;
            this->F12 = other.F12;
            this->escape = other.escape;
            this->home = other.home;
            this->end = other.end;
            this->insert = other.insert;
            this->delete_ = other.delete_;
            this->minus = other.minus;
            this->equal = other.equal;
            this->backspace = other.backspace;
            this->tab = other.tab;
            this->leftBracket = other.leftBracket;
            this->rightBracket = other.rightBracket;
            this->backSlash = other.backSlash;
            this->semicolon = other.semicolon;
            this->apostrophe = other.apostrophe;
            this->enter = other.enter;
            this->comma = other.comma;
            this->period = other.period;
            this->forwardSlash = other.forwardSlash;
            this->up = other.up;
            this->down = other.down;
            this->left = other.left;
            this->right = other.right;
            this->pageUp = other.pageUp;
            this->pageDown = other.pageDown;
        }

        return *this;
    }

    bool operator==(const MKeyboardState &other) const {
        bool same = true;
       
        // Check. Them. All...
        same &= this->zero == other.zero;
        same &= this->one == other.one;
        same &= this->two == other.two;
        same &= this->three == other.three;
        same &= this->four == other.four;
        same &= this->five == other.five;
        same &= this->six == other.six;
        same &= this->seven == other.seven;
        same &= this->eight == other.eight;
        same &= this->nine == other.nine;
        same &= this->a == other.a;
        same &= this->b == other.b;
        same &= this->c == other.c;
        same &= this->d == other.d;
        same &= this->e == other.e;
        same &= this->f == other.f;
        same &= this->g == other.g;
        same &= this->h == other.h;
        same &= this->i == other.i;
        same &= this->j == other.j;
        same &= this->k == other.k;
        same &= this->l == other.l;
        same &= this->m == other.m;
        same &= this->n == other.n;
        same &= this->o == other.o;
        same &= this->p == other.p;
        same &= this->q == other.q;
        same &= this->r == other.r;
        same &= this->s == other.s;
        same &= this->t == other.t;
        same &= this->u == other.u;
        same &= this->v == other.v;
        same &= this->w == other.w;
        same &= this->x == other.x;
        same &= this->y == other.y;
        same &= this->z == other.z;
        same &= this->F1 == other.F1;
        same &= this->F2 == other.F2;
        same &= this->F3 == other.F3;
        same &= this->F4 == other.F4;
        same &= this->F5 == other.F5;
        same &= this->F6 == other.F6;
        same &= this->F7 == other.F7;
        same &= this->F8 == other.F8;
        same &= this->F9 == other.F9;
        same &= this->F10 == other.F10;
        same &= this->F11 == other.F11;
        same &= this->F12 == other.F12;
        same &= this->escape == other.escape;
        same &= this->home == other.home;
        same &= this->end == other.end;
        same &= this->insert == other.insert;
        same &= this->delete_ == other.delete_;
        same &= this->minus == other.minus;
        same &= this->equal == other.equal;
        same &= this->backspace == other.backspace;
        same &= this->tab == other.tab;
        same &= this->leftBracket == other.leftBracket;
        same &= this->rightBracket == other.rightBracket;
        same &= this->backSlash == other.backSlash;
        same &= this->semicolon == other.semicolon;
        same &= this->apostrophe == other.apostrophe;
        same &= this->enter == other.enter;
        same &= this->comma == other.comma;
        same &= this->period == other.period;
        same &= this->forwardSlash == other.forwardSlash;
        same &= this->up == other.up;
        same &= this->down == other.down;
        same &= this->left == other.left;
        same &= this->right == other.right;
        same &= this->pageUp == other.pageUp;
        same &= this->pageDown == other.pageDown;

        return same;
    }

    bool operator!=(const MKeyboardState &other) const {
        return !(*this == other);
    }



private:
};


#endif // M_KEYBOARD_STATE_H
