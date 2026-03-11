#include <iostream>
#include "./bipair.h"
#include "./enigma.h"
#include <vector>

#define TB_IMPL
#include "../external/termbox2/termbox2.h"

using namespace std;
using namespace EnigmaMachine;

int main() {
    try {

        // 1930 Enigma I with rotors I, II and III, reflector B and plugboard connection A-C.
        Rotor first  = Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", 0, 16);
        Rotor second = Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", 0, 4);
        Rotor third  = Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", 0, 21);
        vector<Rotor> rotors = {first, second, third};
        Reflector reflector = Reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT");
        Bipair<char> plugboardWiring = Bipair<char>({{'A', 'C'}});
        Plugboard plugboard = Plugboard(plugboardWiring);
        Enigma enigma = Enigma(rotors, reflector, plugboard);

        tb_init();
        char encryptedChar = enigma.encrypt('E');

        while (true) {
            tb_clear();
            
            tb_event ev;
            tb_poll_event(&ev);

            tb_set_cell(0 , 0 , encryptedChar , TB_WHITE , TB_BLACK);

            if (ev.type == TB_EVENT_KEY && ev.key == TB_KEY_ESC) {
                break;
            }


            tb_present();
        } 
    
        tb_shutdown();
        return 0;

      
    } catch (const std::exception& e) {
        cout << "exception: " << e.what() << "\n";
    }
}