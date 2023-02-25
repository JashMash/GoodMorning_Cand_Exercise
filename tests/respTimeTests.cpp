#include "../headers/ResponseTime.hpp"

#include <string>
#include <iostream>

using namespace std;

// Prototypes
// Functions to retrieve data from Instantiated Object
void dataCollection(ResponseTime& classObj, const string uri, int dataPoints);

// Test messaging
void testSummary(std::string funcName, int testFailures, int maxFailures);
void failMsg(std::string testName, std::string result, std::string expected);
void successMsg(std::string testName, std::string result);


////// Combined Mean function tests //////
void test_mean(void);

// Individual test for Mean
//      Test : No data points and find Mean
int test_mean_noData(ResponseTime& timer, std::string uri, int testFailures);
//      Test : Normal data points and find Mean
int test_mean_normalData(ResponseTime& timer, std::string uri, int testFailures);



////// Combined Standard Deviation function tests //////
void test_standardDeviation(void);

// Individual test for Standard Deviation //////
//      Test : No data points and find Standard Deviation
int test_SD_noData(ResponseTime& timer, std::string uri, int testFailures);
//      Test : One data point and find Standard Deviation
int test_SD_oneData(ResponseTime& timer, std::string uri, int testFailures);
//      Test : Normal data points and find 
int test_SD_normalData(ResponseTime& timer, std::string uri, int testFailures);

// Combined Normalized Histogram function tests
void test_normalizedHistogram(void);
// Individual test for Normalized Histogram 
//      Test : No data points and find Normalized Histogram
int test_Histo_noData(ResponseTime& timer, std::string uri, int testFailures);
//      Test : less bins than m_minBins
int test_Histo_binMinCheck(ResponseTime& timer, 
                                            std::string uri, int testFailures);
//      Test : more bins than m_maxBins
int test_Histo_binMaxCheck(ResponseTime& timer, 
                                            std::string uri, int testFailures);
//      Test : less points than maxBins input
int test_Histo_dataToBinCheck(ResponseTime& timer, 
                                            std::string uri, int testFailures);
//      Test : contains as many points as data points
int test_Histo_totalPointsCheck(std::string uri, int maxBins, 
                                            int dataPoints, int testFailures);
//      Test : edges have data points 
int test_Histo_binEdgeCheck(std::string uri, int maxBins, 
                                            int dataPoints, int testFailures);


int overallTestFailures = 0;


int main(){
    //"uri1"    Mean: 15:90 | Max: 16:65  | Min: 15:55 in 100 datapoints
    //"uri2"    Mean: 27:70 | Max: 29:10  | Min: 26.12 in 100 datapoints

    // Following tests for Mean calculation function
    test_mean();

    // Following tests for Standard Deviation calculation function
    test_standardDeviation();
    
    // Following tests for Histogram calculation function
    test_normalizedHistogram();

    if (overallTestFailures>0)
        printf("Total number of failures: %d\n",  overallTestFailures);
    else
        printf("All Testcases Passed!\n");
}

void dataCollection(ResponseTime& classObj, const string uri, int dataPoints){
    for (int i=0; i<dataPoints; i++)
        classObj.uriTimer(uri);
}

void successMsg(std::string testName, std::string result){
    printf("    SUCCESS! - %s\n", testName.c_str());
    printf("        Expected: %s\n", result.c_str());
}

void failMsg(std::string testName, std::string result, std::string expected){
    printf("    FAILED! - %s\n", testName.c_str());
    printf("        Expected: %s\n", result.c_str());
    printf("        Result  : %s\n\n", expected.c_str());
}

void testSummary(std::string funcName, int testFailures, int maxFailures){
    // Summary
    if (testFailures==maxFailures)
        printf("\nAll %s tests Failed...\n\n", funcName.c_str());
    else if (testFailures>0)
        printf("\n%d - %s tests Failed...\n\n", testFailures, funcName.c_str());
    else
        printf("\nAll %s tests Succeeded.\n\n", funcName.c_str());
    
    overallTestFailures+=testFailures;
}





