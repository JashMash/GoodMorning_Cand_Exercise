#include "../headers/ResponseTime.hpp"

#include <string>
#include <sstream>
#include <iomanip>

#include <cmath>
#include <vector>
#include <tuple>
#include <chrono>
using namespace std;
using namespace std::chrono;


int ResponseTime::pullMinBins(void){
    /*
    Retrieves Minimum Bins allowed for histogram.

    Outputs:
        m_minBins : <int> Minimum bins allowed 
    */
    return m_minBins;
}
int ResponseTime::pullMaxBins(void){
    /*
    Retrieves Maximum Bins allowed for histogram.
    
    Outputs:
        m_maxBins : <int> Maximum bins allowed 
    */
    return m_maxBins;
}


// Public
double ResponseTime::meanRespTime(const std::string& uri){
    /*
    Determines Mean for URI provided, 
    Using data from time data in 'm_URIResponseTimes'
    and querying provided URI

    Inputs:
        'uri'  : <const std::string&> 
                    URI as a string
    Outputs:
        'mean' : <double> Mean of all response times for URI provided
    */
    double total=0;
    std::vector <double>  respTimes;

    if (m_URIResponseTimes.find(uri) == m_URIResponseTimes.end()){
        std::string errorMsg ="ERROR in meanRespTime(): URI provided does not have any response time records.\n";
        throw(errorMsg);
    } else {
        respTimes = m_URIResponseTimes[uri];
    }

    // Summing all response times together
    for (int i=0; i< int(respTimes.size()); i++){
        total+=respTimes[i];
    };

    double mean = total/respTimes.size();
    return mean;
}

// Public
double ResponseTime::standardDeviRespTime(const std::string& uri){
    /*
    Determines Standard Deviation for URI provided, and 
    returns Standard Deviation at string of URI

    Inputs:
        'uri'  : <const std::string&> 
                    URI as a string
    Outputs:
        'standardDeviation' : <double> Standard deviation for URI provided
    */

    std::vector <double>  respTimes;
    std::string errorMsg;
    // Checking for records at URI
    if (m_URIResponseTimes.find(uri) == m_URIResponseTimes.end()){
        errorMsg = "ERROR in standardDeviRespTime(): URI provided does not" 
                                        " have any response time records.\n";
        throw(errorMsg);
    } else {
        respTimes = m_URIResponseTimes[uri];
    }

    // Checking for enough points to calculate Standard Deviation
    if (respTimes.size()<=1){
        errorMsg ="ERROR in standardDeviRespTime(): Standard Deviation "
                                    "calculation needs more than 1 point.\n";
        throw(errorMsg);
    }

    double mean = meanRespTime(uri);
    double sumOfDiffSqr =0;

    for (int i=0; i< int(respTimes.size()); i++){
        sumOfDiffSqr += pow((respTimes[i] - mean),2);
    };
    
    double variance = double(sumOfDiffSqr/double(respTimes.size()));
    double standardDeviation =sqrt(variance);
    return standardDeviation;
}

// Public
std::vector<std::tuple<std::string, int>> ResponseTime::histogramResptime
                                (const std::string& uri, const int maxBins){
    /*
    Creates a vector object populated with a normalized histogram for 
    the URI inputed and maximum bins provided within in reason.

    Inputs:
        'uri'     : <const std::string&> 
                    URI as a string
        'maxBins' : <const int> 
                    Maximum bins for histogram 
    
    Outputs:
        'histogramData' : <std::vector<std::tuple<std::string, int>>>
                        Data type containing Normalized histogram 
                        each data entry contains a tuple
                        containing x axis marker in index 0
                        and number of points at marker in index 1
    
    */

    std::vector<std::tuple<std::string, int>> histogramData;
    std::vector <double> respTimes;
    std::string errorMsg;
    // Checking if URI exists in records
    if (m_URIResponseTimes.find(uri) == m_URIResponseTimes.end()){
        errorMsg ="ERROR in histogramResptime(): URI provided does"
                                        " not have any records.\n";
        throw(errorMsg);
    } else {
        respTimes = m_URIResponseTimes[uri];
    }

// Checking Maximum.Minimum Bins allowed
    // Checking if input bins is less than 'm_minBins'.
    if (maxBins < m_minBins){
        errorMsg ="ERROR in histogramResptime(): Cannot not have less than "
                                            + to_string(m_minBins)+ " bins.\n";
        throw(errorMsg);
    }
    // Checking if input bins is more than 'm_maxBins'.
    if (maxBins > m_maxBins){
        errorMsg ="ERROR in histogramResptime(): Cannot not have more than "
                                             + to_string(m_maxBins)+" bins.\n";
        throw(errorMsg);
    }

    // Checking if more bins than points.
    if (maxBins > respTimes.size()){
        errorMsg = "ERROR in histogramResptime(): More bins than points.\n";
        throw(errorMsg);
    }


    // Creating normalized data
    std::vector<double> normalizedData =  dataNormalizer(respTimes);

    // Finds bin max and # of occurance in each bin
    auto binData = findBinData(maxBins, normalizedData);

    // Creating the object and storing Bin data
    double factor = double(1.0/maxBins), binFactor=0;
    std::stringstream binRange;

    for (int currBinPoints : binData){
        binFactor+=factor;            
        binRange<< std::fixed << setprecision(2)<<"("<<binFactor-factor<<
                                                        " - "<<binFactor<<")";

        histogramData.push_back(std::tuple<std::string, 
                                            int>{binRange.str(), currBinPoints});
        binRange.str("");
    };

    // Eliminating bins without data on edges
    while (std::get<1>(histogramData[0]) ==0 || std::get<1>(histogramData.back()) ==0){
        if (std::get<1>(histogramData[0]) ==0)
            histogramData.erase(histogramData.begin());
        else if (std::get<1>(histogramData.back()))
            histogramData.pop_back();
    }

    return histogramData;
}

