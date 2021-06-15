#include "gputimer.hpp"

GpuTimer::GpuTimer(){
    count = 0;
    enable = false;
}

void GpuTimer::AddQueries(int pid)
{
    enable = true;
    queries.resize(queries.size() + 1);
    _glGenQueries(1, &queries[queries.size()]);
    pids.push_back(pid);
}

void GpuTimer::CreateQueries(int n)
{
    enable = true;
    queries.resize(n);
    pids.resize(n);
    _glGenQueries(queries.size(), &queries[0]);
    // for(int i = 0; i < queries.size(); i++)
    // {
    //       if(!_glIsQuery(queries[i]))
    //         DBG_LOG("GpuTimer::Begin query (%d)%u is not a query", i, queries[i]);
    //     DBG_LOG("Query index: %u", queries[i]);
    // }
    
}


void GpuTimer::DeleteQueries()
{
    queries.clear();
}

std::vector<int> GpuTimer::GetPids()
{
    return pids;
}

void GpuTimer::Begin(int pid)
{
    if(!enable) return;

    unsigned index = count % queries.size();
    if(count >= queries.size())
    {
        DBG_LOG("out of queries\n");

        GLuint available = GL_FALSE;

        _glGetQueryObjectuiv(queries[index], GL_QUERY_RESULT_AVAILABLE, &available);
        if(!_glIsQuery(queries[index]))
            DBG_LOG("GpuTimer::Begin query %u is not a query", index);
        if (!available)
            DBG_LOG("GpuTimer::Begin:: query %u is not available now\n", index);
            
        while(!available)
            _glGetQueryObjectuiv(queries[index], GL_QUERY_RESULT_AVAILABLE, &available);
        
        GLuint64 time = 0;
        _glGetQueryObjectui64vEXT(queries[index], GL_QUERY_RESULT, &time);
        DBG_LOG("GpuTimer::Begin:: time of draw call %u: %lu\n", (unsigned)times.size(), (long)time);
        times.push_back(time);
    }

    pids[index] = pid;
    _glBeginQuery(GL_TIME_ELAPSED_EXT, queries[index]);
}

void GpuTimer::Begin()
{
    if(!enable) return;

    unsigned index = count % queries.size();
    if(count >= queries.size())
    {
        DBG_LOG("out of queries\n");

        GLuint available = GL_FALSE;

        _glGetQueryObjectuiv(queries[index], GL_QUERY_RESULT_AVAILABLE, &available);
        if(!_glIsQuery(queries[index]))
            DBG_LOG("GpuTimer::Begin query %u is not a query", index);
        
        if (!available)
            DBG_LOG("GpuTimer::Begin:: query %u is not available now\n", index);
            
        while(!available)
            _glGetQueryObjectuiv(queries[index], GL_QUERY_RESULT_AVAILABLE, &available);
        
        GLuint64 time = 0;
        _glGetQueryObjectui64vEXT(queries[index], GL_QUERY_RESULT, &time);
        DBG_LOG("GpuTimer::Begin:: time of draw call %u: %lu\n", (unsigned)times.size(), (long)time);
        times.push_back(time);
    }

    _glBeginQuery(GL_TIME_ELAPSED_EXT, queries[index]);
}

void GpuTimer::End()
{
    if(!enable) return;

    _glEndQuery(GL_TIME_ELAPSED_EXT);
    count++;
    // _glFinish();
    // _glFinish();
    // DBG_LOG("============= END OF GPU_TIMER END count : %d===============\n",count);
}

std::vector<long> GpuTimer::GetTimes()
{
    DBG_LOG("query size : %d\n",count);
    if(times.size() != count)
        for (unsigned i = times.size(); i < count; i++)
        {
            unsigned index = i % queries.size();
            GLuint available = GL_FALSE;

            _glGetQueryObjectuiv(queries[index], GL_QUERY_RESULT_AVAILABLE, &available);
            if(!_glIsQuery(queries[index]))
                DBG_LOG("GpuTimer::GetTimes query %u is not a query", index);
            if (!available)
                DBG_LOG("GpuTimer::GetTimes:: time query %u is not available now\n", index);

            while(!available)
                _glGetQueryObjectuiv(queries[index], GL_QUERY_RESULT_AVAILABLE, &available);
            
            GLuint64 time = 0;
            _glGetQueryObjectui64vEXT(queries[index], GL_QUERY_RESULT, &time);
            // DBG_LOG("time of draw call %u: %lu\n", (unsigned)times.size(), (long)time);
            times.push_back(time);
        }

    return times;
}

void GpuTimer::Reset()
{
    times.clear();
    // pids.clear();
    count = 0;
}

GpuTimer::~GpuTimer()
{
    if(!queries.empty())
        _glDeleteQueries(queries.size(), &queries[0]);
}