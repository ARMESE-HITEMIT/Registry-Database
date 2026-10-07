# Registry Database

> **Version:** 2.0A

## DEV NOTE:
- This version store runtime data in binary file (like ram) not memory so u can open & store large file without using many ram (swap & virtual address)
- Support lock data with password (key, value, byte, word, dword, qword) using encryption (AES-256)
- Server / Host with custom ip/port
- fix data safety issues
- include std::exception
- added iterator
- new regx format 2.0A
- added data recovery without server / host (local version)
- added ahead write log
- blocking a regx being open by more than one program (without server / local version) prevent data race writing

THIS VERSION WILL CHANGED `registry_editor.h` file format/symbol/function/usuage

PLEASE NOTE THAT ANY UPDATE VERSION MAY CHANGE FORMATTION OF MMAP OR REGX FILE.

## Questions, feedback, or suggestions? DM via email: helldefense@outlook.com.

## License

Copyright (c) 2022 RANDOM ARMESE HITEMIT. <br>
All rights reserved.

* Software License:<br>
  HREF: `https://docs.google.com/document/d/1_BNwiYPVKE7-OBkRfHusiKhqvkmBwJFBXv6fG7i2ntk/edit?usp=sharing`<br>
  SCAN CODE:<br>
  <img src="Program%20Datas/Resources/Documents/software_license_scancode.png" width="200">
------------------------------------------------------------------------------------------------------------------
* License Agreement - Global Version:<br>
  HREF: `https://docs.google.com/document/d/1s8TCbDmofW26iyK5n1O6HS57ldkwyr0_hbF-Z2FjVtw/edit?usp=sharing`<br>
  SCAN CODE:<br>
  <img src="Program%20Datas/Resources/Documents/license_scancode.png" width="200">
