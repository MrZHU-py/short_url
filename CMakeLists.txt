cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(ShortURL VERSION 1.0)

# 设置C标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(include)

# 查找当前目录下的所有源文件并保存到变量
file(GLOB SOURCES "src/*.c")

# 添加可执行文件
add_executable(short_url ${SOURCES})

# 链接需要的库
target_link_libraries(short_url mysqlclient hiredis)
