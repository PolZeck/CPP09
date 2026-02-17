/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 14:05:37 by pol               #+#    #+#             */
/*   Updated: 2026/02/17 09:12:26 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <cstdlib>

RPN::RPN() {}

RPN::RPN(const RPN &other) { *this = other; }

RPN &RPN::operator=(const RPN &other)
{
	if (this != &other)
	{
		this->_stack = other._stack;
	}
	return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(const std::string &token) const
{
	return (token == "+" || token == "-" || token == "*" || token == "/");
}

void RPN::performOperation(const std::string &op)
{
	// We need at least 2 numbers in the stack to perform an operation
	if (_stack.size() < 2)
		throw std::runtime_error("Error");

	int b = _stack.top();
	_stack.pop();
	int a = _stack.top();
	_stack.pop();

	if (op == "+")
		_stack.push(a + b);
	else if (op == "-")
		_stack.push(a - b);
	else if (op == "*")
		_stack.push(a * b);
	else if (op == "/")
	{
		if (b == 0)
			throw std::runtime_error("Error"); // Division by zero
		_stack.push(a / b);
	}
}

void RPN::calculate(const std::string &expression)
{
	std::stringstream ss(expression);
	std::string token;

	while (ss >> token)
	{
		if (token.length() == 1 && isdigit(token[0]))
		{
			_stack.push(atoi(token.c_str()));
		}
		else if (token.length() == 1 && isOperator(token))
		{
			try
			{
				performOperation(token);
			}
			catch (const std::exception &e)
			{
				std::cerr << e.what() << std::endl; // Error on standard error
				return;
			}
		}
		else
		{
			std::cerr << "Error" << std::endl; // Invalid token
			return;
		}
	}

	// At the end, there must be exactly one result left in the stack
	if (_stack.size() == 1)
	{
		std::cout << _stack.top() << std::endl;
	}
	else
	{
		std::cerr << "Error" << std::endl;
	}
}