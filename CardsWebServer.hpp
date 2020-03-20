#pragma once

#include <memory>

namespace cardsws {

class CardsWebServer
{
public:
    CardsWebServer();
    ~CardsWebServer();

    void launch(const std::string& root, int port=3001);

private:

    struct Impl;
    using ImplPtr = std::unique_ptr<Impl>;
    ImplPtr mImpl;
};

} // namespace cardsws
