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

        public:
            Rotor(int startPosition, string chiffre);        
            void setPosition(int pos);
            void rotate();
            void getReverseWiring();

    };

}

#endif