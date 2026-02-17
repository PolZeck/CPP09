/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:05:45 by pol               #+#    #+#             */
/*   Updated: 2026/02/17 09:13:20 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int main(int argc, char **argv)
{
	// 1. ARGUMENT CHECK: The program must take exactly one argument
	if (argc != 2)
	{
		std::cerr << "Error: Usage: ./RPN \"expression\"" << std::endl;
		return 1;
	}

	// 2. INITIALIZATION: Creating our RPN object
	RPN rpn;

	// 3. EXECUTION: Try to calculate the expression provided in argv[1]
	// The subject requires outputting the result or "Error"
	try
	{
		rpn.calculate(argv[1]);
	}
	catch (const std::exception &e)
	{
		// Any unhandled logic errors will be caught here
		std::cerr << "Error" << std::endl;
		return 1;
	}

	return 0;
}