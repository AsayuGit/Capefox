#pragma once

#include "ReportWriter.hpp"

namespace Capefox {
    class RSTReportWriter : public ReportWriter {
        public:
            std::string WriteMainTitle(const std::string& text) override;
            std::string WriteTitle(const std::string& text) override;
            std::string WriteSubTitle(const std::string& text) override;

            std::string Bold(const std::string& text) override;
            std::string Escape(const std::string& text) override;
    };
};