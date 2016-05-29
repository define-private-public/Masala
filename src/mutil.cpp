#include "mutil.h"
#include <QFileInfo>
#include <QDir>


namespace mUtil {
    // Returns true if `x` is inclusivly in the range [a, b] or [b, a]
    bool inRangeInc(double x, double a, double b) {
        return ((a <= x) && (x <= b)) || ((b <= x) && (x <= a));
    }
    
    
    // Returns true if `x` is exclusivly in the range (a, b) or (b, a)
    bool inRangeEx(double x, double a, double b) {
        return ((a < x) && (x < b)) || ((b < x) && (x < a));
    }
    
    
    // Returns true if the range [a, b] overlaps with [x, y] inclusivly
    bool rangeOverlapInc(double a, double b, double x, double y) {
        return inRangeInc(a, x, y) || inRangeInc(b, x, y);
    }
    
    
    // Return true if the range (a, b) overlaps with (x, y) inclusivly
    bool rangeOverlapEx(double a, double b, double x, double y) {
        return inRangeEx(a, x, y) || inRangeEx(b, x, y);
    }

    /*!
        Retrives the relative path of the directory that contains \a filePath.
    */
    QString getDirPath(QString filePath) {
        return QFileInfo(filePath).dir().path();
    }
};

