#include "Score.h"

namespace yuzu
{
    Score::Score() : Component(0, 0, 0, 0)
    {

    }

    Score *Score::getInstance()
    {
        return new Score();
    }

    double Score::getAccuracy() const
    {
        if (missedRawScore == 0)
            return 1.0;

        return (double) caughtRawScore / (caughtRawScore + missedRawScore);
    }

    std::string Score::getAccuracyString() const
    {
        std::string accuracyText = std::to_string((int) (getAccuracy() * 10000) / 100.0);
        accuracyText = accuracyText.substr(0, accuracyText.find('.') + 3);
        accuracyText += "%";

        return accuracyText;
    }

    std::string Score::getBananaAccuracyString() const
    {
        std::string accuracyText = std::to_string((int) ((double) caughtBananas / (caughtBananas + missedBananas) * 10000) / 100.0);
        accuracyText = accuracyText.substr(0, accuracyText.find('.') + 3);
        accuracyText += "%";

        return accuracyText;
    }


    std::string Score::getGrade() const
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

    void Score::processHitObject(HitObject *ho, bool kiai)
    {
        if (ho->getState() == HitObjectState::HIT || ho->getState() == HitObjectState::PLATED)
        {
            if (ho->addsCombo())
            {
                combo++;

                maxCombo = std::max(maxCombo, combo);
            }

            bool giveComboBonus = combo != 0 && ho->comboDependent();
            double comboBonus = giveComboBonus ? (ho->getScore() * combo / 25) : 0;
            actualScore += static_cast<int>(ho->getScore() + comboBonus) * (kiai ? 1.5 : 1.0); // kiai gives 50% bonus

            if (dynamic_cast<Banana *>(ho) != nullptr)
            {
                caughtBananas++;
                // bananas don't give raw score
            }
            else if (dynamic_cast<JuiceDrop *>(ho) != nullptr)
            {
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
                missedRawScore += ho->getScore();
            }
            else // fruit
            {
                missedFruits++;
                missedRawScore += ho->getScore();
            }
        }
    }

    void Score::update()
    {
        // update the score displayed on the screen
        displayScore = (int) (displayScore + (actualScore - displayScore) * 0.05);
        displayScore = std::min(displayScore, actualScore);
    }
} // yuzu
