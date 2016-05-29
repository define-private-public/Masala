#include "mchai.h"
#include "mutil.h"
#include "mframe.h"
#include "msequence.h"
#include "mkeystate.h"
#include "mkeyboardstate.h"
#include "msprite.h"
#include "mglwidget.h"
#include "masalaapp.h"
#include "mtilemap.h"
#include "msoundeffect.h"
#include "chaiscript/chaiscript.hpp"
#include "chaiscript/utility/utility.hpp"
#include <QPointF>
#include <QSizeF>
#include <QRectF>
using namespace std;
using namespace chaiscript;
using namespace chaiscript::utility;


namespace mChai {
    void bindAll(chaiscript::ChaiScript &chai) {
        ModulePtr m(new Module());

        bindMUtil(chai, m);
        bindMFrame(chai, m);
        bindMSequence(chai, m);
        bindMSprite(chai, m);
        bindMasalaApp(chai, m);
        bindMKeyState(chai, m);
        bindMKeyboardState(chai, m);
        bindMTileMap(chai, m);
        bindMSpriteBatch(chai, m);
        bindMSoundEffect(chai, m);

        chai.add(m);
    }
    
    
    void bindMUtil(chaiscript::ChaiScript &chai, chaiscript::ModulePtr& m) {
        m->add(fun(&mUtil::inRangeInc), "inRangeInc");
        m->add(fun(&mUtil::inRangeEx), "inRangeEx");
        m->add(fun(&mUtil::rangeOverlapInc), "rangeOverlapInc");
        m->add(fun(&mUtil::rangeOverlapEx), "rangeOverlapEx");
    }


