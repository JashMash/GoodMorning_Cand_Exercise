#include <string>

/**
 * Resource request processing class
 *
 * Instantiations of this class do state based processing of resource requests.
 * To use, instantiate an object and call process() on a URI to get the response
 * data. Children of this class can augment functionality by overriding start()
 * and finish().
 */
class Request
{
private:
    /**
     * Simulate a delay that is specific to the URI in question
     *
     * Let's pretend that this function doesn't actually exist in this class
     *
     * @param [in] uri The URI of the request endpoint
     */
    static void simulateLatency(const std::string& uri);

protected:
    /**
     * Start processing the request in the child class
     *
     * @param [in] uri The URI of the request endpoint
     */
    virtual void start(const std::string& uri);

    /** Finish processing the request in the child class */
    virtual void finish();

public:
    /**
     * Process the request
     *
     * @param [in] uri The URI of the request endpoint
     * @return The response data
     */
    std::string process(const std::string& uri);
};
