#ifndef YUZU_CATCH_JUICEDROP_H
#define YUZU_CATCH_JUICEDROP_H

#include "HitObject.h"

namespace yuzu
{

    /**
     * Juice drops are tiny fruits that gives 50 score, based on combo.
     */
    class JuiceDrop : public HitObject {
        int getScore() const override { return 50; }

        bool addsCombo() const override { return false; }
    };

} // yuzu

#endif //YUZU_CATCH_JUICEDROP_H
