#include <iostream>
#include "./bipair.h"
#include "./enigma.h"
#include <vector>
#include <functional>
#include <cmath>

#define TB_IMPL
#include "../external/termbox2/termbox2.h"

using namespace std;
using namespace EnigmaMachine;

#pragma region Global Definitions

// Constant definitions of ASCII characters
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

#define ARROW_UP   0x2191 //↑
#define ARROW_DOWN 0x2193 //↓

// Definitions according to termbox's 256 colours
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
#define COLOR_GRAY       244  // medium gray

#define FOREGROUND COLOR_WHITE
#define BACKGROUND TB_256_BLACK

constexpr int plugboard_colors[13] = {
    COLOR_RED , 
    COLOR_GREEN , 
    COLOR_YELLOW , 
    COLOR_BLUE , 
    COLOR_MAGENTA , 
    COLOR_CYAN , 
    COLOR_ORANGE, 
    COLOR_PINK ,
    COLOR_PURPLE ,
    COLOR_LIME , 
    COLOR_SKY_BLUE , 
    COLOR_TEAL,
    COLOR_GOLD,
};

// Rotor/Keyboard assembly related
constexpr int expected_rotor_count = 3;

// Key bindings
uint32_t rotor_key_bindings[2][3] = { //Up then down.
    {0x005B , 0x003B , 0x002E},
    {0x005D , 0x0027 , 0x002F}
};

const vector<vector<uint32_t>> lampboard = {
    {'Q' , 'W' , 'E' , 'R' , 'T' , 'Z' , 'U' , 'I' , 'O'},
       {'A' , 'S' , 'D' , 'F' , 'G' , 'H' , 'J' , 'K'},
    {'P' , 'Y' , 'X' , 'C' , 'V' , 'B' , 'N' , 'M' , 'L'}
};

// Define global dimensions
constexpr int left_right_margin = 6; // In lamp/plugboard for now, must be a pair
constexpr int border = 1;

//Rotor related 
constexpr int intra_rotor_gap = 1;
constexpr int intra_rotor_numbers = 1; // Gap between rotor numbers
constexpr int rotor_shown_number_count = 3; //Always an odd number to have a center rotor number 
constexpr int rotor_shown_digits_count = 2; // For displaying rotor numbers as 01, 02, etc.
constexpr int rotor_number_lr_margin = 2;
constexpr int rotor_element_updown_margin = 2;

constexpr int rotor_element_width = rotor_shown_digits_count + 2*rotor_number_lr_margin + 2*border; 
constexpr int rotor_element_heigth = (rotor_shown_number_count + 2) + ((rotor_shown_number_count + 2) + 1)*intra_rotor_numbers + 2*border; // (rotor_shown_number_count + 2) are for mover characters

//Lamp/Plugboard
constexpr int max_key_per_lampboard_row = 9;
constexpr int intra_letter_gap = 3;
constexpr int intra_lampboard_row = 1;
constexpr int lampboard_updown_margin = 2;
constexpr int lampboard_row_count = 3;

//Boxes
constexpr int rotor_box_height = rotor_element_heigth + 2*rotor_element_updown_margin + 2*border; 
constexpr int lampboard_height = lampboard_row_count + (lampboard_row_count-1)*intra_lampboard_row + 2*lampboard_updown_margin + 2*border;

constexpr int outer_box_width = max_key_per_lampboard_row + ((max_key_per_lampboard_row-1)*intra_letter_gap) + 2*border+ 2*left_right_margin; // À dynamiser, figurer si le clavier ou les rotors sont le plus grand
constexpr int outer_box_height = rotor_box_height + lampboard_height - border; 

// Pixel font data for each letter (5 wide x 7 tall)
const int FONT_W = 5;
const int FONT_H = 5;

