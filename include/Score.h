#ifndef YUZU_CATCH_SCORE_H
#define YUZU_CATCH_SCORE_H

#include <string>
#include "HitObject.h"
#include "Banana.h"
#include "JuiceDrop.h"

namespace yuzu
{

    class Score : public fruitwork::Component {
    public:
        static Score *getInstance();

        double getAccuracy() const;

        std::string getAccuracyString() const;

        std::string getBananaAccuracyString() const;

        int getDisplayScore() const { return displayScore; }

        int getActualScore() const { return actualScore; }

        int getMaxCombo() const { return maxCombo; }

        int getCombo() const { return combo; }

        int getCaughtFruits() const { return caughtFruits; }

        int getMissedFruits() const { return missedFruits; }

        /** Returns a grade; SS, S, A, B, C, D dependent on the current accuracy */
        std::string getGrade() const;

        void processHitObject(HitObject *ho, bool kiai = false);

        void update() override;

        void draw() const override {};

    private:
        Score();

        int displayScore = 0; // the score displayed on the screen is delayed

        int actualScore = 0;
        int combo = 0;
        int maxCombo = 0;

        int caughtFruits = 0; // (excluding bananas)
        int missedFruits = 0;

        int caughtBananas = 0;
        int missedBananas = 0;

        // raw score is the score before combo is applied
        int caughtRawScore = 0;
        int missedRawScore = 0;
    };

} // yuzu

#endif //YUZU_CATCH_SCORE_H
