/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:53:28 by pol               #+#    #+#             */
/*   Updated: 2026/02/19 13:40:31 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : _vecTime(0), _deqTime(0) {}

PmergeMe::~PmergeMe() {}

// Canonical form requirements
PmergeMe::PmergeMe(const PmergeMe &src) { *this = src; }
PmergeMe &PmergeMe::operator=(const PmergeMe &src)
{
	if (this != &src)
	{
		_vec = src._vec;
		_deq = src._deq;
		_vecTime = src._vecTime;
		_deqTime = src._deqTime;
	}
	return *this;
}

// Generates Jacobsthal numbers: Jn = Jn-1 + 2*Jn-2
std::vector<int> PmergeMe::generateJacobsthal(int n)
{
	std::vector<int> jacob;
	// begin at 3 to have the first bloc as [3, 2, 1]
	int j0 = 3;
	int j1 = 5;

	if (n > 1)
		jacob.push_back(j0);
	if (n > 3)
		jacob.push_back(j1);

	while (true)
	{
		int next = jacob.back() + 2 * jacob[jacob.size() - 2];
		if (next >= n + 2)
			break;
		jacob.push_back(next);
	}
	return jacob;
}

std::vector<int> PmergeMe::buildInsertionOrder(int size)
{
	std::vector<int> order;
	if (size <= 1)
		return order;

	// get jacobsthal numbers
	std::vector<int> jacob = generateJacobsthal(size);

	// We start at 0 because the index 0 is manually inserted before
	// This allows the first block to descend to index 1
	int lastLimit = 0;

	for (size_t i = 0; i < jacob.size(); i++)
	{
		// We can't exceed the last available index (size - 1)
		int upperLimit = (jacob[i] >= size) ? size - 1 : jacob[i];

		// We insert in reverse until the previous limit
		// Exemple : if upperLimit = 3 and lastLimit = 0 -> we add 3, 2, 1
		for (int j = upperLimit; j > lastLimit; j--)
		{
			order.push_back(j);
		}

		// update the limit for the next block
		if (upperLimit > lastLimit)
			lastLimit = upperLimit;

		// If we have reached the end of the Pend, we stop
		if (upperLimit >= size - 1)
			break;
	}

	// Security : we add the remaining elements that would not have been
	// subsequently covered by Jacobsthal (case of very large paintings)
	for (int i = 1; i < size; i++)
	{
		bool found = false;
		for (size_t j = 0; j < order.size(); j++)
		{
			if (order[j] == i)
			{
				found = true;
				break;
			}
		}
		if (!found)
			order.push_back(i);
	}

	return order;
}

void PmergeMe::parseInput(int ac, char **av)
{
	for (int i = 1; i < ac; i++)
	{
		std::string s(av[i]);
		if (s.empty() || s.find_first_not_of("0123456789") != std::string::npos)
			throw ErrorException();

		long val = std::atol(av[i]);
		if (val > 2147483647 || val < 0)
			throw ErrorException();

		_vec.push_back(static_cast<int>(val));
		_deq.push_back(static_cast<int>(val));
	}
}

void PmergeMe::execute(int ac, char **av)
{
	try
	{
		parseInput(ac, av);
		if (_vec.empty())
			return;

		// Display "Before"
		std::cout << "Before: ";
		for (size_t i = 0; i < _vec.size(); i++)
		{
			std::cout << _vec[i] << (i == _vec.size() - 1 ? "" : " ");
			if (i > 10)
			{
				std::cout << "[...]";
				break;
			} // Truncate if too long
		}
		std::cout << std::endl;

		// Measure Vector time
		struct timeval start, end;
		gettimeofday(&start, NULL);
		fordJohnsonSort(_vec);
		// std::cout << "Debug: mainChain size = " << _vec.size() << std::endl;
		gettimeofday(&end, NULL);
		_vecTime = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

		// Measure Deque time
		gettimeofday(&start, NULL);
		fordJohnsonSort(_deq);
		gettimeofday(&end, NULL);
		_deqTime = (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);

		// Display "After"
		std::cout << "After:  ";
		for (size_t i = 0; i < _vec.size(); i++)
		{
			std::cout << _vec[i] << (i == _vec.size() - 1 ? "" : " ");
			if (i > 10)
			{
				std::cout << "[...]";
				break;
			}
		}
		std::cout << std::endl;

		// Display timings
		std::cout << "Time to process a range of " << _vec.size() << " elements with std::vector : " << std::fixed << std::setprecision(5) << _vecTime << " us" << std::endl;
		std::cout << "Time to process a range of " << _deq.size() << " elements with std::deque  : " << std::fixed << std::setprecision(5) << _deqTime << " us" << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}