#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

template<typename Clock = std::chrono::steady_clock, typename Duration = typename Clock::duration>
class timer
{
    using time_point = std::chrono::time_point<Clock, Duration>;
    time_point t0;

    public:
    void start()
    {
        t0 = Clock::now();
    }
    void stop(std::ofstream& file)
    {
        time_point t1 = Clock::now();
        // Seconds, as a double precision float
        file << std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count() << std::endl;
    }
    void stop()
    {
        time_point t1 = Clock::now();
        // Seconds, as a double precision float
        std::cout << std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count() << std::endl;
    }
};
