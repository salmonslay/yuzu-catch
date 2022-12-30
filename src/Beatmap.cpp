#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Wdangling-gsl"
#pragma GCC diagnostic ignored "-Wformat"

#include "Beatmap.h"
#include "Constants.h"
#include "System.h"
#include "JuiceDrop.h"
#include "Banana.h"
#include "BeatmapHelpers.h"
#include <fstream>
#include <vector>
#include <chrono>
#include <SDL_image.h>
#include <algorithm>
#include <regex>

namespace yuzu
{
    Beatmap *Beatmap::loadBeatmap(const std::string &beatmapPath)
    {
        //SDL_Log("Parsing beatmap: %s...", beatmapPath.c_str());
        auto start = std::chrono::high_resolution_clock::now();

        std::ifstream infile(constants::gResPath + "beatmaps/" + beatmapPath);

        auto *beatmap = new Beatmap();
        std::string line;
        BeatmapSection section = BeatmapSection::NONE;
        void *discard; // used to discard unused values

        beatmap->beatmapDir = beatmapPath.substr(0, beatmapPath.find_last_of('/'));
        beatmap->beatmapPath = beatmapPath;

        while (std::getline(infile, line))
        {
            if (line.empty())
                continue;

            if (line[0] == '[') // set new section
            {
                section = BeatmapHelpers::getSectionFromLine(line);
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
                    beatmap->audioFilename = BeatmapHelpers::getStringFromLine(line);
            }
            else if (section == BeatmapSection::METADATA)
            {
                if (line.find("Title:") != std::string::npos)
                    beatmap->title = BeatmapHelpers::getStringFromLine(line);
                else if (line.find("Artist:") != std::string::npos)
                    beatmap->artist = BeatmapHelpers::getStringFromLine(line);
                else if (line.find("Creator:") != std::string::npos)
                    beatmap->creator = BeatmapHelpers::getStringFromLine(line);
                else if (line.find("Version:") != std::string::npos)
                    beatmap->version = BeatmapHelpers::getStringFromLine(line);
                else if (line.find("Tags:") != std::string::npos)
                    beatmap->tags = BeatmapHelpers::getStringFromLine(line);
            }
            else if (section == BeatmapSection::DIFFICULTY)
            {
                if (line.find("SliderMultiplier:") != std::string::npos)
                    beatmap->sliderMultiplier = BeatmapHelpers::getDoubleFromLine(line);
                else if (line.find("SliderTickRate:") != std::string::npos)
                    beatmap->sliderTickRate = BeatmapHelpers::getDoubleFromLine(line);
            }
            else if (section == BeatmapSection::EVENTS)
            {
                if (line.rfind("0,0,\"", 0) == 0)
                {
                    // 0,0,"Splatoon-3.jpeg",0,0
                    beatmap->backgroundFilename = line.substr(line.find('"') + 1, line.find_last_of('"') - line.find('"') - 1);
                }
            }
            else if (section == BeatmapSection::HITOBJECTS)
            {
                int time;
                sscanf(line.c_str(), "%d,%d,%d", &discard, &discard, &time);
                beatmap->length = time + 2500;
            }
        }

        infile.close();

        // set difficulty from version
        std::string lowerVersion = beatmap->version;
        std::transform(lowerVersion.begin(), lowerVersion.end(), lowerVersion.begin(), ::tolower);

        if (lowerVersion.find("easy") != std::string::npos
            || lowerVersion.find("cup") != std::string::npos)
            beatmap->difficulty = Difficulty::CUP;
        else if (lowerVersion.find("normal") != std::string::npos
                 || lowerVersion.find("salad") != std::string::npos)
            beatmap->difficulty = Difficulty::SALAD;
        else if (lowerVersion.find("hard") != std::string::npos
                 || lowerVersion.find("platter") != std::string::npos
                 || lowerVersion.find("advanced") != std::string::npos)
            beatmap->difficulty = Difficulty::PLATTER;
        else if (lowerVersion.find("insane") != std::string::npos
                 || lowerVersion.find("rain") != std::string::npos)
            beatmap->difficulty = Difficulty::RAIN;
        else if (lowerVersion.find("extra") != std::string::npos
                 || lowerVersion.find("expert") != std::string::npos
                 || lowerVersion.find("overdose") != std::string::npos)
            beatmap->difficulty = Difficulty::FEAST;
        else
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknown difficulty \"%s\" for beatmap \"%s\", using default.", lowerVersion.c_str(), beatmap->title.c_str());


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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp" // warning for using rand()

