// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "BaseProcessTask.h"
#include "Poco/Buffer.h"
#include "Poco/PipeStream.h"
#include "Poco/Process.h"
#include <iostream>


namespace of {
namespace Sketch {        
        

BaseProcessTask::BaseProcessTask(const std::string& name,
                                 const std::string& command,
                                 const std::vector<std::string>& args,
                                 std::size_t bufferSize):
    Poco::Task(name),
    _command(command),
    _args(args),
    _bufferSize(bufferSize)
{
}


BaseProcessTask::~BaseProcessTask()
{
}


void BaseProcessTask::runTask()
{

    Poco::Pipe outAndErrPipe;

    Poco::ProcessHandle ph = Poco::Process::launch(_command,
                                                   _args,
                                                   0,
                                                   &outAndErrPipe,
                                                   &outAndErrPipe);

    Poco::PipeInputStream istr(outAndErrPipe);

    Poco::Buffer<char> buffer(_bufferSize);

    while(istr.good() && !istr.fail())
    {
        if(isCancelled())
        {
            Poco::Process::kill(ph);
        }

        istr.getline(buffer.begin(), buffer.size());

        if (buffer.begin())
        {
            std::string str(buffer.begin());

            if (!str.empty())
            {
                // Progress callbacks and custom data events are the
                // responsibility of the subclass.
                processLine(str);
            }
        }
    }

    int exitCode = ph.wait();

}


} } // namespace of::Sketch