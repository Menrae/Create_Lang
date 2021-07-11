#include "tokenizer.hpp"
#include <map>
#include <string>
#include <cctype>
#include <stack>
#include <cstdlib>
#include "push_back_stream.hpp"
#include "errors.hpp"

namespace Create_Lang {
	namespace {
		enum struct character_type {
			eof,
			space,
			alphanum,
			punct,
		};

		character_type get_character_type(int c) {
			if (c < 0) {
				return character_type::eof;
			}
			if (std::isspace(c)) {
				return character_type::space;
			}
			if (std::isalpha(c) || std::isdigit(c) || c == '_') {
				return character_type::alphanum;
			}
		}

		token fetch_word(push_back_stream& stream) {
			size_t line_number = stream.line_number();
			size_t char_index = stream.char_index();

			std::string word;

			int c = stream();

			bool is_number = isdigit(c);

			do {
				word.push_back(char(c));
				c = stream();
			} while (get_character_type(c) == character_type::alphanum || (is_number && c == '.'));

			stream.push_back(c);

			if (std::optional<reserved_token> t = get_keyword(word)) {
				return token(*t, line_number, char_index);
			}
			else {
				if (std::isdigit(word.front())) {
					char* endptr;
					double num = strtol(word.c_str(), &endptr, 0);
					if (*endptr != 0) {
						num = strtod(word.c_str(), &endptr);
						if (*endptr != 0) {
							size_t remaining = word.size() - (endptr - word.c_str());
							throw unexpected_error(std::string(1, char(*endptr)), stream.line_number(), stream.char_index() - remaining);
						}
					}
				}
				return token(num, line_number, char_index);
			} else {
				return token(identifier{ std::move(word) }, line_number, char_index);
			}
		}
	}


}