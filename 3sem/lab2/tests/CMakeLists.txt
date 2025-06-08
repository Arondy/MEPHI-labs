find_package(Catch2 2 REQUIRED)

add_executable(tests_card tests_card.cpp)
add_executable(tests_cards_pile tests_cards_pile.cpp)

target_link_libraries(tests_card cards Catch2::Catch2)
target_link_libraries(tests_cards_pile cards_pile Catch2::Catch2)