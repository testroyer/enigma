#ifndef ENIGMA_H
#define ENIGMA_H

#include <map>
#include <string>

using namespace std;

namespace Enigma {

    class Rotor {
        private:
            int position;
            map<string , string> intWiring;
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