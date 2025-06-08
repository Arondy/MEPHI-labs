#include <iostream>
#include <string>
#include "menu.h"

namespace Menu {
    void create_full_pile_menu(CardsPile &pile){
        CardsPile cp1;
        pile = std::move(cp1);
        pile.print(cout);
    }

    void create_num_pile_menu(CardsPile &pile){
        cout << "Input your number of cards in the pile: ";
        int num = getNum<int>(0, 52);
        pile = CardsPile{num};
        pile.print(cout);
    }

    void create_pile_ny_input_menu(CardsPile &pile){
        CardsPile cp1{0};
        cout << "Input what number of cards you will input: ";
        int num = getNum<int>(0, 52);
        cp1.input(cin, num);
        pile = std::move(cp1);
        pile.print(cout);
    }

    std::vector<InitMenuFunc> init_menu_func = {
            create_full_pile_menu,
            create_num_pile_menu,
            create_pile_ny_input_menu
    };

    void initialization_menu(CardsPile &pile){
        cout << "1. Create full pile\n"
                "2. Create pile with your number of random cards\n"
                "3. Create pile by input of cards" << endl;
        int choice = getNum<int>(1, 3);
        init_menu_func[choice - 1](pile);
    }

    void get_size_menu(CardsPile &pile){
        cout << pile.getSize() << endl;
    }

    void get_cards_by_index_menu(CardsPile &pile){
        cout << "Input index of the card you want to get: ";
        int num = getNum<int>(0, pile.getSize() - 1);
        cout << pile[num] << endl;
    }

    void add_card_menu(CardsPile &pile){
        Card card;
        bool is_valid = false;
        cout << "Enter your card: ";
        while (!is_valid){
            try {
                cin >> card;
                pile.add(card);
                is_valid = true;
            } catch (std::runtime_error &re){
                cout << re.what() << endl;
                if (re.what() == "The pile is already full"){
                    is_valid = true;
                }
            }
        }
        cout << "Your card was successfully added to the pile (if it wasn't full)" << endl;
        pile.print(cout);
    }

    void remove_card_menu(CardsPile &pile){
        cout << "Input index of the card you want to remove: ";
        int index = getNum<int>(0, pile.getSize() - 1);
        pile.remove(index);
        pile.print(cout);
    }

    void shuffle_pile_menu(CardsPile &pile){
        pile.myShuffle();
        pile.print(cout);
    }

    void sort_pile_menu(CardsPile &pile){
        cout << "0 - Indirect sort\n"
                "1 - Direct sort" << endl;
        int dir_ind = getNum<int>(0, 1);
        cout << "0 - Sort by suit\n"
                "1 - Sort by rank" << endl;
        int rank_suit = getNum<int>(0, 1);
        pile.sort(dir_ind, rank_suit);
        pile.print(cout);
    }

    void subpile_menu(CardsPile &pile){
        string suit = inputAndCheckSuit();
        CardsPile scp = pile.getSubPile(suit);
        scp.print(cout);
    }

    void print_menu(CardsPile &pile){
        pile.print(cout);
    }

    void show_menu(){
        cout << "1. Create the pile" << endl;
        cout << "2. Get size of the pile" << endl;
        cout << "3. Get card by its index" << endl;
        cout << "4. Add one card to the pile" << endl;
        cout << "5. Remove card from the pile by index" << endl;
        cout << "6. Shuffle the pile in a random way" << endl;
        cout << "7. Sort the pile" << endl;
        cout << "8. Print subpile of one suit" << endl;
        cout << "9. Print the pile" << endl;
        cout << "0. Stop the program" << endl;
        cout << "Enter your choice:" << endl;
    }

    std::vector<MenuFunction> menu_functions = {
        initialization_menu,
        get_size_menu,
        get_cards_by_index_menu,
        add_card_menu,
        remove_card_menu,
        shuffle_pile_menu,
        sort_pile_menu,
        subpile_menu,
        print_menu
    };

    void menu(CardsPile &pile){
        int choice;
        do {
            show_menu();
            choice = getNum<int>(0, 9);
            if (choice >= 1 && choice <= menu_functions.size()){
                menu_functions[choice - 1](pile);
            } else if (choice == 0){
                return;
            } else {
                cout << "Invalid choice. Try again:" << endl;
            }
        } while (true);
    }
}