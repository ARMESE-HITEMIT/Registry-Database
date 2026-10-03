#include "../../Program Libraries/utilityX/utilityX.h"
#include <iostream>
#define private public
#include "../../Program Libraries/registry_editor/registry_editor-service-local.h"

int main() {
	registry_editor_service_local::registry_database a;
	a.L1_sector = 0;
	a.L1_cache = (BYTE*)malloc(registry_editor_service_local::page_size);
	memset(a.L1_cache, 0, registry_editor_service_local::page_size);
	a.cache_handle = utilityX::filesystem::open_file("test.bin");
	utilityX::filesystem::fetch_chunk(a.cache_handle, a.L1_cache, registry_editor_service_local::page_size, 0);

	BYTE cds = 'A';
	QWORD cd = a.disk_malloc(4096);
	a.write_memory_address((BYTE*)&cds, cd + 4095, 1);
	std::cout << cd << "\n";
	cd = a.disk_malloc(1000);
	std::cout << cd;
	cds = 'B';
	a.write_memory_address((BYTE*)&cds, cd, 1);

	utilityX::filesystem::flush_chunk(a.cache_handle, a.L1_cache, registry_editor_service_local::page_size, a.L1_sector);
	utilityX::filesystem::close_file(a.cache_handle);
	return 0;
}