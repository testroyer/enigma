#include <string>
#include "bipair.h"
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <map>
#include <cctype>
#include <sstream>

using namespace std;

namespace EnigmaMachine {
    //Every class has two ctors, one that ctors with built objects and one that ctors with initializer lists. 
    //TODO: String ctors, chiffres and runs along with chars
    //TODO check int comparizons lengths value domains etc.
    //TODO: Check this-> 'es


    const string enigmaAllowedLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    class Rotor {
        private:
            //[0,25] 
            // Actually it is the antirotation of the rotor. When it's one the Z is in A's place and so on. 
            int position; 
            int notchPlacement; 

        protected:

            // Must use map here as two sides of the rotors are connected.
            map<char ,char> intWiring;
            
            //Chiffre means the order the the letters are cyrpted (For "ZASTQ..." , A->Z ,B->A and so on ) 
            map<char, char> createInternalWiringMap(string chiffre){

                if (chiffre.size() != enigmaAllowedLetters.size()) {
                   throw runtime_error("Error: Size of chiffre does not match 26"); //Could be more explicative maybe
                }

                for (int i = 0; i < enigmaAllowedLetters.size() ; i++){
                    char currentChiffre = toupper(chiffre[i]);
                    if (currentChiffre < 'A' || currentChiffre > 'Z') { // Condition courtesy of Mr. GPT
                        std::ostringstream oss;
                        oss << "Error: a non-Enigma-enabled value has been found in chiffre (at index" << i << ") while initializing a rotor. Object at " << this;
                        throw std::runtime_error(oss.str());
                    }
                    this->intWiring[enigmaAllowedLetters[i]] = currentChiffre;
                }
                return this->intWiring; //Return value just-in-case
            }; 

            map<char , char> checkInternalWiringMap(std::map<char , char> wiring) const {
                
                if (wiring.size() != enigmaAllowedLetters.size()) {
                    std::ostringstream oss;
                    oss << "Error: wiring size does not match the Enigma-allowed characters list. Object at " << this;
                    throw std::runtime_error(oss.str());
                }

                for (const auto& pair : wiring) {
                    if ((pair.first < 'A' || pair.first > 'Z') || (pair.second < 'A' || pair.second > 'Z')) {
                        std::ostringstream oss;
                        oss << "Error: a non-Enigma-enabled value has been found in wiring while initializing a rotor. Object at " << this;
                        throw std::runtime_error(oss.str());
                    }
                }
                return wiring;
            };

        public:
            Rotor() = default;
            
            Rotor(string chiffre, int startPosition = 0, int notchPlacement = 0) 
                : intWiring(createInternalWiringMap(chiffre)), 
                position(((startPosition >= 0) && (startPosition <= 25)) ? startPosition : startPosition % 26), 
                notchPlacement(((notchPlacement >= 0) && (notchPlacement <= 25)) ? notchPlacement : notchPlacement % 26) 
            {};

            Rotor(map<char , char>& wiring, int startPosition = 0, int notchPlacement = 0) 
                : intWiring(checkInternalWiringMap(wiring)), 
                position(((startPosition >= 0) && (startPosition <= 25)) ? startPosition : startPosition % 26), 
                notchPlacement(((notchPlacement >= 0) && (notchPlacement <= 25)) ? notchPlacement : notchPlacement % 26) 
            {};

            void setPosition(int pos){
                if ((pos < 0) || (pos > 25)) {
                    throw runtime_error("Error: Position of a rotor must be an integer within [0 , 25]");
                }
                this->position = pos;
            };
            
            int getPosition() const {
                return this->position;
            };

            int getNotchPosition() const{
                return this->notchPlacement;
            };

            void setNotchPosition(int notchPos) {
                if ((notchPos < 0) || (notchPos > 25)) {
                    throw runtime_error("Error: Notch position of a rotor must be an integer within [0 , 25]");
                }
                this->notchPlacement = notchPos;
            };

            //the return value will determine if the rotor adjacent shall be rotated aswell
            /*TODO: 
            Actually on the second thought the rotation logic is a bit more complex than that.
            Must determine a newer method
            */
            void rotateForwards(){
                this->position++;
                if (this->position == 26) {
                    this->position = 0;
                }
            };

            map<char , char> getReverseWiring() const {
                map<char, char> reverseMap;
                for (int i = 0; i < 26; i++) {
                    reverseMap[intWiring.at(enigmaAllowedLetters[i])] = enigmaAllowedLetters[i];
                }
            };

            char run(char character) const{
                character = toupper(character);
                if (enigmaAllowedLetters.find(character) == string::npos) {
                    std::ostringstream oss;
                    oss << "Error: a non-Enigma-enabled value has been run through the rotor. Object at " << this;
                    throw std::runtime_error(oss.str());
                }
                return intWiring.at(character);
            };

            char reverseRun(char character) const {
                auto reverse = this->getReverseWiring();
                return reverse.at(character);
            };

            //Scrapped function ideas, might be useful later on but for now they just add unneccesary complexity
            // void rotateBackwards();
            // string run();   //Wouldn't hurt to have a string overload lateron
            // string reverseRun();

    };
    
