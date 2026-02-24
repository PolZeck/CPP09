/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:53:25 by pol               #+#    #+#             */
/*   Updated: 2026/02/24 08:58:01 by pol              ###   ########.fr       */
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
// #include <typeinfo> //debug

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
	// Fast index-based access and contiguous memory, ideal for the primary sorting steps.
	std::vector<int> _vec;

	// Fast insertions at the front and back, used to demonstrate algorithm genericity via templates.
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
	if (container.size() <= 1)
		return;

	// Bool created for vector
	// bool isVector = (typeid(container) == typeid(std::vector<int>)); //[DEBUG]

	// if (isVector) //[DEBUG]
	// std::cout << BOLD << "[DEBUG] Starting Ford-Johnson on " << container.size() << " elements." << RESET << std::endl;

	// 1. Straggler handling
	bool hasStraggler = (container.size() % 2 != 0);
	int straggler = 0;
	if (hasStraggler)
	{
		straggler = container.back();
		container.pop_back();
		// if (isVector) //[DEBUG]
		// std::cout << MAGENTA << "[DEBUG] Straggler identified: " << straggler << RESET << std::endl;
	}

	// 2. Pair creation
	typedef std::pair<int, int> IntPair;
	std::vector<IntPair> pairs;
	for (size_t i = 0; i < container.size(); i += 2)
	{
		if (container[i] < container[i + 1])
			pairs.push_back(std::make_pair(container[i + 1], container[i]));
		else
			pairs.push_back(std::make_pair(container[i], container[i + 1]));
	}

	// if (isVector) //[DEBUG]
	//{			  //[DEBUG]
	// std::cout << CYAN << "[DEBUG] Pairs formed (Winner first): " << RESET;
	// for (size_t i = 0; i < pairs.size(); ++i)								  //[DEBUG]
	// std::cout << "(" << pairs[i].first << "," << pairs[i].second << ") "; //[DEBUG]
	// std::cout << std::endl;													  //[DEBUG]
	//} //[DEBUG]

	// 3. Recursive Sort
	T winners;
	for (size_t i = 0; i < pairs.size(); i++)
		winners.push_back(pairs[i].first);

	fordJohnsonSort(winners);

	// 4. Reconstruction
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
				pairs[i].first = -1;
				break;
			}
		}
	}

	// if (isVector) //[DEBUG]
	//{			  //[DEBUG]
	// std::cout << BLUE << "[DEBUG] Main Chain (winners sorted): " << RESET;
	// for (size_t i = 0; i < mainChain.size(); ++i) //[DEBUG]
	// std::cout << mainChain[i] << " ";		  //[DEBUG]
	// std::cout << YELLOW << "\n[DEBUG] Pend (losers): " << RESET;
	// for (size_t i = 0; i < pend.size(); ++i) //[DEBUG]
	// std::cout << pend[i] << " ";		 //[DEBUG]
	// std::cout << std::endl;					 //[DEBUG]
	//} //[DEBUG]

	// 5. Initial insertion
	if (!pend.empty())
	{
		mainChain.insert(mainChain.begin(), pend[0]);
		// if (isVector) //[DEBUG]
		// std::cout << GREEN << "[DEBUG] First pend element " << pend[0] << " inserted at start." << RESET << std::endl;
	}

	// 6. Jacobsthal Insertion
	if (pend.size() > 1)
	{
		std::vector<int> insertionOrder = buildInsertionOrder(pend.size());
		// if (isVector) //[DEBUG]
		//{			  //[DEBUG]
		// std::cout << BOLD << "[DEBUG] Jacobsthal insertion order (indices): " << RESET;
		// for (size_t i = 0; i < insertionOrder.size(); ++i) //[DEBUG]
		// std::cout << insertionOrder[i] << " ";		   //[DEBUG]
		// std::cout << std::endl;							   //[DEBUG]
		//} //[DEBUG]

		for (size_t i = 0; i < insertionOrder.size(); ++i)
		{
			int idx = insertionOrder[i];
			if (idx <= 0 || idx >= (int)pend.size())
				continue;
			int val = pend[idx];
			typename T::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), val);
			mainChain.insert(it, val);
			// if (isVector) //[DEBUG]
			// std::cout << GREEN << "[DEBUG] Inserting pend[" << idx << "] (" << val << ") using binary search." << RESET << std::endl;
		}
	}

	// 7. Final Straggler
	if (hasStraggler)
	{
		typename T::iterator it = std::lower_bound(mainChain.begin(), mainChain.end(), straggler);
		mainChain.insert(it, straggler);
		// if (isVector) //[DEBUG]
		// std::cout << MAGENTA << "[DEBUG] Final straggler " << straggler << " inserted." << RESET << std::endl;
	}

	container = mainChain;
}

#endif