// Private
std::vector<double> ResponseTime::dataNormalizer
                                            (std::vector <double>& responseData){
    /*
    Max-min Normalization perfomed on input Response data set

    Inputs:
        'responseData' : <std::vector <double>>
                            Sorted Response times
    
    Outputs:
        'normalizedData' : <std::vector<double>> 
                            Datatype that contains normalized Response times

    */
    double max = responseData.back();
    double min = responseData[0];
    double normPoint;
    std :: vector <double> normalizedData;

    // Creating normalized data
    for (double i : responseData){
        normPoint = (i-min)/(max-min);
        normalizedData.push_back(normPoint);
    };
    return normalizedData;
}

// Private
std::vector <int> ResponseTime::findBinData
                            (int maxBins, std :: vector <double>& normalizedData){
    /*
    Creates seperate counts for data in bins.
    Determines largest bins.

    Inputs:
        'maxBins'        : <int> maximium bins for histogram
        'normalizedData' : <std :: vector <double>&> 
                            normalized response time data

    Outputs:
        'binData' : <std::vector <int>> 
                    bin with number of points within the range

    */
    // Putting the bin facter little greater than 0 makes sure the point at 1.0 will 
    // be added to last bin.
    double factor = double(1.0/maxBins), binFactor=0.001;
    int totalPoints = int(normalizedData.size());

    int currBinPoints=0, index=0;
    std::vector <int> binData;

    for (int i=0; i<maxBins; i++){
        binFactor+=factor;
        while((binFactor>= normalizedData[index]) && index < totalPoints){
            currBinPoints+=1;
            index+=1;
        }
        binData.push_back(currBinPoints);
        
        currBinPoints=0;
    };
    return  binData;
}

// Public
void ResponseTime::uriTimer(const std::string& uri){
    /*
    Initiates the Request while tracking its duration.

    Duration is then appended to a dictionary of prior response times
    using the function 'responseTimeInserter()'

    Inputs:
        'uri'     : <const std::string&> 
                    URI as a string
    */
    auto startTime = high_resolution_clock::now();

    Request::process(uri);

    auto endTime = high_resolution_clock::now();
    
    // Calculating duration in milliseconds
    auto duration = duration_cast<microseconds>(endTime - startTime)/1e3;

    responseTimeInserter(uri, double(duration.count()));
}

// Private
void ResponseTime::responseTimeInserter(const std::string& uri, double duration){
    /*
    This actively makes sure the durations are kept in a sorted manner.
    A binary search is used to find index at which response time 
    should be stored. Then stores response time at that index.

    Inputs:
        'uri'      : <const std::string&> 
                    URI as a string

        'duration' : <double>
                    Time in milliseconds
    
    */

    // If first entry
    if (m_URIResponseTimes.find(uri) == m_URIResponseTimes.end()){
        m_URIResponseTimes[uri].push_back(duration);
    } else {
        std::vector <double> respTimes = m_URIResponseTimes[uri];
        int lo = 0, mid;
        int hi = int(respTimes.size())-1;

        if (duration > respTimes[hi]){
            m_URIResponseTimes[uri].push_back(duration);
            return;
        } else if (duration < respTimes[lo]){
            m_URIResponseTimes[uri].insert(m_URIResponseTimes[uri].begin() + lo, 
                                                                        duration);
            return;
        }

        while ((hi-lo>1) && (hi>=lo)){
            mid = lo+(hi-lo)/2;

            if (duration> respTimes[mid]){
                lo = mid+1;
            } else {
                hi = mid;
            }
        }

        int indexToInsert;
        if (respTimes[lo] >=duration){
            indexToInsert = lo;
        } else if (respTimes[hi] ==duration){
            indexToInsert =  hi;
        } else {
            indexToInsert =  hi;
        }
        m_URIResponseTimes[uri].insert(m_URIResponseTimes[uri].begin() 
                                                        + indexToInsert, duration);
        return;
    }
}