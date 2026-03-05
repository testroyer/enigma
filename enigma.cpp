#include <string>
#include "bipair.h"
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <map>

using namespace std;

namespace Enigma {
    //Every class has two ctors, one that ctors with built objects and one that ctors with initializer lists. 
    const char enigmaAllowedLetters[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    class Rotor {
        private:
            int position; //[0,25]
            int notchPlacement; 

        protected:

            // Must use map here as two sides of the rotors are connected.
            map<char ,char> intWiring;
            
            //Chiffre means the order the the letters are cyrpted (For "ZASTQ..." , A->Z ,B->A and so on ) 
            map<char, char> createInternalWiringMap(string chiffre){
                if (chiffre.size() != 26) {
                    runtime_error("Error: Size of chiffre does not match 26"); //Could be more explicative maybe
                }

                //I am here
            }; 

        public:
            Rotor(string chiffre, int startPosition = 0, int notchPlacement = 0);        
            Rotor(map<char , char>& wiring, int startPosition = 0, int notchPlacement = 0);
            void setPosition(int pos);
            int getPosition() const;
            int getNotchPosition() const;
            bool rotateForwards(); //the return value will determine if the rotor adjacent shall be rotated aswell
            const map<char , char> getReverseWiring() const;
            char run(char character) const;
            char reverseRun(char character) const;

            // void rotateBackwards();
            // string run();   //Wouldn't hurt to have a string overload lateron
            // string reverseRun();

    };
    
    class Reflector {
        private:
            const Bipair<char> wiring;
        public:
            Reflector(string chiffre);
            Reflector(Bipair<char> wiring);
            const Bipair<char>& getWiring() const;
            char run(char character) const;
    };

    class Plugboard {
        private:
            Bipair<char> connections;

        public:
            Plugboard(const Bipair<char>& plugboardWiring);
            void addConnection(pair<char , char> pair);
            void removeConnection(pair<char , char> pair);
            int getConnectionNumber() const;
            const Bipair<char>& getConnections() const;
            char run(char character) const;
            // get reverse connection ?
    };

    class Enigma {
        private: 
        public:
            //Gets already created objects
            Enigma(vector<Rotor> rotors, Reflector reflector, Plugboard initialPlugboard);
            
            /*
            Gets the data to create the objects itself.
            Problem with these is that they don't determine any notch placement
            */
            Enigma(initializer_list<map<char, char>> rotors, Bipair<char> reflector, Bipair<char> initialPlugboard);
            Enigma(initializer_list<string> rotorsChiffres, string reflectorChiffre, Bipair<char> initialPlugboard);


            vector<Rotor> rotors;
            Reflector reflector;
            Plugboard plugboard;

            string encrypt(string message);
            char encrypt(char character);
            int getRotorCount() const;
            void setRotorPositions(vector<int> positions);
            void setRotorPositions(initializer_list<int> positions);
    };

}