const bool letters[][FONT_H][FONT_W] = {
    // E
    {{1,1,1,1,1},
     {1,0,0,0,0},
     {1,1,1,1,0},
     {1,0,0,0,0},
     {1,1,1,1,1}},
    // N
    {{1,1,1,1,1},
     {1,0,0,0,1},
     {1,0,0,0,1},
     {1,0,0,0,1},
     {1,0,0,0,1}},
    // I
    {{1,1,1,1,1},
     {0,0,1,0,0},
     {0,0,1,0,0},
     {0,0,1,0,0},
     {1,1,1,1,1}},
    // G
    {{1,1,1,1,1},
     {1,0,0,0,0},
     {1,0,1,1,1},
     {1,0,0,0,1},
     {1,1,1,1,1}},
    // M
    {{1,1,1,1,1},
     {1,0,1,0,1},
     {1,0,1,0,1},
     {1,0,1,0,1},
     {1,0,1,0,1}},
    // A
    {{1,1,1,1,1},
     {1,0,0,0,1},
     {1,1,1,1,1},
     {1,0,0,0,1},
     {1,0,0,0,1}}
};

#pragma endregion



#pragma region Helper lambdas

std::function<void()> debug_action = nullptr;
tb_event lastEvent;

auto center = [](int size ,int outer_element_width , bool biggerGapFirst = false) -> int { 
    return (int)(((outer_element_width-2)-size)/2.0)+((int)(biggerGapFirst))+border; 
};

auto get_rotor_number_as_str = [](int active_rotor_number) -> string {
    return to_string(active_rotor_number).length() == 1 ? "0" + to_string(active_rotor_number) : to_string(active_rotor_number); // Pad with zero if single digit 
};

auto find_rotor_keybind = [](uint32_t key) -> pair<bool, pair<int, int>> {
    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < expected_rotor_count; c++ ) {
            if (rotor_key_bindings[r][c] == key){
                return {true , {r,c}};
            }
        }
    }
    return {false , {0,0}};
};
#pragma endregion

#pragma region Drawer Functions
void draw_outer_box() {
    int current_width_limit = outer_box_width - 1; // Adjust for 0-based indexing
    int current_height_limit = outer_box_height - 1; // Adjust for 0-based

    for (int column = 0 ; column <= current_width_limit; column++){
        for (int row = 0; row <= current_height_limit; row++) {
            if (row == 0 && column == 0) { // Top left corner
                tb_set_cell(column, row , BOX_TL , FOREGROUND , BACKGROUND);
            } 
            else if (row == 0 && column == current_width_limit) { // Top right corner
                tb_set_cell(column, row , BOX_TR , FOREGROUND , BACKGROUND);
            } 
            else if (row == current_height_limit && column == 0) { // Bottom left corner
                tb_set_cell(column, row , BOX_BL , FOREGROUND , BACKGROUND);
            } 
            else if (row == current_height_limit && column == current_width_limit) { // Bottom right corner
                tb_set_cell(column, row , BOX_BR , FOREGROUND , BACKGROUND);
            }
            else if (row == rotor_box_height-1 && column == 0) { // T pointing down on left edge
                tb_set_cell(column, row , BOX_T_RIGHT , FOREGROUND , BACKGROUND);
            }
            else if (row == rotor_box_height-1 && column == current_width_limit) { // T pointing down on right edge
                tb_set_cell(column, row , BOX_T_LEFT , FOREGROUND , BACKGROUND);
            } 
            else if (row == 0 || row == rotor_box_height-1 || row == current_height_limit) { // Horizontal lines
                tb_set_cell(column, row , BOX_HL , FOREGROUND , BACKGROUND);
            } 
            else if (column == 0 || column == current_width_limit) { // Vertical lines
                tb_set_cell(column, row , BOX_VL , FOREGROUND , BACKGROUND);
            }
        }
    }
};

