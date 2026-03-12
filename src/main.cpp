#include <iostream>
#include "./bipair.h"
#include "./enigma.h"
#include <vector>
#include <cmath>

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

#define BOX_T_DOWN  0x252C // ┬ T pointing down
#define BOX_T_UP    0x2534 // ┴ T pointing up
#define BOX_T_RIGHT 0x251C // ├ T pointing right
#define BOX_T_LEFT  0x2524 // ┤ T pointing left
#define BOX_CROSS   0x253C // ┼ Cross

#define COLOR_BLACK      0    // black
#define COLOR_RED        1    // red
#define COLOR_GREEN      2    // green
#define COLOR_YELLOW     3    // yellow
#define COLOR_BLUE       4    // blue
#define COLOR_MAGENTA    5    // magenta
#define COLOR_CYAN       6    // cyan
#define COLOR_WHITE      7    // white
#define COLOR_ORANGE     214  // bright orange
#define COLOR_PINK       213  // hot pink
#define COLOR_PURPLE     135  // vibrant purple
#define COLOR_LIME       154  // bright lime green
#define COLOR_SKY_BLUE   117  // bright sky blue
#define COLOR_GOLD       220  // golden yellow
#define COLOR_TEAL        43  // vibrant teal

// Define global dimensions
constexpr int intra_letter_gap = 3;
constexpr int left_right_total_margin = 16;
constexpr int rotor_box_height = 15; // Incl. borders
constexpr int lampboard_height = 11; // Incl. borders
constexpr int outer_box_width = 9 + (8*intra_letter_gap) + 2 + left_right_total_margin - 1; //  outer_box_width = charactersPerRow + gapBetweenCharacters + edgeCharacters + left_right_total_margin - startFromZero;
constexpr int outer_box_height = rotor_box_height + lampboard_height - 1 - 1; // outer_box_height = rotor_box_height + lampboard_height - commun_border - startFromZero;

//                = (((outer_box_width - borders) - (letters*FONT_W)+letter_gaps))/ 2.0) + left_border ;
auto center_element = [](int element_length){ return (int)round(((outer_box_width-2)-element_length)/2.0)+1; };

// Pixel font data for each letter (5 wide x 7 tall)
const int FONT_W = 5;
const int FONT_H = 7;

const bool letters[][FONT_H][FONT_W] = {
    // E
    {{1,1,1,1,1},
     {1,0,0,0,0},
     {1,0,0,0,0},
     {1,1,1,1,0},
     {1,0,0,0,0},
     {1,0,0,0,0},
     {1,1,1,1,1}},
    // N
    {{1,1,1,1,1},
     {1,0,0,0,1},
     {1,0,0,0,1},
     {1,0,0,0,1},
     {1,0,0,0,1},
     {1,0,0,0,1},
     {1,0,0,0,1}},
    // I
    {{1,1,1,1,1},
     {0,0,1,0,0},
     {0,0,1,0,0},
     {0,0,1,0,0},
     {0,0,1,0,0},
     {0,0,1,0,0},
     {1,1,1,1,1}},
    // G
    {{1,1,1,1,1},
     {1,0,0,0,0},
     {1,0,0,0,0},
     {1,0,1,1,1},
     {1,0,0,0,1},
     {1,0,0,0,1},
     {1,1,1,1,1}},
    // M
    {{1,1,1,1,1},
     {1,0,1,0,1},
     {1,0,1,0,1},
     {1,0,1,0,1},
     {1,0,1,0,1},
     {1,0,1,0,1},
     {1,0,1,0,1}},
    // A
    {{1,1,1,1,1},
     {1,0,0,0,1},
     {1,0,0,0,1},
     {1,1,1,1,1},
     {1,0,0,0,1},
     {1,0,0,0,1},
     {1,0,0,0,1}},
};

#pragma endregion

using namespace std;
using namespace EnigmaMachine;



