
//          Copyright 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file BOOST_LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <memory>

#include "common/stdafx.h"
#include "config.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using ptree = boost::property_tree::ptree;

void Config::parse(const char *path_to_ini, gb_opts &opts)
{
	// Not sure if this tree cleans itself up, it probably does, but whatever.
	// Also, this structure should stick around so that there is no overhead when changing configuration dynamically.
	try
	{
		ptree properties;
		//std::unique_ptr<ptree*> prop_wrapper = std::make_unique<ptree*>(properties);
		boost::property_tree::ini_parser::read_ini(path_to_ini, properties);

		opts.enable_bootload = properties.get<bool>("config.bootload");
		opts.enable_debug = properties.get<bool>("config.debug");
		opts.enable_tilemap_view = properties.get<bool>("config.tilemap");
		opts.enable_window_display = properties.get<bool>("config.display");
		opts.bootload_path = properties.get<std::string>("config.bootload_path");
		opts.debug_type = properties.get<std::string>("config.debug_type");
		opts.debug_host = properties.get<std::string>("config.debug_host");
		opts.renderer = properties.get<std::string>("config.renderer");
		opts.rom_path = properties.get<std::string>("config.rom_path");
		opts.last_rom_path = properties.get<std::string>("config.last_rom_path");
		opts.debug_port = properties.get<int>("config.debug_port");
		opts.height = properties.get<int>("config.height");
		opts.width = properties.get<int>("config.width");
		opts.joypad_a = properties.get<int>("config.joypad_a");
		opts.joypad_b = properties.get<int>("config.joypad_b");
		opts.joypad_start = properties.get<int>("config.joypad_start");
		opts.joypad_select = properties.get<int>("config.joypad_select");
		opts.joypad_up = properties.get<int>("config.joypad_up");
		opts.joypad_right = properties.get<int>("config.joypad_right");
		opts.joypad_down = properties.get<int>("config.joypad_down");
		opts.joypad_left = properties.get<int>("config.joypad_left");
	}
	catch (boost::property_tree::ptree_bad_data& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (boost::property_tree::ptree_bad_path& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (boost::property_tree::ptree_error& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Generic exception thrown when trying to access 'config.ini'. Check your 'config.ini' file for invalid syntax." << std::endl;
	}


}