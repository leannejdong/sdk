#pragma once
#include "details/Dispatcher.hpp"

using namespace Bitwyre::Details;

namespace Bitwyre::Rest::Public {

  struct SupportedLanguages {

    [[nodiscard]] static auto uri() noexcept -> std::string {
      return "/public/languages";
    }

    template<typename Dispatcher = Dispatcher>
    [[nodiscard]] static auto get() noexcept -> SupportedLanguagesResponse {
      auto rawResponse = Dispatcher()(uri(), CommonPublicRequest{});
      return processResponse(std::move(rawResponse));
    }

    static auto processResponse(json&& rawResponse)
        -> SupportedLanguagesResponse {
      SupportedLanguagesResponse supportedLanguagesResponse;
      supportedLanguagesResponse.statusCode_ =
          rawResponse["statusCode"].get<int>();
      supportedLanguagesResponse.errors_ = rawResponse["error"].get<ErrorsT>();
      return supportedLanguagesResponse;
    }
  };
} // namespace Bitwyre::Rest::Public