#include <iostream>
#include "bipair.h"
#include "enigma.h"
#include <vector>

using namespace std;
using namespace EnigmaMachine;

int main() {
    // Enigma element initialisation 
    Rotor first = Rotor("JGDQOXUSCAMIFRVTPNEWKBLZYH" , 0 , 0);
    Rotor second = Rotor("NTZPSFBOKMWRCJDIVLAEYUXHGQ" , 0 , 0);
    Rotor third = Rotor("JVIUBHTCDYAKEQZPOSGXNRMWFL" , 0 , 0);
    vector<Rotor> rotors = {first , second, third};

    Reflector reflector = Reflector("QYHOGNECVPUZTFDJAXWMKISRBL");

    Plugboard plugboard = Plugboard(Bipair<char>({})); 

    Enigma enigma = Enigma(rotors , reflector , plugboard);


    cout << enigma.encrypt('A');
}