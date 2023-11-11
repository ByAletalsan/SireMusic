/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atalaver <atalaver@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 16:28:12 by atalaver          #+#    #+#             */
/*   Updated: 2023/11/11 22:17:14 by atalaver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool	isStringDigit( std::string &s )
{
	for (unsigned i = 0; i < s.length(); i++)
		if (!std::isdigit(s[i]))
			return (false);
	return (true);
}

static std::string	limpiarString( std::string &str )
{
    size_t inicio = 0;
    size_t fin = str.length();

    while (inicio < str.length() && !std::isalnum(str[inicio])) {
        inicio++;
    }

    while (fin > inicio && !std::isalnum(str[fin - 1])) {
        fin--;
    }

    return str.substr(inicio, fin - inicio);
}

std::string	ajustarLongitudString( std::string &s, int tam )
{
	std::string	res;
	std::string new_s = limpiarString(s);
	for (int i = 0; i < tam - (int)new_s.size(); i++)
		res += "0";
	return (res + new_s);
}

std::string	stringToLower( const std::string &s )
{
	std::string	res = "";

	for (unsigned i = 0; i < s.length(); i++)
		res += std::tolower(s[i]);
	return (res);
}

std::string	stringToUpper( const std::string &s )
{
	std::string	res = "";

	for (unsigned i = 0; i < s.length(); i++)
		res += std::toupper(s[i]);
	return (res);
}

bool		inVectorString( const std::vector<std::string> &v, const std::string &s )
{
	for (unsigned i = 0; i < v.size(); i++)
	{
		if (v[i] == s)
			return (true);
	}
	return (false);
}
