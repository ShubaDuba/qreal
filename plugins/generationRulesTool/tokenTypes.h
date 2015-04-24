/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#pragma once

#include <QtCore/QHash>
#include <QtCore/QDebug>
#include <type_traits>

/// Tokens recognizable by lexer.
namespace simpleParser {
enum class TokenTypes {
	newline
	, whitespace
	, identifier

	, openingBracket
	, closingBracket
	, openingCurlyBracket
	, closingCurlyBracket

	, dot

	, foreachKeyword
	, newlineKeyword

	, text

	, comment
	, string
};

/// Overload that allows to use token types as hash keys.
inline uint qHash(TokenTypes const &key)
{
	return ::qHash(static_cast<std::underlying_type<TokenTypes>::type>(key));
}

/// Helper operator for debugging.
inline QDebug operator <<(QDebug debug, TokenTypes const &token)
{
	switch (token)
	{
	case TokenTypes::newline:
		debug << "newline ";
		break;

	case TokenTypes::whitespace:
		debug << "whitespace ";
		break;

	case TokenTypes::identifier:
		debug << "identifier ";
		break;

	case TokenTypes::openingBracket:
		debug << "openingBracket ";
		break;

	case TokenTypes::closingBracket:
		debug << "closingBracket ";
		break;

	case TokenTypes::openingCurlyBracket:
		debug << "openingCurlyBracket ";
		break;

	case TokenTypes::closingCurlyBracket:
		debug << "closingCurlyBracket ";
		break;

	case TokenTypes::dot:
		debug << "dot ";
		break;

	case TokenTypes::foreachKeyword:
		debug << "foreachKeyword ";
		break;

	case TokenTypes::newlineKeyword:
		debug << "newlineKeyword ";
		break;

	case TokenTypes::text:
		debug << "text ";
		break;

	default:
		debug << "ERROR ";
		break;
	}

	return debug;
}

}
