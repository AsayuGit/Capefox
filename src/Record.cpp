#include "Record.hpp"

#include <ctime>

using namespace Capefox;

template <> IP Record::valueAs<IP>() const {
    return IP(toString());
}

template<> std::string Record::valueAs<std::string>() const { return valueAsString(); }
template<> uint64_t Record::valueAs<uint64_t>() const { return valueAsInteger(); }
template<> std::tm Record::valueAs<std::tm>() const { return valueAsTime(); }
template<> EventType Record::valueAs<EventType>() const { return valueAsEventType(); }