    void Beatmap::loadGameplayInformation(std::vector<GameScene::HitObjectSet> hitObjectSets,
                                          yuzu::GameScene::HitObjectSet bananaSet,
                                          SDL_Texture *dropTexture,
                                          yuzu::GameScene::HitSampleSet hitSampleSet)
    {
        for (auto &hitObject: hitObjects)
            delete hitObject;
        hitObjects.clear();
        timingPoints.clear();
        comboColours.clear();

        auto start = std::chrono::high_resolution_clock::now();

        std::ifstream infile(constants::gResPath + "beatmaps/" + beatmapPath);

        std::string line;
        BeatmapSection section = BeatmapSection::NONE;

        // temps
        double beatLength = -1;
        Beatmap::TimingPoint lastTimingPoint{};
        void *discard;

        while (std::getline(infile, line))
        {
            if (line.empty())
                continue;

            if (line[0] == '[') // set new section
            {
                section = BeatmapHelpers::getSectionFromLine(line);
                continue;
            }

            if (section == BeatmapSection::TIMINGPOINTS)
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

                if (beatLength == -1)
                    beatLength = timingPoint.beatLength;

                timingPoints.push_back(timingPoint);
            }
            else if (section == BeatmapSection::COLOURS)
            {
                // if the line starts with "Combo", it's a combo colour
                if (line.find("Combo") != std::string::npos)
                {
                    SDL_Color c = {.a = 255};
                    if (sscanf(line.c_str(), "%s : %hhd,%hhd,%hhd", &discard, &c.r, &c.g, &c.b) == EOF)
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to parse combo colour: %d, and %d", c.r, c.g);

                    comboColours.push_back(c);
                }
            }
            else if (section == BeatmapSection::HITOBJECTS)
            {
                /**
                 * @see Source https://github.com/LiterallyFabian/SVB/blob/main/app/public/catch/js/processor.js#L304-L409
                 * @see Docs https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#hit-objects
                 * x,y,time,type,hitSound,objectParams,hitSample
                 */

                // add default combo colors if none are specified
                if (comboColours.empty())
                {
                    comboColours.push_back({0, 202, 0, 255});
                    comboColours.push_back({18, 124, 255, 255});
                    comboColours.push_back({242, 24, 57, 255});
                    comboColours.push_back({255, 192, 0, 255});
                }

                std::vector<std::string> values = BeatmapHelpers::splitToString(line, ',');
                int x = std::stoi(values[0]);
                int time = std::stoi(values[2]);
                unsigned int type = std::stoi(values[3]); // 0 = circle, 1 = slider, 2 = new combo, 3 = spinner
                int hitSound = std::stoi(values[4]);
                bool isNewCombo = type & (1 << 2);

                if (type & 1) // circle - create one fruit
                {
                    SDL_Color c = comboColours[std::rand() % comboColours.size()];
                    GameScene::HitObjectSet fruit = hitObjectSets[std::rand() % hitObjectSets.size()];
                    Fruit *f = Fruit::getInstance(x, time, fruit.baseTexture, fruit.overlayTexture, c, hitSound);
                    hitObjects.push_back(f);

                }
                else if (type & 2) // slider - create fruits and juice drops
                {
                    // override hitsounds
                    bool overrideHitSounds = values.size() > 8;
                    std::vector<int> hitSoundOverrides;
                    int overrideIndex = 0;
                    if (overrideHitSounds)
                        hitSoundOverrides = BeatmapHelpers::splitToInt(values[8], '|');

                    // start fruit
                    SDL_Color c = comboColours[std::rand() % comboColours.size()];
                    GameScene::HitObjectSet fruit = hitObjectSets[std::rand() % hitObjectSets.size()];
                    int hs = overrideHitSounds ? hitSoundOverrides[overrideIndex++] : hitSound;
                    Fruit *f = Fruit::getInstance(x, time, fruit.baseTexture, fruit.overlayTexture, c, hs, isNewCombo);
                    hitObjects.push_back(f);

                    // update beat length by finding the timing point that is closest to the time, but not greater than the time
                    auto timing = std::find_if(timingPoints.begin(), timingPoints.end(), [time](const TimingPoint &tp)
                    {
                        return tp.offset < time;
                    });

                    if (timing != timingPoints.end())
                        beatLength = timing->beatLength;

                    std::vector<std::string> sliderPositions = BeatmapHelpers::splitToString(values[5], '|');
                    std::vector<std::string> sliderEndPosition = BeatmapHelpers::splitToString(sliderPositions.back(), ':');

                    // calculate droplet stuff
                    int sliderEndPos = std::stoi(sliderEndPosition[0]);
                    int repeats = std::stoi(values[6]); // how many times the slider will repeat
                    double sliderLength = std::stoi(values[7]) / (sliderMultiplier * 100) * beatLength * repeats; // how long the slider is in milliseconds
                    double dropletsPerRepeat = std::stoi(values[7]) / 20; // actual value doesn't really matter, it's just for looks
                    int droplets = std::max(static_cast<int>(dropletsPerRepeat * repeats), 1); // amount of droplets slider contains
                    double dropletDelay = sliderLength / droplets; // delay between each droplet
                    double diff = (x - sliderEndPos) / droplets; // difference in x each droplet should have

                    int currentDroplet = 0;
                    for (int i = 0; i < droplets; i++)
                    {
                        int dropletPos = x - (diff * i);
                        if (currentDroplet == dropletsPerRepeat)
                        {
                            hs = overrideHitSounds ? hitSoundOverrides[overrideIndex++] : hitSound;
                            Fruit *midFruit = Fruit::getInstance(dropletPos, time + dropletDelay * i, fruit.baseTexture, fruit.overlayTexture, c, hs);
                            hitObjects.push_back(midFruit);
                            currentDroplet = 0;
                        }
                        else
                        {
                            JuiceDrop *jd = JuiceDrop::getInstance(dropletPos, time + dropletDelay * i, dropTexture, c);
                            hitObjects.push_back(jd);
                        }
                        currentDroplet++;
                    }

                    // slider end fruit
                    hs = overrideHitSounds ? hitSoundOverrides[overrideIndex++] : hitSound;
                    Fruit *f2 = Fruit::getInstance(sliderEndPos, time + droplets * dropletDelay, fruit.baseTexture, fruit.overlayTexture, c, hs, isNewCombo);
                    hitObjects.push_back(f2);
                }
                else if (type & 8) // spinner - create spinner
                {
                    std::vector<SDL_Color> bananaColors = {
                            {255, 240, 0,  255},
                            {255, 192, 0,  255},
                            {214, 221, 28, 255}
                    };

                    for (int i = time; i < std::stoi(values[5]); i += 60)
                    {
                        SDL_Color c = bananaColors[std::rand() % bananaColors.size()];
                        Banana *b = Banana::getInstance(std::rand() % 512, i, bananaSet.baseTexture, bananaSet.overlayTexture, c);
                        hitObjects.push_back(b);
                    }
                }
            }
        }