    void bindMFrame(chaiscript::ChaiScript &chai, chaiscript::ModulePtr& m) {
        add_class<MFrame>(*m,
            "MFrame",
            {
                constructor<MFrame()>(),
                constructor<MFrame(int, int, int, int, QString)>(),
                constructor<MFrame(const MFrame &)>(),
            },
            {
                { fun([](MFrame &f) {
                    return f.str().toStdString();
                }), "to_string" },

                { fun(&MFrame::x), "x" },
                { fun(&MFrame::y), "y" },
                { fun(&MFrame::w), "w" },
                { fun(&MFrame::h), "h" },
                { fun(&MFrame::name), "name" },
                { fun(&MFrame::topLeft), "topLeft" },
                { fun(&MFrame::topLeftF), "topLeftF" },
                { fun(&MFrame::size), "size" },
                { fun(&MFrame::sizeF), "x" },
                { fun(&MFrame::toRect), "toRect" },
                { fun(&MFrame::toRectF), "toRectF" },
                { fun(&MFrame::str), "str" },
            }
        );
    }
    
    
    void bindMSequence(chaiscript::ChaiScript &chai, chaiscript::ModulePtr& m) {
        // Bind MSequence
        add_class<MSequence>(*m,
            "MSequence",
            {
                constructor<MSequence()>(),
                constructor<MSequence(QString)>(),
                constructor<MSequence(const MSequence &)>(),
            },
            {
                { fun([](MSequence &seq) {
                    return seq.str().toStdString();
                }), "to_string" },

                { fun(&MSequence::name), "name" },
                { fun(&MSequence::looping), "looping" },
//                { fun(&MSequence::append), "append" },
//                { fun([](MSequence &seq, MFrame &frame, quint64 ms) { seq.append(frame, ms); }), "append" },
                { fun(&MSequence::atIndex), "atIndex" },
                { fun(&MSequence::atTime), "atTime" },
//                { fun([](MSequence &seq) { return seq.frames().toStdVector(); }), "frames" },
                { fun(&MSequence::numFrames), "numFrames" },
                { fun(&MSequence::duration), "duration" },
                { fun([](MSequence &seq) { return seq.str(M_SEQUENCE_FRAME_INFO_DEFAULT); }), "str" },
                { fun([](MSequence &seq, bool frameInfo) { return seq.str(frameInfo); }), "str" },
            }
        );


        // Also bind MSequence::DelayedFrame
        add_class<MSequence::DelayedFrame>(*m,
            "MSequence_DelayedFrame",
            {
                constructor<MSequence::DelayedFrame()>(),
                constructor<MSequence::DelayedFrame(MFrame &, quint64)>(),
                constructor<MSequence::DelayedFrame(const MSequence::DelayedFrame &)>(),
            },
            {
                { fun([](MSequence::DelayedFrame &df) {
                    return df.str().toStdString();
                }), "to_string" },

                { fun(&MSequence::DelayedFrame::str), "str" },
                { fun(&MSequence::DelayedFrame::frame), "frame" },
                { fun(&MSequence::DelayedFrame::delay), "delay" },
            }
        );
    }
    
    
    void bindMSprite(chaiscript::ChaiScript &chai, chaiscript::ModulePtr& m) {
        // Bind MSprite
        add_class<MSprite>(*m,
            "MSprite",
            {
                constructor<MSprite(QString)>(),
                constructor<MSprite(const MSprite &)>(),
            },
            {
                { fun([](MSprite &s) {
                    return s.str().toStdString();
                }), "to_string" },

                { fun(&MSprite::pos), "pos" },
//                { fun(&MSprite::z), "z" },
                { fun(&MSprite::origin), "origin" },
                { fun(&MSprite::autoCenterOrigin), "autoCenterOrigin" },
                { fun(&MSprite::visibility), "visibility" },
                { fun(&MSprite::angle), "angle" },
                { fun(&MSprite::customRotationOrigin), "customRotationOrigin" },
                { fun(&MSprite::rotationOrigin), "rotationOrigin" },
                { fun(&MSprite::scale), "scale" },
                { fun(&MSprite::drawOutline), "drawOutline" },
                { fun(&MSprite::outlineColor), "outlineColor" },
                { fun([](MSprite &s) { return s.pos.rx(); }), "x" },
                { fun([](MSprite &s) { return s.pos.ry(); }), "y" },
                { fun([](MSprite &s, const QString &frameName) { s.show(frameName); }), "show" },
                { fun([](MSprite &s, const string &frameName) { s.show(frameName.c_str()); }), "show" },
                { fun([](MSprite &s) { s.hide(M_SPRITE_HIDE_IT_DEFAULT); }), "hide"},
                { fun([](MSprite &s, bool hideIt) { s.hide(hideIt); }), "hide"},
                { fun([](MSprite &s, const QString &seqName) { s.play(seqName, M_SPRITE_START_FROM_DEFAULT); }), "play"},
                { fun([](MSprite &s, const string &seqName) { s.play(seqName.c_str(), M_SPRITE_START_FROM_DEFAULT); }), "play"},
                { fun([](MSprite &s, const QString &seqName, quint64 startFrom) { s.play(seqName, startFrom); }), "play"},
                { fun([](MSprite &s, const string &seqName, quint64 startFrom) { s.play(seqName.c_str(), startFrom); }), "play"},
                { fun(&MSprite::pause), "pause" },
                { fun(&MSprite::resume), "resume" },
                { fun(&MSprite::update), "update" },
                { fun(&MSprite::draw), "draw" },
                { fun(&MSprite::id), "id" },
                { fun(&MSprite::curFrame), "curFrame" },
                { fun(&MSprite::curSeq), "curSeq" },
                { fun(&MSprite::playbackTime), "playbackTime" },
                { fun(&MSprite::isHidden), "isHidden" },
                { fun(&MSprite::isPlaying), "isPlaying" },
                { fun(&MSprite::isPaused), "isPaused" },
            }
        );
    }
    
    
    void bindMasalaApp(chaiscript::ChaiScript &chai, chaiscript::ModulePtr& m) {
        add_class<MasalaApp>(*m,
            "MasalaApp",
            {
            },
            {
                { fun([](MasalaApp &app) {
                    return string("MasalaApp");
                }), "to_string" },

                { fun(&MasalaApp::windowSize), "windowSize" },
                { fun(&MasalaApp::initWindowSize), "initWindowSize" },
                { fun(&MasalaApp::shutdown), "shutdown" },
                { fun([](MasalaApp &app, const string &title) { app.setWindowTitle(title.c_str()); }), "setWindowTitle"},
            }
        );
    }