void draw_welcome_box() {
    int current_width_limit = outer_box_width - 1; // Adjust for 0-based indexing
    int current_height_limit = outer_box_height - 1; // Adjust for 0-based

    for (int column = 0 ; column <= current_width_limit; column++){
        for (int row = 0; row <= current_height_limit; row++) {
            if (row == 0 && column == 0) { // Top left corner
                tb_set_cell(column, row , BOX_TL , FOREGROUND , BACKGROUND);
            } 
            else if (row == 0 && column == current_width_limit) { // Top right corner
                tb_set_cell(column, row , BOX_TR , FOREGROUND , BACKGROUND);
            } 
            else if (row == current_height_limit && column == 0) { // Bottom left corner
                tb_set_cell(column, row , BOX_BL , FOREGROUND , BACKGROUND);
            } 
            else if (row == current_height_limit && column == current_width_limit) { // Bottom right corner
                tb_set_cell(column, row , BOX_BR , FOREGROUND , BACKGROUND);
            }
            else if (row == 0 || row == current_height_limit) { // Horizontal lines
                tb_set_cell(column, row , BOX_HL , FOREGROUND , BACKGROUND);
            } 
            else if (column == 0 || column == current_width_limit) { // Vertical lines
                tb_set_cell(column, row , BOX_VL , FOREGROUND , BACKGROUND);
            }
        }
    }
};

