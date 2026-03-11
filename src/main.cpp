#include <iostream>
#include "./bipair.h"
#include "./enigma.h"
#include <vector>

#define TB_IMPL
#include "../external/termbox2/termbox2.h"

#pragma region Termbox Definitions

// Constant definitions of ASCII box building characters
#define BOX_TL 0x250C // Top left corner 
#define BOX_TR 0x2510 // Top right corner
#define BOX_BL 0x2514 // Bottom left corner
#define BOX_BR 0x2518 // Bottom right corner
#define BOX_HL 0x2500 // Horizontal line
#define BOX_VL 0x2502 // Vertical line

#define COLOR_ORANGE     214  // bright orange
#define COLOR_PINK       213  // hot pink
#define COLOR_PURPLE     135  // vibrant purple
#define COLOR_LIME       154  // bright lime green
#define COLOR_SKY_BLUE   117  // bright sky blue
#define COLOR_GOLD       220  // golden yellow
#define COLOR_TEAL        43  // vibrant teal

#pragma endregion

using namespace std;
using namespace EnigmaMachine;

/*
 * ====================Views====================
 * Intro Screen
 * Encryption
 * Set Rotors
 * Set Plugboard
 * */

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
	tb_set_output_mode(TB_OUTPUT_256);

        while (true) {
            tb_clear();
            
            tb_event ev;
            tb_poll_event(&ev);

            tb_set_cell(0 , 0 , encryptedChar , TB_WHITE , TB_BLACK);

            if (ev.type == TB_EVENT_KEY ) {
		if (ev.key == TB_KEY_ESC) {
		    break;
		}
            }


            tb_present();
        } 
    
        tb_shutdown();
        return 0;

      
    } catch (const std::exception& e) {
        cout << "exception: " << e.what() << "\n";
    }
}
