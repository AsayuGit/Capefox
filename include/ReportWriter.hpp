#pragma once

#include <string>
#include <iostream>

namespace Capefox {
    class ReportWriter {
        public:
            virtual ~ReportWriter() = default;

            virtual std::string WriteMainTitle(const std::string& text) = 0;
            virtual std::string WriteTitle(const std::string& text) = 0;
            virtual std::string WriteSubTitle(const std::string& text) = 0;

            virtual std::string Bold(const std::string& text) = 0;
            virtual std::string Escape(const std::string& text) = 0;
    };
};