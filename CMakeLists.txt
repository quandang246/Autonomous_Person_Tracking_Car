cmake_minimum_required(VERSION 3.2)

project(APTC
        VERSION 1.0.0
        LANGUAGES C CXX)

include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(JETSON_GPIO_POST_INSTALL "Create gpio group and reload udev rules" ON)
set(UDEVRULES_INSTALL_DIR "/lib/udev/rules.d" CACHE STRING "Base directory for udev rules")

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Compilation flags
add_definitions(-Wall -O2 -fPIC -g -DAPI_EXPORTS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(OpenCV REQUIRED)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_BUILD_TYPE Release)

find_package(CUDA REQUIRED)
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++14 -g -G -gencode arch=compute_53,code=sm_53")

if(WIN32)
    enable_language(CUDA)
endif()

include_directories(${PROJECT_SOURCE_DIR}/deepsort/include)
# include and link dirs of cuda and tensorrt, you need to adapt them if yours are different
# cuda
include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)
# tensorrt
include_directories(/usr/include/aarch64-linux-gnu/)
link_directories(/usr/lib/aarch64-linux-gnu/)

include_directories(
    ${CUDA_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/deepsort/include
)
aux_source_directory(${PROJECT_SOURCE_DIR}/deepsort/src SRC_DIR)

# ===== deepsort =====
add_library(deepsort SHARED ${SRC_DIR})
target_link_libraries(deepsort 
    ${CUDA_LIBRARIES} ${OpenCV_LIBS} 
    cudart nvinfer nvonnxparser
)

# ===== yolo =====
include_directories(${PROJECT_SOURCE_DIR}/yolo/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/yolo/src YOLO_SRC_DIR)

cuda_add_library(yolov5_trt SHARED 
    ${PROJECT_SOURCE_DIR}/yolo/include/yololayer.cu 
    ${PROJECT_SOURCE_DIR}/yolo/src/yolov5_lib.cpp
)
target_link_libraries(yolov5_trt nvinfer cudart deepsort)

# ===== jetson-gpio =====
set(JetsonGPIO_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/JetsonGPIO.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MainModule.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PWM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PythonFunctions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ExceptionHandling.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GPIOPinData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GPIOEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Callback.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DictionaryLike.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ModelUtility.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/WaitResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LazyString.cpp
)

configure_file(cmake/JetsonGPIOConfig.h.in JetsonGPIOConfig.h)

add_library(JetsonGPIO SHARED ${JetsonGPIO_LIB_SRCS})
target_link_libraries(JetsonGPIO Threads::Threads)
set_target_properties(JetsonGPIO PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories(JetsonGPIO PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

add_library(JetsonGPIO::JetsonGPIO ALIAS JetsonGPIO)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateModelHeader.cmake)

install(TARGETS JetsonGPIO
        EXPORT JetsonGPIOTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/JetsonGPIO.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file(GLOB JetsonGPIO_Public_Headers ${CMAKE_CURRENT_SOURCE_DIR}/include/JetsonGPIO/*.h)
install(FILES ${JetsonGPIO_Public_Headers} DESTINATION include/JetsonGPIO)

install(FILES ${PROJECT_BINARY_DIR}/JetsonGPIOConfig.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT JetsonGPIOTargets
        NAMESPACE JetsonGPIO::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JetsonGPIO)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/JetsonGPIOConfigVersion.cmake
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/JetsonGPIOConfig.cmake.in
                              ${CMAKE_CURRENT_BINARY_DIR}/JetsonGPIOConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JetsonGPIO)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/JetsonGPIOConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/JetsonGPIOConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/JetsonGPIO)

install(FILES
        ${CMAKE_CURRENT_LIST_DIR}/LICENSE.txt
        DESTINATION ${CMAKE_INSTALL_DATADIR}/JetsonGPIO)

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/99-gpio.rules DESTINATION ${UDEVRULES_INSTALL_DIR})

if (JETSON_GPIO_POST_INSTALL)
install (CODE "execute_process(COMMAND bash ./scripts/post_install.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})")
endif()

# ===== main =====
aux_source_directory(${PROJECT_SOURCE_DIR}/src M_SRC_DIR)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${M_SRC_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE JetsonGPIO nvinfer cudart yolov5_trt)
if(UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -O2 -pthread)
endif()
