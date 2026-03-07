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
    // Every class has two ctors, one that ctors with built objects and one that ctors with initializer lists. 
    // The value 26 is usually hard coded for enigmaAllowedLetters.size();
    //TODO: String ctors, chiffres and runs along with chars
    //TODO check int comparizons lengths value domains etc.
    //TODO: Check this-> 'es


    const string enigmaAllowedLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    const map<char, int> enigmaLetterToIndex = {{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23}, {'Y', 24}, {'Z', 25}};

    class Rotor {
        private:
            //[0,25] 
            // Actually it is the antirotation of the rotor. When it's one the Z is in A's place and so on. 
            int position; 
            
            // This notch placement variable is rather an ofset reference than a physical position on the rotor itself.
            // To simplify le démarche the left rotor will be rotated when the current rotor is rotated whilst the notch is at the top.
            // So in short rotate the left rotor when (currentIsBeingRotated && (this->notchPlacement==this->position))
            int notchPlacement; 

        protected:

            // Must use map here as two sides of the rotors are connected.
            map<char ,char> intWiring;
            
            //Chiffre means the order the the letters are cyrpted (For "ZASTQ..." , A->Z ,B->A and so on ) 
            map<char, char> createInternalWiringMap(string chiffre){

                if (chiffre.size() != 26) {
                   throw runtime_error("Error: Size of chiffre does not match 26"); //Could be more explicative maybe
                }

                for (int i = 0; i < 26 ; i++){
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
                
                if (wiring.size() != 26) {
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

            /*
            A rotation is the rotation of the rotor towards the user in the enigma machine.
            Althoug it is percieved that incrementing the position variable will rotate the rotor up, the reality is the quite contrary.
            */
            bool rotate(){
                this->position++;
                if (this->position == 26) {
                    this->position = 0;
                }
                return this->position == this->notchPlacement;
            };

            map<char , char> getReverseWiring() const {
                map<char, char> reverseMap;
                for (int i = 0; i < 26; i++) {
                    reverseMap[intWiring.at(enigmaAllowedLetters[i])] = enigmaAllowedLetters[i];
                }
                return reverseMap;
            };

            char run(char character) const{
                character = toupper(character);
                if (enigmaAllowedLetters.find(character) == string::npos) {
                    std::ostringstream oss;
                    oss << "Error: a non-Enigma-enabled value has been run through the rotor. Object at " << this;
                    throw std::runtime_error(oss.str());
                }
                return this->intWiring.at(character);
            };

            char reverseRun(char character) const {
                map<char, char> reverse = this->getReverseWiring();
                return reverse.at(character);
            };

            //Scrapped function ideas, might be useful later on but for now they just add unneccesary complexity
            // void rotateBackwards();
            // string run();   //Wouldn't hurt to have a string overload lateron
            // string reverseRun();

    };
    
    class Reflector { // I gather that inheritence would be an more efficent way to write this class but it is not worth the hassle
        private:
            const Bipair<char> wiring;

            Bipair<char> createWiring(string chiffre) {
                Bipair<char> bipair;

                if (chiffre.size() != 26) {
                    throw runtime_error("Error: Size of chiffre does not match 26");
                }
                for (int i = 0; i < 26 ; i++) {
                    bipair.addPair(enigmaAllowedLetters[i] , chiffre.at(i));
                }
                return bipair;
            };

            Bipair<char> checkInternalWiringMap(Bipair<char> wiring) const {
                
                if (wiring.size() != 26) {
                    std::ostringstream oss;
                    oss << "Error: wiring size does not match the Enigma-allowed characters list. Object at " << this;
                    throw std::runtime_error(oss.str());
                }

                for (const auto& pair : wiring.getPairs()) {
                    if ((pair.first < 'A' || pair.first > 'Z') || (pair.second < 'A' || pair.second > 'Z')) {
                        std::ostringstream oss;
                        oss << "Error: a non-Enigma-enabled value has been found in wiring while initializing a rotor. Object at " << this;
                        throw std::runtime_error(oss.str());
                    }
                }
                return wiring;
            };

        public:
            Reflector() = default;

            Reflector(string chiffre) : wiring(createWiring(chiffre)) {};

            Reflector(Bipair<char> wiring) : wiring(checkInternalWiringMap(wiring)) {};

            const Bipair<char>& getWiring() const {
                return this->wiring;
            };

            char run(char character) const {
                // This is unneeded as the reflector input comes frım an Enigma element.
                // if ((character < 'A') || (character > 'Z')) {
                //     std::ostringstream oss;
                //     oss << "Error: a non-Enigma-enabled value has been run through the reflector. Object at " << this;
                //     throw std::runtime_error(oss.str());
                // }
                
                return (this->wiring.checkElementExistence(character) ? this->wiring.getCorrespondant(character) : character);
            };

    };

    class Plugboard {
        private:
            Bipair<char> connections;
            int maximumConnections;
        public:
            Plugboard() = default;

            Plugboard(const Bipair<char>& plugboardWiring) :  maximumConnections(13) { // Set to 13 by default. (Hard Coded)
                if (plugboardWiring.size() > 13) {
                    throw runtime_error("Error: tried to construct a plugboard with more than phyically possible connections");
                }
                this->connections = plugboardWiring;
            };

            Plugboard(const Bipair<char>& plugboardWiring  , int maximumConnections) {
                if (maximumConnections > 13) {
                    throw runtime_error("Error: tried to set maximumConnections beyond the pysical limit.");
                }
                this->maximumConnections = maximumConnections;

                if (plugboardWiring.size() > 13) {
                    throw runtime_error("Error: tried to construct a plugboard with more than phyically possible connections");
                }
                this->connections = plugboardWiring;
            };

            // Here
            void addConnection(pair<char , char> pair) {

                if (connections.size() == maximumConnections) {
                    throw runtime_error("Error: Maximum number of connections reached.");
                }

                this->connections.addPair(pair.first, pair.second);
            };

            void removeConnection(pair<char , char> pair) {
                this->connections.removePair(pair.first, pair.second);
            }; 

            int getConnectionNumber() const {
                return this->connections.size();
            };

            int getMaximumConnections() const {
                return this->maximumConnections;
            };

            void setMaximumConenctions(int newMax) {
                if (newMax < 0) {
                    throw runtime_error("Error: number of maximum connections on the plugboard may not be set to a negative value");
                }
                this->maximumConnections = newMax;
            };

            const Bipair<char>& getConnections() const {
                return this->connections;
            };

            char run(char character) const {
                // Error here FAULTY CODE
                if (connections.checkPairExistence(character, character)) {
                    return connections.getCorrespondant(character);
                } else {
                    return character;
                }
            };
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

            /*
            Important formula: ===================================================================
            rotor_input = previous_rotor_exit - previous_rotor_position + current_rotor_position
            where these are calculated as integers mapped from A to Z
            Not sure if A should start from 0 or 1 yet in theory is shan't matter.
            =====================================================================================*/
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
