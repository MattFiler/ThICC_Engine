#include "pch.h"
#include "KeybindManager.h"

json KeybindManager::config = "";
json KeybindManager::glyph_config = "";
ThICC_InputData* KeybindManager::m_ID = nullptr;