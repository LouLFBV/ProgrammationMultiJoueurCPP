#include "Item.h"

Item::Item(ItemType itemType) :  m_itemType(itemType){}

ItemType Item::GetItemType() const
{
	return m_itemType;
}

	

