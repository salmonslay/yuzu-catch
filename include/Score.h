#ifndef YUZU_CATCH_SCORE_H
#define YUZU_CATCH_SCORE_H

#include <chrono>
#include <thread>
#include "HitObject.h"
#include "Banana.h"
#include "JuiceDrop.h"

namespace yuzu
{

    /**
     * Container for the score of a single play.
     */
    struct Score {
        int displayScore = 0; // the score displayed on the screen is delayed

        int score = 0; // the actual score
        int combo = 0;
        int maxCombo = 0;

        int caughtFruits = 0;
        int missedFruits = 0;

        int caughtBananas = 0;
        int missedBananas = 0;

        int caughtJuiceDrops = 0;
        int missedJuiceDrops = 0;

        int caughtRawScore = 0;
        int missedRawScore = 0;

        double getAccuracy() const
        {
            if (missedRawScore == 0)
                return 1.0;

            return (double) caughtRawScore / (caughtRawScore + missedRawScore);
        }

        std::string getAccuracyString() const
        {
            std::string accuracyText = std::to_string((int) (getAccuracy() * 10000) / 100.0);
            accuracyText = accuracyText.substr(0, accuracyText.find('.') + 3);
            accuracyText += "%";

            return accuracyText;
        }

        std::string getGrade() const
        {
            double accuracy = getAccuracy();

            if (accuracy == 1.0)
                return "SS";
            if (accuracy > 0.98)
                return "S";
            if (accuracy > 0.94)
                return "A";
            if (accuracy > 0.90)
                return "B";
            if (accuracy > 0.85)
                return "C";

            return "D";
        }

        void processHitObject(HitObject *ho, bool kiai = false)
        {
            int previousScore = score;

            if (ho->getState() == HitObjectState::HIT || ho->getState() == HitObjectState::PLATED)
            {
                if (ho->addsCombo())
                {
                    combo++;

                    maxCombo = std::max(maxCombo, combo);
                }

                bool giveComboBonus = combo != 0 && ho->comboDependent();
                int comboBonus = giveComboBonus ? (ho->getScore() * combo / 25) : 0;
                score += (ho->getScore() + comboBonus) * (kiai ? 1.5 : 1.0); // kiai gives 50% bonus

                if (dynamic_cast<Banana *>(ho) != nullptr)
                {
                    caughtBananas++;
                    // bananas don't give raw score
                }
                else if (dynamic_cast<JuiceDrop *>(ho) != nullptr)
                {
                    caughtJuiceDrops++;
                    caughtRawScore += ho->getScore();
                }
                else // fruit
                {
                    caughtFruits++;
                    caughtRawScore += ho->getScore();
                }
            }
            else if (ho->getState() == HitObjectState::MISSED)
            {
                if (ho->addsCombo())
                {
                    combo = 0;
                }

                if (dynamic_cast<Banana *>(ho) != nullptr)
                {
                    missedBananas++;
                    // bananas don't give raw score
                }
                else if (dynamic_cast<JuiceDrop *>(ho) != nullptr)
                {
                    missedJuiceDrops++;
                    missedRawScore += ho->getScore();
                }
                else // fruit
                {
                    missedFruits++;
                    missedRawScore += ho->getScore();
                }
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