    void bindMKeyState(chaiscript::ChaiScript &chai, chaiscript::ModulePtr& m) {
        add_class<MKeyState>(*m,
            "MKeyState",
            {
                constructor<MKeyState()>(),
                constructor<MKeyState(const MKeyState &)>(),
            },
            {
                { fun(&MKeyState::down), "down" },
                { fun(&MKeyState::isAutoRepeat), "isAutoRepeat" },
                { fun(&MKeyState::shift), "shift" },
                { fun(&MKeyState::control), "control" },
                { fun(&MKeyState::alt), "alt" },
                { fun(&MKeyState::isDown), "isDown" },
                { fun(&MKeyState::isUp), "isUp" },

                { fun(&MKeyState::operator=), "=" },
                { fun(&MKeyState::operator==), "==" },
                { fun(&MKeyState::operator!=), "!=" },
            }
        );
    }


    void bindMKeyboardState(chaiscript::ChaiScript &chai, chaiscript::ModulePtr& m) {
        add_class<MKeyboardState>(*m,
            "MKeyboardState",
            {
                constructor<MKeyboardState()>(),
                constructor<MKeyboardState(const MKeyboardState &)>(),
            },
            {
                { fun(&MKeyboardState::zero), "zero" },
                { fun(&MKeyboardState::one), "one" },
                { fun(&MKeyboardState::two), "two" },
                { fun(&MKeyboardState::three), "three" },
                { fun(&MKeyboardState::four), "four" },
                { fun(&MKeyboardState::five), "five" },
                { fun(&MKeyboardState::six), "six" },
                { fun(&MKeyboardState::seven), "seven" },
                { fun(&MKeyboardState::eight), "eight" },
                { fun(&MKeyboardState::nine), "nine" },
                { fun(&MKeyboardState::a), "a" },
                { fun(&MKeyboardState::b), "b" },
                { fun(&MKeyboardState::c), "c" },
                { fun(&MKeyboardState::d), "d" },
                { fun(&MKeyboardState::e), "e" },
                { fun(&MKeyboardState::f), "f" },
                { fun(&MKeyboardState::g), "g" },
                { fun(&MKeyboardState::h), "h" },
                { fun(&MKeyboardState::i), "i" },
                { fun(&MKeyboardState::j), "j" },
                { fun(&MKeyboardState::k), "k" },
                { fun(&MKeyboardState::l), "l" },
                { fun(&MKeyboardState::m), "m" },
                { fun(&MKeyboardState::n), "n" },
                { fun(&MKeyboardState::o), "o" },
                { fun(&MKeyboardState::p), "p" },
                { fun(&MKeyboardState::q), "q" },
                { fun(&MKeyboardState::r), "r" },
                { fun(&MKeyboardState::s), "s" },
                { fun(&MKeyboardState::t), "t" },
                { fun(&MKeyboardState::u), "u" },
                { fun(&MKeyboardState::v), "v" },
                { fun(&MKeyboardState::w), "w" },
                { fun(&MKeyboardState::x), "x" },
                { fun(&MKeyboardState::y), "y" },
                { fun(&MKeyboardState::z), "z" },
                { fun(&MKeyboardState::F1), "F1" },
                { fun(&MKeyboardState::F2), "F2" },
                { fun(&MKeyboardState::F3), "F3" },
                { fun(&MKeyboardState::F4), "F4" },
                { fun(&MKeyboardState::F5), "F5" },
                { fun(&MKeyboardState::F6), "F6" },
                { fun(&MKeyboardState::F7), "F7" },
                { fun(&MKeyboardState::F8), "F8" },
                { fun(&MKeyboardState::F9), "F9" },
                { fun(&MKeyboardState::F10), "F10" },
                { fun(&MKeyboardState::F11), "F11" },
                { fun(&MKeyboardState::F12), "F12" },
                { fun(&MKeyboardState::escape), "escape" },
                { fun(&MKeyboardState::home), "home" },
                { fun(&MKeyboardState::end), "end" },
                { fun(&MKeyboardState::insert), "insert" },
                { fun(&MKeyboardState::delete_), "delete" },
                { fun(&MKeyboardState::minus), "minus" },
                { fun(&MKeyboardState::equal), "equal" },
                { fun(&MKeyboardState::backspace), "backspace" },
                { fun(&MKeyboardState::tab), "tab" },
                { fun(&MKeyboardState::leftBracket), "leftBracket" },
                { fun(&MKeyboardState::rightBracket), "rightBracket" },
                { fun(&MKeyboardState::backSlash), "backSlash" },
                { fun(&MKeyboardState::semicolon), "semicolon" },
                { fun(&MKeyboardState::apostrophe), "apostrophe" },
                { fun(&MKeyboardState::enter), "enter" },
                { fun(&MKeyboardState::comma), "comma" },
                { fun(&MKeyboardState::period), "period" },
                { fun(&MKeyboardState::forwardSlash), "forwardSlash" },
                { fun(&MKeyboardState::up), "up" },
                { fun(&MKeyboardState::down), "down" },
                { fun(&MKeyboardState::left), "left" },
                { fun(&MKeyboardState::right), "right" },
                { fun(&MKeyboardState::pageUp), "pageUp" },
                { fun(&MKeyboardState::pageDown), "pageDown" }, 

                { fun(&MKeyboardState::operator=), "=" },
                { fun(&MKeyboardState::operator==), "==" },
                { fun(&MKeyboardState::operator!=), "!=" },
            }
        );
    }


