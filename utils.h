//
// Created by viruser on 2022/12/27.
//

#ifndef UNTITLED_UTILS_H
#define UNTITLED_UTILS_H
#include <algorithm>
#include <sstream>
#include <iterator>
#include <ctime>
#include <sys/time.h>

template<typename InputIt>
static std::string join(InputIt begin,
                 InputIt end,
                 const std::string & separator =", ",  // see 1.
                 const std::string & concluder ="")    // see 1.
{
    std::ostringstream ss;
    using value_type = typename std::iterator_traits<InputIt>::value_type;
    std::copy(begin, end, std::ostream_iterator<value_type>(ss, separator.c_str()));

    ss << concluder;
    return ss.str();
}

static time_t get_time() {
    struct timeval time_now{};
    gettimeofday(&time_now, nullptr);
    time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
    return msecs_time;
}

#endif //UNTITLED_UTILS_H
