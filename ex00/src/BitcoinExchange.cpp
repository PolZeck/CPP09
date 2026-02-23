/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* BitcoinExchange.cpp                                :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/02/12 11:20:06 by pol               #+#    #+#             */
/* Updated: 2026/02/17 09:02:53 by pol              ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) { *this = other; }

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
    {
        this->_data = other._data;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

// check if the date is in YYYY-MM-DD format
bool BitcoinExchange::isValidDate(const std::string &date) const
{
    if (date.length() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    int y, m, d;
    std::stringstream ssY(date.substr(0, 4));
    std::stringstream ssM(date.substr(5, 2));
    std::stringstream ssD(date.substr(8, 2));

    if (!(ssY >> y) || !(ssM >> m) || !(ssD >> d))
        return false;
    if (m < 1 || m > 12 || d < 1 || d > 31)
        return false;
    if (m == 2)
    {
        bool leap = (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
        if (d > (leap ? 29 : 28))
            return false;
    }
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30)
        return false;
    return true;
}

void BitcoinExchange::loadDatabase(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    std::string line, date;
    float rate;

    if (!file.is_open())
    {
        std::cerr << "Error: could not open database." << std::endl;
        return;
    }
    std::getline(file, line); // Skip header
    while (std::getline(file, line))
    {
        size_t comma = line.find(',');
        if (comma != std::string::npos)
        {
            date = line.substr(0, comma);
            rate = static_cast<float>(atof(line.substr(comma + 1).c_str()));
            _data[date] = rate;
        }
    }
    file.close();
}

void BitcoinExchange::processInput(const std::string &filename)
{
    // 1. OPEN INPUT FILE: Open the file provided as an argument (e.g., input.txt)
    std::ifstream file(filename.c_str());
    std::string line;

    if (!file.is_open())
    {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    // Read the first line (header)
    if (std::getline(file, line))
    {
        if (line != "date | value")
        {
            std::cerr << "Error: invalid header format => " << line << std::endl;
            return;
        }
    }

    // 3. PROCESSING LOOP: Read the input file line by line
    while (std::getline(file, line))
    {
        // Skip empty lines
        if (line.empty())
            continue;

        // A. FIND DELIMITER: Search for the pipe '|' separator
        size_t sep = line.find('|');
        if (sep == std::string::npos)
        {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        // B. EXTRACT DATE: Get the string before the separator
        std::string date = line.substr(0, sep);
        // Remove spaces
        date.erase(remove(date.begin(), date.end(), ' '), date.end());

        // C. EXTRACT VALUE: Rigorous check for numeric value
        std::string valStr = line.substr(sep + 1);
        std::stringstream ss(valStr);
        float val;
        std::string extra;

        // Check if it's a valid float AND if there is no "trash" after the number
        if (!(ss >> val))
        {
            std::cout << "Error: bad input => " << valStr << std::endl;
            continue;
        }
        if (ss >> extra)
        { // If this succeeds, it means there's more content after the float
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        // D. DATA VALIDATION
        if (!isValidDate(date))
            std::cout << "Error: bad input => " << date << std::endl;
        else if (val < 0)
            std::cout << "Error: not a positive number." << std::endl;
        else if (val > 1000)
            std::cout << "Error: too large a number." << std::endl;
        else
        {
            // E. DATABASE SEARCH
            std::map<std::string, float>::iterator it = _data.upper_bound(date);

            if (it != _data.begin())
            {
                --it;
                // F. OUTPUT RESULT
                std::cout << date << " => " << val << " = " << val * it->second << std::endl;
            }
            else
            {
                std::cout << "Error: date too early." << std::endl;
            }
        }
    }
}