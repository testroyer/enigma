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
            int notchPlacement; 
            Bipair<string> createInternalWiringMap(string chiffre); 
            //Chiffre means the order the the letters are cyrpted (For "ZASTQ..." , A->Z ,B->A and so on ) 

        public:
            Rotor(int startPosition, string chiffre);        
            void setPosition(int pos);
            int getPosition();
            void rotateForwards();
            void rotateBackwards();
            void getReverseWiring();

    };

    class Plugboard {
        private:
            Bipair<string> wiring;

        public:
            Plugboard(string connections);
            void addConnection(string pair);
            void removeConnection(string pair);
            int getConnectionNumber();
            // get reverse connection ?
    };

    class Enigma {
        private: 
            Bipair<string> reflectorWiring;
        public:
            const string enigmaAllowedLetters[26];
            Enigma();
    };

}

#endif