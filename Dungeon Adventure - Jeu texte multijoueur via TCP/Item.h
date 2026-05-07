#pragma once

#include "ItemType.h"

#include <string>

class Item
{
protected: 
	ItemType m_itemType;

public:
	Item(ItemType itemType);
	virtual ~Item() = default;
	ItemType GetItemType() const;
	virtual std::string GetName() const = 0;
};