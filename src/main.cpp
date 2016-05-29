#include "masalaapp.h"
#include "masalawindow.h"
#include "chaiscript/chaiscript.hpp"
#include <QStyle>
#include <QDesktopWidget>


int main(int argc, char *argv[])
{
    MasalaApp app(argc, argv);
    MasalaWindow wnd;
    app.window = &wnd;

    // Place the window in the center
    wnd.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            wnd.size(),
            app.desktop()->availableGeometry()
        )
    );
    wnd.show();

    // Load content
    app.chai()("load();");

    // Init
    app.chai()("init();");

    return app.exec();
}
