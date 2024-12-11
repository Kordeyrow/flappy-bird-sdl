#pragma once
#include <vector>
#include "mobile_input.h"
#include "pc_input.h"

class InputGroup {
public:
	std::vector<MobileInput> mobile_input_list;
	std::vector<PCInput> pc_input_list;
};

