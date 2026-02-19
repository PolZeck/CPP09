/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 10:53:30 by pol               #+#    #+#             */
/*   Updated: 2026/02/19 10:24:18 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error: Wrong number of arguments." << std::endl;
		return 1;
	}

	PmergeMe sorter;
	sorter.execute(ac, av);

	return 0;
}