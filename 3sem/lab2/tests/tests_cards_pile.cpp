#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../cards/cards_pile.h"

using namespace CardsPiles;
using std::cin, std::cout;

TEST_CASE("Constructor"){
    SECTION("Full"){
        CardsPile cp1;
        REQUIRE(cp1.getSize() == 52);
    }SECTION("Initialized by number of cards"){
        SECTION("Empty"){
            CardsPile cp2{0};
            REQUIRE(cp2.getSize() == 0);
        }SECTION("Some number"){
            CardsPile cp3{10};
            REQUIRE(cp3.getSize() == 10);
        }SECTION("Full"){
            CardsPile cp4{52}, full_pile;
            REQUIRE(cp4.getSize() == full_pile.getSize());
        }
    }SECTION("Copy"){
        CardsPile cp1;
        CardsPile cp2{cp1};
        REQUIRE(cp2.getSize() == cp1.getSize());
    }SECTION("Move"){
        CardsPile cp1;
        int len = cp1.getSize();
        CardsPile cp2 = std::move(cp1);
        REQUIRE(cp2.getSize() == len);
    }SECTION("Exceptions"){
        REQUIRE_THROWS(CardsPile{53});
        REQUIRE_THROWS(CardsPile{-10});
    }
}

TEST_CASE("Overload"){
    SECTION(">> - Success"){
        CardsPile cp1{0}, cp2{20};
        Card tmpCard;
        tmpCard = cp2[19];
        cp2 >> cp1;
        REQUIRE(cp1[0] == tmpCard);
    }SECTION(">> - Exceptions"){
        CardsPile cp1{0}, cp2{20};
        REQUIRE_THROWS(cp1 >> cp2);
        CardsPile cp3{20}, cp4;
        REQUIRE_THROWS(cp3 >> cp4);
        CardsPile cp5, cp6;
        cp5.remove(51);
        REQUIRE_THROWS(cp5 >> cp6);
        CardsPile cp7, cp8{0};
        cp7.remove(0);
        cp8.add(Card{6, "Hearts"});
        REQUIRE_THROWS(cp8 >> cp7);
    }SECTION("[] - Success"){
        CardsPile cp1;
        REQUIRE(cp1.getPile()[0] == cp1[0]);
        REQUIRE(cp1.getPile()[10] == cp1[10]);
        REQUIRE(cp1.getPile()[51] == cp1[51]);
    }SECTION("[] - Exceptions"){
        CardsPile cp1;
        REQUIRE_THROWS(cp1[-2]);
        REQUIRE_THROWS(cp1[52]);
        CardsPile cp2{20};
        REQUIRE_THROWS(cp2[25]);
    }SECTION("+ - Success"){
        CardsPile cp1{0}, cp2{20};
        CardsPile cp3 = cp1 + cp2;
        REQUIRE(cp3.getSize() == 20);
        CardsPile cp4 = cp2 + cp1;
        REQUIRE(cp4.getSize() == cp3.getSize());
        REQUIRE(cp3[0] == cp4[0]);
        REQUIRE(cp3[10] == cp4[10]);
    }SECTION("= - Copy"){
        CardsPile cp1;
        CardsPile cp2{10};
        cp2 = cp1;
        REQUIRE(cp1.getSize() == 52);
        REQUIRE(cp2.getSize() == 52);
        REQUIRE(&cp1 != &cp2);
    }SECTION("= - Move"){
        CardsPile cp1;
        CardsPile cp2;
        cp2 = CardsPile{20};
        REQUIRE(cp2.getSize() == 20);
    }
}

