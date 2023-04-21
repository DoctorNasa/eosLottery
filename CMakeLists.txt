# Include AtomicAssets library
add_subdirectory(atomicassets)

# Link against AtomicAssets library
target_link_libraries(${CMAKE_PROJECT_NAME} atomicassets)
