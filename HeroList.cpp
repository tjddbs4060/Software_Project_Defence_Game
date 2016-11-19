#include "HeroList.h"

USING_NS_CC;

HeroList::HeroList()
{
}

HeroList::~HeroList()
{
}

void HeroList::setHero(char* name)
{
	char szFile[64] = { 0, };

	sprintf(szFile, "%s.png", name);
	hero = Sprite::createWithSpriteFrameName(szFile);
}

void HeroList::setMap(char* name)
{
	map = Sprite::createWithSpriteFrameName(name);
}

void HeroList::setBoss(char* name)
{
	boss = Sprite::createWithSpriteFrameName(name);
}

void HeroList::setHelp(char* name)
{
	help = Sprite::createWithSpriteFrameName(name);
}

void HeroList::setType(char* name)
{
	strcpy(type, name);
}

void HeroList::setAtk(int damage)
{
	atk = damage;
}

Sprite* HeroList::getHero()
{
	return hero;
}

Sprite* HeroList::getMap()
{
	return map;
}

Sprite* HeroList::getBoss()
{
	return boss;
}

Sprite* HeroList::getHelp()
{
	return help;
}

char* HeroList::getType()
{
	return type;
}

int HeroList::getAtk()
{
	return atk;
}

void HeroList::release()
{
	if (NULL != map)
	{
		map->removeFromParentAndCleanup(true);
		map = NULL;
	}

	if (NULL != boss)
	{
		boss->removeFromParentAndCleanup(true);
		boss = NULL;
	}

	if (NULL != help)
	{
		help->removeFromParentAndCleanup(true);
		help = NULL;
	}
	
	if (NULL != hero)
	{
		hero->removeFromParentAndCleanup(true);
		hero = NULL;
	}
}

void HeroList::init(int size)
{
	Point pt = hero->getContentSize();
	char szFile[32] = { 0, };
	int count = 0;
	
	hero->setPosition(Point(26, 140 - (size * 35)));
	hero->setTag(TAG_HERO_LIST_MAIN + size);

	sprintf(szFile, "%s_hero_list.png", type);
	Sprite* hero_type = Sprite::createWithSpriteFrameName(szFile);
	hero_type->setPosition(Point(37, 0) + pt/2);
	
	while (count < 6)
	{
		int temp = atk;
		int div = 1;

		count++;

		for (int i = 0; i < count; i++)
			div *= 10;

		if (temp / div == 0)
			break;
	}

	float xPos = 68 + (pt.x/2) + (count*4);
	Sprite* sprite_number = NULL;

	for (int i = 0; i < count; i++)
	{
		sprintf(szFile, "hero_list_number_%d.png", atk%10);
		sprite_number = Sprite::createWithSpriteFrameName(szFile);
		sprite_number->setPosition(Point(xPos, pt.y/2));
		hero->addChild(sprite_number);

		atk /= 10;
		xPos -= 6;
	}

	map->setPosition(Point(120, 0) + pt/2);
	map->setTag(TAG_HERO_LIST_MAIN_MAP);
	boss->setPosition(Point(155, 0) + pt/2);
	boss->setTag(TAG_HERO_LIST_MAIN_BOSS);
	help->setPosition(Point(190, 0) + pt/2);
	help->setTag(TAG_HERO_LIST_MAIN_HELP);

	hero->addChild(hero_type);
	hero->addChild(map);
	hero->addChild(boss);
	hero->addChild(help);
}

void HeroList::setCount(int num)
{
	count = num;
}

int HeroList::getCount()
{
	return count;
}

void HeroList::setLocation(int num)
{
	location = num;
}

int HeroList::getLocation()
{
	return location;
}