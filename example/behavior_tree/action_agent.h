﻿#pragma once
#include "agent.h"
#include "nodes.h"
namespace behavior
{
	class action_agent : public agent
	{
		// actions return true or false for immediate result
		// if the result is not immediate then return nullopt
		// all the arguments should take the form const T& or T

		Meta(behavior_action) bool has_key(const std::string& bb_key);
		Meta(behavior_action) bool set_key_value(const std::string& bb_key, const meta::serialize::any_value_type& new_value);
		Meta(behavior_action) bool has_key_value(const std::string& bb_key, const meta::serialize::any_value_type& value);
		Meta(behavior_action) bool number_add(const std::string& bb_key, const meta::serialize::any_value_type& value);
		Meta(behavior_action) bool number_dec(const std::string& bb_key, const meta::serialize::any_value_type& value);
		Meta(behavior_action) bool number_multiply(const std::string& bb_key, const meta::serialize::any_value_type& value);
		Meta(behavior_action) bool number_div(const std::string& bb_key, const meta::serialize::any_value_type& value);
		Meta(behavior_action) bool number_larger_than(const std::string& bb_key, const meta::serialize::any_value_type& other_value);
		Meta(behavior_action) bool number_less_than(const std::string& bb_key, const meta::serialize::any_value_type& other_value);
		Meta(behavior_action) std::optional<bool> wait_for_seconds(double duration);
	};
}