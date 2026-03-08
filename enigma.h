#ifndef ENIGMA_H
#define ENIGMA_H

#include <string>
#include "bipair.h"
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <map>

/// @todo: doxygen comments

/*
Note: Enigma Machine works only by capital letters so lowercase letters are transformed into uppercase implicitly
*/
namespace EnigmaMachine {
    
    /** 
    * char[27] enigmaAllowedLetters: List of enigma enabled Latin characters
    * 
    */
    const std::string enigmaAllowedLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    const std::map<char, int> enigmaLetterToIndex = {{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23}, {'Y', 24}, {'Z', 25}};

    //Check if the character is an enigma-enabled character
    bool checkIfEngimaEnabledChar(char character) noexcept;

    void checkAndThrowIfNotEnigmaEnabledChar(char character, const void* objectAddress);

    /** 
    *Class simulating the rotorrotorss of an Enigma Machine.
    */
    class Rotor {
        private:
            // Determines the current oriantation of the rotor.
            int position; 

            //The position that rotates the next rotor with itself.
            int notchPlacement; 

        protected:
            // Internal map "forwards" wiring of the rotor.
            std::map<char ,char> intWiring;

            /*
            Creates internal wiring by relating the chiffre to the enigmaAllowedLetters in order.
            Chiffre is a 26 char string. One by one it is maped to the enigma allowed lettres. ("A" -> [0] , "B" -> [1] and so o and so on).
            */
            std::map<char, char> createInternalWiringMap(std::string chiffre); 

            /*
            Checks if the wiring map is valid. It must contain 26 entries and all keys and values must be in the enigmaAllowedLetters list.
            */
            std::map<char , char> checkInternalWiringMap(const std::map<char , char> wiring) const;

        public:

            //Default constructor. Creates a rotor with no internal wiring and position and notchPlacement set to 0.
            Rotor();

            /*
            Rotor constructor.
            Chiffre is a 26 char string. One by one it is maped to the enigma allowed lettres. ("A" -> [0] , "B" -> [1] and so o and so on).
            */
            Rotor(std::string chiffre, int startPosition = 0, int notchPlacement = 0);        

            /*
            Rotor constructor by map object.
            */
            Rotor(std::map<char , char>& wiring, int startPosition = 0, int notchPlacement = 0);
        
            // Sets int position porperty.
            void setPosition(int pos);

            // Gets int position porperty.
            int getPosition() const noexcept;

            // Gets int notchPosition porperty.
            int getNotchPosition() const noexcept;

            // Sets int notchPosition porperty.
            void setNotchPosition();

            /*
            Increments rotor position by one.
            If the return value is true, that means the notch has passed and the left adjacent motor must be rotated.
            */
            bool rotate() noexcept;

            //Retuns the reverse wiring of the motor (key and value of the intWiring reversed).
            const std::map<char , char> getReverseWiring() const;

            //Runs a letter through the internal wiring and returns the result as a char.
            char run(char character) const;

            //Runs a letter through the internal wiring and returns the result as a char.
            char reverseRun(char character) const;
    };
    
    /*
    Class simulating the Reflector of an Enigma Machine.
    */
    class Reflector {
        private:
            // Bidirectional map that contains connected letter couples.
            const Bipair<char> wiring;
            
            // Creates the bipair object for the wiring property
            Bipair<char> createWiring(std::string chiffre);

        public:
           
            // Default constructor. Creates a reflector with no internal wiring.
            Reflector();

            /*
            Reflector constructor by chiffre.
            Chiffre is a 26 char string. One by one it is maped to the enigma allowed lettres. ("A" -> [0] , "B" -> [1] and so o and so on).
            */
            Reflector(std::string chiffre);

            /*
            Reflector constructor by a Bipair object.
            */
            Reflector(Bipair<char> wiring);

            // Returns const Bipair<char> wiring object.
            const Bipair<char>& getWiring() const noexcept;

