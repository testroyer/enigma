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
            std::unordered_set<T> elements;
            for (const auto& p : initialPairs) {
                if (elements.insert(p.first).second == false || elements.insert(p.second).second == false) {
                    return true; // Found a duplicate element
                }

            }
            return false; // No duplicates found
        }; 

        bool initialHasDoublePairing(const std::initializer_list<std::pair<T, T>>& initialPairs) {
            std::unordered_set<T> elements;
            for (const auto& p : initialPairs) {
                if (elements.count(p.first) || elements.count(p.second)) {
                    return true; // Found a duplicate element
                }

                elements.insert(p.first);
                elements.insert(p.second);
            }
            return false; // No duplicates found
        }; 

    public:
        Bipair() = default;
        Bipair (const std::vector<std::pair<T, T>>& initialPairs) {
            if (initialHasDoublePairing(initialPairs)) {
                throw std::runtime_error("Initial pairs of Bipair construction contain duplicate elements.");
            }
            pairs = initialPairs;
        };
        Bipair (std::initializer_list<std::pair<T, T>> initialPairs) {
            
            std::vector<std::pair<T, T>> vectorPairs(initialPairs);
            if (initialHasDoublePairing(initialPairs)) {
                throw std::runtime_error("Initial pairs of Bipair construction contain duplicate elements.");
            }
            pairs = initialPairs;
        }
        
        void addPair(const T& first, const T& second) {
            if (checkPairExistence(first , second)) { 
                throw std::runtime_error("Error: Attempting to add a pair that contains an element that already exists in another pair.");
            }
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
        
        bool checkPairExistence(const T& first, const T& second) const { // checks if any is present
            return pairs.end() != std::find_if(pairs.begin() , pairs.end() , [&first , &second](const auto& p){
                return (p.first == first || p.second == first) || (p.first == second || p.second == second);
            }); 
        };
        
        bool checkExactPairExistence(const std::pair<T,T>& param) const {
            return pairs.end() != std::find_if(pairs.begin() , pairs.end() , [&param](const auto& p){
                return (p.first == param.first && p.second == param.second) || (p.first == param.second && p.second == param.first);
            }); 
        };

        bool checkElementExistence(const T& element) const {
            return pairs.end() != std::find_if(pairs.begin() , pairs.end() , [&element](const auto& p){
                return p.first == element || p.second == element;
            }); 
        };

        int size() const noexcept {
            return pairs.size();
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