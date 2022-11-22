#ifndef YUZU_CATCH_BEATMAP_H
#define YUZU_CATCH_BEATMAP_H

#include <string>
#include <vector>
#include <SDL.h>
#include "Fruit.h"

namespace yuzu
{

    struct Beatmap {

        /***
         * Loads and returns a beatmap from the given path.
         * @param beatmapPath The path to the beatmap file
         * @return The beatmap
         */
        static Beatmap loadBeatmap(const std::string &beatmapPath);

        enum class SampleType {
            DEFAULT,
            NORMAL,
            SOFT,
            DRUM
        };

        /**
         * The different sections in the .osu file.
         */
        enum class BeatmapSection {
            NONE,
            GENERAL,
            METADATA,
            DIFFICULTY,
            EVENTS,
            TIMINGPOINTS,
            COLOURS,
            HITOBJECTS
        };

        // [General]
        SampleType sampleType = SampleType::NORMAL;

        std::string beatmapDir;
        std::string audioFilename;
        std::string backgroundFilename;

        // [Metadata]
        /** The title of the song. */
        std::string title;

        /** The artist of the song. */
        std::string artist;

        /** The username of the creator of the beatmap, not song artist. */
        std::string creator;

        /** Usually referred to as "Difficulty" in osu!. */
        std::string version;

        std::string tags;

        // [Difficulty]
        double sliderMultiplier;
        double sliderTickRate;

        // [TimingPoints]
        struct TimingPoint {

            /** Start time of the timing section, in milliseconds. */
            int offset;

            /** The duration of a beat in milliseconds, OR slider velocity for inherited timing points. */
            double beatLength;

            /** Override sample type for this timing section. */
            SampleType sampleType;

            bool isUninherited;
            bool isKiai;
        };

        std::vector<TimingPoint> timingPoints;

        // [Colours]
        std::vector<SDL_Color> comboColours;

        // [HitObjects]
        std::vector<HitObject> getHitObjects();


    };

} // yuzu
#endif //YUZU_CATCH_BEATMAP_H
