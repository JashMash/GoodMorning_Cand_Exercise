#include "Request.hpp"

#include <random>
#include <chrono>
#include <thread>
#include <map>
#include <algorithm>

void Request::simulateLatency(const std::string& uri)
{
    static std::map<std::string, std::normal_distribution<double>> distributions{
        {{"uri1"}, std::normal_distribution<double>(10000.0, 2500.0)},
        {{"uri2"}, std::normal_distribution<double>(20000.0, 7500.0)}};
    static std::normal_distribution<double> defaultDistribution(
            15000.0,
            5000.0);
    static std::random_device device;

    const auto it = distributions.find(uri);
    std::normal_distribution<double>& distribution(
            it == distributions.end() ? defaultDistribution : it->second);

    const auto responseTime = distribution(device);
    if(responseTime >= 1.0)
        std::this_thread::sleep_for(std::chrono::microseconds(
                    static_cast<std::chrono::microseconds::rep>(responseTime)));
}

std::string Request::process(const std::string& uri)
{
    start(uri);

    // Let's pretend the following line is doing something instead of just
    // simulating response latency
    simulateLatency(uri);

    finish();

    return "Sample response.";
}

void Request::start(const std::string& uri)
{
    // Base class version does nothing
}

void Request::finish()
{
    // Base class version does nothing
}
