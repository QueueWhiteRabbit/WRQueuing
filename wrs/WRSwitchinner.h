//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef WRSWITCHINNER_H_
#define WRSWITCHINNER_H_

#include "QueueingDefs.h"

namespace queueing {

class WRSwitch_inner : public cCompoundModule {
public:
	WRSwitch_inner();
	virtual ~WRSwitch_inner();
};

} /* namespace queueing */
#endif /* WRSWITCHINNER_H_ */