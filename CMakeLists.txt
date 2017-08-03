cmake_minimum_required(VERSION 3.7)
project(caffemachine)

set(CMAKE_CXX_STANDARD 11)

#Websocketpp
find_package(websocketpp)
include_directories(${WEBSOCKETPP_INCLUDE_DIR})

#BOOST
find_package( Boost 1.40 COMPONENTS system REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

#OpenCV
find_package( OpenCV REQUIRED )

#Caffe
set(Caffe_DIR ~/caffe/build)
find_package(Caffe)
include_directories(~/caffe/include)
add_definitions(${Caffe_DEFINITIONS})


set(SOURCE_FILES src/main.cpp src/caffe/Classifier.cpp src/caffe/Classifier.h src/session/SessionManager.cpp src/session/SessionManager.h src/session/Session.cpp src/session/Session.h vendors/json.h src/rpc/RpcRequest.cpp src/rpc/RpcRequest.h src/rpc/RpcError.cpp src/rpc/RpcError.h src/server/CaffeServer.cpp src/server/CaffeServer.h src/rpc/RpcResponse.cpp src/rpc/RpcResponse.h src/server/ServerHelper.h src/caffe/Model.cpp src/caffe/Model.h src/caffe/ModelManager.cpp src/caffe/ModelManager.h vendors/ThreadPool.h src/server/ServerConfig.cpp src/server/ServerConfig.h src/Utils.h)
add_executable(caffemachine ${SOURCE_FILES})


target_link_libraries(caffemachine ${OpenCV_LIBS} ${Boost_LIBRARIES} glog caffe)