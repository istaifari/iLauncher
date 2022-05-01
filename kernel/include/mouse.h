#pragma once
#include <stddef.h>
#include <stdint.h>
#include "../kernel.h"
#include "../include/vga.h"
#include "../include/keyboard.h"

int GetMousePos();
void HandlePS2Mouse(uint8_t data);
void InitPS2Mouse();
void MouseSetup();