#pragma once

#include <memory>

namespace cardsws {

struct Card
{
    using Int = std::int8_t;
    Card(Int r, Int s) : rank{r}, suit{s} {}

    Card() = delete;
    ~Card() = default;

    Card(const Card&) = default;
    Card& operator=(const Card&) = default;
    Card& operator=(Card&&) = delete;

    static constexpr Int kRanks{13};

    static Card make(Int card)
    {
        Int rank = card % kRanks;
        Int suit = card / kRanks;
        return Card{rank, suit};
    }

    const Int rank;
    const Int suit;
};



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
