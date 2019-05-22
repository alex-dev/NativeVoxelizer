#pragma once

extern "C" {
void* allocate() noexcept;
bool deallocate(void* master) noexcept;
}
