/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:53:28 by pol               #+#    #+#             */
/*   Updated: 2026/02/19 10:58:06 by pol              ###   ########.fr       */
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
	if (n <= 0)
		return jacob;
	jacob.push_back(1);
	if (n == 1)
		return jacob;
	jacob.push_back(3);

	while (true)
	{
		int next = jacob.back() + 2 * jacob[jacob.size() - 2];
		if (next >= n)
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

	std::vector<int> jacob = generateJacobsthal(size);
	int lastLimit = 1; // Index 0 is already manually inserted as the first element

	for (size_t i = 0; i < jacob.size(); i++)
	{
		// Must not exceed the last available index (size - 1)
		int upperLimit = (jacob[i] >= size) ? size - 1 : jacob[i];

		// Insert indices of the current block in reverse order
		for (int j = upperLimit; j > lastLimit; j--)
		{
			order.push_back(j);
		}

		// Update the limit for the next block
		if (upperLimit > lastLimit)
			lastLimit = upperLimit;
	}

	// Safety check: if any indices were missed (rare but possible depending on n)
	// they are added at the end.
	for (int i = 1; i < size; i++)
	{
		bool found = false;
		for (size_t j = 0; j < order.size(); j++)
		{
			if (order[j] == i)
				found = true;
		}
		if (!found && i != 0)
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
		std::cout << "Debug: mainChain size = " << _vec.size() << std::endl;
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