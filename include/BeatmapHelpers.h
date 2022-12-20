#ifndef YUZU_CATCH_BEATMAPHELPERS_H
#define YUZU_CATCH_BEATMAPHELPERS_H

#include "Beatmap.h"

namespace yuzu
{

    class BeatmapHelpers {
    public:
        /**
         * Extracts a double from a beatmap line.
         * @example "SliderMultiplier:1.4" -> 1.4
         * @return The extracted double.
         */
        static double getDoubleFromLine(const std::string &);

        /**
         * Extracts a string from a beatmap line. The returned string will be trimmed.
         * @example "Title:Yuzu Catch" -> "Yuzu Catch"
         * @example "Title: Yuzu Catch " -> "Yuzu Catch"
         * @return The extracted string.
         */
        static std::string getStringFromLine(const std::string &);

        /**
         * Extracts a beatmap section from a beatmap line.
         * @example "[General]" -> Beatmap::BeatmapSection::GENERAL
         * @return The extracted beatmap section, or Beatmap::BeatmapSection::NONE if the line is not a section.
         */
        static Beatmap::BeatmapSection getSectionFromLine(const std::string &);

        /**
         * Splits a string into a vector of strings, using the given delimiter.
         * @param s The string to split
         * @param delimiter The delimiter to split the string with
         * @return A vector of strings
         */
        static std::vector<std::string> splitToString(const std::string &s, char delimiter);

        /**
         * Splits a string into a vector of ints, using the given delimiter.
         * @param s The string to split
         * @param delimiter The delimiter to split the string with
         * @return A vector of ints
         */
        static std::vector<int> splitToInt(const std::string &s, char delimiter);
    };

} // yuzu

#endif //YUZU_CATCH_BEATMAPHELPERS_H
