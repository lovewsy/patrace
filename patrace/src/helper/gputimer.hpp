#ifndef GPUTIMER_HPP
#define GPUTIMER_HPP

#include <common/gl_extension_supported.hpp>

class GpuTimer
{
public:
    GpuTimer();
    ~GpuTimer();
    void CreateQueries(int n=1);
    void AddQueries(int pid);
    void DeleteQueries();
    void Begin();
    void Begin(int pid);
    void End();
    std::vector<long> GetTimes();
    std::vector<int> GetPids();
    void Reset();
    bool enable;
    unsigned GetCount(){return count;}
private:
    unsigned count;
    std::vector<GLuint> queries;
    std::vector<long> times;
    std::vector<int> pids;
};

#endif