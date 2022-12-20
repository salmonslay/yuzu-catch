#include <string>
#include <algorithm>
#include <sstream>
#include "BeatmapHelpers.h"
#include "Beatmap.h"

namespace yuzu
{
    double BeatmapHelpers::getDoubleFromLine(const std::string &line)
    {
        return std::stod(line.substr(line.find(':') + 1));
    }

    std::string BeatmapHelpers::getStringFromLine(const std::string &line)
    {
        std::string s = line.substr(line.find(':') + 1);

        // trim
        // @see https://stackoverflow.com/a/217605/11420970
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
        {
            return !std::isspace(ch);
        }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
        {
            return !std::isspace(ch);
        }).base(), s.end());

        return s;
    }

    Beatmap::BeatmapSection BeatmapHelpers::getSectionFromLine(const std::string &line)
    {
        if (line.find("[General]") != std::string::npos)
            return Beatmap::BeatmapSection::GENERAL;
        else if (line.find("[Metadata]") != std::string::npos)
            return Beatmap::BeatmapSection::METADATA;
        else if (line.find("[Difficulty]") != std::string::npos)
            return Beatmap::BeatmapSection::DIFFICULTY;
        else if (line.find("[Events]") != std::string::npos)
            return Beatmap::BeatmapSection::EVENTS;
        else if (line.find("[TimingPoints]") != std::string::npos)
            return Beatmap::BeatmapSection::TIMINGPOINTS;
        else if (line.find("[Colours]") != std::string::npos)
            return Beatmap::BeatmapSection::COLOURS;
        else if (line.find("[HitObjects]") != std::string::npos)
            return Beatmap::BeatmapSection::HITOBJECTS;
        else
            return Beatmap::BeatmapSection::NONE;
    }

    std::vector<std::string> BeatmapHelpers::splitToString(const std::string &s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    std::vector<int> BeatmapHelpers::splitToInt(const std::string &s, char delimiter)
    {
        std::vector<int> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(std::stoi(token));
        }
        return tokens;
    }
} // yuzu