    class Reflector {
        private:
            const Bipair<char> wiring;

            Bipair<char> createWiring(string chiffre) {
                Bipair<char> bipair;

                if (chiffre.size() != 26) {
                    throw runtime_error("Error: Size of chiffre does not match 26");
                }
                for (int i = 0; i < 26; i++) {
                    bipair.addPair(enigmaAllowedLetters[i] , chiffre.at(i));
                }
                return bipair;
            };
        public:
            Reflector() = default;

            Reflector(string chiffre) : wiring(createWiring(chiffre)) {};

            Reflector(Bipair<char> wiring) : wiring(wiring) {};

            const Bipair<char>& getWiring() const {
                return this->wiring;
            };

            char run(char character) const {
                return this->wiring.getCorrespondant(character);
            };

    };

    class Plugboard {
        private:
            Bipair<char> connections;
            int maximumConnections;
        public:
            Plugboard() = default;

            Plugboard(const Bipair<char>& plugboardWiring) : connections(plugboardWiring), maximumConnections(10) {};

            void addConnection(pair<char , char> pair) {

                if (connections.length() == maximumConnections) {
                    throw runtime_error("Error: Maximum number of connections reached.");
                }

                connections.addPair(pair.first, pair.second);
            };

            void removeConnection(pair<char , char> pair) {
                connections.removePair(pair.first, pair.second);
            }; 

            int getConnectionNumber() const {
                return connections.length();
            };

            int getMaximumConnections() const {
                return this->maximumConnections;
            };

            void setMaximumConenctions(int newMax) {
                this->maximumConnections = newMax;
            };
            const Bipair<char>& getConnections() const {
                return this->connections;
            };

            char run(char character) const {
                if (connections.checkElementExistence(character, character)) {
                    return connections.getCorrespondant(character);
                } else {
                    return character;
                }
            };
            // get reverse connection ?
    };

    class Enigma {
        private: 
        public:

            vector<Rotor> rotors;
            Reflector reflector;
            Plugboard plugboard;

            Enigma() = default;

            //Gets already created objects
            Enigma(vector<Rotor> rotors, Reflector reflector, Plugboard initialPlugboard) 
                : rotors(rotors), 
                reflector(reflector), 
                plugboard(initialPlugboard) 
            {};
            
            /*
            Gets the data to create the objects itself.
            Problem with these is that they don't determine any notch placement
            */
            Enigma(initializer_list<map<char, char>> rotors, Bipair<char> reflector, Bipair<char> initialPlugboard) 
                : reflector(reflector), 
                plugboard(plugboard) 
            {
                for (int i = 0 ; i < rotors.size() ; i++) {
                    this->rotors.emplace_back(*(rotors.begin()+i)); // Pointer maigic cause you know C++
                }
            };

            Enigma(initializer_list<string> rotorsChiffres, string reflectorChiffre, Bipair<char> initialPlugboard)
                : reflector(Reflector(reflectorChiffre)),
                plugboard(initialPlugboard)
            {
                for (int i = 0; i < rotorsChiffres.size() ; i++) {
                    (*this).rotors.emplace_back(*(rotorsChiffres.begin()+i));
                }
            };

            char encrypt(char character) {

            };

            string encrypt(string message);

            int getRotorCount() const {
                return this->rotors.size();
            };

            void setRotorPositions(vector<int> positions) {
                if (this->rotors.size() != positions.size()) {
                    throw runtime_error("Error: positions size does not match rotors size.");
                }

                for (int i = 0; i < this->rotors.size(); i++) {
                    this->rotors[i].setPosition(positions[i]);
                }

            }

            void setRotorPositions(initializer_list<int> positions){

                if (this->rotors.size() != positions.size()) {
                    throw runtime_error("Error: positions size does not match rotors size.");
                }

                for (int i = 0; i < this->rotors.size(); i++) {
                    this->rotors[i].setPosition(*(positions.begin()+i));
                }

            }
    };

}
