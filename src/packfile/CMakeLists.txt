file(GLOB PF_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)


add_executable(PackFile ${PF_SRC})


target_compile_features(PackFile PRIVATE cxx_std_17)
target_include_directories(PackFile PRIVATE "${CMAKE_SOURCE_DIR}/3rdpty/olcPixelGameEngine")