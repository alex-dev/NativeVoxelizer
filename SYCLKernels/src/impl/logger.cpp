#include "logger.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace ext;

namespace {
  using namespace native_voxelizer::sycl_kernels::impl;

  std::string_view to_string(logger::level level) {
    switch (level) {
      case logger::level::fatal: return "FATAL";
      case logger::level::error: return "ERROR";
      case logger::level::debug: return "DEBUG";
      case logger::level::info: return "INFO";
    }
  }
}

namespace native_voxelizer::sycl_kernels::impl {
  ostream& logger::global_stream_wrapper::get_stream() {
    static ostream& stream = instance->stream;
    return stream;
  }

  logger::level logger::global_stream_wrapper::get_level() {
    static logger::level level = instance->level;
    return level;
  }

  ostringstream logger::stream_wrapper::create_stringstream(string_view const& prefix,
                                                            string_view const& guid,
                                                            string_view const& name,
                                                            string_view const& function) {
    ostringstream stream;
    stream << "\n[" << prefix << "] ";
#if __cplusplus > 202001L && USE_TIMESTAMP
    to_stream(stream << '[', "%Y-%m-%d %H:%M:%S", system_clock::now()) << ']';
#endif
    stream << " [" << name << " (" << guid << ") => " << function << "] ";
    return stream;
  }

  logger::stream_wrapper::optional_wrapper logger::make_wrapper(string_view const& function,
                                                                logger::level level) {
    return global_stream_wrapper::get_level() < level
               ? nullopt
               : make_optional<stream_wrapper>(global_stream_wrapper::get_stream(),
                                               to_string(level), guid, name, function);
  }
}