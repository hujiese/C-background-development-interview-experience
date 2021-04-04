#pragma once
class DrawAPI
{
public:
	DrawAPI();
	virtual void drawCircle(int radius, int x, int y) = 0;
	virtual ~DrawAPI();
};

