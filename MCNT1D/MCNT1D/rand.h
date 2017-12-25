#pragma once
#ifndef RAND_H
#define RAND_H

/*-------------------------------------------
	功能：产生一个（0，1）之间均匀分布的浮点数。
	参数：无
	返回：随机数
	示例：random();
-------------------------------------------*/
double random();

/*---------------------------------------------------------------
	功能：产生一个随机方向角余弦，即（-1，1）之间均匀分布的随机数。
	参数：无
	返回：随机数
	示例：randomDirection();
---------------------------------------------------------------*/
double randomDirection();

#endif // !RAND_H
