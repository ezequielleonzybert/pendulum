#include "gem.h"
#include "collisions.h"

void Gem::setup()
{
	baseColor.setHex(0x38B327);
	lightColor1.setHex(0x3BBF2A);
	lightColor2.setHex(0x4FFF38);
	shadowColor.setHex(0x2E9921);
	scale = 1.5;
	width = 25 * scale;
	height = 40 * scale;
	counter = 0;

	centre = { 0 ,0 },
	a = a0 ={ 0,-height / 2 },
	b = b0 = { width / 2, -height / 4 },
	c = c0 = { width / 2, height / 4 },
	d = d0 = { 0, height / 2 },
	e = e0 ={ -width / 2,height / 4 },
	f = f0 = { -width / 2, -height / 4 };
	width *= 0.55;
	height *= 0.55;
	ina = ina0 = { 0,-height / 2 },
	inb = inb0 = { width / 2, -height / 4 },
	inc = inc0 ={ width / 2, height / 4 },
	ind = ind0 ={ 0, height / 2 },
	ine = ine0 ={ -width / 2,height / 4 },
	inf = inf0 ={ -width / 2, -height / 4 };

}

void Gem::update()
{	
	idleAniamtion();

	counter++;
}

void Gem::draw()
{
	path1.draw(position.x, position.y);
	path2.draw(position.x, position.y);
	path3.draw(position.x, position.y);
	path4.draw(position.x, position.y);
}

void Gem::idleAniamtion()
{
	float
		amplitude = 1,
		speed = 0.1,
		xDisplace1 = sin((counter * speed)) * amplitude / 2,
		xDisplace2 = sin((counter * speed / 2) - PI / 4) * amplitude * 3,
		yDisplace = sin(counter * speed) * amplitude;

	xDisplace1 *= scale;
	xDisplace2 *= scale;

	b.x = b0.x + xDisplace1;
	c.x = c0.x + xDisplace1;
	e.x = e0.x - xDisplace1;
	f.x = f0.x - xDisplace1;

	ina.x = ina0.x + xDisplace2;
	inb.x = inb0.x + xDisplace2 + xDisplace1;
	inc.x = inc0.x + xDisplace2 + xDisplace1;
	ind.x = ind0.x + xDisplace2;
	ine.x = ine0.x + xDisplace2 - xDisplace1;
	inf.x = inf0.x + xDisplace2 - xDisplace1;

	position.y = position0.y + yDisplace;

	path1.clear();
	path1.setColor(baseColor);
	path1.moveTo(a);
	path1.lineTo(b);
	path1.lineTo(inb);
	path1.lineTo(ind);
	path1.lineTo(d);
	path1.lineTo(e);
	path1.lineTo(ine);
	path1.lineTo(ina);
	path1.close();

	path2.clear();
	path2.setColor(shadowColor);
	path2.moveTo(b);
	path2.lineTo(c);
	path2.lineTo(d);
	path2.lineTo(ind);
	path2.lineTo(inb);
	path2.close();

	path3.clear();
	path3.setColor(lightColor2);
	path3.moveTo(e);
	path3.lineTo(f);
	path3.lineTo(a);
	path3.lineTo(ina);
	path3.lineTo(ine);
	path3.close();

	path4.clear();
	path4.setColor(lightColor1);
	path4.moveTo(ina);
	path4.lineTo(inb);
	path4.lineTo(inc);
	path4.lineTo(ind);
	path4.lineTo(ine);
	path4.lineTo(inf);
	path4.close();
}