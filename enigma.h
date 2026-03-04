#ifndef ENIGMA_H
#define ENIGMA_H

#include <map>
#include <string>
#include <bipair.h>

using namespace std;
// À revoir après la création de la classe Biapir, pour voir si on peut l'utiliser pour les rotors et le plugboard
namespace Enigma {

    class Rotor {
        private:
            int position;
            Bipair<string> intWiring;
            map<string, string> createInternalWiringMap(string chiffre); 
            int notchPlacement; 

        public:
            Rotor(int startPosition, string chiffre);        
            void setPosition(int pos);
            void rotateForwards();
            void rotateBackwards();
            void getReverseWiring();

    };

    class Plugboard {
        private:
            map<string, string> wiring;

        public:
            Plugboard(string connections);
            void addConnection(string pair);
            void removeConnection(string pair);
            int getConnectionNumber();
            // get reverse connection ?
    };

    class Enigma {
        private: 
            map<string, string> reflectorWiring;
        public:
            const string enigmaAllowedLetters[26];
            Enigma();
    };

}

#endif