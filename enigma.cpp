#include <string>
#include "bipair.h"
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <map>
#include <set>
#include <cctype>
#include <sstream>
#include "enigma.h"
#include <iostream>

using namespace std;

namespace EnigmaMachine {
    // Every class has two ctors, one that ctors with built objects and one that ctors with initializer lists. 
    // The value 26 is usually hard coded for enigmaAllowedLetters.size();
    //TODO: String ctors, chiffres and runs along with chars
    //TODO: Iter increments
    //TODO: AIACC Fixes

    #pragma region Enigma-allowed characters and related functions
    
    // enigmaAllowedLetters(i) = enigmaLetterToIndex^-1(i)
    const std::string enigmaAllowedLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // character - 'A' Works best so depracated 
    const map<char, int> enigmaLetterToIndex = {{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23}, {'Y', 24}, {'Z', 25}};

    bool checkIfEngimaEnabledChar(char character) noexcept {
        return ((character >= 'A' && character <= 'Z')); // Removed isupper cause it is between the bounds.
    }

    void checkAndThrowIfNotEnigmaEnabledChar(char character, const void* objectAddress) {
            if (!checkIfEngimaEnabledChar(character)) { // Condition courtesy of Mr. GPT
                std::ostringstream oss;
                oss << "Error: a non-Enigma-enabled value has been found while initializing. Object at " << objectAddress;
                throw std::runtime_error(oss.str());
            }
    }

    int normalisePosition(int position) {
        return ((position % 26) + 26) % 26;
    };
    
    #pragma endregion

    #pragma region Rotor
            
        //Chiffre means the order the the letters are cyrpted (For "ZASTQ..." , A->Z ,B->A and so on ) 
        map<char, char> Rotor::createInternalWiringMap(std::string chiffre){
            map<char, char> intWiring;
            if (chiffre.size() != 26) {
                std::ostringstream oss;
                oss << "Error: Size of chiffre does not match 26 while initializing. Object at " << this;
                throw std::runtime_error(oss.str());
            }

            set<char> seenCharacters; // To check for duplicates
            for (int i = 0; i < 26 ; i++){
                char currentChiffre = toupper(chiffre[i]);
                checkAndThrowIfNotEnigmaEnabledChar(currentChiffre, this); 

                if (!seenCharacters.insert(currentChiffre).second) { // If the character was already seen, it's a duplicate
                    std::ostringstream oss;
                    oss << "Error: duplicate character in chiffre while initializing. Object at " << this;
                    throw std::runtime_error(oss.str());
                }
                intWiring[enigmaAllowedLetters[i]] = currentChiffre;
            }
            return intWiring; //Return value just-in-case
        }; 

        map<char , char> Rotor::checkInternalWiringMap(const std::map<char , char> wiring) const {
            
            if (wiring.size() != 26) {
                std::ostringstream oss;
                oss << "Error: wiring size does not match the Enigma-allowed characters list. Object at " << this;
                throw std::runtime_error(oss.str());
            }

            for (const auto& pair : wiring) {
                checkAndThrowIfNotEnigmaEnabledChar(pair.first, this);
                checkAndThrowIfNotEnigmaEnabledChar(pair.second, this);
            }
            return wiring;
        };

        Rotor::Rotor(std::string chiffre, int startPosition, int notchPlacement) 
            : intWiring(createInternalWiringMap(chiffre)), 
            position(((startPosition >= 0) && (startPosition <= 25)) ? startPosition : startPosition % 26), 
            notchPlacement(((notchPlacement >= 0) && (notchPlacement <= 25)) ? notchPlacement : notchPlacement % 26) 
        {
            this->reverseWiring = this->getReverseWiring();
        };

        Rotor::Rotor(const map<char , char>& wiring, int startPosition, int notchPlacement) 
            : intWiring(checkInternalWiringMap(wiring)), 
            position(((startPosition >= 0) && (startPosition <= 25)) ? startPosition : startPosition % 26), 
            notchPlacement(((notchPlacement >= 0) && (notchPlacement <= 25)) ? notchPlacement : notchPlacement % 26) 
        {
            this->reverseWiring = this->getReverseWiring();
        };

        void Rotor::setPosition(int pos){
            if ((pos < 0) || (pos > 25)) {
                throw runtime_error("Error: Position of a rotor must be an integer within [0 , 25]");
            }
            this->position = pos;
        };
        
        int Rotor::getPosition() const noexcept {
            return this->position;
        };

