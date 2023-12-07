#include "base/logging.hpp"

#include "base/assert.hpp"
#include "base/thread.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <mutex>
#include <sstream>

namespace base
{

using namespace std::literals;
auto constexpr kDebug    = "DEBUG"sv;
auto constexpr kInfo     = "INFO"sv;
auto constexpr kWarning  = "WARN"sv;
auto constexpr kError    = "ERROR"sv;
auto constexpr kCritical = "CRIT"sv;

std::mutex g_logMutex;

std::string ToString(LogLevel level)
{
  auto const & names = GetLogLevelNames();
  CHECK_LESS(level, names.size(), ());
  return std::string(names[level]);
}

bool FromString(std::string const & s, LogLevel & level)
{
  auto const & names = GetLogLevelNames();
  auto it = find(names.begin(), names.end(), s);
  if (it == names.end())
    return false;
  level = static_cast<LogLevel>(std::distance(names.begin(), it));
  return true;
}

static std::array<std::string_view, NUM_LOG_LEVELS> const kNames = {
    {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"}};

std::array<std::string_view, NUM_LOG_LEVELS> const constexpr & GetLogLevelNames()
{
  // If you're going to modify the behavior of the function, please,
  // check validity of LogHelper ctor.
  return kNames;
}

// static
LogHelper & LogHelper::Instance()
{
  static LogHelper instance;
  return instance;
}

LogHelper::LogHelper() : m_threadsCount(0)
{
  // This code highly depends on the fact that GetLogLevelNames()
  // always returns the same constant array of strings.

  // m_names = GetLogLevelNames();
  // for (size_t i = 0; i < m_lens.size(); ++i)
  //   m_lens[i] = std::strlen(m_names[i]);
}

int LogHelper::GetThreadID()
{
  int & id = m_threadID[threads::GetCurrentThreadID()];
  if (id == 0)
    id = ++m_threadsCount;
  return id;
}

void LogHelper::WriteProlog(std::ostream & s, LogLevel level)
{
  auto const strLevel = ToString(level);
  s << strLevel << ' ' << GetThreadID() << ' ';

  double const sec = m_timer.ElapsedSeconds();
  s << " " << std::setfill(' ') << std::setw(static_cast<int>(16 - m_lens[level])) << sec << " ";
}

void LogMessageDefault(LogLevel level, SrcPoint const & srcPoint, std::string const & msg)
{
  std::lock_guard lock(g_logMutex);

  auto & logger = LogHelper::Instance();

  std::ostringstream out;
  logger.WriteProlog(out, level);
  out << DebugPrint(srcPoint) << msg << std::endl;

  out << DebugPrint(srcPoint) << msg << std::endl;
  std::cerr << out.str();

  CHECK_LESS(level, g_LogAbortLevel, ("Abort. Log level is too serious", level));
}

void LogMessageTests(LogLevel level, SrcPoint const &, std::string const & msg)
{
  std::lock_guard lock(g_logMutex);

  std::ostringstream out;
  out << msg << std::endl;
  std::cerr << out.str();

  CHECK_LESS(level, g_LogAbortLevel, ("Abort. Log level is too serious", level));
}

LogMessageFn LogMessage = &LogMessageDefault;

LogMessageFn SetLogMessageFn(LogMessageFn fn)
{
  std::swap(LogMessage, fn);
  return fn;
}

LogLevel GetDefaultLogLevel()
{
#if defined(DEBUG)
  return LDEBUG;
#else
  return LINFO;
#endif  // defined(DEBUG)
}

LogLevel GetDefaultLogAbortLevel()
{
#if defined(DEBUG)
  return LERROR;
#else
  return LCRITICAL;
#endif  // defined(DEBUG)
}

AtomicLogLevel g_LogLevel = {GetDefaultLogLevel()};
AtomicLogLevel g_LogAbortLevel = {GetDefaultLogAbortLevel()};
}  // namespace base
