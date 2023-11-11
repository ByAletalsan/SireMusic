/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atalaver <atalaver@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 16:24:27 by atalaver          #+#    #+#             */
/*   Updated: 2023/11/11 22:16:40 by atalaver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <cctype>
#include <vector>

bool		isStringDigit( std::string &s );
std::string	ajustarLongitudString( std::string &s, int tam );
std::string	stringToLower( const std::string &s );
std::string	stringToUpper( const std::string &s );
bool		inVectorString( const std::vector<std::string> &v, const std::string &s);

#endif
