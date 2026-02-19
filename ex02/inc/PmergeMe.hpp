/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:53:25 by pol               #+#    #+#             */
/*   Updated: 2026/02/19 11:33:41 by pol              ###   ########.fr       */
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
	void displayResults(const std::vector<int> &before);

	// Custom exception for error handling
	class ErrorException : public std::exception
	{
		virtual const char *what() const throw() { return "Error"; }
	};
};

/**
 * FORD-JOHNSON ALGORITHM (Template Implementation)
 * This needs to be in the header or included by it because it's a template.
 */
template <typename T>
void PmergeMe::fordJohnsonSort(T &container)
{
	if (container.size() <= 1)
		return;

	// --- DEBUG: START ---
	// std::cout << "[DEBUG] Starting Ford-Johnson on " << container.size() << " elements." << std::endl;

	// 1. Straggler handling
	bool hasStraggler = (container.size() % 2 != 0);
	int straggler = 0;
	if (hasStraggler)
	{
		straggler = container.back();
		container.pop_back();
		// std::cout << "[DEBUG] Straggler identified: " << straggler << std::endl;
	}

	// 2. Pairs creation
	typedef std::pair<int, int> IPair;
	std::vector<IPair> pairs;
	for (size_t i = 0; i < container.size(); i += 2)
	{
		if (container[i] < container[i + 1])
			pairs.push_back(std::make_pair(container[i + 1], container[i]));
		else
			pairs.push_back(std::make_pair(container[i], container[i + 1]));
	}

	// std::cout << "[DEBUG] Pairs formed (Winner first): ";
	// for(size_t i=0; i<pairs.size(); ++i) std::cout << "(" << pairs[i].first << "," << pairs[i].second << ") ";
	// std::cout << std::endl;

	// 3. Sort winners
	for (size_t i = 0; i < pairs.size(); i++)
	{
		for (size_t j = i + 1; j < pairs.size(); j++)
		{
			if (pairs[i].first > pairs[j].first)
				std::swap(pairs[i], pairs[j]);
		}
	}

	// 4. Split into Main Chain and Pend
	T mainChain;
	T pend;
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		mainChain.push_back(pairs[i].first);
		pend.push_back(pairs[i].second);
	}
	// std::cout << "[DEBUG] Main Chain (winners sorted): ";
	// for(size_t i=0; i<mainChain.size(); ++i) std::cout << mainChain[i] << " ";
	// std::cout << "\n[DEBUG] Pend (losers): ";
	// for(size_t i=0; i<pend.size(); ++i) std::cout << pend[i] << " ";
	// std::cout << std::endl;

	// 5. Initial Insertion (pend[0])
	if (!pend.empty())
	{
		mainChain.insert(mainChain.begin(), pend[0]);
		// std::cout << "[DEBUG] First pend element " << pend[0] << " inserted at start." << std::endl;
	}

	// 6. Jacobsthal Insertion
	if (pend.size() > 1)
	{
		std::vector<int> insertionOrder = buildInsertionOrder(pend.size());

		// std::cout << "[DEBUG] Jacobsthal insertion order (indices): ";
		// for(size_t i=0; i<insertionOrder.size(); ++i) std::cout << insertionOrder[i] << " ";
		// std::cout << std::endl;

		for (size_t i = 0; i < insertionOrder.size(); ++i)
		{
			int idx = insertionOrder[i];
			if (idx <= 0 || idx >= (int)pend.size())
				continue;

			int val = pend[idx];
			typename T::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), val);
			mainChain.insert(it, val);
			// std::cout << "[DEBUG] Inserting pend[" << idx << "] (" << val << ") using binary search." << std::endl;
		}
	}

	// 7. Straggler
	if (hasStraggler)
	{
		typename T::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(it, straggler);
		// std::cout << "[DEBUG] Final straggler " << straggler << " inserted." << std::endl;
	}

	container = mainChain;
}

#endif