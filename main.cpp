#include <iostream>
#include "./bipair.h"
#include "./enigma.h"
#include <vector>
using namespace std;
using namespace EnigmaMachine;

int main() {
    try {
        Rotor first  = Rotor("JGDQOXUSCAMIFRVTPNEWKBLZYH", 0, 0);
        Rotor second = Rotor("NTZPSFBOKMWRCJDIVLAEYUXHGQ", 0, 0);
        Rotor third  = Rotor("JVIUBHTCDYAKEQZPOSGXNRMWFL", 0, 0);
        vector<Rotor> rotors = {first, second, third};
        Reflector reflector = Reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT");
        Bipair<char> plugboardWiring = Bipair<char>({{'A', 'C'}});
        Plugboard plugboard = Plugboard(plugboardWiring);
        Enigma enigma = Enigma(rotors, reflector, plugboard);


    } catch (const std::exception& e) {
        cout << "exception: " << e.what() << "\n";
    }
}