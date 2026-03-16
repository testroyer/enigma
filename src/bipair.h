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

        /**
         * @brief Checks whether a vector of pairs contains any duplicate elements.
         * @details An element is considered duplicate if it appears in more than one pair,
         *          either as the first or second member.
         * @param initialPairs The vector of pairs to validate.
         * @return true if a duplicate element is found, false otherwise.
         */
        bool initialHasDoublePairing(const std::vector<std::pair<T, T>>& initialPairs) {
            std::unordered_set<T> elements;
            for (const auto& p : initialPairs) {
                if (elements.insert(p.first).second == false || elements.insert(p.second).second == false) {
                    return true;
                }
            }
            return false;
        }; 

        /**
         * @brief Checks whether an initializer list of pairs contains any duplicate elements.
         * @details An element is considered duplicate if it appears in more than one pair,
         *          either as the first or second member.
         * @param initialPairs The initializer list of pairs to validate.
         * @return true if a duplicate element is found, false otherwise.
         */
        bool initialHasDoublePairing(const std::initializer_list<std::pair<T, T>>& initialPairs) {
            std::unordered_set<T> elements;
            for (const auto& p : initialPairs) {
                if (elements.count(p.first) || elements.count(p.second)) {
                    return true;
                }
                elements.insert(p.first);
                elements.insert(p.second);
            }
            return false;
        }; 

    public:
        /**
         * @brief Default constructor. Constructs an empty Bipair.
         */
        Bipair() = default;

        /**
         * @brief Constructs a Bipair from a vector of pairs.
         * @param initialPairs A vector of pairs to initialize the Bipair with.
         * @throws std::runtime_error if any element appears more than once across all pairs.
         */
        Bipair(const std::vector<std::pair<T, T>>& initialPairs) {
            if (initialHasDoublePairing(initialPairs)) {
                throw std::runtime_error("Initial pairs of Bipair construction contain duplicate elements.");
            }
            pairs = initialPairs;
        };

        /**
         * @brief Constructs a Bipair from an initializer list of pairs.
         * @param initialPairs An initializer list of pairs to initialize the Bipair with.
         * @throws std::runtime_error if any element appears more than once across all pairs.
         */
        Bipair(std::initializer_list<std::pair<T, T>> initialPairs) {
            std::vector<std::pair<T, T>> vectorPairs(initialPairs);
            if (initialHasDoublePairing(initialPairs)) {
                throw std::runtime_error("Initial pairs of Bipair construction contain duplicate elements.");
            }
            pairs = initialPairs;
        }

        /**
         * @brief Adds a new pair to the Bipair.
         * @details Neither element may already exist in any currently stored pair.
         * @param first The first element of the new pair.
         * @param second The second element of the new pair.
         * @throws std::runtime_error if either element already exists in a stored pair.
         */
        void addPair(const T& first, const T& second) {
            if (checkPairExistence(first, second)) { 
                throw std::runtime_error("Error: Attempting to add a pair that contains an element that already exists in another pair.");
            }
            pairs.emplace_back(first, second);
        };

        /**
         * @brief Removes a pair from the Bipair.
         * @details The pair is matched regardless of the order of its elements.
         *          If the pair does not exist, the call has no effect.
         * @param first The first element of the pair to remove.
         * @param second The second element of the pair to remove.
         */
        void removePair(const T& first, const T& second) {
            pairs.erase(std::remove_if(pairs.begin(), pairs.end(), [&first, &second](const auto& p) {
                return (p.first == first && p.second == second) || (p.first == second && p.second == first);
            }), pairs.end());
        };

        /**
         * @brief Returns a const reference to the underlying vector of pairs.
         * @return A const reference to the internal pairs vector.
         */
        const std::vector<std::pair<T,T>>& getPairs() const {
            return pairs;
        }

        /**
         * @brief Checks whether either of the given elements exists in any stored pair.
         * @details Returns true if at least one of the two elements is found anywhere
         *          in the collection, regardless of its position within a pair.
         * @param first The first element to search for.
         * @param second The second element to search for.
         * @return true if either element is present in any pair, false otherwise.
         */
        bool checkPairExistence(const T& first, const T& second) const {
            return pairs.end() != std::find_if(pairs.begin(), pairs.end(), [&first, &second](const auto& p){
                return (p.first == first || p.second == first) || (p.first == second || p.second == second);
            }); 
        };

        /**
         * @brief Checks whether an exact pair exists in the collection.
         * @details The match is order-insensitive: (a, b) is considered equal to (b, a).
         * @param param The pair to search for.
         * @return true if the exact pair exists, false otherwise.
         */
        bool checkExactPairExistence(const std::pair<T,T>& param) const {
            return pairs.end() != std::find_if(pairs.begin(), pairs.end(), [&param](const auto& p){
                return (p.first == param.first && p.second == param.second) || (p.first == param.second && p.second == param.first);
            }); 
        };

        /**
         * @brief Checks whether a single element exists in any stored pair.
         * @param element The element to search for.
         * @return true if the element is found in any pair, false otherwise.
         */
        bool checkElementExistence(const T& element) const {
            return pairs.end() != std::find_if(pairs.begin(), pairs.end(), [&element](const auto& p){
                return p.first == element || p.second == element;
            }); 
        };

        /**
         * @brief Returns the number of pairs currently stored.
         * @return The count of pairs as an int.
         */
        int size() const noexcept {
            return pairs.size();
        }; 

        /**
         * @brief Retrieves the paired counterpart of the given element.
         * @details Searches all pairs for an occurrence of @p query and returns
         *          the other element of that pair.
         * @param query The element whose counterpart is to be retrieved.
         * @return The element paired with @p query.
         * @throws std::runtime_error if @p query is not found in any pair.
         */
        T getCorrespondant(const T& query) const {
            auto result = std::find_if(pairs.begin(), pairs.end(), [&query](const auto& p){
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