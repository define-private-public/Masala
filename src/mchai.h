// Bindings for the engine

#ifndef M_CHAI_H
#define M_CHAI_H

#include <memory>

// Forward decl
namespace chaiscript {
    class ChaiScript;
    class Module;

    typedef std::shared_ptr<Module> ModulePtr;
};


namespace mChai {
    void bindAll(chaiscript::ChaiScript &chai);

    void bindMUtil(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindMFrame(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindMSequence(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindMSprite(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindMasalaApp(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindMKeyState(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindMKeyboardState(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindMTileMap(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindMSpriteBatch(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindMSoundEffect(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
};


#endif // M_CHAI_H