        int Rotor::getNotchPosition() const noexcept {
            return this->notchPlacement;
        };

        void Rotor::setNotchPosition(int notchPos) {
            if ((notchPos < 0) || (notchPos > 25)) {
                throw runtime_error("Error: Notch position of a rotor must be an integer within [0 , 25]");
            }
            this->notchPlacement = notchPos;
        };

        /*
        A rotation is the rotation of the rotor towards the user in the enigma machine.
        Although it is percieved that incrementing the position variable will rotate the rotor up, the reality is the quite contrary.
        */
        bool Rotor::rotate() noexcept{
            int startPosition = this->position;
            this->position++;
            if (this->position == 26) {
                this->position = 0;
            }
            return startPosition == this->notchPlacement;
        };

        map<char , char> Rotor::getReverseWiring() const {
            //No need to check here as it is from the object itself.
            map<char, char> reverseMap;
            for (int i = 0; i < 26; i++) {
                reverseMap[this->intWiring.at(enigmaAllowedLetters[i])] = enigmaAllowedLetters[i];
            }
            return reverseMap;
        };

        char Rotor::run(char character) const{
            character = toupper(character);
            checkAndThrowIfNotEnigmaEnabledChar(character, this);
            return this->intWiring.at(character);
        };

        char Rotor::reverseRun(char character) const {
            character = toupper(character);
            checkAndThrowIfNotEnigmaEnabledChar(character, this);
            return this->reverseWiring.at(character);
        };

        //Scrapped function ideas, might be useful later on but for now they just add unneccesary complexity
        // void rotateBackwards();
        // string run();   //Wouldn't hurt to have a string overload lateron
        // string reverseRun();

    #pragma endregion 

    #pragma region Reflector 
        // I gather that inheritence would be an more efficent way to write this class but it is not worth the hassle

        Bipair<char> Reflector::createWiring(std::string chiffre) {
            Bipair<char> bipair;
            if (chiffre.size() != 26) {
                throw runtime_error("Error: Size of chiffre does not match 26");
            }
            for (int i = 0; i < 26 ; i++) {
                char currentChiffre = toupper(chiffre[i]);
                checkAndThrowIfNotEnigmaEnabledChar(currentChiffre, this);
                if (!bipair.checkElementExistence(enigmaAllowedLetters[i])) {
                    bipair.addPair(enigmaAllowedLetters[i], currentChiffre);
                }
            }
            return bipair;
        }

        Bipair<char> Reflector::checkInternalWiringMap(Bipair<char> wiring) const {
            
            if (wiring.size() != 26) {
                std::ostringstream oss;
                oss << "Error: wiring size does not match the Enigma-allowed characters list. Object at " << this;
                throw std::runtime_error(oss.str());
            }

            for (const auto& pair : wiring.getPairs()) {
                checkAndThrowIfNotEnigmaEnabledChar(pair.first, this);
                checkAndThrowIfNotEnigmaEnabledChar(pair.second, this);
            }
            return wiring;
        };


        Reflector::Reflector(std::string chiffre) : wiring(createWiring(chiffre)) {};

        Reflector::Reflector(Bipair<char> wiring) : wiring(checkInternalWiringMap(wiring)) {};

        const Bipair<char>& Reflector::getWiring() const noexcept {
            return this->wiring;
        };

        char Reflector::run(char character) const {
            character = toupper(character);
            checkAndThrowIfNotEnigmaEnabledChar(character, this);   
            if (!this->wiring.checkElementExistence(character))  {
                throw runtime_error("Error: character not found in reflector wiring, most likely due to false initialization");
            }
            return this->wiring.getCorrespondant(character);
        };

    #pragma endregion  

    #pragma region Plugboard

        Plugboard::Plugboard(const Bipair<char>& plugboardWiring) :  maximumConnections(13) { // Set to 13 by default. (Hard Coded)
            if (plugboardWiring.size() > 13) {
                throw runtime_error("Error: tried to intialize a plugboard with more than phyically possible connections");
            }
            this->connections = plugboardWiring;
        };

        Plugboard::Plugboard(const Bipair<char>& plugboardWiring  , int maximumConnections) {
            if (maximumConnections < 0) {
                throw runtime_error("Error: number of maximum connections on the plugboard may not be set to a negative value");
            }
            if (maximumConnections > 13) {
                throw runtime_error("Error: number of maximum connections on the plugboard may not be set beyond the physical limit.");
            }
            this->maximumConnections = maximumConnections;

            if (plugboardWiring.size() > 13 || plugboardWiring.size() > maximumConnections) {
                throw runtime_error("Error: tried to initialize a plugboard with more than phyically possible connections");
            }
            this->connections = plugboardWiring;
        };

