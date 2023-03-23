
#pragma once

#include "opts.h"

class Config
{
public:
	static void parse(const char *path_to_ini, gb_opts &opts);
};