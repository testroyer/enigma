#ifndef ENIGMA_H
#define ENIGMA_H

#include <string>
#include <bipair.h>

// À revoir après la création de la classe Biapir, pour voir si on peut l'utiliser pour les rotors et le plugboard
namespace Enigma {

    class Rotor {
        private:
            int position;
            int notchPlacement; 

        protected:
            Bipair<std::string> intWiring;
            Bipair<std::string> createInternalWiringMap(std::string chiffre); 
            //Chiffre means the order the the letters are cyrpted (For "ZASTQ..." , A->Z ,B->A and so on ) 

        public:
            Rotor(int startPosition, std::string chiffre);        
            void setPosition(int pos);
            int getPosition();
            void rotateForwards();
            void rotateBackwards();
            void getReverseWiring();
            std::string run();
            std::string reverseRun();

    };
    
    class Reflector : private Rotor {
        private:
        public:
            Reflector(std::string chiffre);
            using Rotor::getReverseWiring;
            using Rotor::run;
            using Rotor::reverseRun;

    };

    class Plugboard {
        private:
            Bipair<std::string> wiring;

        public:
            Plugboard(std::string connections);
            void addConnection(std::string pair);
            void removeConnection(std::string pair);
            int getConnectionNumber();
            std::string run();
            std::string reverseRun();
            // get reverse connection ?
    };

    class Enigma {
        private: 
        public:
            const std::string enigmaAllowedLetters[26];
            Enigma();
    };

}

#endif