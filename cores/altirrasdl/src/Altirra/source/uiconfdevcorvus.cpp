//	Altirra - Atari 800/800XL/5200 emulator
//	Copyright (C) 2009-2026 Avery Lee
//
//	This program is free software; you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation; either version 2 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License along
//	with this program. If not, see <http://www.gnu.org/licenses/>.

#include <stdafx.h>
#include <at/atcore/propertyset.h>
#include "uiconfgeneric.h"

bool ATUIConfDevCorvus(VDGUIHandle hParent, ATPropertySet& props) {
	return ATUIShowDialogGenericConfig(hParent, props, L"Corvus Interface Options",
		[](IATUIConfigView& view) {
			view.AddBoolChoice()
				.SetLabel(L"&Controller port")
				.SetTag("altports")
				.SetDefault(false)
				.SetFirstChoiceValue(false)
				.SetFirstChoiceText(L"Ports 3+4 (standard, but 400/800 only)")
				.SetSecondChoiceText(L"Ports 1+2 (XL/XE compatible)");
		}
	);
}
