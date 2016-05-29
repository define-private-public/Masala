#include <QDebug>
#include "mterminal.h"
#include "masalaapp.h"
#include "chaiscript/chaiscript.hpp"
#include <iostream>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
using namespace std;


// Static data
bool MTerminal::__rlDone = false;


MTerminal::MTerminal(QObject *parent) :
    QThread(parent)
{
}


MTerminal::~MTerminal() {
}


void MTerminal::end() {
    __runningLock.lock();
    __running = false;
    __runningLock.unlock();
}


void MTerminal::run() {
    setPriority(LowestPriority);

    // I <3 C++11 lambdas
//    rl_callback_handler_install("> ", (rl_vcpfunc_t *)([](char *line) { __rlHandler(this, line); }));
    rl_callback_handler_install("> ", (rl_vcpfunc_t *)__rlHandler);

    // Keep polling for input forevera
    QString cmd = "";
    bool running = true;
    while (running) {
        // Read one character at a time
        if (!__rlDone)
            rl_callback_read_char();

        // Check the kill flag
        if (__runningLock.tryLock()) {
            running = __running;
            __runningLock.unlock();
        }
        
        // sleep
        msleep(5);
    }

    // We Always Clean-up, clean-up
    rl_callback_handler_remove();
}


/*!
    callback that is used by GNU Readline.  This is a private static function.

    Code inspired from: http://www.mcld.co.uk/blog/blog.php?274
*/
void MTerminal::__rlHandler(char *line) {
    /*== check for shutdown ==*/
    // EOF (^D)
    if (!line) {
        mApp->shutdown();
        __rlDone = true;
        rl_callback_handler_remove();
        cout << endl;
        return;
    }
        
    QString cmd = QString(line);

    // "quit" or "exit" was typed
    bool exit = (cmd == "exit");
    bool quit = (cmd == "quit");
    if (exit || quit) {
        mApp->shutdown();
        __rlDone = true;
        rl_callback_handler_remove();
        return;
    }
    /*== end shutdown checks ==*/

        
    // Else, it must have been a chaiscript command
    if (!cmd.isEmpty()) {
        add_history(line);
        try {
            mApp->chai().eval(cmd.toStdString());
        } catch (exception &ex) {
            // TODO explicity catch chaiscript stuff instead of using a generic error
            cout << ex.what() << endl;
        }
    }

    free(line);
}