        void Plugboard::addConnection(pair<char , char> pair) {
            char first = toupper(pair.first);
            char second = toupper(pair.second);
            checkAndThrowIfNotEnigmaEnabledChar(first, this);
            checkAndThrowIfNotEnigmaEnabledChar(second, this);

            if (this->connections.size() == maximumConnections) {
                throw runtime_error("Error: Maximum number of connections reached.");
            }

            this->connections.addPair(first, second);
        };

        void Plugboard::removeConnection(pair<char , char> pair) {
            //This bit is a bit boilerplate-ish but making it a function is not worth the hassle.
            char first = toupper(pair.first);
            char second = toupper(pair.second);
            checkAndThrowIfNotEnigmaEnabledChar(first, this);
            checkAndThrowIfNotEnigmaEnabledChar(second, this);
            this->connections.removePair(first, second);
        }; 

        int Plugboard::getConnectionNumber() const noexcept {
            return this->connections.size();
        };

        int Plugboard::getMaximumConnections() const noexcept {
            return this->maximumConnections;
        };

        void Plugboard::setMaximumConnections(int newMax) {
            if (newMax < 0) {
                throw runtime_error("Error: number of maximum connections on the plugboard may not be set to a negative value");
            }
            if (newMax > 13) {
                throw runtime_error("Error: number of maximum connections on the plugboard may not be set beyond the physical limit.");
            }
            this->maximumConnections = newMax;
        };

        const Bipair<char>& Plugboard::getConnections() const noexcept {
            return this->connections;
        };

        char Plugboard::run(char character) const {
            character = toupper(character); 
            checkAndThrowIfNotEnigmaEnabledChar(character, this);
            if (this->connections.checkElementExistence(character)) {
                return this->connections.getCorrespondant(character);
            } else {
                return character;
            }
        };
    #pragma endregion

    #pragma region Enigma


        //Gets already created objects
        Enigma::Enigma(vector<Rotor> rotors, Reflector reflector, Plugboard initialPlugboard) 
            :reflector(reflector), 
            plugboard(initialPlugboard) 
        {
            if (rotors.size() == 0) {
                std::ostringstream oss;
                oss << "Error: An enigma machine must have at least one rotor. Object at:" << this;
                throw std::runtime_error(oss.str());
            }
            this->rotors = rotors;
        };
        
        /*
        Gets the data to create the objects itself.
        Problem with these is that they don't determine any notch placement
        */
        Enigma::Enigma(initializer_list<map<char, char>> rotors, initializer_list<int> rotorPositions, initializer_list<int> notchPositions, Bipair<char> reflector, Bipair<char> initialPlugboard) 
            : reflector(reflector), 
            plugboard(initialPlugboard) 
        {
            if ((rotors.size() != rotorPositions.size() || rotors.size() != notchPositions.size()) || (rotors.size() == 0 || rotorPositions.size() == 0 || notchPositions.size() == 0)) {
                std::ostringstream oss;
                oss << "Error: Size of rotors, rotorPositions and notchPositions must be the same and mustn't be empty. Object at" << this;
                throw std::runtime_error(oss.str());
            }
            for (int i = 0; i < rotors.size() ; i++) {
                (*this).rotors.emplace_back(*(rotors.begin()+i) , *(rotorPositions.begin()+i) , *(notchPositions.begin()+i)); // Pointer magic cause you know C++ TODO: Might be a bit unsafe. do an iter for each maybe?
            }
        };

        Enigma::Enigma(initializer_list<string> rotorsChiffres, initializer_list<int> rotorPositions, initializer_list<int> notchPositions, string reflectorChiffre, Bipair<char> initialPlugboard)
            : reflector(Reflector(reflectorChiffre)),
            plugboard(initialPlugboard)
        {
            if ((rotorsChiffres.size() != rotorPositions.size() || rotorsChiffres.size() != notchPositions.size()) || (rotorsChiffres.size() == 0 || rotorPositions.size() == 0 || notchPositions.size() == 0)) {
                std::ostringstream oss;
                oss << "Error: Size of rotorsChiffres, rotorPositions and notchPositions must be the same and mustn't be empty. Object at" << this;
                throw std::runtime_error(oss.str());
            }
            for (int i = 0; i < rotorsChiffres.size() ; i++) {
                (*this).rotors.emplace_back(*(rotorsChiffres.begin()+i) , *(rotorPositions.begin()+i) , *(notchPositions.begin()+i)); // Pointer magic cause you know C++
            }
        };

