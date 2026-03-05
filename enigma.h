#ifndef ENIGMA_H
#define ENIGMA_H

#include <string>
#include "bipair.h"
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <map>

namespace EnigmaMachine {
    
    //List of enigma enabled Latin characters
    const char enigmaAllowedLetters[27];

    class Rotor {
        private:
            // Value [0, 25], determines the current orientation of the rotor.
            int position; 

            
            int notchPlacement; 

        protected:
            // Must use map here as two sides of the rotors are connected.

            std::map<char ,char> intWiring;
            std::map<char, char> createInternalWiringMap(std::string chiffre); 

        public:

            Rotor(std::string chiffre, int startPosition = 0, int notchPlacement = 0);        
            Rotor(std::map<char , char>& wiring, int startPosition = 0, int notchPlacement = 0);
            void setPosition(int pos);
            int getPosition() const;
            int getNotchPosition() const;
            bool rotateForwards();
            const std::map<char , char> getReverseWiring() const;
            char run(char character) const;
            char reverseRun(char character) const;

            // void rotateBackwards();
            // std::string run();   //Wouldn't hurt to have a string overload lateron
            // std::string reverseRun();

    };
    
    class Reflector {
        private:
            const Bipair<char> wiring;
        public:
            Reflector(std::string chiffre);
            Reflector(Bipair<char> wiring);
            const Bipair<char>& getWiring() const;
            char run(char character) const;
    };

    class Plugboard {
        private:
            Bipair<char> connections;

        public:
            Plugboard(const Bipair<char>& plugboardWiring);
            void addConnection(std::pair<char , char> pair);
            void removeConnection(std::pair<char , char> pair);
            int getConnectionNumber() const;
            const Bipair<char>& getConnections() const;
            char run(char character) const;
            // get reverse connection ?
    };

    class Enigma {
        private: 
        public:
            //Gets already created objects
            Enigma(std::vector<Rotor> rotors, Reflector reflector, Plugboard initialPlugboard);
            
            /*
            Gets the data to create the objects itself.
            Problem with these is that they don't determine any notch placement
            */
            Enigma(std::initializer_list<std::map<char, char>> rotors, Bipair<char> reflector, Bipair<char> initialPlugboard);
            Enigma(std::initializer_list<std::string> rotorsChiffres, std::string reflectorChiffre, Bipair<char> initialPlugboard);


            std::vector<Rotor> rotors;
            Reflector reflector;
            Plugboard plugboard;

            std::string encrypt(std::string message);
            char encrypt(char character);
            int getRotorCount() const;
            void setRotorPositions(std::vector<int> positions);
            void setRotorPositions(std::initializer_list<int> positions);
    };

}

#endif