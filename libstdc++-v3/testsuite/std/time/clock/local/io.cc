// { dg-options "-std=gnu++20" }
// { dg-do run { target c++20 } }
// { dg-timeout-factor 2 }

#include <chrono>
#include <format>
#include <testsuite_hooks.h>

void
test_ostream()
{
  using namespace std::chrono;

  auto lt = local_time<seconds>(1722198122s);

  std::ostringstream ss;
  ss << lt;
  auto s = ss.str();
  ss.str("");
  ss.clear();
  ss << sys_time<seconds>{lt.time_since_epoch()};
  auto s2 = ss.str();
  VERIFY( s == s2 );
}

void
test_format()
{
  using std::format;
  using namespace std::chrono;

  auto lt = local_seconds(1722198122s);

#if __cpp_exceptions
  auto args = std::make_format_args(lt);
  try
  {
    (void) std::vformat("{:%Z}", args);
    VERIFY(false);
  }
  catch (const std::format_error&)
  {
  }
  try
  {
    (void) std::vformat("{:%z}", args);
    VERIFY(false);
  }
  catch (const std::format_error&)
  {
  }
#endif

  auto s = format("{0:%F %T %a}", lt);
  VERIFY( s == "2024-07-28 20:22:02 Sun" );

  s = format("{}", lt);
  VERIFY( s == "2024-07-28 20:22:02" );

  // Test formatting for chrono::local_time_format and local-time-format-t too:
  auto ltf = local_time_format(lt);
  s = std::format("{:%F %T %a %b}", ltf);
  VERIFY( s == "2024-07-28 20:22:02 Sun Jul" );
#if __cpp_exceptions
  try
  {
    (void) std::format("{:%Z}", ltf);
    VERIFY(false);
  }
  catch (const std::format_error&)
  {
  }
  try
  {
    (void) std::format("{:%z}", ltf);
    VERIFY(false);
  }
  catch (const std::format_error&)
  {
  }
#endif
  std::string abbrev = "FOO";
  seconds off = -3600s;
  ltf = local_time_format(lt, &abbrev, &off);
  s = std::format("{}", ltf);
  VERIFY( s == "2024-07-28 20:22:02 FOO" );
  s = std::format("{:%Z %T %F %z %Ez}", ltf);
  __builtin_puts(s.c_str());
  VERIFY( s == "FOO 20:22:02 2024-07-28 -0100 -01:00" );

  s = std::format("{}", local_seconds{});
  VERIFY( s == "1970-01-01 00:00:00" );

  s = std::format("{}", local_days{}); // PR libstdc++/120293
  VERIFY( s == "1970-01-01" );
}

int main()
{
  test_ostream();
  test_format();
}
