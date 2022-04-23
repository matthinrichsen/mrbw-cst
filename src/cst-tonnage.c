/*************************************************************************
Title:    Tonnage support for Control Stand Throttle
Authors:  Michael D. Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     cst-tonnage.c
License:  GNU General Public License v3

LICENSE:
    Copyright (C) 2017 Michael Petersen & Nathan Holmes
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*************************************************************************/

#include <stdlib.h>

#include "lcd.h"

#include "cst-common.h"

static uint8_t currentTonnage = 0;
static uint8_t lastDisplayedTonnage = 0;

const uint8_t BarGraphBottomEmpty[8] =
{
	0b00010001,
	0b00010001,
	0b00010001,
	0b00010001,
	0b00010001,
	0b00010001,
	0b00010001,
	0b00011111
};

const uint8_t BarGraphBottomHalf[8] =
{
	0b00010001,
	0b00010001,
	0b00010001,
	0b00011111,
	0b00011111,
	0b00011111,
	0b00011111,
	0b00011111
};

const uint8_t BarGraphTopEmpty[8] =
{
	0b00011111,
	0b00010001,
	0b00010001,
	0b00010001,
	0b00010001,
	0b00010001,
	0b00010001,
	0b00010001
};

const uint8_t BarGraphTopHalf[8] =
{
	0b00011111,
	0b00010001,
	0b00010001,
	0b00010001,
	0b00010001,
	0b00011111,
	0b00011111,
	0b00011111
};

const uint8_t BarGraphFull[8] =
{
	0b00011111,
	0b00011111,
	0b00011111,
	0b00011111,
	0b00011111,
	0b00011111,
	0b00011111,
	0b00011111
};

void setupTonnageChars(void)
{
	switch(currentTonnage)
	{
		case 0:
			lcd_setup_custom(TONNAGE_TOP, BarGraphTopEmpty);
			lcd_setup_custom(TONNAGE_BOTTOM, BarGraphBottomEmpty);
			break;
		case 1:
			lcd_setup_custom(TONNAGE_TOP, BarGraphTopEmpty);
			lcd_setup_custom(TONNAGE_BOTTOM, BarGraphBottomHalf);
			break;
		case 2:
			lcd_setup_custom(TONNAGE_TOP, BarGraphTopHalf);
			lcd_setup_custom(TONNAGE_BOTTOM, BarGraphFull);
			break;
		case 3:
			lcd_setup_custom(TONNAGE_TOP, BarGraphFull);
			lcd_setup_custom(TONNAGE_BOTTOM, BarGraphFull);
			break;
	}
}

void printTonnage(void)
{
	lcd_gotoxy(0,0);
	switch(currentTonnage)
	{
		case 0:
			lcd_puts("LIGHT ");
			break;
		case 1:
			lcd_puts("LOW   ");
			break;
		case 2:
			lcd_puts("MEDIUM");
			break;
		case 3:
			lcd_puts("HEAVY ");
			break;
	}
	lcd_gotoxy(0,1);
	switch(currentTonnage)
	{
		case 0:
			lcd_puts("ENGINE");
			break;
		case 1:
		case 2:
		case 3:
			lcd_puts("WEIGHT");
			break;
	}

	if(lastDisplayedTonnage != currentTonnage)
	{
		setupTonnageChars();  // Draw new characters
		lastDisplayedTonnage = currentTonnage;
	}
	lcd_gotoxy(7,0);
	lcd_putc(TONNAGE_TOP);
	lcd_gotoxy(7,1);
	lcd_putc(TONNAGE_BOTTOM);
}

void incrementTonnage(void)
{
	if(currentTonnage >= 3)
		currentTonnage = 3;
	else
		currentTonnage++;
}

void decrementTonnage(void)
{
	if ((0 == currentTonnage) || (currentTonnage > 3))
		currentTonnage = 0;
	else
		currentTonnage--;
}

/*
	Linear tonnage curve
	y=x
	|							x
	|						x
	|					x
	|				x
	|			x
	|		x
	|	x
	----------------------
*/
const uint8_t LinearTonnage[16] =
	{
		0b0000000,
		0b0000100,
		0b0001000,
		0b0001100,
		0b0010000,
		0b0010100,
		0b0011000,
		0b0011100,
		0b0100000,
		0b0100100,
		0b0101000,
		0b0101100,
		0b0110000,
		0b0110100,
		0b0111000,
		0b0111111};

/*
	Sublinear tonnage curve
	y=x^2
	|							x
	|						x
	|
	|					x
	|				x
	|		x	x
	|	x
	-------------------------------
*/
const uint8_t SublinearTonnage[16] =
	{
		0b0000000,
		0b0000100,
		0b0001000,
		0b0001100,
		0b0010000,
		0b0010100,
		0b0011000,
		0b0011100,
		0b0100000,
		0b0100100,
		0b0101000,
		0b0101100,
		0b0110000,
		0b0110100,
		0b0111000,
		0b0111111};

/*
	Sublinear tonnage curve
	y=x^3
	|							x
	|
	|						x
	|
	|					x
	|			x	x
	|	x	x
	-------------------------------
*/
const uint8_t Sub3linearTonnage[16] =
	{
		0b0000000,
		0b0000100,
		0b0001000,
		0b0001100,
		0b0010000,
		0b0010100,
		0b0011000,
		0b0011100,
		0b0100000,
		0b0100100,
		0b0101000,
		0b0101100,
		0b0110000,
		0b0110100,
		0b0111000,
		0b0111111};

/*
	Sublinear tonnage curve
	y=x^4
	|							x
	|
	|
	|						x
	|
	|					x
	|	x	x	x	x
	-------------------------------
*/
const uint8_t Sub4linearTonnage[16] =
	{
		0b0000000,
		0b0000100,
		0b0001000,
		0b0001100,
		0b0010000,
		0b0010100,
		0b0011000,
		0b0011100,
		0b0100000,
		0b0100100,
		0b0101000,
		0b0101100,
		0b0110000,
		0b0110100,
		0b0111000,
		0b0111111};

const __uint8_t dutyWidth = 128;
const __uint8_t tonnageDuration = 128;
const __uint8_t numSteps = 16;
__uint8_t currentStep = 0;
__uint8_t tonnageChart[16] = LinearTonnage;

void evaluateTonnage(void)
{
}

void stepTonnage(void)
{
	if (currentStep < 15)
		currentStep++;
}