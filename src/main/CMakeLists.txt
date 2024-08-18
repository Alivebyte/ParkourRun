file(GLOB MAIN_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)


add_executable(ParkourRun ${MAIN_SRC})

target_compile_features(ParkourRun PRIVATE cxx_std_17)
target_include_directories(ParkourRun PRIVATE "${CMAKE_SOURCE_DIR}/3rdpty/olcPixelGameEngine")
target_include_directories(ParkourRun PRIVATE "${CMAKE_SOURCE_DIR}/include")