    void bindMTileMap(chaiscript::ChaiScript &chai, chaiscript::ModulePtr& m) {
        add_class<MTileMap>(*m,
            "MTileMap",
            {
                constructor<MTileMap(QString)>(),
//                constructor<MTileMap><const MTileMap *)>(),
            },
            {
                { fun([](MTileMap &t) {
                    return t.str().toStdString();
                }), "to_string" },

                // MTileMap specific
                { fun(&MTileMap::isGood), "isGood" },
                { fun(&MTileMap::tileWidth), "tileWidth" },
                { fun(&MTileMap::tileHeight), "tileHeight" },
                { fun(&MTileMap::mapWidth), "mapWidth" },
                { fun(&MTileMap::mapHeight), "mapHeight" },
                { fun(&MTileMap::asSprite), "asSprite" },
            }
        );

        chai.add_global(var(&MTileMap::FrameName), "MTileMap_FrameName");
    }


    void bindMSpriteBatch(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m) {
        add_class<MSpriteBatch>(*m,
            "MSpriteBatch",
            {
                constructor<MSpriteBatch()>(),
            },
            {
                { fun([](const MSpriteBatch &sb) {
                    return string("MSpriteBatch");
                }), "to_string" },

                { fun(&MSpriteBatch::add), "add" },
                { fun(&MSpriteBatch::addBatch), "addBatch" },
                { fun(&MSpriteBatch::clear), "clear" },
                { fun(&MSpriteBatch::drawAll), "drawAll" },
            }
        );
    }
    
    
    void bindMSoundEffect(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m) {
        add_class<MSoundEffect>(*m,
            "MSoundEffect",
            {
                constructor<MSoundEffect(QString)>(),
                constructor<MSoundEffect(const MSoundEffect &)>()
            },
            {
                { fun([](const MSoundEffect &se) {
                    return se.str().toStdString();
                }), "to_string" },

                { fun(&MSoundEffect::isNull), "isNull" },
                { fun(&MSoundEffect::filename), "filename" },
                { fun(&MSoundEffect::isMuted), "isMuted" },
                { fun(&MSoundEffect::isPlaying), "isPlaying" },
                { fun(&MSoundEffect::loopCount), "loopCount" },
                { fun(&MSoundEffect::loopsRemaining), "loopsRemaining" },
                { fun(&MSoundEffect::setLoopCount), "setLoopCount" },
                { fun(&MSoundEffect::setMuted), "setMuted" },
                { fun(&MSoundEffect::setVolume), "setVolume" },
                { fun(&MSoundEffect::volume), "volume" },
                { fun(&MSoundEffect::play), "play" },
                { fun(&MSoundEffect::stop), "stop" },
            }
        );
        chai.add_global(var(&MSoundEffect::InfiniteLoop), "MSoundEffect_InfiniteLoop");
    }
};

