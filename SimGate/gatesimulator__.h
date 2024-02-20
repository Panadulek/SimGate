#ifndef GATESIMULATOR_H
#define GATESIMULATOR_H

#include <QObject>
#include <QLibrary>
#include <QFunctionPointer>
#include <map>
#include <utility>
#include <functional>
#include <tuple>
#include <type_traits>
#include <Windows.h>
#include <string>
#include <QDebug>
#include <QDir>
#include "D:\Projects\Common\Common.h"
std::string getLastErrorAsString();
/*class LibFunNotFound
{
    std::string m_funName;
public:

    LibFunNotFound(const std::string name) : m_funName(name){}
};

class GateSimulator
{
    GateSimulator();
    std::map<std::string, QFunctionPointer> m_functionMap;
    std::string m_libname;
    typedef void(*add)(int, int);
public:
    static GateSimulator& Instance();
    void findFunction(std::string name)
    {
        auto isExsit = m_functionMap.find(name);
        if(isExsit != m_functionMap.end())
            return;
        QFunctionPointer cb = QLibrary::resolve(m_libname.c_str(), name.c_str());
        if(!cb)
            throw LibFunNotFound(name);
        m_functionMap.insert(std::make_pair(name, cb));
    }
    template<typename RET, typename FUNCTION_TYPE, typename... ARGS>
    RET call(const char* name, ARGS... args)
    {
        std::map<std::string, QFunctionPointer>::iterator cb = m_functionMap.find(name);
        if(cb == m_functionMap.end())
        {
            qDebug() << QDir::currentPath();
            QLibrary lib(m_libname.c_str());
            bool isLoaded = lib.load();
            if(!isLoaded)
                throw LibFunNotFound(name);
            QFunctionPointer callback = lib.resolve(name);
            if(!callback)
                throw LibFunNotFound(name);
            bool inserted = false;
            std::tie(cb, inserted) = m_functionMap.insert(std::make_pair(name, callback));
            if(!inserted)
                throw LibFunNotFound(name);
        }
        FUNCTION_TYPE fun = (FUNCTION_TYPE)cb->second;
        if(std::is_same<RET, void>::value)
            fun(args...);
        else

            return fun(args...);
    }

    QFunctionPointer getFunction(const std::string& name)
    {
       auto iterator = m_functionMap.find(name);
       if(iterator == m_functionMap.end())
       {
           findFunction(name);
           iterator = m_functionMap.find(name);
           if(iterator == m_functionMap.end())
                   return nullptr;
       }
       return iterator->second;

    }
};*/

#endif // GATESIMULATOR_H
