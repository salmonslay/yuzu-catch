#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Wdangling-gsl"
#pragma GCC diagnostic ignored "-Wformat"

#include "Beatmap.h"
#include "Constants.h"
#include "System.h"
#include <fstream>
#include <vector>
#include <chrono>
#include <SDL_image.h>
#include <algorithm>
#include <regex>

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

    Beatmap *Beatmap::loadBeatmap(const std::string &beatmapPath)
    {
        //SDL_Log("Parsing beatmap: %s...", beatmapPath.c_str());
        auto start = std::chrono::high_resolution_clock::now();

        std::ifstream infile(constants::gResPath + "beatmaps/" + beatmapPath);

        Beatmap *beatmap = new Beatmap();
        std::string line;
        BeatmapSection section = BeatmapSection::NONE;
        TimingPoint lastTimingPoint;
        void *discard; // used to discard unused values

        beatmap->beatmapDir = beatmapPath.substr(0, beatmapPath.find_last_of('/'));

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
                else if (line.find("[Events]") != std::string::npos)
                    section = BeatmapSection::EVENTS;
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
                        beatmap->sampleType = SampleType::SOFT;
                    else if (line.find("Drum") != std::string::npos)
                        beatmap->sampleType = SampleType::DRUM;
                }
                else if (line.find("AudioFilename:") != std::string::npos)
                    beatmap->audioFilename = getStringFromLine(line);
            }
            else if (section == BeatmapSection::METADATA)
            {
                if (line.find("Title:") != std::string::npos)
                    beatmap->title = getStringFromLine(line);
                else if (line.find("Artist:") != std::string::npos)
                    beatmap->artist = getStringFromLine(line);
                else if (line.find("Creator:") != std::string::npos)
                    beatmap->creator = getStringFromLine(line);
                else if (line.find("Version:") != std::string::npos)
                    beatmap->version = getStringFromLine(line);
                else if (line.find("Tags:") != std::string::npos)
                    beatmap->tags = getStringFromLine(line);
            }
            else if (section == BeatmapSection::DIFFICULTY)
            {
                if (line.find("SliderMultiplier:") != std::string::npos)
                    beatmap->sliderMultiplier = getDoubleFromLine(line);
                else if (line.find("SliderTickRate:") != std::string::npos)
                    beatmap->sliderTickRate = getDoubleFromLine(line);
            }
            else if (section == BeatmapSection::EVENTS)
            {
                if (line.rfind("0,0,\"", 0) == 0)
                {
                    // 0,0,"Splatoon-3.jpeg",0,0
                    beatmap->backgroundFilename = line.substr(line.find('"') + 1, line.find_last_of('"') - line.find('"') - 1);
                }
            }
            else if (section == BeatmapSection::COLOURS)
            {
                // if the line starts with "Combo", it's a combo colour
                if (line.find("Combo") != std::string::npos)
                {
                    SDL_Color c = {.a = 255};
                    if (sscanf(line.c_str(), "%s : %hhd,%hhd,%hhd", &discard, &c.r, &c.g, &c.b) == EOF)
                        ERR("Failed to parse combo colour: %d, and %d", c.r, c.g)

                    beatmap->comboColours.push_back(c);
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

                beatmap->timingPoints.push_back(timingPoint);
            }
        }

        infile.close();

        // set difficulty from version
        std::string lowerVersion = beatmap->version;
        std::transform(lowerVersion.begin(), lowerVersion.end(), lowerVersion.begin(), ::tolower);

        if (lowerVersion.find("easy") != std::string::npos || lowerVersion.find("cup") != std::string::npos)
            beatmap->difficulty = Difficulty::CUP;
        else if (lowerVersion.find("normal") != std::string::npos || lowerVersion.find("salad") != std::string::npos)
            beatmap->difficulty = Difficulty::SALAD;
        else if (lowerVersion.find("hard") != std::string::npos || lowerVersion.find("platter") != std::string::npos || lowerVersion.find("advanced") != std::string::npos)
            beatmap->difficulty = Difficulty::PLATTER;
        else if (lowerVersion.find("insane") != std::string::npos || lowerVersion.find("rain") != std::string::npos)
            beatmap->difficulty = Difficulty::RAIN;
        else if (lowerVersion.find("expert+") != std::string::npos || lowerVersion.find("overdose+") != std::string::npos || lowerVersion.find("extra") != std::string::npos)
            beatmap->difficulty = Difficulty::FEAST;
        else if (lowerVersion.find("expert") != std::string::npos || lowerVersion.find("overdose") != std::string::npos)
            beatmap->difficulty = Difficulty::OVERDOSE;
        else
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknown difficulty %s for beatmap \"%s\", using default.", lowerVersion.c_str(), beatmap->title.c_str());


        // this monster of a regex was made by me a year ago and removes unnecessary stuff from titles
        // tests: https://regex101.com/r/BfLme6/1
        // src: https://github.com/LiterallyFabian/SVB/blob/main/app/public/catch/js/fillFeed.js#L30
        std::regex titleRegex(R"(((\[.+\]$)|(\(.+\)$)|(\-.+\-$)|(\~.+\~$)|((feat|ft| ver ).+)|(\/|\-.+remix))$)", std::regex_constants::icase);
        beatmap->cleanedTitle = std::regex_replace(beatmap->title, titleRegex, "");

        auto finish = std::chrono::high_resolution_clock::now();
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

        SDL_Log("Beatmap \"%s (%s)\" loaded and parsed in %lld µs.", beatmap->title.c_str(), beatmap->version.c_str(), microseconds.count());

        return beatmap;
    }

    std::vector<HitObject> Beatmap::getHitObjects()
    {
        std::vector<HitObject> hitObjects;


    }

    SDL_Texture *Beatmap::getBackgroundTexture()
    {
        if (backgroundTexture != nullptr)
            return backgroundTexture;

        std::string fullPath = constants::gResPath + "beatmaps/" + beatmapDir + "/" + backgroundFilename;
        backgroundTexture = IMG_LoadTexture(fruitwork::sys.get_renderer(), fullPath.c_str());

        if (backgroundTexture == nullptr)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background texture %s: %s", fullPath.c_str(), IMG_GetError());

        return backgroundTexture;
    }

    Mix_Music *Beatmap::getAudio()
    {
        if (music != nullptr)
            return music;

        std::string fullPath = constants::gResPath + "beatmaps/" + beatmapDir + "/" + audioFilename;
        music = Mix_LoadMUS(fullPath.c_str());

        if (music == nullptr)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load audio %s: %s", fullPath.c_str(), Mix_GetError());

        return music;
    }

    Beatmap::~Beatmap()
    {
        if (backgroundTexture != nullptr)
            SDL_DestroyTexture(backgroundTexture);

        if (music != nullptr)
            Mix_FreeMusic(music);
    }

} // yuzu

#pragma GCC diagnostic pop