void test_mean(void){
    printf("\nTesting Mean Functionality:\n");
    
    std::string uri ="uri1";

    ResponseTime timer;
    int testFailures = 0;
    int testCount= 0;
    
    testCount+=1;
    //      Test : No data points and find Mean
    testFailures = test_mean_noData(timer, uri, testFailures);

    testCount+=1;
    //      Test : Normal data points and find Mean
    testFailures = test_mean_normalData(timer, uri, testFailures);
    
    testSummary("meanRespTime()", testFailures, testCount);
}

int test_mean_noData(ResponseTime& timer, std::string uri, int testFailures){
    std::string testName ="meanRespTime() with no data";
    std::string expectedResult = "ERROR in meanRespTime(): URI provided does"
                                " not have any response time records.\n";
    try{
        double result = timer.meanRespTime(uri);
        // Will only get here if it fails 
        failMsg(testName, to_string(result), expectedResult);
        testFailures+=1;
    } catch (std::string errorMsg){
        if (errorMsg == expectedResult)
            successMsg(testName, errorMsg);
        else{
            failMsg(testName, errorMsg, expectedResult);
            testFailures+=1;
        }
    }
    return testFailures;
}

int test_mean_normalData(ResponseTime& timer, std::string uri, int testFailures){
    int dataPoints = 200;
    dataCollection(timer, uri, dataPoints);
    std::string testName ="meanRespTime() with 200 points of data";
    std::string expectedResult = "A value of type double. In range 15 - 17 ms.";
    try{
        double result = timer.meanRespTime(uri);
        if (result>15 && result < 17) 
            successMsg(testName, to_string(result));
        else{
            failMsg(testName, to_string(result), expectedResult);
            testFailures+=1;
            }
    } catch (std::string errorMsg){
        failMsg(testName, errorMsg, expectedResult);
        testFailures+=1;
    }
    return testFailures;
}



void test_standardDeviation(void){
    printf("\nTesting Standard Deviation Functionality:\n");
    
    std::string uri ="uri1"; 

    ResponseTime timer;
    int testFailures = 0;
    int testCount =0;

    testCount+=1;
    //      Test : No data points and find Standard Deviation
    testFailures = test_SD_noData(timer, uri, testFailures);

    testCount+=1;
    //      Test : One data point and find Standard Deviation
    testFailures = test_SD_oneData(timer, uri, testFailures);

    testCount+=1;
    //      Test : Normal data points and find 
    testFailures =  test_SD_normalData(timer, uri, testFailures);

    testSummary("standardDeviRespTime()", testFailures, testCount);
}

int test_SD_noData(ResponseTime& timer, std::string uri, int testFailures){
    std::string testName ="standardDeviRespTime() with no data";
    std::string expectedResult = "ERROR in standardDeviRespTime(): URI provided "
                                "does not have any response time records.\n";
    try{
        double result = timer.standardDeviRespTime(uri);
        // Will only get here if it fails 
        failMsg(testName, to_string(result), expectedResult);
        testFailures+=1;
    } catch (std::string errorMsg){
        if (errorMsg == expectedResult)
            successMsg(testName, expectedResult);
        else{
            failMsg(testName, errorMsg, expectedResult);
            testFailures+=1;
        }
    }
    return testFailures;
}

int test_SD_oneData(ResponseTime& timer, std::string uri, int testFailures){
    int dataPoints = 1;
    dataCollection(timer, uri, dataPoints);
    std::string testName ="standardDeviRespTime() with 1 point of data";
    std::string expectedResult = "ERROR in standardDeviRespTime(): "
                            "Standard Deviation calculation needs more than 1 point.\n";

    try{
        double result = timer.standardDeviRespTime(uri);
        // Will only get here if it successful
        failMsg(testName, to_string(result), expectedResult);
        testFailures+=1;
    } catch (std::string errorMsg){
        // IMPORTANT
        if (errorMsg == expectedResult)
            successMsg(testName, expectedResult);
        else{
            failMsg(testName, errorMsg, expectedResult);
            testFailures+=1;
        }
    }
    return testFailures;
}

