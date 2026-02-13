/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 11:20:19 by pol               #+#    #+#             */
/*   Updated: 2026/02/13 08:20:05 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

class BitcoinExchange
{
private:
	// using std::map to stock DB (date, price)
	std::map<std::string, float> _data;

	bool isValidDate(const std::string &date) const;
	bool isValidValue(const float value) const;

public:
	// canonical form
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	void loadDatabase(const std::string &filename);
	void processInput(const std::string &filename);
};

#endif