void draw_outer_box() {

    for (int column = 0 ; column <= outer_box_width; column++){
        for (int row = 0; row <= outer_box_height; row++) {
            if (row == 0 && column == 0) { // Top left corner
                tb_set_cell(column, row , BOX_TL , COLOR_WHITE , TB_256_BLACK);
            } 
            else if (row == 0 && column == outer_box_width) { // Top right corner
                tb_set_cell(column, row , BOX_TR , COLOR_WHITE , TB_256_BLACK);
            } 
            else if (row == outer_box_height && column == 0) { // Bottom left corner
                tb_set_cell(column, row , BOX_BL , COLOR_WHITE , TB_256_BLACK);
            } 
            else if (row == outer_box_height && column == outer_box_width) { // Bottom right corner
                tb_set_cell(column, row , BOX_BR , COLOR_WHITE , TB_256_BLACK);
            }
            else if (row == 14 && column == 0) { // T pointing down on left edge
                tb_set_cell(column, row , BOX_T_RIGHT , COLOR_WHITE , TB_256_BLACK);
            }
            else if (row == 14 && column == outer_box_width) { // T pointing down on right edge
                tb_set_cell(column, row , BOX_T_LEFT , COLOR_WHITE , TB_256_BLACK);
            } 
            else if (row == 0 || row == 14 || row == outer_box_height) { // Horizontal lines
                tb_set_cell(column, row , BOX_HL , COLOR_WHITE , TB_256_BLACK);
            } 
            else if (column == 0 || column == outer_box_width) { // Vertical lines
                tb_set_cell(column, row , BOX_VL , COLOR_WHITE , TB_256_BLACK);
            }
        }
    }
};

void draw_welcome_box() {


    for (int column = 0 ; column <= outer_box_width; column++){
        for (int row = 0; row <= outer_box_height; row++) {
            if (row == 0 && column == 0) { // Top left corner
                tb_set_cell(column, row , BOX_TL , COLOR_WHITE , TB_256_BLACK);
            } 
            else if (row == 0 && column == outer_box_width) { // Top right corner
                tb_set_cell(column, row , BOX_TR , COLOR_WHITE , TB_256_BLACK);
            } 
            else if (row == outer_box_height && column == 0) { // Bottom left corner
                tb_set_cell(column, row , BOX_BL , COLOR_WHITE , TB_256_BLACK);
            } 
            else if (row == outer_box_height && column == outer_box_width) { // Bottom right corner
                tb_set_cell(column, row , BOX_BR , COLOR_WHITE , TB_256_BLACK);
            }
            else if (row == 0 || row == outer_box_height) { // Horizontal lines
                tb_set_cell(column, row , BOX_HL , COLOR_WHITE , TB_256_BLACK);
            } 
            else if (column == 0 || column == outer_box_width) { // Vertical lines
                tb_set_cell(column, row , BOX_VL , COLOR_WHITE , TB_256_BLACK);
            }
        }
    }
};

void pretty_print(const string& text, int x, int y, uint16_t fg_color = COLOR_WHITE, uint16_t bg_color = TB_256_BLACK) {
    for (size_t i = 0; i < text.size(); ++i) {
        tb_set_cell(x + i, y, text[i], fg_color, bg_color);
    }
};

void draw_enigma_title(int start_x, int start_y, int fg_color, int bg_color) { // 
    const int letter_count = 6; // E N I G M A
    const int letter_gap = 1; // Space between letters

    for (int l = 0; l < letter_count; l++) {
        int letter_x = start_x + l * (FONT_W + letter_gap);
        for (int row = 0; row < FONT_H; row++) {
            for (int col = 0; col < FONT_W; col++) {
                int color = letters[l][row][col] ? fg_color : bg_color;
                tb_set_cell(letter_x + col, start_y + row, ' ', color, color);
            }
        }
    }
}


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
        char lastPressed = ' ';
        int state = 0; // 0 = Intro Screen, 1 = Encryption, 2 = Set Rotors, 3 = Set Plugboard

        while (true) {
            tb_clear();
            
            tb_event ev;
            tb_poll_event(&ev);
            if (ev.type == TB_EVENT_KEY ) {
                if (ev.key == TB_KEY_CTRL_Q) {
                    break;
                }
            }


            switch (state) {
                case 0: // Intro Screen
                    draw_welcome_box();
                    draw_enigma_title(center_element(((6*FONT_W)+5)), 4, COLOR_GOLD, TB_256_BLACK); // Removes the border characters then readds the left border.
                    pretty_print("Press Enter to Start", center_element(20), 12, COLOR_GREEN, TB_256_BLACK); 
                    pretty_print("Press Ctrl+Q to Quit", center_element(20), 14, COLOR_RED, TB_256_BLACK);

                    if (ev.type == TB_EVENT_KEY ) {
                        if (ev.key == TB_KEY_ENTER) {
                            state = 1; // Move to encryption screen
                        }
                    }

                    break;
                case 1: // Encryption
                    draw_outer_box();
                
                    break;
                case 2: // Set Rotors
                    draw_outer_box();
                
                    break;
                case 3: // Set Plugboard
                    draw_outer_box();

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