int test_SD_normalData(ResponseTime& timer, std::string uri, int testFailures){
    int dataPoints = 200;
    dataCollection(timer, uri, dataPoints);
    std::string testName ="standardDeviRespTime() with 200 points of data";
    std::string expectedResult = "A value of type double.";
    try{
        double result = timer.standardDeviRespTime(uri);
        // Will only get here if it scu 
        successMsg(testName, to_string(result));
    } catch (std::string errorMsg){
        failMsg(testName, errorMsg, expectedResult);
        testFailures+=1;
    }
    return testFailures;
}





int dataPointCounter(std::vector<std::tuple<std::string, int>> histogram){
    int points=0;
    for (std::tuple<std::string, int> i: histogram){
        points+=std::get<1>(i);
    }
    return points;
}


void test_normalizedHistogram(void){
    printf("\nTesting Normalized Histogram Functionality:\n");
    
    std::string uri ="uri1";

    ResponseTime timer;
    int testFailures = 0;
    int testCount =0;
    
    //      Test : No data points and find Normalized Histogram
    testCount+=1;
    testFailures = test_Histo_noData(timer, uri, testFailures);

    //      Test : less bins than m_minBins
    testCount+=1;
    testFailures = test_Histo_binMinCheck(timer, uri, testFailures);

    //      Test : more bins than m_maxBins
    testCount+=1;
    testFailures = test_Histo_binMaxCheck(timer, uri, testFailures);

    //      Test : less points than maxBins input
    testCount+=1;
    testFailures = test_Histo_dataToBinCheck(timer, uri, testFailures);

    //      Test : edges have data points 
    testCount+=1;
    int maxBins =5;
    int dataPoints = 200;
    for (int i =1; i<5; i++)
        testFailures = test_Histo_binEdgeCheck(uri, maxBins*i, 
                                                dataPoints*i, testFailures);

    //      Test : contains as many points as data points
    testCount+=1;
    dataPoints = 200;
    for (int i =1; i<5; i++)
        testFailures = test_Histo_totalPointsCheck(uri, maxBins*i, 
                                                    dataPoints*i, testFailures);

    testSummary("histogramResptime()", testFailures, testCount);
}


int test_Histo_noData(ResponseTime& timer, std::string uri, int testFailures){
    int maxBins =10;
    std::string testName ="histogramResptime() with no data";
    std::string expectedResult = "ERROR in histogramResptime(): "
                            "URI provided does not have any records.\n";
    try{
        auto result = timer.histogramResptime(uri, maxBins);
        // Will only get here if it fails 
        failMsg(testName, "Received a histogram object.", expectedResult);
        testFailures+=1;
    } catch (std::string errorMsg){
        if (errorMsg == expectedResult)
            successMsg(testName, errorMsg);
        else{
            failMsg(testName, errorMsg, expectedResult);
            testFailures+=1;
        }
    }
    return testFailures;
}

int test_Histo_binMinCheck(ResponseTime& timer, 
                                        std::string uri, int testFailures){
    int dataPoints = 1;
    dataCollection(timer, uri, dataPoints);

    int maxBins=timer.pullMinBins()-1;
    std::string testName ="histogramResptime() with bin value less than m_minBins value\n";
    std::string expectedResult = "ERROR in histogramResptime(): Cannot "
                        "not have less than "+ to_string(timer.pullMinBins())+" bins.\n";

    try{
        auto result = timer.histogramResptime(uri, maxBins);
        // Will only get here if it fails 
        failMsg(testName, "Received a histogram object.", expectedResult);
        testFailures+=1;
    } catch (std::string errorMsg){
        if (errorMsg == expectedResult)
            successMsg(testName, errorMsg);
        else{
            failMsg(testName, errorMsg, expectedResult);
            testFailures+=1;
        }
    }
    return testFailures;
}

