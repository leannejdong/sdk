#pragma once
#include "../../details/Dispatcher.hpp"

using namespace Bitwyre::Details;
using AsyncFiatAssetResponse = std::future<FiatAssetResponse>;

namespace Bitwyre::Rest::Public {

  struct FiatAsset {

    using Callback = std::function<void(const FiatAssetResponse&)>;

    [[nodiscard]] static auto uri() noexcept -> std::string {
      return "/public/assets/fiat";
    }

    template<typename Dispatcher = Dispatcher>
    [[nodiscard]] static auto getAsync(Callback cb) noexcept -> void {
      static_assert( std::is_nothrow_invocable_v<decltype(cb), FiatAssetResponse> );
      auto result = getAsync();
      return cb(result.get());
    }

    template<typename Dispatcher = Dispatcher>
    [[nodiscard]] static auto getAsync() noexcept ->  AsyncFiatAssetResponse {
      return std::async(std::launch::async, [](){return get<Dispatcher>();});
    }

    template<typename Dispatcher = Dispatcher>
    [[nodiscard]] static auto get() noexcept -> FiatAssetResponse {
      auto rawResponse = Dispatcher()(uri(), CommonPublicRequest{});
      return processResponse(std::move(rawResponse));
    }

    static auto processResponse(json&& rawResponse) -> FiatAssetResponse {
      FiatAssetResponse assetResponse;
      assetResponse.statusCode_ = rawResponse["statusCode"].get<int>();
      assetResponse.errors_ = rawResponse["error"].get<ErrorsT>();

      for (const auto& asset : rawResponse["result"]) {
        AssetElement parsedAsset;
        parsedAsset.name = asset["name"].get<std::string>();
        assetResponse.assets.push_back(std::move(parsedAsset));
      }
      return assetResponse;
    }
  };
} // namespace Bitwyre::Rest::Public
