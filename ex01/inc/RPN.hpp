/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:05:33 by pol               #+#    #+#             */
/*   Updated: 2026/02/17 09:33:52 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>

class RPN
{
private:
	// We use std::stack as it's the most efficient LIFO container for RPN
	std::stack<int> _stack;

	// Helper functions for internal logic
	bool isOperator(const std::string &token) const;
	void performOperation(const std::string &op);

public:
	RPN();
	RPN(const RPN &other);
	RPN &operator=(const RPN &other);
	~RPN();

	// Main function to process the expression
	void calculate(const std::string &expression);
};

#endif