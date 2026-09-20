#include <iostream>
#include "../../Program Libraries/registry_editor/registry_database_host_controller.h"

const char* HELP = R"(TOPIC
  Shell Registry Editor Help System
OPTIONS
  -help / /? : Display this help information.
  shutdown : Shutdown registry database host.
  <path to regx file> : open editor
  	+   0:> EXIT : Exit and save changes to registry file
  	+   1:> CREATE_KEY <keyname> : create key
  	+   2:> CREATE_VALUE <value name> <value data (string)> : create value
  	+   3:> CREATE_BYTE <byte name> <byte data (int)> : create byte
  	+   4:> CREATE_WORD <word name> <word data (int)> : create word
  	+   5:> CREATE_DWORD <dword name> <dword data (int)> : create dword
  	+   6:> CREATE_QWORD <qword name> <qword data (int)> : create qword
  	+   7:> MODIFY_VALUE <value name> <new value data (string)> : modify value
  	+   8:> MODIFY_BYTE <byte name> <new byte data (int)> : modify byte
  	+   9:> MODIFY_WORD <word name> <new word data (int)> : modify word
  	+  10:> MODIFY_DWORD <dword name> <new dword data (int)> : modify dword
  	+  11:> MODIFY_QWORD <qword name> <new qword data (int)> : modify qword
  	+  12:> READ_VALUE <value name> : read value
  	+  13:> READ_BYTE <byte name> : read byte
  	+  14:> READ_WORD <word name> : read word
  	+  15:> READ_DWORD <dword name> : read dword
  	+  16:> READ_QWORD <qword name> : read qword
  	+  17:> GET_TYPE <object name> : get type of object
  	+  18:> RENAME <object old name> <object new name> : rename object
  	+  19:> CONTAINS <object name>	: check if object exist in this key
  	+  20:> LIST : print out name of every element in this key
  	+  21:> READ_ELEMENT <object name> : read element data
  	+  22:> DELETE_ELEMENT <object name> : remove element in this key
  	+  23:> GET_KEYNAME_PATH : print out this registry file path
  	+  24:> SAVE : save changes to registry file
  	+  25:> PROPERTIES : print out name and type of every element in this key 
  	+  26:> CHANGE_PATH <...> :
  	     *  path : move to this absolute/relative path
  	     *  keyname : move to this key
  	     *  .. : return to previous key
    +  27:> HELP : Display help information.)";

int main(int argc, char* argv[])
{
	if (argc > 2) {
		std::cout << "\nShell Registry Editor doesn't recognize command line option " << argv[2] << "\nFor more information, refer to the help by running 'shell-dregedit -help' / 'shell-dregedit /?'\n\n";
	}
	if (argc == 1 || std::strcmp(argv[1], "-help") == 0 || std::strcmp(argv[1], "/?") == 0) {
		std::cout << HELP;
	}
    else {
        if (std::strcmp(argv[1], "shutdown") == 0) {
            if (registry_database_host_controller::server_state()) {
                registry_database_host_controller::server_shutdown();
                std::cout << "\nRegistry Database Host Server shutdown successfuly\n\n";
            }
            else {
                std::cout << "\nRegistry Database Host Server is already stopped.\n\n";
            }
        }
        else {
            if (registry_database_host_controller::server_state()) {
                std::string current_key_name_path = "*__root__*";
                while (true) {
                    std::string argument = "";
                    std::cout << "[" << current_key_name_path << "] >>> ";
                    std::getline(std::cin, argument);
                    if (argument == "") continue;
                    std::cout << argument << "\n";
                }
            }
            else {
                std::cout << "\nRegistry Database Host Server is not running\n\n";
            }
        }
    }
	return 0;
}