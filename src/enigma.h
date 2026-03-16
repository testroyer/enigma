#ifndef ENIGMA_H
#define ENIGMA_H

#include <string>
#include "bipair.h"
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <map>

/**
 * @file enigma.h
 * @brief Enigma Machine simulation classes and utilities.
 */

namespace EnigmaMachine {
    
    /**
     * @brief List of Enigma-enabled Latin characters.
     */
    extern const std::string enigmaAllowedLetters;

    /**
     * @brief Deprecated: Maps Enigma letters to their indices.
     */
    extern const std::map<char, int> enigmaLetterToIndex;

    /**
     * @brief Checks if the character is an Enigma-enabled character.
     * @param character Character to check.
     * @return True if enabled, false otherwise.
     */
    bool checkIfEngimaEnabledChar(char character) noexcept;

    /**
     * @brief Throws if the character is not Enigma-enabled.
     * @param character Character to check.
     * @param objectAddress Address of the object for error context.
     * @throws std::invalid_argument if character is not enabled.
     */
    void checkAndThrowIfNotEnigmaEnabledChar(char character, const void* objectAddress);

    /**
     * @brief Normalizes a position to the Enigma alphabet range.
     * @param position Position to normalize.
     * @return Normalized position.
     */
    int normalisePosition(int position);

    /**
     * @class Rotor
     * @brief Simulates a rotor of an Enigma Machine.
     */
    class Rotor {
        private:
            std::map<char ,char> intWiring; ///< Internal "forwards" wiring.
            int position; ///< Current orientation of the rotor.
            int notchPlacement; ///< Position that rotates the next rotor.
            std::map<char, char> reverseWiring; ///< Internal "backwards" wiring.

            /**
             * @brief Creates internal wiring by mapping chiffre to enigmaAllowedLetters.
             * @param chiffre 26-char string representing wiring.
             * @return Wiring map.
             */
            std::map<char, char> createInternalWiringMap(std::string chiffre); 

            /**
             * @brief Checks if the wiring map is valid.
             * @param wiring Wiring map to check.
             * @return Checked wiring map.
             * @throws std::invalid_argument if invalid.
             */
            std::map<char , char> checkInternalWiringMap(const std::map<char , char> wiring) const;

        public:

            /**
             * @brief Default constructor. No wiring, position and notchPlacement set to 0.
             */
            Rotor() = default;

            /**
             * @brief Rotor constructor from chiffre.
             * @param chiffre 26-char string for wiring.
             * @param startPosition Initial position.
             * @param notchPlacement Notch position.
             */
            Rotor(std::string chiffre, int startPosition = 0, int notchPlacement = 0);        

            /**
             * @brief Rotor constructor from wiring map.
             * @param wiring Wiring map.
             * @param startPosition Initial position.
             * @param notchPlacement Notch position.
             */
            Rotor(const std::map<char , char>& wiring, int startPosition = 0, int notchPlacement = 0);

            /**
             * @brief Sets the rotor position.
             * @param pos New position.
             */
            void setPosition(int pos);

            /**
             * @brief Gets the rotor position.
             * @return Current position.
             */
            int getPosition() const noexcept;

            /**
             * @brief Gets the notch position.
             * @return Notch position.
             */
            int getNotchPosition() const noexcept;

            /**
             * @brief Sets the notch position.
             * @param notchPos New notch position.
             */
            void setNotchPosition(int notchPos);

            /**
             * @brief Increments rotor position by one.
             * @return True if notch passed, false otherwise.
             */
            bool rotate() noexcept;

            /**
             * @brief Returns the reverse wiring of the rotor.
             * @return Reverse wiring map.
             */
            std::map<char , char> getReverseWiring() const;

            /**
             * @brief Runs a letter through the rotor wiring.
             * @param character Input character.
             * @return Output character.
             */
            char run(char character) const;

            /**
             * @brief Runs a letter through the reverse rotor wiring.
             * @param character Input character.
             * @return Output character.
             */
            char reverseRun(char character) const;
    };
    
    /**
     * @class Reflector
     * @brief Simulates the reflector of an Enigma Machine.
     */
    class Reflector {
        private:
            const Bipair<char> wiring; ///< Bidirectional wiring.

            /**
             * @brief Creates wiring from chiffre.
             * @param chiffre 26-char string.
             * @return Bipair wiring.
             */
            Bipair<char> createWiring(std::string chiffre);

            /**
             * @brief Checks if the wiring bipair is valid.
             * @param wiring Bipair to check.
             * @return Checked bipair.
             * @throws std::invalid_argument if invalid.
             */
            Bipair<char> checkInternalWiringMap(Bipair<char> wiring) const;

        public:
            /**
             * @brief Default constructor. No wiring.
             */
            Reflector() = default;

            /**
             * @brief Reflector constructor from chiffre.
             * @param chiffre 26-char string.
             */
            Reflector(std::string chiffre);

            /**
             * @brief Reflector constructor from Bipair.
             * @param wiring Bipair wiring.
             */
            Reflector(Bipair<char> wiring);

            /**
             * @brief Gets the wiring.
             * @return Const reference to wiring.
             */
            const Bipair<char>& getWiring() const noexcept;

