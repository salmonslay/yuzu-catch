#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"

#include "Beatmap.h"
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <vector>

#define ERR(s, ...) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, s, __VA_ARGS__);

namespace yuzu
{
    static inline double getDoubleFromLine(const std::string &line)
    {
        return std::stod(line.substr(line.find(':') + 1));
    }

    static inline std::string getStringFromLine(const std::string &line)
    {
        return line.substr(line.find(':') + 1);
    }

    Beatmap Beatmap::loadBeatmap(const std::string &beatmapName)
    {
        SDL_Log("Parsing beatmap: %s...", beatmapName.c_str());

        std::ifstream infile(constants::gResPath + "beatmaps/" + beatmapName + ".osu");

        Beatmap beatmap;
        std::string line;
        BeatmapSection section = BeatmapSection::NONE;
        TimingPoint lastTimingPoint;
        int discard; // used to discard unused values

        while (std::getline(infile, line))
        {
            if (line.empty())
                continue;

            if (line[0] == '[') // set new section
            {
                if (line.find("[General]") != std::string::npos)
                    section = BeatmapSection::GENERAL;
                else if (line.find("[Metadata]") != std::string::npos)
                    section = BeatmapSection::METADATA;
                else if (line.find("[Difficulty]") != std::string::npos)
                    section = BeatmapSection::DIFFICULTY;
                else if (line.find("[TimingPoints]") != std::string::npos)
                    section = BeatmapSection::TIMINGPOINTS;
                else if (line.find("[Colours]") != std::string::npos)
                    section = BeatmapSection::COLOURS;
                else if (line.find("[HitObjects]") != std::string::npos)
                    section = BeatmapSection::HITOBJECTS;
                else
                    section = BeatmapSection::NONE;

                continue;
            }

            if (section == BeatmapSection::GENERAL)
            {
                if (line.find("SampleSet:") != std::string::npos)
                {
                    if (line.find("Soft") != std::string::npos)
                        beatmap.sampleType = SampleType::SOFT;
                    else if (line.find("Drum") != std::string::npos)
                        beatmap.sampleType = SampleType::DRUM;
                }
            }
            else if (section == BeatmapSection::METADATA)
            {
                if (line.find("TitleUnicode:") != std::string::npos)
                    beatmap.title = getStringFromLine(line);
                else if (line.find("ArtistUnicode:") != std::string::npos)
                    beatmap.artist = getStringFromLine(line);
                else if (line.find("Creator:") != std::string::npos)
                    beatmap.creator = getStringFromLine(line);
                else if (line.find("Version:") != std::string::npos)
                    beatmap.version = getStringFromLine(line);
                else if (line.find("Tags:") != std::string::npos)
                    beatmap.tags = getStringFromLine(line);
            }
            else if (section == BeatmapSection::DIFFICULTY)
            {
                if (line.find("SliderMultiplier:") != std::string::npos)
                    beatmap.sliderMultiplier = getDoubleFromLine(line);
                else if (line.find("SliderTickRate:") != std::string::npos)
                    beatmap.sliderTickRate = getDoubleFromLine(line);
            }
            else if (section == BeatmapSection::COLOURS)
            {
                // if the line starts with "Combo", it's a combo colour
                if (line.find("Combo") != std::string::npos)
                {
                    SDL_Color c = {.a = 255};
                    if (sscanf(line.c_str(), "%s : %hhd,%hhd,%hhd", &discard, &c.r, &c.g, &c.b) == EOF)
                        ERR("Failed to parse combo colour: %d, and %d", c.r, c.g)

                    beatmap.comboColours.push_back(c);
                }
            }
            else if (section == BeatmapSection::TIMINGPOINTS)
            {
                /**
                  * @see Source https://github.com/LiterallyFabian/SVB/blob/main/app/public/catch/js/processor.js#L256-L302
                  * @see Docs https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#timing-points
                  */
                Beatmap::TimingPoint timingPoint{};

                if (sscanf(line.c_str(), "%d,%lf,%d,%d,%d,%d,%d,%d", &timingPoint.offset, &timingPoint.beatLength, &discard,
                           &timingPoint.sampleType, &discard, &discard, &timingPoint.isUninherited, &timingPoint.isKiai) == EOF)
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to parse timing point: %s", line.c_str());

                // inherited timing points changes slider velocity, basically beat length
                if (timingPoint.isUninherited)
                    lastTimingPoint = timingPoint;
                else if (lastTimingPoint.offset)
                    timingPoint.beatLength = lastTimingPoint.beatLength / (-100 / timingPoint.beatLength);

                beatmap.timingPoints.push_back(timingPoint);
            }
        }

        SDL_Log("Beatmap parsed successfully!");
        infile.close();

        return beatmap;
    }

    std::vector<HitObject> Beatmap::getHitObjects()
    {
        std::vector<HitObject> hitObjects;


    }

} // yuzu

#pragma GCC diagnostic pop