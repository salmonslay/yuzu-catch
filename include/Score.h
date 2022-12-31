#ifndef YUZU_CATCH_SCORE_H
#define YUZU_CATCH_SCORE_H

#include <chrono>
#include <thread>
#include "HitObject.h"
#include "Banana.h"

namespace yuzu
{

    /**
     * Container for the score of a single play.
     */
    struct Score {
        enum class Grade {
            D, // <= 85
            C, // <= 90
            B, // <= 94
            A, // <= 98
            S, // <= 100
            SS, // = 100
        };

        int displayScore = 0; // the score displayed on the screen is delayed

        int score = 0; // the actual score
        int combo = 0;
        int maxCombo = 0;

        int caughtFruits = 0;
        int missedFruits = 0;

        int caughtRawScore = 0;
        int missedRawScore = 0;

        double getAccuracy() const
        {
            if (missedRawScore == 0)
                return 1.0;

            return (double) caughtRawScore / (caughtRawScore + missedRawScore);
        }

        Grade getGrade() const
        {
            double accuracy = getAccuracy();

            if (accuracy == 1.0)
                return Grade::SS;
            if (accuracy > 0.98)
                return Grade::S;
            if (accuracy > 0.94)
                return Grade::A;
            if (accuracy > 0.90)
                return Grade::B;
            if (accuracy > 0.85)
                return Grade::C;

            return Grade::D;
        }

        void processHitObject(HitObject *ho)
        {
            int previousScore = score;

            if (ho->getState() == HitObjectState::HIT || ho->getState() == HitObjectState::PLATED)
            {

                if (ho->addsCombo())
                {
                    combo++;
                    caughtFruits++;

                    maxCombo = std::max(maxCombo, combo);
                }

                if (combo != 0 && ho->comboDependent())
                    score += ho->getScore() * combo / 25;
                score += ho->getScore();

                if (dynamic_cast<Banana *>(ho) == nullptr)
                    caughtRawScore += ho->getScore();
            }
            else if (ho->getState() == HitObjectState::MISSED)
            {
                if (ho->addsCombo())
                {
                    combo = 0;
                    missedFruits++;
                }

                if (dynamic_cast<Banana *>(ho) == nullptr)
                    missedRawScore += ho->getScore();
            }

            // add score to display score slowly to make it look nice
            std::thread([=]
                        {
                            int scoreToAdd = (score - previousScore) / 25;
                            for (int i = 0; i < 25; i++)
                            {
                                displayScore += scoreToAdd;
                                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 25));
                            }
                        }).detach();
        }

    };

} // yuzu

#endif //YUZU_CATCH_SCORE_H