TEST_CASE("Methods"){
    SECTION("Empty method"){
        CardsPile cp1, cp2{0}, cp3{20};
        REQUIRE(!cp1.empty());
        REQUIRE(cp2.empty());
        REQUIRE(!cp3.empty());
    }SECTION("Add - Success"){
        CardsPile cp1{0};
        cp1.add(Card{8, "Hearts"});
        REQUIRE(cp1[0] == Card{8, "Hearts"});
        CardsPile cp2;
        cp2.remove(0); // 2 Hearts
        cp2.add(Card{2, "Hearts"});
        REQUIRE(cp2[51] == Card{2, "Hearts"});
    }SECTION("Add - Exceptions"){
        CardsPile cp1;
        REQUIRE_THROWS(cp1.add(Card{2, "Hearts"}));
        cp1.remove(0);
        REQUIRE_THROWS(cp1.add(Card{3, "Diamonds"}));
    }SECTION("Remove - Success"){
        CardsPile cp1;
        cp1.remove(0);
        REQUIRE_NOTHROW(cp1.add(Card{2, "Hearts"}));
        cp1.remove(51);
        cp1.remove(30);
    }SECTION("Remove - Exceptions"){
        CardsPile cp1{20};
        REQUIRE_THROWS(cp1.remove(-3));
        REQUIRE_THROWS(cp1.remove(30));
    }SECTION("Shuffle - Success"){
        CardsPile cp1;
        REQUIRE_NOTHROW(cp1.myShuffle());
    }SECTION("Sort - Success"){
        CardsPile cp1;
        cp1.sort(0, 1);
        REQUIRE(cp1[0].getRank() == 14);
        REQUIRE(cp1[0].getRank() == cp1[1].getRank());
        REQUIRE(cp1[1].getRank() == cp1[2].getRank());
        REQUIRE(cp1[2].getRank() == cp1[3].getRank());
        CardsPile cp2{51};
        cp2.sort(1, 1);
        REQUIRE(cp2[0].getRank() == 2);
        REQUIRE(cp2[0].getRank() == cp2[1].getRank());
        REQUIRE(cp2[1].getRank() == cp2[2].getRank());
        CardsPile cp3;
        cp3.sort(0, 0);
        REQUIRE(cp3[0].getSuit() == "Pikes");
        REQUIRE(cp3[0].getSuit() == cp3[5].getSuit());
        REQUIRE(cp3[5].getSuit() == cp3[12].getSuit());
        CardsPile cp4;
        cp4.sort(1, 0);
        REQUIRE(cp4[0].getSuit() == "Clubs");
        REQUIRE(cp4[39].getSuit() == "Pikes");
        REQUIRE(cp4[39].getSuit() == cp4[45].getSuit());
        REQUIRE(cp4[45].getSuit() == cp4[51].getSuit());
        CardsPile cp5{0};
        REQUIRE_NOTHROW(cp5.sort(0, 0));
        CardsPile cp6{1};
        REQUIRE_NOTHROW(cp6.sort(1, 1));
    }SECTION("Get sub-pile of one suit"){
        CardsPile cp1;
        CardsPile cp2 = cp1.getSubPile("Hearts");
        bool all_are_needed_suit = true;
        for (int i = 0; i < cp2.getSize(); i++){
            if (cp2[i].getSuit() != "Hearts"){
                all_are_needed_suit = false;
            }
        }
        REQUIRE(all_are_needed_suit);
    }
}

TEST_CASE("Input by stream"){
    SECTION("Success"){
        std::istringstream input1("5 Hearts 6 Clubs\n");
        std::istringstream input2("13 Hearts 6 Diamonds 12 Pikes\n");
        CardsPile cp1{0};
        cp1.input(input1, 2);
        REQUIRE(cp1[0] == Card{5, "Hearts"});
        REQUIRE(cp1[1] == Card{6, "Clubs"});
        CardsPile cp2;
        REQUIRE_NOTHROW(cp2.input(input2, 3));
    }SECTION("Throw caught in the function"){
        std::istringstream input1("5 Hearts\n");
        CardsPile cp1;
        cp1.remove(0);
        cp1.input(input1, 1);
    }
}

TEST_CASE("Output by stream"){
    SECTION("Success"){
        std::ostringstream output1;
        CardsPile cp1{0};
        cp1.add(Card{2, "Hearts"});
        cp1.add(Card{12, "Clubs"});
        cp1.print(output1);
        REQUIRE(output1.str() == "2♥ Q♣ \n");
    }SECTION("Empty"){
        std::ostringstream output1;
        CardsPile cp1{0};
        cp1.print(output1);
        REQUIRE(output1.str() == "Pile is empty\n");
    }
}