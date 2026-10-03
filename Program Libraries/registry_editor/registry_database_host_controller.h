/*
    *	Registry Host Controller Library
    *
    *	Copyright (c) 2022 RANDOM ARMESE HITEMIT - REGISTRY HOST CONTROLLER
    *	All rights reserved.
*/

// ------------------------------------------------------ -
// 12 / 20 / 2025 - 11:30 : 54PM
// ------------------------------------------------------ -
// 
// LICENSE
// 
// ====================================================== =
// 
// Copyright(c) 2022 RANDOM ARMESE HITEMIT
// All Rights Reserved
// 
// -- -
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
// -- -

#pragma message(" *	Registry Host Controller Library")
#pragma message(" *")
#pragma message(" *	Copyright (c) 2022 RANDOM ARMESE HITEMIT - REGISTRY HOST CONTROLLER")
#pragma message(" *	All rights reserved.")
#pragma message(" ")
#pragma message(" ")
#pragma message(" ------------------------------------------------------ -")
#pragma message(" 12 / 20 / 2025 - 11:30 : 54PM")
#pragma message(" ------------------------------------------------------ -")
#pragma message(" ")
#pragma message(" LICENSE")
#pragma message(" ")
#pragma message(" ====================================================== =")
#pragma message(" ")
#pragma message(" Copyright(c) 2022 RANDOM ARMESE HITEMIT")
#pragma message(" All Rights Reserved")
#pragma message(" ")
#pragma message(" -- -")
#pragma message(" ")
#pragma message(" Permission is hereby granted, free of charge, to any person obtaining a copy")
#pragma message(" of this software and associated documentation files(the \"Software\"), to deal")
#pragma message(" in the Software without restriction, including without limitation the rights")
#pragma message(" to use, copy, modify, merge, publish, distribute, sublicense, and /or sell")
#pragma message(" copies of the Software, and to permit persons to whom the Software is")
#pragma message(" furnished to do so, subject to the following conditions :")
#pragma message(" ")
#pragma message(" The above copyright notice and this permission notice shall be included in all")
#pragma message(" copies or substantial portions of the Software.")
#pragma message(" ")
#pragma message(" THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR")
#pragma message(" IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,")
#pragma message(" FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL ")
#pragma message(" AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER")
#pragma message(" LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,")
#pragma message(" OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE")
#pragma message(" SOFTWARE.")
#pragma message(" ")
#pragma message(" -- -")

#pragma once

#define NO_RECORDER
#define NO_ERROR_LOOKINGUP
#define NO_UAC

#include "../../Program Libraries/system/system.h"

#undef NO_UAC
#undef NO_RECORDER
#undef NO_ERROR_LOOKINGUP

#include "../../Program Libraries/utilityX/utilityX.h"

namespace registry_database_host_controller {
    inline bool server_state() {
        return utilityX::test_netconnection("127.0.0.1", 41420);
    }
    inline void server_startup(const std::string& REG_PATH = "..\\..\\Program Execute Files\\registry_editor host") {
        if (utilityX::test_netconnection("127.0.0.1", 41420)) {
            throw platform_core::ecc{ 2, 35 };
        }
        else {
            try {
                utilityX::initiate(REG_PATH);
            }
			catch (...) {
				throw platform_core::ecc{ 2, 37 };
			}
        }
    }
    inline void server_shutdown() {
        if (utilityX::test_netconnection("127.0.0.1", 41420)) {
            platform_core::connector close_port = platform_core::connector::connector_begin("127.0.0.3", 1);
            close_port.connect("127.0.0.3", 2);
            try { close_port.notify(1); }
            catch (...) {}
            platform_core::connector::connector_end(close_port);
        }
        else {
            throw platform_core::ecc{ 2, 36 };
        }
    }
}

#undef REG_PATH