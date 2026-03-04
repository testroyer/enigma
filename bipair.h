#ifndef BIPAIR_H
#define BIPAIR_H

#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_set>
#include <stdexcept>

template <typename T>
class Bipair {
    private:
        std::vector<std::pair<T, T>> pairs;  

        bool initialHasDoublePairing(const std::vector<std::pair<T, T>>& initialPairs) {
            //TODO: Check if the pair is of same data type.
            std::unordered_set<T> elements;
            for (const auto& p : initialPairs) {
                if (elements.insert(p.first).second == false || elements.insert(p.second).second == false) {
                    return true; // Found a duplicate element
                }

            }
            return false; // No duplicates found
        }; 

    public:
        Bipair() = default;
        Bipair (const std::vector<std::pair<T, T>>& initialPairs) {
            if (initialHasDoublePairing(initialPairs)) {
                throw std::runtime_error("Initial pairs contain duplicate elements.");
            }
            pairs = initialPairs;
        };
        
        void addPair(const T& first, const T& second) {
            if (checkElementExistence(first , second)) { return; }
            pairs.emplace_back(first, second);
        };

        void removePair(const T& first, const T& second) {
            pairs.erase(std::remove_if(pairs.begin(), pairs.end(), [&first, &second](const auto& p) {
                return (p.first == first && p.second == second) || (p.first == second && p.second == first);
            }), pairs.end());
        };

        const std::vector<std::pair<T,T>>& getPairs() const {
            return pairs;
        }
        
        bool checkElementExistence(const T& first, const T& second) const {
            return pairs.end() != std::find_if(pairs.begin() , pairs.end() , [&first , &second](const auto& p){
                return (p.first == first || p.second == first) || (p.first == second || p.second == second);
            }); 
        };

        T getCorrespondant (const T& query) const {
            auto result = std::find_if(pairs.begin() , pairs.end() , [&query](const auto& p){
                return p.first == query || p.second == query;
            }); 

            if (result != pairs.end()) {
                if (result->first == query) {
                    return result->second;
                } else {
                    return result->first;
                }
            } else {
                throw std::runtime_error("Query not found in any pair.");
            }
        }
};


#endif