        /*
        Important formula: ===================================================================
        rotor_input = previous_rotor_exit - previous_rotor_position + current_rotor_position
                        char(cast to int)   - int                     + int
        where these are calculated as integers mapped from A to Z
        Not sure if A should start from 0 or 1 yet in theory is shan't matter.
        =====================================================================================*/
        char Enigma::encrypt(char character) {
            if (character == ' ') {
                return character;
            }

            /*
            Signal flow:
            Input -> Plugboard -> Rotors[] -> Reflector -> Rotors[] (reverse) -> Plugboard -> Output 
            */
            char currentCharacterState = toupper(character); // Uppercase cast as needed
            checkAndThrowIfNotEnigmaEnabledChar(currentCharacterState, this);

            // Plugboard first run
            currentCharacterState = this->plugboard.run(currentCharacterState); 

            //TODO: Sperate the rotor assembly to a different function

            // Rotors first run
            for (auto rotor = this->rotors.begin() ; rotor != this->rotors.end() ; rotor++) {
                currentCharacterState = rotor->run(this->determineRotorInput(
                    currentCharacterState, 
                    rotor->getPosition(), 
                    (rotor != this->rotors.begin() ? (*(rotor-1)).getPosition() : 0)
                ));
            }
                
            // Reflector run
            currentCharacterState = this->reflector.run(this->determineRotorInput(currentCharacterState, 0, this->rotors.back().getPosition()));

            // Rotors (reverse) run
            for (auto rotor = this->rotors.rbegin(); rotor != this->rotors.rend(); ++rotor) {
                int prevPos = (rotor != this->rotors.rbegin()) ? (*(rotor-1)).getPosition() : 0;
                int nexPos = rotor->getPosition();
                
                currentCharacterState = rotor->reverseRun(
                    this->determineRotorInput(currentCharacterState, nexPos, prevPos)
                );
            }
            
            // Reset the rotor offset before going into the plugboard
            currentCharacterState = this->determineRotorInput(currentCharacterState , 0 , rotors[0].getPosition());

            // Last plugbaord run
            currentCharacterState = this->plugboard.run(currentCharacterState);

            // Rotate logic
            for (auto rotor = rotors.begin() ; rotor != rotors.end() ; ++rotor) {
                bool rotateNext = rotor->rotate();
                if (!rotateNext) { // If the current rotor doesn't cause the next one to rotate or if it is the last rotor,
                    break;
                }
            }

            return currentCharacterState;

        };

        //Caution while calling this as the paramater and formula placement are mixed
        char Enigma::determineRotorInput(char character, int nextPosition, int previousPos) const {
            return enigmaAllowedLetters[(
                normalisePosition((normalisePosition((character - 'A') - previousPos)) + nextPosition)
                )];

        }

        string Enigma::encrypt(string message) {
            string result;
            result.reserve(message.size()); // Reservation of memory. Suggested by AIACC
            for (char& character : message) {
                result += this->encrypt(character);
            }
            return result;
        };

        int Enigma::getRotorCount() const noexcept{
            return this->rotors.size();
        };

        void Enigma::setRotorPositions(vector<int> positions) {
            if (this->rotors.size() != positions.size()) {
                throw runtime_error("Error: positions size does not match rotors size.");
            }

            for (int i = 0; i < this->rotors.size(); i++) {
                if (positions[i] < 0 || positions[i] > 25) {
                    throw runtime_error("Error: rotor positions must be integers within [0 , 25].");
                }
                this->rotors[i].setPosition(positions[i]);
            }

        }

        void Enigma::setRotorPositions(initializer_list<int> positions){

            if (this->rotors.size() != positions.size()) {
                throw runtime_error("Error: positions size does not match rotors size.");
            }

            for (int i = 0; i < this->rotors.size(); i++) {
                int currentPosition = *(positions.begin()+i);
                if (currentPosition < 0 || currentPosition > 25) {
                    throw runtime_error("Error: rotor positions must be integers within [0 , 25].");
                }
                this->rotors[i].setPosition(currentPosition);
            }

        }

        vector<int> Enigma::getRotorPositions() const {
            vector<int> positions;
            for (const auto& rotor : this->rotors) {
                positions.push_back(rotor.getPosition());
            }
            return positions;
        }

    #pragma endregion
}
