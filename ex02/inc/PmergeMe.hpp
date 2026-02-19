/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:53:25 by pol               #+#    #+#             */
/*   Updated: 2026/02/19 15:13:10 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <ctime>
#include <sys/time.h>
#include <iomanip>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

class PmergeMe
{
private:
	// Containers for the two required implementations
	std::vector<int> _vec;
	std::deque<int> _deq;

	// Time storage (in microseconds)
	double _vecTime;
	double _deqTime;

	// Internal tools for the Ford-Johnson algorithm
	std::vector<int> generateJacobsthal(int n);
	std::vector<int> buildInsertionOrder(int size);

public:
	// Canonical Form
	PmergeMe();
	PmergeMe(const PmergeMe &src);
	PmergeMe &operator=(const PmergeMe &src);
	~PmergeMe();

	// Main execution flow
	void execute(int ac, char **av);

	// Template function to handle both vector and deque with the same logic
	template <typename T>
	void fordJohnsonSort(T &container);

	// Parsing and validation
	void parseInput(int ac, char **av);

	// Custom exception for error handling
	class ErrorException : public std::exception
	{
		virtual const char *what() const throw() { return "Error"; }
	};
};

/**
 * FORD-JOHNSON ALGORITHM (Template Implementation)
 */
template <typename T>
void PmergeMe::fordJohnsonSort(T &container)
{
	// 1. Base case: if container has 0 or 1 element, it is already sorted
	if (container.size() <= 1)
		return;

	// 2. Straggler handling: if size is odd, remove the last element temporarily
	bool hasStraggler = (container.size() % 2 != 0);
	int straggler = 0;
	if (hasStraggler)
	{
		straggler = container.back();
		container.pop_back();
	}

	// 3. Pair creation: pair up elements and ensure the larger one is first
	// Typedef used to avoid C++98 ">>" nested template error
	typedef std::pair<int, int> IntPair;
	std::vector<IntPair> pairs;

	for (size_t i = 0; i < container.size(); i += 2)
	{
		if (container[i] < container[i + 1])
			pairs.push_back(std::make_pair(container[i + 1], container[i]));
		else
			pairs.push_back(std::make_pair(container[i], container[i + 1]));
	}

	// 4. Recursive Sort: Extract winners and sort them by calling the algorithm on itself
	T winners;
	for (size_t i = 0; i < pairs.size(); i++)
		winners.push_back(pairs[i].first);

	fordJohnsonSort(winners);

	// 5. Reconstruction: build Main Chain and Pend while maintaining winner-loser relationships
	T mainChain;
	T pend;

	for (typename T::iterator it = winners.begin(); it != winners.end(); ++it)
	{
		int winner = *it;
		mainChain.push_back(winner);

		for (size_t i = 0; i < pairs.size(); i++)
		{
			if (pairs[i].first == winner)
			{
				pend.push_back(pairs[i].second);
				pairs[i].first = -1; // Mark as used to handle duplicate values
				break;
			}
		}
	}

	// 6. Initial insertion: the first element of pend is always smaller or equal to the first winner
	if (!pend.empty())
		mainChain.insert(mainChain.begin(), pend[0]);

	// 7. Jacobsthal insertion: insert remaining pend elements using binary search in specific order
	if (pend.size() > 1)
	{
		std::vector<int> insertionOrder = buildInsertionOrder(pend.size());

		for (size_t i = 0; i < insertionOrder.size(); ++i)
		{
			int idx = insertionOrder[i];
			if (idx <= 0 || idx >= (int)pend.size())
				continue;

			int val = pend[idx];
			// Binary search to find the correct insertion position
			typename T::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), val);
			mainChain.insert(it, val);
		}
	}

	// 8. Final Straggler insertion: insert the odd element back if it exists
	if (hasStraggler)
	{
		typename T::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(it, straggler);
	}

	// Final result is assigned back to the original container
	container = mainChain;
}

#endif