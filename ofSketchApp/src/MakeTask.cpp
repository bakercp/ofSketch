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


#include "MakeTask.h"
#include "Poco/TaskNotification.h"


namespace of {
namespace Sketch {


MakeTask::Settings::Settings():
    ofRoot(ofToDataPath("openFrameworks")),
    numProcessors(1),
    isSilent(false),
    useDistccServer(false),
    cxx(""),
    cc("")
{
}


MakeTask::MakeTask(const Settings& settings,
                   const Project& project,
                   const std::string& target):
    BaseProcessTask(project.getPath(), "make"),
    _settings(settings),
    _project(project),
    _target(target)
{
    _args.push_back("--directory=" + ofToDataPath(_project.getPath()));

    if(_settings.numProcessors > 1)
    {
        _args.push_back("-j" + ofToString(_settings.numProcessors));
    }

    if(_settings.isSilent)
    {
        _args.push_back("-s");
    }

    _args.push_back(_target);
//    _args.push_back("OF_ROOT=" + _settings.ofRoot);

    cout << "--------" << endl;
    cout << ofToString(_args) << endl;
    cout << "--------" << endl;
}


MakeTask::~MakeTask()
{
}


void MakeTask::processLine(const std::string& line)
{
    postNotification(new Poco::TaskCustomNotification<std::string>(this, line));
}


} } // namespace of::Sketch
