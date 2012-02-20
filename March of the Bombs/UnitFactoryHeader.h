#pragma once
#include <iostream>
//http://login2win.blogspot.com/2008/05/c-factory-pattern.html

class UnitFactory
{
private:
	void makeNormalBomb();
	void makeFastBomb();
	void makeClusterBomb();
	void makeDud();
};
class Bomb 
{
public:
	virtual int bombType() = 0;
	virtual int team() = 0;
};