int test_Histo_binMaxCheck(ResponseTime& timer, 
                                        std::string uri, int testFailures){
    int maxBins=timer.pullMaxBins()+1;
    std::string testName ="histogramResptime() with bin value more than m_maxBins value";
    std::string expectedResult = "ERROR in histogramResptime(): Cannot "
                        "not have more than "+ to_string(timer.pullMaxBins())+" bins.\n";

    try{
        auto result = timer.histogramResptime(uri, maxBins);
        // Will only get here if it fails 
        failMsg(testName, "Received a histogram object.", expectedResult);
        testFailures+=1;
    } catch (std::string errorMsg){
        if (errorMsg == expectedResult)
            successMsg(testName, errorMsg);
        else{
            failMsg(testName, errorMsg, expectedResult);
            testFailures+=1;
        }
    }
    return testFailures;
}

int test_Histo_dataToBinCheck(ResponseTime& timer, 
                                        std::string uri, int testFailures){
    int maxBins=timer.pullMaxBins()-1;
    std::string testName ="histogramResptime() with bin value more than number of data points stored";
    std::string expectedResult = "ERROR in histogramResptime(): More bins than points.\n";

    try{
        auto result = timer.histogramResptime(uri, maxBins);
        // Will only get here if it fails 
        failMsg(testName, "Received a histogram object.", expectedResult);
        testFailures+=1;
    } catch (std::string errorMsg){
        if (errorMsg == expectedResult)
            successMsg(testName, errorMsg);
        else{
            failMsg(testName, errorMsg, expectedResult);
            testFailures+=1;
        }
    }
    return testFailures;
}

int test_Histo_binEdgeCheck(std::string uri, int maxBins, 
                                            int dataPoints, int testFailures){
    ResponseTime timer;
    dataCollection(timer, uri, dataPoints);

    std::string testName ="histogramResptime() edge bins have data";
    std::string expectedResult = "Histogram with Data on edge bins. Bins: "
                                +to_string(maxBins) +
                                    " Points: "+to_string(dataPoints)+"\n";

    try{
        auto result = timer.histogramResptime(uri, maxBins);
        if (std::get<1>(result[0]) >0 && std::get<1>(result.back()) >0){
            successMsg(testName, "Histogram");
            // First element
            std::cout <<"        " << std::get<0>(result[0])<<' ';
            std::cout << std::get<1>(result[0])<<'\n';
            // Last element
            std::cout << "        "<<std::get<0>(result.back())<<' ';
            std::cout << std::get<1>(result.back())<<'\n';

        } else {
            failMsg(testName, "", expectedResult);
            testFailures+=1;
            // First element
            std::cout <<"        " << std::get<0>(result[0])<<' ';
            std::cout <<std::get<1>(result[0])<<'\n';
            // Last element
            std::cout << "        "<<std::get<0>(result.back())<<' ';
            std::cout << "        "<<std::get<1>(result.back())<<'\n';
        }

        printf("\n\n");
    } catch (std::string errorMsg){
        failMsg(testName, errorMsg, expectedResult);
        testFailures+=1;
    }
    return testFailures;
}

int test_Histo_totalPointsCheck(std::string uri, int maxBins, 
                                            int dataPoints, int testFailures){
    ResponseTime diffTimer;
    dataCollection(diffTimer, uri, dataPoints);
    std::string testName ="histogramResptime() points add up to data points added";
    std::string expectedResult = "Histogram with  Bins: "
                                +to_string(maxBins) +" Points: "+to_string(dataPoints)+"\n";

    try{
        auto result = diffTimer.histogramResptime(uri, maxBins);
        if (dataPointCounter(result) == dataPoints){
            successMsg(testName, expectedResult);
            for (std::tuple<std::string, int> i : result){
                std::cout <<"        "<< std::get<0>(i)<<' ';
                std::cout << std::get<1>(i) <<'\n';
            };
        } else {
            failMsg(testName, "", expectedResult);
            testFailures+=1;
            for (std::tuple<std::string, int> i : result){
                std::cout <<"        "<< std::get<0>(i)<<' ';
                std::cout << std::get<1>(i) <<'\n';
            };
        }

        printf("\n\n");
    } catch (std::string errorMsg){
        failMsg(testName, errorMsg, expectedResult);
        testFailures+=1;
    }
    return testFailures;
}



