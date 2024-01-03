//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

namespace oe
{
	class IBaseComponent
	{
	public:
		virtual void Serialize() = 0;
		virtual void Deserialize() = 0;

		void SetActive(bool value)
		{
			m_IsActive = value;
		}

		bool IsActive() const
		{
			return m_IsActive;
		}

	private:
		bool m_IsActive{true};
	};
} // namespace oe