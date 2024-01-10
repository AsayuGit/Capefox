#include "ReportWriters/RSTReportWriter.hpp"

#include <sstream>

using namespace Capefox;

// This method format the incoming text to produce the main title in .rst format
std::string RSTReportWriter::WriteMainTitle(const std::string& text) {
    std::stringstream ss;

    // To do so we sourround the test by equaly sized '=' bars
    ss << std::string(text.length(), '=') << std::endl;
    ss << text << std::endl;
    ss << std::string(text.length(), '=') << std::endl;
    return ss.str();
}

// This method format the incoming text to produce a standard title in .rst format
std::string RSTReportWriter::WriteTitle(const std::string& text) {
    std::stringstream ss;

    ss << std::endl;
    ss << text << std::endl;
    // To do so we underline the text with an equaly sized '=' bar
    ss << std::string(text.length(), '=') << std::endl;
    return ss.str();
}

// This method format the incoming text to produce a subtile in .rst format
std::string RSTReportWriter::WriteSubTitle(const std::string& text) {
    std::stringstream ss;

    ss << std::endl;
    ss << text << std::endl;
    // To do so we underline the text with an equaly sized '_' bar
    ss << std::string(text.length(), '_') << std::endl;
    return ss.str();
}

// This method format the incoming text to put it in bold
std::string RSTReportWriter::Bold(const std::string& text) {
    return "**" + text + "** ";
}

// This method format the incoming text to ensure it doesn't break the .rst format
std::string RSTReportWriter::Escape(const std::string& text) {
    std::stringstream ss;
    
    // To do so we step on each caracter
    for (char c : text) {
        // if we match on a character that need to be escaped
        switch (c) {
            case '*':
                // we escape it with backslashes
                ss << '\\' << c;
                break;

            default:
                // Otherwise we copy the character straight through
                ss << c;
                break;
        }
    }

    // And return the newly built text
    return ss.str();
}