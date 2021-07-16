#include <iostream>
#include <sstream>
#include "tokenizer.h"
#include <errors.h>

// When debugging in Xcode, setting the breakpoint will send eof to stdin. We need to ignore it.
// #define XCODE_DEBUG_HACK

int main() {
	using namespace stork;
	std::cerr << "Enter the line to tokenize, or newline to exit." << std::endl;
	std::string line;

	do {
		if (std::cin.clear()) {
			std::cin.clear();
		}
		std::getline(std::cin, line);
		if (!line.empty()) {
			std::istringstream strstream(line);

			get_character input = [&strstream]() {
				return strstream.get();
			};

			try {
				push_back_stream stream(input);
				for (token t = tokenize(stream); !t.is_eof(); t = tokenize(stream)) {
					if (t.is_reserved_token()) {
						std::cout << "Reserved: " << t.get_reserved_token() << std::endl;
					}
					else if (t.is_identifier()) {
						std::cout << "Identifier"
					}
				}
			}
		}
	}
}