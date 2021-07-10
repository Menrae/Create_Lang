#include "tokens.h"
#include "lookup.h"

// this is where the reserved token enumeration goes ^.^



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