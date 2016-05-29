// Chaiscript Qt binding functions

#ifndef M_CHAI_QT_H
#define M_CHAI_QT_H

#include <memory>

// Forward decl
namespace chaiscript {
    class ChaiScript;
    class Module;

    typedef std::shared_ptr<Module> ModulePtr;
};


namespace chaiQt {
    void bindAll(chaiscript::ChaiScript &chai);

    void bindQString(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindQObject(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindQPoint(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindQPointF(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindQSize(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindQSizeF(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindQRect(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindQRectF(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindQColor(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
    void bindQVariant(chaiscript::ChaiScript &chai, chaiscript::ModulePtr &m);
};

#endif // M_CHIA_QT_H

