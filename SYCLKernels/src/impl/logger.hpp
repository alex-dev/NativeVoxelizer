#include <fstream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <guid.hpp>

#if __has_include(<syncstream>)
#include <syncstream>
#endif

namespace native_voxelizer::sycl_kernels::impl {
  class logger {
  public:
    enum class level { fatal, error, debug, info };

#pragma region Static Flags
  private:
    static constexpr std::ios::openmode openmode = std::ios::trunc | std::ios::app | std::ios::out;
    static constexpr std::ios::iostate exceptions_bits = std::ios::eofbit | std::ios::failbit
                                                         | std::ios::badbit;
    static constexpr level default_level = NDEBUG ? level::error : level::debug;
#pragma endregion

#pragma region Global Stream Wrapper
  private:
    class global_stream_wrapper {
#pragma region Global Singleton
    private:
      static std::unique_ptr<global_stream_wrapper> instance = nullptr;

    public:
      static void init(std::string_view const& log_file, level level) {
        instance = std::make_unique<global_stream_wrapper>(log_file);
      }

      static std::ostream& get_stream();
      static level get_level();
#pragma endregion

    public:
      global_stream_wrapper(std::string_view const& log_file, level level)
          : stream{log_file.data(), openmode}, level{level} {}

    private:
      level level;
#if __has_include(<syncstream>)
      std::ofstream inner_stream;
      std::osyncstream stream = {inner_stream};
#else
      std::ofstream stream;
#endif
    };
#pragma endregion

#pragma region Stream Wrapper
  public:
    class stream_wrapper final {
    public:
      using optional_wrapper = std::optional<stream_wrapper>;

    private:
      static std::ostringstream
          create_stringstream(std::string_view const& prefix, std::string_view const& guid,
                              std::string_view const& name, std::string_view const& function);

#pragma region Memory Management
    public:
      ~stream_wrapper() noexcept { logger << stream.str(); }

      stream_wrapper(std::ostream& logger, std::string_view const& prefix,
                     std::string_view const& guid, std::string_view const& name,
                     std::string_view const& function)
          : logger{logger}, stream{create_stringstream(prefix, guid, name, function)} {
        stream.exceptions(exceptions_bits);
      }

      stream_wrapper(stream_wrapper const&) = delete;
      stream_wrapper(stream_wrapper&&) = default;
#pragma endregion

    public:
      template<typename T>
      friend optional_wrapper& operator<<(optional_wrapper& stream, T const& arg) {
        using insertion_return
            = std::remove_reference_t<decltype(std::declval<stream_wrapper>().stream << arg)>;
        static_assert(std::is_base_of_v<std::ostream, insertion_return>,
                      "Argument is not forwardable to std::ostream& << T const&");
        if (stream)
          stream->stream << arg;

        return stream;
      }

      template<typename T>
      friend optional_wrapper& operator<<(optional_wrapper&& stream, T const& arg) {
        return stream << arg;
      }

    private:
      std::ostream& logger;
      std::ostringstream stream;
    };

  private:
    stream_wrapper::optional_wrapper make_wrapper(std::string_view const& function, level level);
#pragma endregion

  public:
    logger(ext::guid const& guid, std::string name) : guid{guid}, name{std::move(name)} {}

#pragma region Outputs Manipulators
  public:
    stream_wrapper::optional_wrapper fatal(std::string_view const& function) {
      return make_wrapper(function, level::fatal);
    }

    stream_wrapper::optional_wrapper error(std::string_view const& function) {
      return make_wrapper(function, level::error);
    }

    stream_wrapper::optional_wrapper debug(std::string_view const& function) {
      return make_wrapper(function, level::debug);
    }

    stream_wrapper::optional_wrapper info(std::string_view const& function) {
      return make_wrapper(function, level::info);
    }
#pragma endregion

  private:
    std::string guid;
    std::string name;
  };
}
