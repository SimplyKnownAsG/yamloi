
#include <string>
#include <exception>

namespace yamloi {

    class Exception : public std::exception {

    private:
        const std::string _msg;
 
    public:
        Exception(const std::string &msg) : _msg(msg) {
        };

        std::string what() {
            return this->_msg;
        };

    };

    class TypeError : public Exception {

    public:
        TypeError(const std::string &msg) : Exception(msg) {
        };

    };

}

