#pragma once
#include <vector>
#include <string>
#include "CardEffect.h"

//アイテム関係
struct Item {
	std::string name;
	std::vector<Effect> effects; // アイテムが持つ効果
	bool isConsumable;           // 使いきりアイテムならTRUE
};