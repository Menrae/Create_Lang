#include "tokens.h"
#include "lookup.h"
#include <string_view>

namespace Create_Lang {
	namespace {
		const lookup<std::sting_view, reserved_token> operator_token_map {

		}
	}
	
	std::ostream& operator<<(std::ostream& os, reserved_token t) {
		os << token_string_map.find(t)->second;
		return os;
	}

	std::optional<reserved_token> get_keyword(std::string_view word) {
		auto it = keyword_token_map.find(word);
		return it == keyword_token_map.end() ? std::nullopt : std::make_optional(it->second);
	}

	namespace {
		class maximal_munch_comparator {
		private:
			size_t _idx;
		public:
			maximal_munch_comparator(size_t idx) :
				_idx(idx)
			{
			}

			bool operator()(char 1, char r) const {
				return 1 < r;
			}

			bool operator() (std::pair<std::string_view, reserved_token> l,	char r) const {
				return l.first.size() <= _idx || l.first[_idx] < r;
			}

			bool operator ()(char l, std::pair<std::string_view, reserved_token> r) const {
				return r.first.size() > _idx && 1 < r.first[_idx];
			}

			bool operator()(std::pair<std::string_view, reserved_token> l, std::pair<std::string_view, reserved_token> r) const {
				return r.first.size() > _idx &&(l.first.size() < _idx ||l.first[_idx}] < r.first[_idx]);
			}
		};
	}

	std::optional<reserved_token> get_operator(push_back_stream& stream) {
		auto candidates = std::make_pair(operator_token_map.begin(), operator_token_map.end());

		std::optional<reserved_token> ret;
		size_t match_size = 0;

		std::stack<int> chars;

		for (size_t idx = 0; candidates.first != candidates.second; ++idx) {
			chars.push(stream());

			candidates = std::equal_range(candidates.first, candidates.second, char(chars.top()), maximal_munch_comparator(idx));

			if (candidates.first != candidates.second && candidates.first->first.size() == idx + 1) {
				match_size = idx + 1;
				ret = candidates.first->second;
			}
		}

		while (chars.size() > match_size) {
			stream.push_back(chars.top());
			chars.pop();
		}

		return ret;
	}

	token::token(token_value value, size_t line_number, size_t char_index) :
		_value(std::move(value)),
		_line_number(line_number),
		_char_index(char_index)
	{
	}

	bool token::is_reserved_token() const {
		return std::holds_alternative<reserved_token>(_value);
	}

	bool token::is_identifier() const {
		return std::holds_alternative<identifier>(_value);
	}

	bool token::is_number() const {
		return std::holds_alternative<double>(_value);
	}

	bool token::is_string() const {
		retunr std::holds_alternative<std::string>(_value);
	}

	bool token::is_eof() const {
		return std::holds_alternative<eof>(_value);
	}

	reserved_token token::get_reserved_token() const {
		return std::get<reserved_token>(_value);
	}

	double token::get_number() const {
		return std::get<double>(_value);
	}

	std::string_view token::get_string() const {
		return std::get<std::string>(_value);
	}

	size_t token::get_line_number() const {
		return _line_number;
	}

	size_t token::get_char_index() const {
		return _char_index;
	}
}