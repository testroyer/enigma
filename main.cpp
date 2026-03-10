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

        // Encrypt char by char
        char e1 = enigma.encrypt('A');
        cout << "Encrypted A -> " << e1 << "\n";
        cout << "Positions after: " << enigma.rotors[0].getPosition() << "," << enigma.rotors[1].getPosition() << "," << enigma.rotors[2].getPosition() << "\n";

        char e2 = enigma.encrypt('Z');
        cout << "Encrypted Z -> " << e2 << "\n";
        cout << "Positions after: " << enigma.rotors[0].getPosition() << "," << enigma.rotors[1].getPosition() << "," << enigma.rotors[2].getPosition() << "\n";

        // Reset and decrypt
        enigma.setRotorPositions({0, 0, 0});
        cout << "\n--- Reset ---\n\n";

        char d1 = enigma.encrypt(e1);
        cout << "Decrypted " << e1 << " -> " << d1 << "\n";
        cout << "Positions after: " << enigma.rotors[0].getPosition() << "," << enigma.rotors[1].getPosition() << "," << enigma.rotors[2].getPosition() << "\n";

        char d2 = enigma.encrypt(e2);
        cout << "Decrypted " << e2 << " -> " << d2 << "\n";
        cout << "Positions after: " << enigma.rotors[0].getPosition() << "," << enigma.rotors[1].getPosition() << "," << enigma.rotors[2].getPosition() << "\n";

    } catch (const std::exception& e) {
        cout << "exception: " << e.what() << "\n";
    }
}