            //Runs a char character through the reflector and returns the result as a char.
            char run(char character) const;

    };

    /*
    Class simulating the Plugboard of an Enigma Machine.
    */
    class Plugboard {
        private:
            // Bidirectional map that contains connected letter couples.
            Bipair<char> connections;

            // The maximum number of connections. By default this will be set to 10.
            int maximumConnections;

        public:
            
            // Default constructor. Creates a plugboard with no connections and maximumConnections set to 10.
            Plugboard();

            /*
            Plugboard constructor by a Bipair object.
            Auto-assumes the maximum connections as 13 (max physically possible.)
            */
            Plugboard(const Bipair<char>& plugboardWiring);

            /*
            Plugboard constructor by a Bipair object and int maximumConnections.
            */
            Plugboard(const Bipair<char>& plugboardWiring  , int maximumConnections);

            // Adds a connection to the plugboard if the maximum connection number won't be passed and both plugs are free.
            void addConnection(std::pair<char , char> pair);

            //Removes a connection if that exact connection exists.
            void removeConnection(std::pair<char , char> pair);
            
            //Gets the number of present connections.
            int getConnectionNumber() const noexcept;

            //Gets the current number for maximum amount of connections.
            int getMaximumConnections() const noexcept;

            //Sets the current number for maximum amount of connections.
            void setMaximumConenctions(int newMax);
            
            //Returns the Bipair<char> connections property.
            const Bipair<char>& getConnections() const noexcept;

            //Runs a letter through the plugboard and returns the result as a char.
            char run(char character) const;
    };

    /*
    The class that brings together the rotors, the reflector and the plugboard of an enigma machine.
    */
    class Enigma {
        private: 
        public:

            // The rotors of the enigma machine. The first element is the rightmost rotor, the last element is the leftmost rotor.
            std::vector<Rotor> rotors;

            // The reflector of the enigma machine.
            Reflector reflector;

            // The plugboard of the enigma machine.
            Plugboard plugboard;

            // Default constructor. Creates an enigma machine with no rotors, no reflector and no plugboard connections.
            Enigma() = default;

            //Enigma constructor by already created objects
            Enigma(std::vector<Rotor> rotors, Reflector reflector, Plugboard initialPlugboard);
            
            /*
            Gets the data to create the objects itself.
            This constructor doesn't specify a notch position for the rotors. You must set it yourself.
            */
            Enigma(std::initializer_list<std::map<char, char>> rotors, Bipair<char> reflector, Bipair<char> initialPlugboard);

            /*
            Gets the data to create the objects itself.
            This constructor doesn't specify a notch position for the rotors. You must set it yourself.
            Chiffre is a 26 char string. One by one it is maped to the enigma allowed lettres. ("A" -> [0] , "B" -> [1] and so o and so on).
            */
            Enigma(std::initializer_list<std::string> rotorsChiffres, std::string reflectorChiffre, Bipair<char> initialPlugboard);

            // Encryption of a single char character. The result is returned as a char.
            char encrypt(char character);


            // Formula to determine which rotor output correlates to which rotor input.
            char determineRotorInput(char character, Rotor& current, Rotor& previous) const;

            /*
            Encryption of a string message. The message is encrypted char by char and the result is returned as a string.
            */
            std::string encrypt(std::string message);

            // Returns the size of the rotors vector.
            int getRotorCount() const noexcept;

            /*
            Set the orientation of the rotors by a vector of integers starting from the rightmost rotor.
            Size of the vector must be equal to the number of rotors. 
            The integers must be between 0 and 25
            */
            void setRotorPositions(std::vector<int> positions);

            /*
            Set the orientation of the rotors by a list of integers starting from the rightmost rotor.
            Size of the list must be equal to the number of rotors. 
            The integers must be between 0 and 25
            Can be initialized with curly braces.
            */
            void setRotorPositions(std::initializer_list<int> positions);
    };

}

#endif