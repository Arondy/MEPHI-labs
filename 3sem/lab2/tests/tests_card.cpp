#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../cards/cards.h"

using namespace Cards;
using std::cin, std::cout;

TEST_CASE("Constructor"){
    SECTION("Default (random)"){
        Cards::Card c1;
        REQUIRE(c1.getRank() >= 2);
        REQUIRE(c1.getRank() <= 14);
        REQUIRE(std::find(mySuits.begin(), mySuits.end(), c1.getSuit()) != mySuits.end());
    }SECTION("Given values"){
        Cards::Card c2{5, "Hearts"};
        REQUIRE(c2.getRank() == 5);
        REQUIRE(c2.getSuit() == "Hearts");
    }SECTION("Exception"){
        REQUIRE_THROWS(Card(15, "Hearts"));
        REQUIRE_THROWS(Card(10, "he"));
        REQUIRE_THROWS(Card(18, "smth"));
    }
}

TEST_CASE("Setters"){
    SECTION("Success"){
        Cards::Card c3{6, "Diamonds"};
        c3.setRank(13);
        c3.setSuit("Clubs");
        REQUIRE(c3.getRank() == 13);
        REQUIRE(c3.getSuit() == "Clubs");
    }SECTION("Exception"){
        Cards::Card c4{6, "Diamonds"};
        REQUIRE_THROWS(c4.setRank(15));
        REQUIRE_THROWS(c4.setRank(0));
        REQUIRE_THROWS(c4.setSuit("clubs"));
        REQUIRE_THROWS(c4.setSuit("someNonsense"));
    }
}

TEST_CASE("Comparison"){
    SECTION("Lesser"){
        Cards::Card c5{5, "Diamonds"};
        Cards::Card c6{6, "Diamonds"};
        REQUIRE(compare(c5 <=> c6) == -1);
    }SECTION("Equal"){
        Cards::Card c7{4, "Clubs"};
        Cards::Card c8{4, "Clubs"};
        REQUIRE(compare(c7 <=> c8) == 0);
    }SECTION("Greater"){
        Cards::Card c9{11, "Hearts"};
        Cards::Card c10{3, "Hearts"};
        REQUIRE(compare(c9 <=> c10) == 1);
    }SECTION("Exception"){
        Cards::Card c9{11, "Diamonds"};
        Cards::Card c10{3, "Hearts"};
        REQUIRE(compare(c9 <=> c10) == 1);
    }
}

TEST_CASE("Format string"){
    SECTION("Format"){
        Cards::Card c1{3, "Hearts"};
        REQUIRE(c1.format() == "3♥");
        Cards::Card c2{10, "Clubs"};
        REQUIRE(c2.format() == "10♣");
        Cards::Card c3{13, "Pikes"};
        REQUIRE(c3.format() == "K♠");
    }
}

TEST_CASE("Input by stream"){
    SECTION("Success"){
        std::istringstream input("5 Hearts\n");
        Cards::Card c1;
        input >> c1;
        REQUIRE(c1.getRank() == 5);
        REQUIRE(c1.getSuit() == "Hearts");
    }SECTION("Exception"){
        std::istringstream input1("100 Hearts\n");
        Cards::Card c2;
        REQUIRE_THROWS(input1 >> c2);
        std::istringstream input2("10 clu\n");
        Cards::Card c3;
        REQUIRE_THROWS(input2 >> c3);
        std::istringstream input3("15 smth\n");
        Cards::Card c4;
        REQUIRE_THROWS(input3 >> c4);
    }
}

TEST_CASE("Output by stream"){
    SECTION("Success"){
        std::ostringstream output;
        Cards::Card c1{8, "Clubs"};
        output << c1;
        REQUIRE(output.str() == "8♣ ");
    }
}

TEST_CASE("Comparison by suit"){
    SECTION("Less"){
        Card c1{10, "Clubs"}, c2{10, "Hearts"};
        REQUIRE(is_less(c1, c2) == true);
        Card c3{3, "Clubs"};
        REQUIRE(is_less(c3, c1) == false);
        Card c4{12, "Diamonds"};
        REQUIRE(is_less(c2, c4) == false);
    }SECTION("Greater"){
        Card c1{10, "Clubs"}, c2{10, "Hearts"};
        REQUIRE(is_greater(c1, c2) == false);
        Card c3{3, "Clubs"};
        REQUIRE(is_greater(c3, c1) == false);
        Card c4{12, "Diamonds"};
        REQUIRE(is_greater(c2, c4) == true);
    }
}