            /**
             * @brief Runs a character through the reflector.
             * @param character Input character.
             * @return Output character.
             */
            char run(char character) const;
    };

    /**
     * @class Plugboard
     * @brief Simulates the plugboard of an Enigma Machine.
     */
    class Plugboard {
        private:
            Bipair<char> connections; ///< Plugboard connections.
            int maximumConnections; ///< Maximum number of connections.

        public:
            /**
             * @brief Default constructor. No connections, max 10.
             */
            Plugboard() = default;

            /**
             * @brief Plugboard constructor from Bipair. Max connections set to 13.
             * @param plugboardWiring Bipair wiring.
             */
            Plugboard(const Bipair<char>& plugboardWiring);

            /**
             * @brief Plugboard constructor from Bipair and max connections.
             * @param plugboardWiring Bipair wiring.
             * @param maximumConnections Maximum allowed connections.
             */
            Plugboard(const Bipair<char>& plugboardWiring, int maximumConnections);

            /**
             * @brief Adds a connection if possible.
             * @param pair Pair of characters to connect.
             */
            void addConnection(std::pair<char , char> pair);

            /**
             * @brief Removes a connection if it exists.
             * @param pair Pair of characters to disconnect.
             */
            void removeConnection(std::pair<char , char> pair);
            
            /**
             * @brief Gets the number of present connections.
             * @return Number of connections.
             */
            int getConnectionNumber() const noexcept;

            /**
             * @brief Gets the maximum number of connections.
             * @return Maximum connections.
             */
            int getMaximumConnections() const noexcept;

            /**
             * @brief Sets the maximum number of connections.
             * @param newMax New maximum.
             */
            void setMaximumConnections(int newMax);
            
            /**
             * @brief Gets the current connections.
             * @return Const reference to connections.
             */
            const Bipair<char>& getConnections() const noexcept;

            /**
             * @brief Runs a letter through the plugboard.
             * @param character Input character.
             * @return Output character.
             */
            char run(char character) const;
    };

    /**
     * @class Enigma
     * @brief Combines rotors, reflector, and plugboard to simulate an Enigma machine.
     */
    class Enigma {
        private: 
        public:

            /**
             * @brief Rotors of the Enigma machine. First is rightmost, last is leftmost.
             */
            std::vector<Rotor> rotors;

            /**
             * @brief Reflector of the Enigma machine.
             */
            Reflector reflector;

            /**
             * @brief Plugboard of the Enigma machine.
             */
            Plugboard plugboard;

            /**
             * @brief Default constructor. No rotors, reflector, or plugboard.
             */
            Enigma() = default;

            /**
             * @brief Constructor from objects.
             * @param rotors Vector of rotors.
             * @param reflector Reflector object.
             * @param initialPlugboard Plugboard object.
             */
            Enigma(std::vector<Rotor> rotors, Reflector reflector, Plugboard initialPlugboard);
            
            /**
             * @brief Constructor from data to create objects.
             * @param rotors List of rotor wirings.
             * @param rotorPositions List of rotor positions.
             * @param notchPositions List of notch positions.
             * @param reflector Reflector wiring.
             * @param initialPlugboard Plugboard wiring.
             */
            Enigma(std::initializer_list<std::map<char, char>> rotors, std::initializer_list<int> rotorPositions, std::initializer_list<int> notchPositions, Bipair<char> reflector, Bipair<char> initialPlugboard);

            /**
             * @brief Constructor from chiffres and data.
             * @param rotorsChiffres List of rotor chiffres.
             * @param rotorPositions List of rotor positions.
             * @param notchPositions List of notch positions.
             * @param reflectorChiffre Reflector chiffre.
             * @param initialPlugboard Plugboard wiring.
             */
            Enigma(std::initializer_list<std::string> rotorsChiffres, std::initializer_list<int> rotorPositions, std::initializer_list<int> notchPositions, std::string reflectorChiffre, Bipair<char> initialPlugboard);

            /**
             * @brief Encrypts a single character.
             * @param character Input character.
             * @return Encrypted character.
             */
            char encrypt(char character);

            /**
             * @brief Determines rotor input mapping.
             * @param character Input character.
             * @param currentPos Current rotor position.
             * @param previousPos Previous rotor position.
             * @return Mapped character.
             */
            char determineRotorInput(char character, int currentPos, int previousPos) const;

            /**
             * @brief Encrypts a string message.
             * @param message Input message.
             * @return Encrypted message.
             */
            std::string encrypt(std::string message);

            /**
             * @brief Gets the number of rotors.
             * @return Rotor count.
             */
            int getRotorCount() const noexcept;

            /**
             * @brief Sets rotor positions from vector.
             * @param positions Vector of positions (rightmost first).
             */
            void setRotorPositions(std::vector<int> positions);

            /**
             * @brief Sets rotor positions from initializer list.
             * @param positions List of positions (rightmost first).
             */
            void setRotorPositions(std::initializer_list<int> positions);

            /**
             * @brief Gets current rotor positions (rightmost first).
             * @return Vector of positions.
             */
            std::vector<int> getRotorPositions() const;
    };

}

#endif