void pretty_print(const string& text, int x, int y, uint16_t fg_color = FOREGROUND, uint16_t bg_color = BACKGROUND) {
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

void draw_rotor_assembly(Enigma enigma, bool mode_set = false ) {
    // Rotor assembly dimensions and positioning
    int brute_rotor_assembly_width = (enigma.rotors.size() * rotor_element_width) + ((enigma.rotors.size() - 1) * intra_rotor_gap);
    int rotor_assembly_x = center(brute_rotor_assembly_width , outer_box_width);
    int rotor_assembly_y = center(rotor_element_heigth , rotor_box_height);

    //Get border placements
    for (size_t i = 0; i < enigma.rotors.size(); i++) {
        //Top-left of the current rotor box
        int reversed_i = enigma.rotors.size() - 1 - i;
        int rotor_x = rotor_assembly_x + reversed_i * (rotor_element_width + intra_rotor_gap);
        int rotor_y = rotor_assembly_y;

        int current_width_limit = rotor_element_width - 1; // Adjust for 0-based indexing
        int current_height_limit = rotor_element_heigth - 1; // Adjust for 0-based indexing

        int current_rotor_x = center(2 , rotor_element_width); // Center the rotor number in the box
        int current_rotor_y = center(1 , rotor_element_heigth); // Center the rotor number in the box;

        int current_rotor_number = enigma.rotors[i].getPosition() + 1; // Get the current rotor number (1-based index)
        string rotor_number_str = get_rotor_number_as_str(current_rotor_number);

        // Draw rotor box
        for (int column = 0; column <= current_width_limit; column++) {
            for (int row = 0; row <= current_height_limit; row++) {
                if (row == 0 && column == 0) { // Top left corner
                    tb_set_cell(rotor_x + column, rotor_y + row, BOX_TL, FOREGROUND, BACKGROUND);
                } 
                else if (row == 0 && column == current_width_limit) { // Top right corner
                    tb_set_cell(rotor_x + column, rotor_y + row, BOX_TR, FOREGROUND, BACKGROUND);
                } 
                else if (row == current_height_limit && column == 0) { // Bottom left corner
                    tb_set_cell(rotor_x + column, rotor_y + row, BOX_BL, FOREGROUND, BACKGROUND);
                } 
                else if (row == current_height_limit && column == current_width_limit) { // Bottom right corner
                    tb_set_cell(rotor_x + column, rotor_y + row, BOX_BR, FOREGROUND, BACKGROUND);
                }
                else if (row == current_rotor_y && 
                    column >= current_rotor_x && 
                    column < current_rotor_x + (int)rotor_number_str.size()) { // Print rotor number in the center

                    char digit = rotor_number_str[column - current_rotor_x];
                    tb_set_cell(rotor_x + column, rotor_y + row, digit, COLOR_GOLD, BACKGROUND);
                }
                else if ((row - current_rotor_y) % (1+intra_rotor_numbers) == 0 && 
                    abs(((row - current_rotor_y) / (1+intra_rotor_numbers))) == (rotor_shown_number_count-1)/2 && 
                    column >= current_rotor_x && 
                    column < current_rotor_x + (int)rotor_number_str.size()) {

                    int normalised = normalisePosition(current_rotor_number + (-(row-current_rotor_y)) / (intra_rotor_numbers+1));
                    string offset_rotor_number = get_rotor_number_as_str(normalised ? normalised : 26);
                    char digit = offset_rotor_number[column - current_rotor_x];
                    tb_set_cell(rotor_x + column, rotor_y + row, digit, FOREGROUND, BACKGROUND);
                }
                else if ((row - current_rotor_y) % (1+intra_rotor_numbers) == 0 &&
                    abs(((row - current_rotor_y) / (1+intra_rotor_numbers))) == ((rotor_shown_number_count-1)/2)+1 && 
                    column >= current_rotor_x && 
                    column < current_rotor_x + (int)rotor_number_str.size()) {

                    uint32_t digit = (column - current_rotor_x) == 0 ? rotor_key_bindings[(row-current_rotor_y)>0][i] : ((row-current_rotor_y)>0) ? ARROW_DOWN : ARROW_UP;
                    tb_set_cell(rotor_x + column, rotor_y + row, digit, FOREGROUND, (mode_set ? COLOR_BLUE : BACKGROUND));
                }
                else if (row == 0 || row == current_height_limit) { // Horizontal lines
                    tb_set_cell(rotor_x + column, rotor_y + row, BOX_HL, FOREGROUND, BACKGROUND);
                } 
                else if (column == 0 || column == current_width_limit) { // Vertical lines
                    tb_set_cell(rotor_x + column, rotor_y + row, BOX_VL, FOREGROUND, BACKGROUND);
                }
            }
        }
    }

}

void draw_keyboard(Enigma enigma, uint32_t last_char = 0x0000 ,bool mode_set = false) {
    int lampboard_assembly_width = max_key_per_lampboard_row + (max_key_per_lampboard_row + 1)*intra_letter_gap;
    int lampboard_assembly_height = lampboard_row_count + ((lampboard_row_count-1) * intra_lampboard_row);
    int outer_x = center(lampboard_assembly_width, outer_box_width);
    int outer_y = center(lampboard_assembly_height , lampboard_height);

    for (size_t row = 0; row < lampboard.size() ; row++){
        int rowSize = lampboard[row].size();
        int inner_x = center((rowSize + (rowSize-1)*intra_letter_gap) , lampboard_assembly_width); 
        for (int column = 0; column < rowSize; column++ ){
            int raw_row = outer_y + rotor_box_height + row*(intra_lampboard_row+1) - 1;
            int raw_column = outer_x + inner_x + column*(intra_letter_gap+1);
            if (!mode_set) {
                tb_set_cell(raw_column , raw_row , lampboard[row][column] , FOREGROUND , (lampboard[row][column] == last_char ? COLOR_BLUE : BACKGROUND));
            }else {
                vector<pair<char , char>> couplings = enigma.plugboard.getConnections().getPairs();
                auto it = find_if(couplings.begin() , couplings.end() , [&row , &column](const auto& p){
                    return lampboard[row][column] == (uint32_t)p.first || lampboard[row][column] == (uint32_t)p.second;
                });
                tb_set_cell(raw_column, raw_row , lampboard[row][column] , (it == couplings.end()) ? FOREGROUND : plugboard_colors[it - couplings.begin()] , BACKGROUND);

            }
        }
    }

}

void draw_info(tb_event ev) { //Hardcoded info
    pretty_print("1930 Enigma I with rotors I, II and III, reflector B" , outer_box_width+1 , 1 , FOREGROUND , BACKGROUND);
    pretty_print("Rotor 1:   EKMFLGDQVZNTOWYHXUSPAIBRCJ" , outer_box_width+1 , 2 , FOREGROUND , BACKGROUND);
    pretty_print("Rotor 2:   AJDKSIRUXBLHWTMCQGZNPYFVOE" , outer_box_width+1 , 3 , FOREGROUND , BACKGROUND);
    pretty_print("Rotor 3:   BDFHJLCPRTXVZNYEIWGAKMUSQO" , outer_box_width+1 , 4 , FOREGROUND , BACKGROUND);
    pretty_print("Reflector: YRUHQSLDPXNGOKMIEBFZCWVJAT" , outer_box_width+1 , 5 , FOREGROUND , BACKGROUND);

    char dbg[64];
    snprintf(dbg, sizeof(dbg), "type:%d key:%d ch:%d ch:\"%c\"", ev.type, ev.key, ev.ch , (toupper(ev.ch)));
    pretty_print(dbg, outer_box_width+1, 7, FOREGROUND, BACKGROUND);
}

#pragma endregion

#pragma region Program
int main() {
    try {

        #pragma region Initialization
            // 1930 Enigma I with rotors I, II and III, reflector B 
            Rotor first  = Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", 0, 16);
            Rotor second = Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", 0, 4);
            Rotor third  = Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", 0, 21);
            vector<Rotor> rotors = {first, second, third};
            if (rotors.size() != expected_rotor_count) {
                cout << "Error rotors size does not match expected rotors count.";
                return 1;
            }
            Reflector reflector = Reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT");
            Bipair<char> plugboardWiring = Bipair<char>({});
            Plugboard plugboard = Plugboard(plugboardWiring);
            Enigma enigma = Enigma(rotors, reflector, plugboard);

            tb_init();
            tb_set_output_mode(TB_OUTPUT_256);
            tb_set_input_mode(TB_INPUT_ESC);

            //State variables
            uint32_t lastPressed = 0x0000;
            int state = 0; // 0 = Intro Screen, 1 = Encryption, 2 = Set
            bool info = false;
            bool running = true;
            pair<uint32_t , uint32_t> primal_connection = {0x0000 , 0x0000};
        #pragma endregion


        while (running) {
            tb_clear();

            #pragma region Drawers loop
                switch (state) {
                    case 0: {// Intro Screen
                        draw_welcome_box();
                        draw_enigma_title(center(((6*FONT_W)+5) , outer_box_width), 5, COLOR_GOLD, BACKGROUND); 
                        pretty_print("Press Enter to Start", center(20 , outer_box_width), 12, COLOR_GREEN, BACKGROUND); 
                        pretty_print("Press SPCACE to change modes", center(28, outer_box_width), 14, COLOR_BLUE , BACKGROUND);
                        pretty_print("Press Ctrl+Q to Quit", center(20, outer_box_width), 16, COLOR_RED, BACKGROUND);

                        if (debug_action) { debug_action(); debug_action = nullptr;}

                        break;
                    }
                    case 1: { // Encryption
                        draw_outer_box();
                        draw_rotor_assembly(enigma , false);
                        draw_keyboard(enigma, lastPressed);
                        if (info) draw_info(lastEvent);
                        if (debug_action) { debug_action(); debug_action = nullptr;}
                    
                        break;
                    }
                    case 2: { // Set Rotors
                        draw_outer_box();
                        draw_rotor_assembly(enigma , true);
                        draw_keyboard(enigma , 0x0000 , true);
                        if (info) draw_info(lastEvent);
                        if (debug_action) { debug_action(); debug_action = nullptr;}
                    
                        break;
                    }
                }
                
                tb_present();
            #pragma endregion

            #pragma region Event loop
                tb_event ev;
                tb_poll_event(&ev);
                lastEvent = ev;
                if (ev.type != TB_EVENT_KEY ) continue;;
                if (ev.type == TB_EVENT_KEY ) {
                    if (ev.key == TB_KEY_CTRL_Q) {
                        running = false;
                    }
                    else if (ev.ch == '"') {
                        info = !info;
                        debug_action = [info]() {
                            if (info) {
                                pretty_print("[Debug mode on]" , 0 , outer_box_height+1 , FOREGROUND , COLOR_YELLOW);
                            } else {

                                pretty_print("[Debug mode off]" , 0 , outer_box_height+1 , FOREGROUND , COLOR_YELLOW);
                            }
                        };
                    }
                    switch (state){
                        case 0: { // Intro
                            if (ev.key == TB_KEY_ENTER) {
                                state = 1; // Move to encryption screen
                            }
                            break;
                        }

                        case 1: { //Encrypt
                            if (ev.ch == TB_KEY_SPACE) {
                                state = 2;
                                debug_action = [&]() -> void {
                                    pretty_print("Switched to \"Set\" mode" , 0 , outer_box_height+1 , FOREGROUND , COLOR_YELLOW);
                                };
                            }
                            if (checkIfEngimaEnabledChar(toupper(ev.ch))){
                                lastPressed = enigma.encrypt(ev.ch);
                            }
                            break;
                        }

                        case 2: { // Set 
                            if (ev.ch == TB_KEY_SPACE) {
                                state = 1;
                                primal_connection.first = 0x0000;
                                primal_connection.second = 0x0000;
                                debug_action = [&]() -> void {
                                    pretty_print("Switched to \"Encrypt\" mode" , 0 , outer_box_height+1 , FOREGROUND , COLOR_YELLOW);
                                };
                            } 

                            if (checkIfEngimaEnabledChar(toupper(ev.ch))) {
                                if (primal_connection.first == 0x0000){
                                    primal_connection.first = toupper(ev.ch);
                                    debug_action = [&]() -> void {
                                        pretty_print(string("Listening for couple of: ") + (char)primal_connection.first, 0, outer_box_height+1, FOREGROUND, COLOR_CYAN);
                                    };
                                    break;
                                } else if(primal_connection.second == 0x0000) {
                                    primal_connection.second = toupper(ev.ch); // I see this may be unneeded but best practice is to keep to avoid possible confusion later
                                    try {
                                        pair<char, char> new_connection = {(char)primal_connection.first , (char)primal_connection.second};
                                        if (enigma.plugboard.getConnections().checkExactPairExistence(new_connection)) {
                                            uint32_t a = primal_connection.first; //Capture before thay are gone
                                            uint32_t b = primal_connection.second;
                                            debug_action = [a , b]() -> void {
                                                pretty_print((string("Removed: ") + (char)a + string("-") +(char)b) , 0 , outer_box_height+1 , FOREGROUND , COLOR_RED);
                                            };
                                            enigma.plugboard.removeConnection(new_connection);
                                        } else {
                                            enigma.plugboard.addConnection(new_connection);
                                        }
                                    } catch (std::exception& e) {
                                        debug_action = [&]() -> void {
                                            pretty_print(e.what() , 0 , outer_box_height+1 ,FOREGROUND , COLOR_RED);
                                        };
                                        break;
                                    }
                                    primal_connection.first = 0x0000;
                                    primal_connection.second = 0x0000;
                                    break;

                                }
                            }
                            if (primal_connection.first != 0x0000)
                            primal_connection = {0x0000 , 0x0000}; //reset the primal connections if not caught any

                            auto rot_keybind = find_rotor_keybind(ev.ch);
                            if (rot_keybind.first) {
                                vector<int> current_rotor_pos = enigma.getRotorPositions();
                                current_rotor_pos[rot_keybind.second.second] += (rot_keybind.second.first == 0 ? 1 : -1);
                                current_rotor_pos[rot_keybind.second.second] = normalisePosition(current_rotor_pos[rot_keybind.second.second]);
                                enigma.setRotorPositions(current_rotor_pos);
                            }
                            break;
                        }
                    }
                    
                }

            } 
        #pragma endregion
    
        tb_shutdown();
        return 0;

      
    } catch (const std::exception& e) {
        tb_shutdown();
        cout << "Exception: " << e.what() << "\n";
        return 1;
    }
}
#pragma endregion
