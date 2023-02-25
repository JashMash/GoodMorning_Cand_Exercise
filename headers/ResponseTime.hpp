#include "Request.hpp"

#include <string>
#include <map>
#include <vector>
#include <chrono>
using namespace std;

/**
 * Reponse Time analyzing class
 * 
 * Instantiations of this class are used to track amount of time each process 
 * call takes. Using the URI this class provides access to mean response times,
 * standard deviation of response times. Also gives ability to create a 
 * normalized histogram.
 */
class ResponseTime : public Request
{
private:
    // All response time storage variable:
    std::map< std::string, std::vector<double> > m_URIResponseTimes{}; 

    // Histogram bin limits
    int m_maxBins =20;
    int m_minBins =5;

    // Inserts found response time into sorted array for URI 
    virtual void responseTimeInserter(const std::string& uri, 
                                        double duration);

    // Histogram Functions
    std::vector<double> dataNormalizer(std::vector <double>& responseData);
    std::vector <int> findBinData(int maxBins, 
                                    std :: vector <double>& normalizedData);

public:
    // Calculates and stores response time
    virtual void uriTimer(const std::string& uri);

    // Creates Normalized histogram 
    virtual std::vector<std::tuple<std::string, int>> histogramResptime
                            (const std::string& uri, const int maxBins);

    // Calculation funtions:
    // Mean
    virtual double meanRespTime(const std::string& uri);

    // Standard Deviation
    virtual double standardDeviRespTime(const std::string& uri);

    // Retrieving Min and Max bins 
    virtual int pullMinBins(void);
    virtual int pullMaxBins(void);
    
};
