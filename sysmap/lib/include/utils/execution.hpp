#ifndef __ADAFS_UTILS_EXECUTION_HPP__
#define __ADAFS_UTILS_EXECUTION_HPP__

#include <vector>
#include <map>
#include <string>
#include <functional>

#include <unistd.h>

namespace adafs { namespace utils { namespace exec {


struct result {

    result(bool s, int ec, std::string out, std::string err, size_t p) :
        success(s), exit_code(ec), output(out), error(err), pid(p) {}

    result() {}

    bool success = true;

    int exit_code = 0;

    std::string output;

    std::string error;

    size_t pid = 0;
};


struct descriptor {

    descriptor(int fd_nr, std::string fd_name, std::function<bool(std::string&)> const& cb) :
        name(fd_name), callback(cb), read(true), fd_(fd_nr) {}

    descriptor(int fd_nr, std::string fd_name, std::string buf) :
         name(fd_name), buffer(buf), read(false), fd_(fd_nr) {}

    ~descriptor() {  if (fd_ < 0) close(fd_); }

    void release() { close(fd_); fd_ = -1; }

    int fd() const { return fd_; }

    const std::string name;
    std::string buffer;
    std::function<bool(std::string&)> callback;
    bool read;

private:
    int fd_;
};


std::string which(const std::string& prog_name);

result execute(const std::string& program,
               const std::vector<std::string>& arguments,
               std::function<bool(std::string&)> const &stdout_callback,
               std::function<bool(std::string&)> const &stderr_callback,
               const std::map<std::string, std::string>& environment,
               const char* input = nullptr,
               unsigned int timeout = 0);


}}} /* closing namespace adafs::utils::exec */

namespace environment {

std::vector<std::string> path();

} /* closing namespace environment */

#endif /* __ADAFS_UTILS_EXECUTION_HPP__ */