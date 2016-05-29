// A collection of utility functions
//
// A lot of these are used inside of the ChaiScript interpreter, but are
// implemented on the C++ side for speed reasons (see functions.chai)
#include <QString>

namespace mUtil {
    bool inRangeInc(double x, double a, double b);
    bool inRangeEx(double x, double a, double b);
    bool rangeOverlapInc(double a, double b, double x, double y);
    bool rangeOverlapEx(double a, double b, double x, double y);

    QString getDirPath(QString filePath);
};