        infile.close();

        auto finish = std::chrono::high_resolution_clock::now();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);

        SDL_Log("Gameplay information for beatmap \"%s (%s)\"  parsed in %lld ms.", title.c_str(), version.c_str(), milliseconds.count());
    }

#pragma clang diagnostic pop

    SDL_Texture *Beatmap::getBackgroundTexture() // TODO: free texture
    {
        if (backgroundTexture != nullptr)
            return backgroundTexture;

        std::string fullPath = constants::gResPath + "beatmaps/" + beatmapDir + "/" + backgroundFilename;
        backgroundTexture = IMG_LoadTexture(fruitwork::sys.getRenderer(), fullPath.c_str());

        if (backgroundTexture == nullptr)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load background texture %s: %s", fullPath.c_str(), IMG_GetError());

        return backgroundTexture;
    }

    Mix_Music *Beatmap::getAudio() // TODO: free audio
    {
        if (music != nullptr)
            return music;

        std::string fullPath = beatmapDir + "/" + audioFilename;
        music = Mix_LoadMUS(fullPath.c_str());

        if (music == nullptr)
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load audio %s: %s", fullPath.c_str(), Mix_GetError());

        return music;
    }

    Beatmap::~Beatmap()
    {
        for (auto &hitObject: hitObjects)
            delete hitObject;
        hitObjects.clear();

        if (backgroundTexture != nullptr)
            SDL_DestroyTexture(backgroundTexture);

        if (music != nullptr)
            Mix_FreeMusic(music);
    }

    std::string Beatmap::getLengthString() const
    {
        int minutes = length / 60000;
        int seconds = (length - minutes * 60000) / 1000;

        char buffer[6];
        sprintf(buffer, "%02d:%02d", minutes, seconds);

        return buffer;
    }

} // yuzu

#pragma GCC diagnostic pop